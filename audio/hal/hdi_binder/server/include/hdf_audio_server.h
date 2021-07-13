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
#ifndef HDF_AUDIO_SERVER_H
#define HDF_AUDIO_SERVER_H

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "hdf_log.h"
#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "hdf_remote_service.h"
#include "audio_internal.h"
#include "hdf_audio_server_render.h"
#include "hdf_audio_server_capture.h"

enum AudioHdiServerCmdId {
    /*************public*************/
    AUDIO_HDI_MGR_GET_FUNCS = 0,
    AUDIO_HDI_MGR_GET_ALL_ADAPTER,
    AUDIO_HDI_MGR_LOAD_ADAPTER,
    AUDIO_HDI_MGR_UNLOAD_ADAPTER,

    AUDIO_HDI_ADT_INIT_PORTS,
    AUDIO_HDI_ADT_GET_PORT_CAPABILITY,
    AUDIO_HDI_ADT_SET_PASS_MODE,
    AUDIO_HDI_ADT_GET_PASS_MODE,
    /***********render***************/
    AUDIO_HDI_RENDER_CREATE_RENDER,
    AUDIO_HDI_RENDER_DESTROY,
    AUDIO_HDI_RENDER_START,
    AUDIO_HDI_RENDER_STOP,
    AUDIO_HDI_RENDER_PAUSE,
    AUDIO_HDI_RENDER_RESUME,
    AUDIO_HDI_RENDER_FLUSH,
    AUDIO_HDI_RENDER_GET_FRAME_SIZE,
    AUDIO_HDI_RENDER_GET_FRAME_COUNT,
    AUDIO_HDI_RENDER_SET_SAMPLE_ATTR,
    AUDIO_HDI_RENDER_GET_SAMPLE_ATTR,
    AUDIO_HDI_RENDER_GET_CUR_CHANNEL_ID,
    AUDIO_HDI_RENDER_CHECK_SCENE_CAPABILITY,
    AUDIO_HDI_RENDER_SELECT_SCENE,
    AUDIO_HDI_RENDER_GET_MUTE,
    AUDIO_HDI_RENDER_SET_MUTE,
    AUDIO_HDI_RENDER_SET_VOLUME,
    AUDIO_HDI_RENDER_GET_VOLUME,
    AUDIO_HDI_RENDER_GET_GAIN_THRESHOLD,
    AUDIO_HDI_RENDER_GET_GAIN,
    AUDIO_HDI_RENDER_SET_GAIN,
    AUDIO_HDI_RENDER_GET_LATENCY,
    AUDIO_HDI_RENDER_RENDER_FRAME,
    AUDIO_HDI_RENDER_GET_RENDER_POSITION,
    AUDIO_HDI_RENDER_GET_SPEED,
    AUDIO_HDI_RENDER_SET_SPEED,
    AUDIO_HDI_RENDER_SET_CHANNEL_MODE,
    AUDIO_HDI_RENDER_GET_CHANNEL_MODE,
    /***********capture*************/
    AUDIO_HDI_CAPTURE_CREATE_CAPTURE,
    AUDIO_HDI_CAPTURE_DESTROY,
    AUDIO_HDI_CAPTURE_START,
    AUDIO_HDI_CAPTURE_STOP,
    AUDIO_HDI_CAPTURE_PAUSE,
    AUDIO_HDI_CAPTURE_RESUME,
    AUDIO_HDI_CAPTURE_FLUSH,
    AUDIO_HDI_CAPTURE_GET_FRAME_SIZE,
    AUDIO_HDI_CAPTURE_GET_FRAME_COUNT,
    AUDIO_HDI_CAPTURE_SET_SAMPLE_ATTR,
    AUDIO_HDI_CAPTURE_GET_SAMPLE_ATTR,
    AUDIO_HDI_CAPTURE_GET_CUR_CHANNEL_ID,
    AUDIO_HDI_CAPTURE_CHECK_SCENE_CAPABILITY,
    AUDIO_HDI_CAPTURE_SELECT_SCENE,
    AUDIO_HDI_CAPTURE_GET_MUTE,
    AUDIO_HDI_CAPTURE_SET_MUTE,
    AUDIO_HDI_CAPTURE_SET_VOLUME,
    AUDIO_HDI_CAPTURE_GET_VOLUME,
    AUDIO_HDI_CAPTURE_GET_GAIN_THRESHOLD,
    AUDIO_HDI_CAPTURE_GET_GAIN,
    AUDIO_HDI_CAPTURE_SET_GAIN,
    AUDIO_HDI_CAPTURE_CAPTURE_FRAME,
    AUDIO_HDI_CAPTURE_GET_CAPTURE_POSITION
};

typedef int32_t (*AudioAllfunc)(struct HdfDeviceIoClient *client, struct HdfSBuf *data, struct HdfSBuf *reply);
struct HdiServiceDispatchCmdHandleList {
    enum AudioHdiServerCmdId cmd;
    AudioAllfunc func;
};

#endif
