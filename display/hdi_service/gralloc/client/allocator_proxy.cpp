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

#include "allocator_proxy.h"
#include <message_parcel.h>
#include "buffer_handle_parcel.h"
#include "iservmgr_hdi.h"
#include "parcel_utils.h"

#define HDF_LOG_TAG HDI_DISP_PROXY

namespace OHOS {
namespace HDI {
namespace Display {
namespace V1_0 {
sptr<IDisplayAllocator> IDisplayAllocator::Get(const char *serviceName)
{
    do {
        using namespace OHOS::HDI::ServiceManager::V1_0;
        static sptr<IServiceManager> servMgr = IServiceManager::Get();
        if (servMgr == nullptr) {
            HDF_LOGE("%{public}s: IServiceManager failed", __func__);
            break;
        }
        auto remote = servMgr->GetService(serviceName);
        if (remote != nullptr) {
            sptr<AllocatorProxy> hostSptr = iface_cast<AllocatorProxy>(remote);
            if (hostSptr == nullptr) {
                HDF_LOGE("%{public}s: IServiceManager GetService null ptr", __func__);
                return nullptr;
            }
            HDF_LOGE("%{public}s: GetService %{public}s ok", __func__, serviceName);
            return hostSptr;
        }
        HDF_LOGE("%{public}s: IServiceManager GetService %{public}s failed", __func__, serviceName);
    } while(false);

    return nullptr;
}

int32_t AllocatorProxy::AllocMem(const AllocInfo &info, BufferHandle *&handle)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    auto ret = ParcelUtils::PackAllocInfo(data, &info);
    if (ret != DISPLAY_SUCCESS) {
        return ret;
    }
    int32_t retCode = Remote()->SendRequest(CMD_REMOTE_ALLOCATOR_ALLOCMEM, data, reply, option);
    if (retCode != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: SendRequest failed, error code is %{public}x", __func__, retCode);
        return retCode;
    }

    retCode = reply.ReadInt32();
    if (retCode != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: Read retrun code failed, error code is %{public}x", __func__, retCode);
        return retCode;
    }

    auto retHandle = ReadBufferHandle(reply);
    if (retHandle != nullptr) {
        handle = retHandle;
        retCode = DISPLAY_SUCCESS;
    } else {
        retCode = DISPLAY_NULL_PTR;
    }
    HDF_LOGE("%{public}s: %{public}s(%{public}d) end", __FILE__, __func__, __LINE__);
    return retCode;
}
} // namespace V1_0
} // namespace Display
} // namespace HDI
} // namespace OHOS
