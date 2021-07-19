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

#include <hdf_log.h>
#include <hdf_remote_service.h>
#include <hdf_sbuf.h>
#include <servmgr_hdi.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include "cdcacm.h"
#include "osal_time.h"

#define HDF_LOG_TAG   cdc_acm_speed

struct HdfSBuf *g_data;
struct HdfSBuf *g_reply;
struct HdfRemoteService *g_acmService;
static bool g_readRuning = false;
static void TestSpeed()
{
    HdfSbufFlush(g_reply);
    int status = g_acmService->dispatcher->Dispatch(g_acmService, USB_SERIAL_WRITE_SPEED, g_data, g_reply);
    if (status) {
        HDF_LOGE("%{public}s: Dispatch USB_SERIAL_WRITE_SPEED failed status = %{public}d", __func__, status);
        return;
    }
}

static void GetTempSpeed()
{
    float speed;
    HdfSbufFlush(g_reply);
    int status = g_acmService->dispatcher->Dispatch(g_acmService, USB_SERIAL_WRITE_GET_TEMP_SPEED, g_data, g_reply);
    if (status) {
        HDF_LOGE("%{public}s: Dispatch USB_SERIAL_WRITE_GET_TEMP_SPEED failed status = %{public}d", __func__, status);
        return;
    }
    if (!HdfSbufReadFloat(g_reply, &speed)) {
        HDF_LOGE("%{public}s: HdfSbufReadFloat failed", __func__);
        return;
    }
    printf("speed : %f MB/s\n", speed);
}

static void GetSpeedDone()
{
    uint8_t isDone = 0;
    HdfSbufFlush(g_reply);
    int status = g_acmService->dispatcher->Dispatch(g_acmService, USB_SERIAL_WRITE_SPEED_DONE, g_data, g_reply);
    if (status) {
        HDF_LOGE("%{public}s: Dispatch USB_SERIAL_WRITE_SPEED_DONE failed status = %{public}d", __func__, status);
        return;
    }
    if (!HdfSbufReadUint8(g_reply, &isDone)) {
        HDF_LOGE("%{public}s: HdfSbufReadFloat failed", __func__);
        return;
    }
    if (isDone) {
        g_readRuning = false;
    }
}

int main(int argc, char *argv[])
{
    int status;
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    if (servmgr == NULL) {
        HDF_LOGE("%{public}s: HDIServiceManagerGet err", __func__);
        return HDF_FAILURE;
    }
    g_acmService = servmgr->GetService(servmgr, "usbfn_cdcacm");
    HDIServiceManagerRelease(servmgr);
    if (g_acmService == NULL) {
        HDF_LOGE("%{public}s: GetService err", __func__);
        return HDF_FAILURE;
    }

    g_data = HdfSBufTypedObtain(SBUF_IPC);
    g_reply = HdfSBufTypedObtain(SBUF_IPC);
    if (g_data == NULL || g_reply == NULL) {
        HDF_LOGE("%{public}s: GetService err", __func__);
        return HDF_FAILURE;
    }

    status = g_acmService->dispatcher->Dispatch(g_acmService, USB_SERIAL_OPEN, g_data, g_reply);
    if (status) {
        HDF_LOGE("%{public}s: Dispatch USB_SERIAL_OPEN err", __func__);
        return HDF_FAILURE;
    }

    TestSpeed();
    g_readRuning = true;
    while (g_readRuning) {
        sleep(2);
        GetTempSpeed();
        GetSpeedDone();
    }

    status = g_acmService->dispatcher->Dispatch(g_acmService, USB_SERIAL_CLOSE, g_data, g_reply);
    if (status) {
        HDF_LOGE("%{public}s: Dispatch USB_SERIAL_CLOSE err", __func__);
        return HDF_FAILURE;
    }

    HdfSBufRecycle(g_data);
    HdfSBufRecycle(g_reply);

    return 0;
}