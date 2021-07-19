/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "usbfn_io_mgr.h"

#define HDF_LOG_TAG usbfn_io_mgr

static int ReqToIoData(struct UsbFnRequest *req, struct IoData *ioData,
    uint32_t aio, uint32_t timeout)
{
    if (req == NULL || ioData == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    struct ReqList *reqList = (struct ReqList *) req;
    ioData->aio = aio;
    if (req->type == USB_REQUEST_TYPE_PIPE_WRITE) {
        ioData->read = 0;
    } else if (req->type == USB_REQUEST_TYPE_PIPE_READ) {
        ioData->read = 1;
    }
    ioData->buf = reqList->buf;
    ioData->len = req->length;
    ioData->timeout = timeout;

    return 0;
}

int OpenEp0AndMapAddr(struct UsbFnFuncMgr *funcMgr)
{
    int ret;
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    funcMgr->fd = fnOps->openPipe(funcMgr->name, 0);
    if (funcMgr->fd <= 0) {
        HDF_LOGE("%{public}s:%{public}d openPipe failed", __func__, __LINE__);
        return HDF_ERR_IO;
    }

    ret = fnOps->queueInit(funcMgr->fd);
    if (ret) {
        HDF_LOGE("%{public}s:%{public}d queueInit failed", __func__, __LINE__);
        return HDF_ERR_IO;
    }
    return 0;
}

static UsbFnRequestType GetReqType(struct UsbHandleMgr *handle, uint8_t pipe)
{
    int ret;
    struct UsbFnPipeInfo info;
    UsbFnRequestType type = USB_REQUEST_TYPE_INVALID;
    if (pipe > 0) {
        ret = UsbFnIoMgrInterfaceGetPipeInfo(&(handle->intfMgr->interface), pipe - 1, &info);
        if (ret) {
            HDF_LOGE("%{public}s:%{public}d UsbFnMgrInterfaceGetPipeInfo err", __func__, __LINE__);
            type = USB_REQUEST_TYPE_INVALID;
        }
        if (info.dir == USB_PIPE_DIRECTION_IN) {
            type = USB_REQUEST_TYPE_PIPE_WRITE;
        } else if (info.dir == USB_PIPE_DIRECTION_OUT) {
            type = USB_REQUEST_TYPE_PIPE_READ;
        }
    }
    return type;
}

struct UsbFnRequest *UsbFnIoMgrRequestAlloc(struct UsbHandleMgr *handle, uint8_t pipe, uint32_t len)
{
    int ret;
    int ep;
    uint8_t *mapAddr = NULL;
    struct UsbFnRequest *req = NULL;
    struct ReqList *reqList = NULL;
    struct UsbFnInterfaceMgr *intfMgr = handle->intfMgr;
    struct UsbFnFuncMgr *funcMgr = intfMgr->funcMgr;
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    if (pipe == 0) {
        if (funcMgr->fd <= 0) {
            ret = OpenEp0AndMapAddr(funcMgr);
            if (ret) {
                return NULL;
            }
        }
        ep = funcMgr->fd;
    } else {
        ep = handle->fds[pipe - 1];
    }
    mapAddr = fnOps->mapAddr(ep, len);
    if (mapAddr == NULL) {
        HDF_LOGE("%{public}s:%{public}d mapAddr failed", __func__, __LINE__);
        return NULL;
    }

    reqList = OsalMemCalloc(sizeof(struct ReqList));
    if (reqList == NULL) {
        HDF_LOGE("%{public}s:%{public}d OsalMemCalloc err", __func__, __LINE__);
        return NULL;
    }
    req = &reqList->req;

    if (pipe == 0) {
        DListInsertTail(&reqList->entry, &funcMgr->reqEntry);
    } else {
        DListInsertTail(&reqList->entry, &handle->reqEntry);
    }
    reqList->handle = handle;
    reqList->fd = ep;
    reqList->buf = (uint32_t)mapAddr;
    reqList->pipe = pipe;
    req->length = len;
    req->obj = handle->intfMgr->interface.object;
    req->buf = mapAddr;
    req->type = GetReqType(handle, pipe);

    return req;
}

int UsbFnIoMgrRequestFree(struct UsbFnRequest *req)
{
    struct GenericMemory mem;
    int ret;
    if (req == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct ReqList *reqList = (struct ReqList *) req;
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();

    ret = fnOps->unmapAddr(req->buf, req->length);
    if (ret) {
        HDF_LOGE("%{public}s:%{public}d ummapAddr failed, ret=%{public}d ", __func__, __LINE__, ret);
        return HDF_ERR_DEVICE_BUSY;
    }
    mem.size = req->length;
    mem.buf = (uint32_t)req->buf;
    ret = fnOps->releaseBuf(reqList->fd, &mem);
    if (ret) {
        HDF_LOGE("%{public}s:%{public}d releaseBuf err", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }

    DListRemove(&reqList->entry);
    OsalMemFree(reqList);
    return 0;
}

int UsbFnIoMgrRequestSubmitAsync(struct UsbFnRequest *req)
{
    int ret;
    struct IoData ioData = {0};
    struct ReqList *reqList = NULL;
    if (req == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    reqList = (struct ReqList *) req;
    if (ReqToIoData(req, &ioData, 1, 0)) {
        return HDF_ERR_IO;
    }
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    ret = fnOps->pipeIo(reqList->fd, &ioData);

    return ret;
}

int UsbFnIoMgrRequestCancel(struct UsbFnRequest *req)
{
    int ret;
    struct IoData ioData = {0};
    struct ReqList *reqList = NULL;
    if (req == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    reqList = (struct ReqList *) req;
    if (ReqToIoData(req, &ioData, 1, 0)) {
        return HDF_ERR_IO;
    }
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    ret = fnOps->cancelIo(reqList->fd, &ioData);

    return ret;
}

int UsbFnIoMgrRequestGetStatus(struct UsbFnRequest *req, UsbRequestStatus *status)
{
    struct IoData ioData = {0};
    struct ReqList *reqList;
    if (req == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    reqList = (struct ReqList *) req;
    if (ReqToIoData(req, &ioData, 1, 0)) {
        return HDF_ERR_IO;
    }
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    *status = -(fnOps->getReqStatus(reqList->fd, &ioData));

    return 0;
}

int UsbFnIoMgrRequestSubmitSync(struct UsbFnRequest *req, uint32_t timeout)
{
    int ret;
    struct IoData ioData = {0};
    struct ReqList *reqList;

    if (req == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    reqList = (struct ReqList *) req;
    if (ReqToIoData(req, &ioData, 0, timeout)) {
        return HDF_ERR_IO;
    }
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    ret = fnOps->pipeIo(reqList->fd, &ioData);
    if (ret > 0) {
        req->status = USB_REQUEST_COMPLETED;
        req->actual = ret;
        return 0;
    }

    return ret;
}

static int HandleInit(struct UsbHandleMgr *handle, struct UsbFnInterfaceMgr *interfaceMgr)
{
    int ret;
    uint32_t i, j;
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();

    DListHeadInit(&handle->reqEntry);
    handle->numFd = interfaceMgr->interface.info.numPipes;
    for (i = 0; i < handle->numFd; i++) {
        handle->fds[i] = fnOps->openPipe(interfaceMgr->funcMgr->name, interfaceMgr->startEpId + i);
        if (handle->fds[i] <= 0) {
            return HDF_ERR_IO;
        }

        ret = fnOps->queueInit(handle->fds[i]);
        if (ret) {
            HDF_LOGE("%{public}s: queueInit failed ret = %{public}d", __func__, ret);
            return HDF_ERR_IO;
        }

        handle->reqEvent[i] = OsalMemCalloc(sizeof(struct UsbFnReqEvent) * MAX_REQUEST);
        if (handle->reqEvent[i] == NULL) {
            HDF_LOGE("%{public}s: OsalMemCalloc failed", __func__);
            goto FREE_EVENT;
        }
    }
    handle->intfMgr = interfaceMgr;
    return 0;

FREE_EVENT:
   for (j = 0; j < i; j++) {
        OsalMemFree(handle->reqEvent[j]);
    }
    return HDF_ERR_IO;
}

struct UsbHandleMgr *UsbFnIoMgrInterfaceOpen(struct UsbFnInterface *interface)
{
    int ret;
    if (interface == NULL) {
        return NULL;
    }
    struct UsbFnInterfaceMgr *interfaceMgr = (struct UsbFnInterfaceMgr *)interface;
    if (interfaceMgr->isOpen) {
        HDF_LOGE("%{public}s: interface has opened", __func__);
        return NULL;
    }
    struct UsbHandleMgr *handle = OsalMemCalloc(sizeof(struct UsbHandleMgr));
    if (handle == NULL) {
        return NULL;
    }

    ret = HandleInit(handle, interfaceMgr);
    if (ret) {
        HDF_LOGE("%{public}s: HandleInit failed", __func__);
        OsalMemFree(handle);
        return NULL;
    }

    interfaceMgr->isOpen = true;
    interfaceMgr->handle = handle;
    return handle;
}

int UsbFnIoMgrInterfaceClose(struct UsbHandleMgr *handle)
{
    int ret;
    uint32_t i;
    if (handle == NULL) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    struct UsbFnInterfaceMgr *interfaceMgr = handle->intfMgr;

    if (interfaceMgr == NULL || interfaceMgr->isOpen == false) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    for (i = 0; i < handle->numFd; i++) {
        ret = fnOps->queueDel(handle->fds[i]);
        if (ret) {
            HDF_LOGE("%{public}s:%{public}d queueDel failed, ret=%{public}d ", __func__, __LINE__, ret);
            return HDF_ERR_DEVICE_BUSY;
        }

        ret = fnOps->closePipe(handle->fds[i]);
        if (ret) {
            HDF_LOGE("%{public}s:%{public}d closePipe failed, ret=%{public}d ", __func__, __LINE__, ret);
            return HDF_ERR_DEVICE_BUSY;
        }
        handle->fds[i] = -1;
        OsalMemFree(handle->reqEvent[i]);
    }

    OsalMemFree(handle);
    interfaceMgr->isOpen = false;
    interfaceMgr->handle = NULL;
    return 0;
}

int UsbFnIoMgrInterfaceGetPipeInfo(struct UsbFnInterface *interface,
    uint8_t pipeId, struct UsbFnPipeInfo *info)
{
    int ret;
    int fd;
    if (info == NULL || interface == NULL || pipeId >= interface->info.numPipes) {
        HDF_LOGE("%{public}s:%{public}d INVALID_PARAM", __func__, __LINE__);
        return HDF_ERR_INVALID_PARAM;
    }
    struct UsbFnAdapterOps *fnOps = UsbFnAdapterGetOps();
    struct UsbFnInterfaceMgr *interfaceMgr = (struct UsbFnInterfaceMgr *)interface;
    if (interfaceMgr->isOpen) {
        fd = interfaceMgr->handle->fds[pipeId];
        ret = fnOps->getPipeInfo(fd, info);
        if (ret) {
            HDF_LOGE("%{public}s: getPipeInfo failed", __func__);
            return HDF_ERR_DEVICE_BUSY;
        }
    } else {
        fd = fnOps->openPipe(interfaceMgr->funcMgr->name, interfaceMgr->startEpId + pipeId);
        if (fd <= 0) {
            HDF_LOGE("%{public}s: openPipe failed", __func__);
            return HDF_ERR_IO;
        }
        ret = fnOps->getPipeInfo(fd, info);
        if (ret) {
            fnOps->closePipe(fd);
            HDF_LOGE("%{public}s: getPipeInfo failed", __func__);
            return HDF_ERR_DEVICE_BUSY;
        }
        fnOps->closePipe(fd);
    }

    info->id = pipeId;
    return ret;
}