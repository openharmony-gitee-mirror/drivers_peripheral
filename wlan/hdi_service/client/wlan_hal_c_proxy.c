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

#include "wlan_hal_c_proxy.h"
#include "wlan_hdi_service_stub.h"
#include <string.h>
#include <hdf_base.h>
#include <hdf_log.h>
#include <hdf_sbuf.h>
#include <osal_mem.h>
#include <servmgr_hdi.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ETH_ADDR_LEN  6

static int32_t WlanProxyCall(struct IWifiInterface *self,
    int32_t id, struct HdfSBuf *data, struct HdfSBuf *reply)
{
    if (self->remote == NULL || self->remote->dispatcher == NULL ||
        self->remote->dispatcher->Dispatch == NULL) {
            HDF_LOGE("%{public}s: obj is null", __func__);
            return HDF_ERR_INVALID_OBJECT;
    }
    return self->remote->dispatcher->Dispatch(self->remote, id, data, reply);
}

static int32_t WlanConstruct(struct IWifiInterface *self)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_CONSTRUCT, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanDestruct(struct IWifiInterface *self)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_DECONSTRUCT, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanStart(struct IWifiInterface *self)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_START, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanStop(struct IWifiInterface *self)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_STOP, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanCreateFeature(struct IWifiInterface *self, const int32_t type, struct WlanFeatureInfo **ifeature)
{
    bool ec = 0;
    int32_t wlanType = 0;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, type)) {
        HDF_LOGE("%{public}s: write type failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_CREATE_FEATURE, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    *ifeature = (struct WlanFeatureInfo*)OsalMemAlloc(sizeof(struct WlanFeatureInfo));
    const char *ifname = HdfSbufReadString(reply);
    (*ifeature)->ifName = strdup(ifname);
    HdfSbufReadInt32(reply, &wlanType);
    (*ifeature)->wlanType = wlanType;
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanDestroyFeature(struct IWifiInterface *self, struct WlanFeatureInfo *ifeature)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, ifeature->wlanType)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_DESTROY_FEATURE, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    OsalMemFree(ifeature);
    if (ifeature->ifName != NULL) {
        free(ifeature->ifName);
    }
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetAsscociatedStas(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature,
    struct StaInfo *staInfo, uint32_t count, uint32_t *num)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_ASSCOCIATE_STA, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if(!HdfSbufReadUint32(reply, num)){
        HDF_LOGE("%{public}s: read num failed! error code is %{public}d", __func__, ec);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if(!HdfSbufReadBuffer(reply, (const void **)&staInfo, &count)){
        HDF_LOGE("%{public}s: read num failed! error code is %{public}d", __func__, ec);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetChipId(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature, uint8_t *chipId)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, ifeature->wlanType)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_CHIPID, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if (!HdfSbufReadUint8(reply, chipId)) {
        HDF_LOGE("%{public}s: get chipId failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetDeviceMacAddress(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature,
    unsigned char *mac, uint8_t len)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, ifeature->wlanType)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_MAC_ADDR, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    mac = (unsigned char *)HdfSbufReadUnpadBuffer(reply, len);
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetFeatureByIfName(struct IWifiInterface *self,  const char *ifName, struct WlanFeatureInfo **ifeature)
{
    int32_t ec = HDF_FAILURE;
    int32_t wlanType = 0;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_FEATURE_NAME, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if (!HdfSbufReadInt32(reply, &wlanType)) {
        HDF_LOGE("%{public}s: read wlantype failed! error code is %{public}d", __func__, ec);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    (*ifeature)->wlanType = wlanType;
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetFeatureType(struct IWifiInterface *self, struct WlanFeatureInfo *ifeature)
{
    int32_t ec = HDF_FAILURE;
    int32_t wlanType = 0;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, ifeature->wlanType)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_FEATURE_TYPE, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if (!HdfSbufReadInt32(reply, &wlanType)) {
        HDF_LOGE("%{public}s: read wlantype failed! error code is %{public}d", __func__, ec);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ifeature->wlanType = wlanType;

finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetFreqsWithBand(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature, int32_t band, int32_t *freqs,
    uint32_t count, uint32_t *num)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, band)) {
        HDF_LOGE("%{public}s: write band failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_FREQ_WITHBAND, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if (!HdfSbufReadUint32(reply, num)) {
        HDF_LOGE("%{public}s: read num failed! error code is %{public}d", __func__, ec);
        ec = HDF_FAILURE;
        goto finished;
    }
    for (uint32_t i = 0; i < (*num); i++) {
        if (!HdfSbufReadInt32(reply, &freqs[i])) {
            HDF_LOGE("%s: write freq failed", __func__);
            ec = HDF_FAILURE;
            goto finished;
        }
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetIfNamesByChipId(struct IWifiInterface *self, const uint8_t chipId, char **ifNames, uint32_t *num)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteUint8(data, chipId)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_NAME_BYCHIPID, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if (!HdfSbufReadUint32(reply, num)) {
        HDF_LOGE("%s: write num failed", __func__);
        ec = HDF_ERR_INVALID_PARAM;
        goto finished;
    }
    *ifNames = (char *)HdfSbufReadString(reply);

finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetNetworkIfaceName(struct IWifiInterface *self, struct WlanFeatureInfo *ifeature)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_NETWORK_NAME, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    const char *ifName = HdfSbufReadString(reply);
    HDF_LOGE("%{public}s: ifName is %{public}s!", __func__, ifName);
    ifeature->ifName = strdup(ifName);
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetSupportCombo(struct IWifiInterface *self, uint64_t *combo)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_SUPPORT_COMBO, data, reply);
    if ((ec != HDF_SUCCESS) && (ec != HDF_ERR_NOT_SUPPORT)) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    if (ec == HDF_SUCCESS) {
        for (int i = 0; i < ETH_ADDR_LEN; i++) {
            int ret = HdfSbufReadUint64(reply, &combo[i]);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("%s: write combo failed!, error code: %d", __func__, ret);
            }
        }
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanGetSupportFeature(struct IWifiInterface *self, uint8_t *supType)
{
    int32_t ec = HDF_FAILURE;
    uint32_t size = 0;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_GET_SUPPORT_FEATURE, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    HdfSbufReadUint32(reply, &size);
    supType = (uint8_t *)HdfSbufReadUnpadBuffer(reply, size);
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

int32_t (*callback_)(uint32_t event, void *data, const char *ifName);

int32_t CallbackWlanProxy(uint32_t event, void *data, const char *ifName)
{
    HDF_LOGI("%s: enter", __func__);
    if (data == NULL) {
        HDF_LOGE("%s: ptr NULL", __func__);
        return HDF_ERR_INVALID_PARAM;
    }
    callback_(event, data, ifName);
    return HDF_SUCCESS;
}

static int ServiceManagerTestCallbackDispatch(struct HdfRemoteService *service, int code, 
    struct HdfSBuf *data, struct HdfSBuf *reply)
{
    int32_t ret = 0;

    switch (code){
        case RESET_STATUS_GET:
            ret = CallbackWlanProxy((int32_t)code, data, "wlan0");
            if (ret != 0) {
                HDF_LOGE("%s: failed, error code is %d", __func__, ret);
            }
            break;
        default:
            break;
    }
    return HDF_SUCCESS;
}

static struct HdfRemoteDispatcher g_callbackDispatcher = {
    .Dispatch = ServiceManagerTestCallbackDispatch,
};

static int32_t WlanRegisterEventCallback(struct IWifiInterface *self, CallbackFunc cbFunc)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfRemoteService *callback = HdfRemoteServiceObtain(NULL, &g_callbackDispatcher);
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    HdfSBufWriteRemoteService(data, callback);
    ec = WlanProxyCall(self, WLAN_SERVICE_REGISTER_CALLBACK, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
    callback_ = cbFunc;
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanUnregisterEventCallback(struct IWifiInterface *self)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_UNREGISTER_CALLBACK, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanResetDriver(struct IWifiInterface *self, const uint8_t chipId)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteUint8(data, chipId)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_RESET_DRIVER, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanSetCountryCode(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature, const char *code, uint32_t len)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, code)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_SET_COUNTRY_CODE, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanSetMacAddress(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature, unsigned char *mac, uint8_t len)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    for (int i = 0; i < ETH_ADDR_LEN; i++) {
        if (!HdfSbufWriteUint8(data, mac[i])) {
            HDF_LOGE(" %s: read mac failed", __func__);
            ec = HDF_ERR_INVALID_PARAM;
            goto finished;
        }
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_SET_MAC_ADDR, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanSetScanningMacAddress(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature, unsigned char *scanMac, uint8_t len)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteUint8(data, len)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    for (int i = 0; i < ETH_ADDR_LEN; i++) {
        if (!HdfSbufWriteUint8(data, scanMac[i])) {
            HDF_LOGE(" %s: read scanMac failed", __func__);
            ec = HDF_ERR_INVALID_PARAM;
            goto finished;
        }
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_SET_SACN_MACADDR, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static int32_t WlanSetTxPower(struct IWifiInterface *self, const struct WlanFeatureInfo *ifeature, int32_t power)
{
    int32_t ec = HDF_FAILURE;

    if (self == NULL) {
        return HDF_ERR_INVALID_PARAM;
    }
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    if (data == NULL || reply == NULL) {
        HDF_LOGE("%{public}s: HdfSubf malloc failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteString(data, ifeature->ifName)) {
        HDF_LOGE("%{public}s: write ifeature->ifName failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    if (!HdfSbufWriteInt32(data, power)) {
        HDF_LOGE("%{public}s: write wlanType failed!", __func__);
        ec = HDF_ERR_MALLOC_FAIL;
        goto finished;
    }
    ec = WlanProxyCall(self, WLAN_SERVICE_SET_TX_POWR, data, reply);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: call failed! error code is %{public}d", __func__, ec);
        goto finished;
    }
finished:
    if (data != NULL) {
        HdfSBufRecycle(data);
    }
    if (reply != NULL) {
        HdfSBufRecycle(reply);
    }
    return ec;
}

static void IwifiConstruct(struct IWifiInterface *inst)
{
    inst->construct = WlanConstruct;
    inst->destruct = WlanDestruct;
    inst->start = WlanStart;
    inst->stop = WlanStop;
    inst->createFeature = WlanCreateFeature;
    inst->destroyFeature = WlanDestroyFeature;
    inst->getAsscociatedStas = WlanGetAsscociatedStas;
    inst->getChipId = WlanGetChipId;
    inst->getDeviceMacAddress =WlanGetDeviceMacAddress;
    inst->getFeatureByIfName = WlanGetFeatureByIfName;
    inst->getFeatureType = WlanGetFeatureType;
    inst->getFreqsWithBand = WlanGetFreqsWithBand;
    inst->getIfNamesByChipId = WlanGetIfNamesByChipId;
    inst->getNetworkIfaceName = WlanGetNetworkIfaceName;
    inst->getSupportCombo = WlanGetSupportCombo;
    inst->getSupportFeature = WlanGetSupportFeature;
    inst->registerEventCallback = WlanRegisterEventCallback;
    inst->unregisterEventCallback = WlanUnregisterEventCallback;
    inst->resetDriver = WlanResetDriver;
    inst->setCountryCode = WlanSetCountryCode;
    inst->setMacAddress = WlanSetMacAddress;
    inst->setScanningMacAddress = WlanSetScanningMacAddress;
    inst->setTxPower = WlanSetTxPower;
}

struct IWifiInterface *HdIWifiInterfaceGet(const char *serviceName)
{
    struct HDIServiceManager *serviceMgr = HDIServiceManagerGet();
    HDF_LOGI("%{public}s: HDIServiceManager start! serviceName = %{public}s", __func__, serviceName);
    if (serviceMgr == NULL) {
        HDF_LOGE("%{public}s: HDIServiceManager not found!", __func__);
        return NULL;
    }
    struct HdfRemoteService *remote = serviceMgr->GetService(serviceMgr, serviceName);
    if (remote == NULL) {
        HDF_LOGE("%{public}s: HdfRemoteService not found!", __func__);
        return NULL;
    }
    struct IWifiInterface *wlanClient = (struct IWifiInterface *)OsalMemAlloc(sizeof(struct IWifiInterface));
    if (wlanClient == NULL) {
        HDF_LOGE("%{public}s: malloc sample instance failed!", __func__);
        HdfRemoteServiceRecycle(remote);
        return NULL;
    }
    wlanClient->remote = remote;
    IwifiConstruct(wlanClient);
    return wlanClient;
}

void HdiSampleRelease(struct IWifiInterface *instance)
{
    if (instance == NULL) {
        return;
    }
    HdfRemoteServiceRecycle(instance->remote);
    OsalMemFree(instance);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */