/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "vo_node.h"
#include "mpi_device_manager.h"

namespace OHOS::Camera{
VoNode::VoNode(const std::string& name, const std::string& type, const int streamId)
        :MpiNode(name, type, streamId)
{
    CAMERA_LOGI("%s enter, type(%s), stream id = %d\n", name_.c_str(), type_.c_str(), streamId);
}

RetCode VoNode::GetDeviceController()
{
    GetMpiDeviceManager();
    voController_ = std::static_pointer_cast<VoController>
        ((std::static_pointer_cast<VoManager>(deviceManager_->GetManager(DM_M_VO)))->GetController(DM_C_VO));
    if (voController_ == nullptr) {
        CAMERA_LOGE("get device controller failed");
        return RC_ERROR;
    }
    return RC_OK;
}

RetCode VoNode::Start()
{
    RetCode rc = RC_OK;
    rc = GetDeviceController();
    if (rc == RC_ERROR) {
        CAMERA_LOGE("GetDeviceController failed.");
        return RC_ERROR;
    }
    rc = voController_->ConfigVo();
    if (rc == RC_ERROR) {
        CAMERA_LOGE("configvo failed.");
        return RC_ERROR;
    }
    rc = voController_->StartVo();
    if (rc == RC_ERROR) {
        CAMERA_LOGE("startvo failed.");
        return RC_ERROR;
    }
    streamRunning_ = true;
    return RC_OK;
}
RetCode VoNode::Stop()
{
    RetCode rc = RC_OK;
    if (streamRunning_ == false) {
        CAMERA_LOGI("vo node : streamrunning is already false");
        return RC_OK;
    }
    streamRunning_ = false;
    rc = DisConnectMpi();
    if (rc == RC_ERROR) {
        CAMERA_LOGE("DisConnectMpi failed!");
        return RC_ERROR;
    }
    rc = voController_->StopVo();
    if (rc == RC_ERROR) {
        CAMERA_LOGE("stopvo failed!");
        return RC_ERROR;
    }
    return RC_OK;
}

REGISTERNODE(VoNode, {"vo"})
} // namespace OHOS::Camera
