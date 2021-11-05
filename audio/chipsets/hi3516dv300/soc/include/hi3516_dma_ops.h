/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#ifndef HI3516_AUDIO_DMA_OPS_H
#define HI3516_AUDIO_DMA_OPS_H

#include "audio_core.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

int32_t AudioDmaDeviceInit(const struct AudioCard *card, const struct PlatformDevice *platform);
int32_t Hi3516DmaBufAlloc(struct PlatformData *data, enum AudioStreamType streamType);
int32_t Hi3516DmaBufFree(struct PlatformData *data, enum AudioStreamType streamType);
int32_t Hi3516DmaRequestChannel(struct PlatformData *data);
int32_t Hi3516DmaConfigChannel(struct PlatformData *data);
int32_t Hi3516DmaPrep(struct PlatformData *data);
int32_t Hi3516DmaSubmit(struct PlatformData *data);
int32_t Hi3516DmaPending(struct PlatformData *data);
int32_t Hi3516DmaPause(struct PlatformData *data);
int32_t Hi3516DmaResume(struct PlatformData *data);
int32_t Hi3516DmaPointer(struct PlatformData *data, uint32_t *pointer);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* HI3516_CODEC_OPS_H */