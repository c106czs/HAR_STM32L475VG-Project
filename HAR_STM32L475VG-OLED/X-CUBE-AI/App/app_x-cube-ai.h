/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_AI_H
#define __APP_AI_H
#ifdef __cplusplus
 extern "C" {
#endif
/**
  ******************************************************************************
  * @file           : app_x-cube-ai.h
  * @brief          : AI entry function definitions
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ai_platform.h"
#include "har_network.h"
#include "har_network_data.h"

#define MIN_HEAP_SIZE 0x200
#define MIN_STACK_SIZE 0x400

#define AI_MNETWORK_IN_1_SIZE_BYTES 4800
#define AI_MNETWORK_IN_NUM 1
#define DEF_DATA_IN \
  AI_ALIGNED(4) ai_i8 data_in_1[AI_MNETWORK_IN_1_SIZE_BYTES]; \
  static ai_i8* data_ins[] = { \
    data_in_1 \
  }; \

#define AI_MNETWORK_OUT_1_SIZE_BYTES 48
#define AI_MNETWORK_OUT_NUM 1
#define DEF_DATA_OUT \
  AI_ALIGNED(4) ai_i8 data_out_1[AI_MNETWORK_OUT_1_SIZE_BYTES]; \
  static ai_i8* data_outs[] = { \
    data_out_1 \
  }; \

#define AI_HAR_NETWORK_DATA_ACTIVATIONS_START_ADDR 0xFFFFFFFF

#define AI_MNETWORK_DATA_ACTIVATIONS_INT_SIZE AI_HAR_NETWORK_DATA_ACTIVATIONS_SIZE

void MX_X_CUBE_AI_Init(void);
void MX_X_CUBE_AI_Process(void);
int aiInit(const ai_u8* activations);
int aiRun(const void *in_data, void *out_data);

/* Helper macro */
#define AI_MIN(x_, y_) \
  ( ((x_)<(y_)) ? (x_) : (y_) )

#define AI_MAX(x_, y_) \
  ( ((x_)>(y_)) ? (x_) : (y_) )

#define AI_CLAMP(x_, min_, max_, type_) \
  (type_) (AI_MIN(AI_MAX(x_, min_), max_))

#define AI_ROUND(v_, type_) \
  (type_) ( ((v_)<0) ? ((v_)-0.5f) : ((v_)+0.5f) )
  
/* USER CODE BEGIN includes */
/* USER CODE END includes */
#define AI_MMETWORK_IN                AI_HAR_NETWORK_IN
#define AI_MNETWORK_OUT               AI_HAR_NETWORK_OUT

#define AI_MNETWORK_NUMBER  (1)

#ifdef __cplusplus
}
#endif

#endif /*__STMicroelectronics_X-CUBE-AI_5_0_0_H */
