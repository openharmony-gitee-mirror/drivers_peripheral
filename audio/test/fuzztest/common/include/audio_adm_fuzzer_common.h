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

/*
 * @addtogroup Audio
 * @{
 *
 * @brief Test audio-related APIs, including custom data types and functions for loading drivers,
 * accessing a driver ADM interface lib.
 *
 * @since 1.0
 * @version 1.0
 */

/*
 * @file audio_adm_fuzzer_common.h
 *
 * @brief Declares APIs for operations related to the audio ADM interface lib.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AUDIO_ADM_FUZZER_COMMON_H
#define AUDIO_ADM_FUZZER_COMMON_H

#include "audio_hdi_fuzzer_common.h"

namespace HMOS {
namespace Audio {
const string BIND_CONTROL = "control";
const string BIND_RENDER = "render";
const string BIND_CAPTURE = "capture";
const string ADAPTER_NAME_USB = "usb";
const string ADAPTER_NAME_INTERNAL = "internal";
const int G_PERIODSIZE = 4096;
const int G_PERIODCOUNT = 8;
const int G_BYTERATE = 48000;
const int G_BUFFERFRAMESIZE = 0;
const int G_BUFFERSIZE1 = 128;
const int G_SILENCETHRESHOLE = 0;
const int G_PORTID = 0;
const int STOP_THRESHOLD = 32;
const int START_THRESHOLD = 8;

int32_t InitRenderFramepara(struct AudioFrameRenderMode& frameRenderMode);

int32_t InitHwCaptureFramepara(struct AudioFrameCaptureMode& frameCaptureMode);

int32_t InitHwRenderMode(struct AudioHwRenderMode& renderMode);

int32_t InitHwCaptureMode(struct AudioHwCaptureMode& captureMode);

int32_t InitHwRender(struct AudioHwRender *&hwRender);

int32_t InitHwCapture(struct AudioHwCapture *&hwCapture);

int32_t BindServiceAndHwRender(struct AudioHwRender *&hwRender);

int32_t BindServiceAndHwCapture(struct AudioHwCapture *&hwCapture);
}
}
#endif // AUDIO_LIB_COMMON_H

