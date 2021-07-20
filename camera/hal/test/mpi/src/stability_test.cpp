/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file expected in compliance with the License.
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

#include "stability_test.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void StabilityTest::SetUpTestCase(void) {}
void StabilityTest::TearDownTestCase(void) {}
void StabilityTest::SetUp(void)
{
    Test_ = std::make_shared<OHOS::Camera::Test>();
    Test_->Init();
}
void StabilityTest::TearDown(void)
{
    Test_->Close();

}

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera, 100 times.
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(StabilityTest, Camera_Stability_0001, TestSize.Level3)
{
    if (Test_->cameraDevice == nullptr) {
        std::cout << "==========[test log] Check Performance: OpenCamera, 100 times."<< std::endl;
        std::vector<int> FailTimes;
        Test_->service->GetCameraIds(Test_->cameraIds);
        Test_->deviceCallback = new CameraDeviceCallback();
        for (int i = 0; i < 100; i++) {
            Test_->rc =
                Test_->service->OpenCamera(Test_->cameraIds.front(), Test_->deviceCallback, Test_->cameraDevice);
            EXPECT_EQ(Test_->rc, Camera::NO_ERROR);
            EXPECT_EQ(true, Test_->cameraDevice != nullptr);
            if (Test_->rc != Camera::NO_ERROR) {
                FailTimes.push_back(i);
            }
        }
        std::cout << "Total fail times: "<< FailTimes.size() << ", at :" << std::endl;
        for (auto it = FailTimes.begin(); it != FailTimes.end(); ++it) {
            std::cout << *it << std::endl;
        }
    }
}

