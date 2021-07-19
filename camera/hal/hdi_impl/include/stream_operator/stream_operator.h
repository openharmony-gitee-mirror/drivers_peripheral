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

#ifndef STREAM_OPERATOR_STREAM_OPERATOR_H
#define STREAM_OPERATOR_STREAM_OPERATOR_H

#include "istream_operator.h"
#include "stream_base.h"
#include <surface.h>
#include "stream_operator_service_stub.h"

namespace OHOS::Camera {
class CameraDevice;
class BufferClientProducer;
class StreamOperator : public StreamOperatorStub {
public:
    static std::shared_ptr<StreamOperator> CreateStreamOperator(
        const std::shared_ptr<IStreamOperatorCallback> &callback,
        const std::shared_ptr<CameraDevice> &device);

public:
    StreamOperator() = default;
    virtual ~StreamOperator() = default;
    StreamOperator(const StreamOperator &other) = delete;
    StreamOperator(StreamOperator &&other) = delete;
    StreamOperator& operator=(const StreamOperator &other) = delete;
    StreamOperator& operator=(StreamOperator &&other) = delete;
};
} // end namespace OHOS::Camera
#endif // STREAM_OPERATOR_STREAM_OPERATOR_H
