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

#include "audio_adm_fuzzer_common.h"

using namespace HMOS::Audio;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    struct DevHandle *(*BindServiceCapture)(const char *) = nullptr;
    char resolvedPath[] = "//system/lib/libhdi_audio_interface_lib_capture.z.so";
    void *PtrHandle = dlopen(resolvedPath, RTLD_LAZY);
    if (PtrHandle == nullptr) {
        return HDF_FAILURE;
    }
    BindServiceCapture = (struct DevHandle *(*)(const char *))dlsym(PtrHandle, "AudioBindServiceCapture");
    if (BindServiceCapture == nullptr) {
        dlclose(PtrHandle);
        return HDF_FAILURE;
    }
    uint8_t *dataFuzz = const_cast<uint8_t *>(data);
    char *bindFuzz = reinterpret_cast<char *>(dataFuzz);
    BindServiceCapture(bindFuzz);
    dlclose(PtrHandle);
    return HDF_SUCCESS;
}