/**
  * @tc.name: Preview
  * @tc.desc: Preview for 100 times.
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(PerformanceTest, Camera_Performance_preview_0001, TestSize.Level2)
{
    std::cout << "==========[test log] Check Performance: Preview for 100 times." << std::endl;
    for (int i = 1; i < 101; i++) {
        std::cout << "==========[test log] Check Performance: Preview: " << i << " times. " << std::endl;
        // 打开相机
        Test_->Open();
        // 配置两路流信息
        Test_->intents = {Camera::PREVIEW};
        Test_->StartStream(Test_->intents);
        // 捕获预览流
        Test_->StartCapture(Test_->streamId_preview, Test_->captureId_preview, false, true);
        // 后处理
        Test_->captureIds = {Test_->captureId_preview};
        Test_->streamIds = {Test_->streamId_preview};
        Test_->StopStream(Test_->captureIds, Test_->streamIds);
    }
}

/**
  * @tc.name: preview and capture stream, for 100 times.
  * @tc.desc: Commit 2 streams together, Preview and still_capture streams, for 100 times.
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(PerformanceTest, Camera_Performance_capture_0002, TestSize.Level2)
{
    std::cout << "==========[test log] Commit 2 streams together, Preview and still_capture streams,";
    std::cout << " for 100 times." << std::endl;
    // 打开相机
    Test_->Open();
    // 配置两路流信息
    Test_->intents = {Camera::PREVIEW, Camera::STILL_CAPTURE};
    Test_->StartStream(Test_->intents);
    // 捕获预览流
    Test_->StartCapture(Test_->streamId_preview, Test_->captureId_preview, false, true);
    // 循环拍照流捕获100次
    for (int i = 1; i < 101; i ++) {
        Test_->StartCapture(Test_->streamId_capture, Test_->captureId_capture, false, false);
    }
    // 后处理
    Test_->captureIds = {Test_->captureId_preview};
    Test_->streamIds = {Test_->streamId_preview, Test_->streamId_capture};
    Test_->StopStream(Test_->captureIds, Test_->streamIds);
}

/**
  * @tc.name: Preview + Video stream, for 100 times.
  * @tc.desc: Preview + video, commit together, for 100 times.
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(PerformanceTest, Camera_Performance_video_0001, TestSize.Level2)
{
    std::cout << "==========[test log] Performance: Preview + video, commit together, 100 times." << std::endl;
    // 打开相机
    Test_->Open();
    for (int i = 1; i < 101; i++) {
        std::cout << "==========[test log] Performance: Preview + video, commit together, success." << std::endl;
        // 配置两路流信息
        Test_->intents = {Camera::PREVIEW, Camera::VIDEO};
        Test_->StartStream(Test_->intents);
        // 捕获预览流
        Test_->StartCapture(Test_->streamId_preview, Test_->captureId_preview, false, true);
        // 捕获录像流
        Test_->StartCapture(Test_->streamId_video, Test_->captureId_video, false, true);
        // 后处理
        Test_->captureIds = {Test_->captureId_preview, Test_->captureId_video};
        Test_->streamIds = {Test_->streamId_preview, Test_->streamId_video};
        Test_->StopStream(Test_->captureIds, Test_->streamIds);
    }
}

/**
  * @tc.name: set 3A 100 times
  * @tc.desc: set 3A 100 times, check result.
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(PerformanceTest, Camera_Performance_3a_0002, TestSize.Level2)
{
    std::cout << "==========[test log] Check Performance: Set 3A 100 times, check result." << std::endl;
    Test_->Open();
    // 下发3A参数
    std::shared_ptr<Camera::CameraSetting> meta = std::make_shared<Camera::CameraSetting>(100, 2000);
    std::vector<uint8_t> awbMode = {
        OHOS_CONTROL_AWB_MODE_OFF,
        OHOS_CONTROL_AWB_MODE_AUTO,
        OHOS_CONTROL_AWB_MODE_INCANDESCENT,
        OHOS_CONTROL_AWB_MODE_FLUORESCENT,
        OHOS_CONTROL_AWB_MODE_WARM_FLUORESCENT,
        OHOS_CONTROL_AWB_MODE_DAYLIGHT,
        OHOS_CONTROL_AWB_MODE_CLOUDY_DAYLIGHT,
        OHOS_CONTROL_AWB_MODE_TWILIGHT,
        OHOS_CONTROL_AWB_MODE_SHADE
    };
    for (int round = 0; round < 10; round ++) {
        for (int i = 0; i < awbMode.size(); i ++) {
            int times = (round * 10) + i + 1;
            std::cout << "==========[test log] Check Performance: Set 3A Times: " << times << std::endl;
            meta->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode.at(i), 1);
            std::cout << "==========[test log] UpdateSettings, awb mode :" << awbMode.at(i) << std::endl;
            Test_->rc = Test_->cameraDevice->UpdateSettings(meta);
            if (Test_->rc == Camera::NO_ERROR) {
                std::cout << "==========[test log] Check Performance: UpdateSettings success." << std::endl;
            } else {
                std::cout << "==========[test log] Check Performance: UpdateSettings fail, , at the " << (i+1);
                std::cout <<"times, RetCode is " << Test_->rc << std::endl;
            }
            sleep(2);
        }
    }
}

/**
  * @tc.name: flashlight
  * @tc.desc: Turn on and off the flashlight, for 1000 times.
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(PerformanceTest, Camera_Performance_flashlight_0001, TestSize.Level2)
{
    std::cout << "==========[test log]Performance: Turn on and off the flashlight, 1000 times." << std::endl;
    // 打开相机
    Test_->Open();
    // 循环打开、关闭手电筒
    bool status= true;
    for (int i = 0; i < Times; i++) {
        Test_->rc = Test_->service->SetFlashlight(Test_->cameraIds.front(), status);
        if (Test_->rc != Camera::NO_ERROR) {
            std::cout << "==========[test log] Check Performance: Flashlight turn on fail, at the " << (i+1);
            std::cout <<"times, RetCode is " << Test_->rc << std::endl;
        }
        status = false;
        Test_->rc = Test_->service->SetFlashlight(Test_->cameraIds.front(), status);
        if (Test_->rc != Camera::NO_ERROR) {
            std::cout << "==========[test log] Check Performance: Flashlight turn off fail, at the " <<(i+1);
            std::cout<<"times, RetCode is " << Test_->rc << std::endl;
        }
    }
}