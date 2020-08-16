
#ifdef __cplusplus
 extern "C" {
#endif
/**
  ******************************************************************************
  * @file           : app_x-cube-ai.c
  * @brief          : AI program body
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
#include <string.h>
#include "app_x-cube-ai.h"
#include "main.h"
//#include "constants_ai.h"
#include "ai_datatypes_defines.h"

/* USER CODE BEGIN includes */
/* USER CODE END includes */
/* USER CODE BEGIN initandrun */
#include <stdlib.h>

/* Global handle to reference the instance of the NN */
static ai_handle network_har = AI_HANDLE_NULL;
static ai_buffer ai_input[AI_NETWORK_HAR_IN_NUM] = AI_NETWORK_HAR_IN ;
static ai_buffer ai_output[AI_NETWORK_HAR_OUT_NUM] = AI_NETWORK_HAR_OUT ;

/*
 * Init function to create and initialize a NN.
 */
int aiInit(const ai_u8* activations)
{
    ai_error err;

    /* 1 - Specific AI data structure to provide the references of the
     * activation/working memory chunk and the weights/bias parameters */
    const ai_network_params params = {
            AI_NETWORK_HAR_DATA_WEIGHTS(ai_network_har_data_weights_get()),
            AI_NETWORK_HAR_DATA_ACTIVATIONS(activations)
    };

    /* 2 - Create an instance of the NN */
    err = ai_network_har_create(&network_har, AI_NETWORK_HAR_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
	    return -1;
    }

    /* 3 - Initialize the NN - Ready to be used */
    if (!ai_network_har_init(network_har, &params)) {
        err = ai_network_har_get_error(network_har);
        ai_network_har_destroy(network_har);
        network_har = AI_HANDLE_NULL;
	    return -2;
    }

    return 0;
}

/*
 * Run function to execute an inference.
 */
int aiRun(const void *in_data, void *out_data)
{
    ai_i32 nbatch;
    ai_error err;

    /* Parameters checking */
    if (!in_data || !out_data || !network_har)
        return -1;

    /* Initialize input/output buffer handlers */
    ai_input[0].n_batches = 1;
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].n_batches = 1;
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    /* 2 - Perform the inference */
    nbatch = ai_network_har_run(network_har, &ai_input[0], &ai_output[0]);
    if (nbatch != 1) {
        err = ai_network_har_get_error(network_har);
        // ...
        return err.code;
    }

    return 0;
}
/* USER CODE END initandrun */

/*************************************************************************
  *
  */
void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 0 */
    /* Activation/working buffer is allocated as a static memory chunk
     * (bss section) */
    AI_ALIGNED(4)
    static ai_u8 activations[AI_NETWORK_HAR_DATA_ACTIVATIONS_SIZE];

    aiInit(activations);
    /* USER CODE END 0 */
}

void MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 1 */
	int nb_run = 20;
    int res;

    /* Example of definition of the buffers to store the tensor input/output */
    /*  type is dependent of the expected format                             */
    AI_ALIGNED(4)
    static ai_i8 in_data[AI_NETWORK_HAR_IN_1_SIZE_BYTES];

    AI_ALIGNED(4)
    static ai_i8 out_data[AI_NETWORK_HAR_OUT_1_SIZE_BYTES];

    /* Retrieve format/type of the first input tensor - index 0 */
    const ai_buffer_format fmt_ = AI_BUFFER_FORMAT(&ai_input[0]);
    const uint32_t type_ = AI_BUFFER_FMT_GET_TYPE(fmt_);

    /* Prepare parameters for float to Qmn conversion */
    const ai_i16 N_ = AI_BUFFER_FMT_GET_FBITS(fmt_);
    const ai_float scale_ = (0x1U << N_);
    const ai_i16 M_ =  AI_BUFFER_FMT_GET_BITS(fmt_)
                       - AI_BUFFER_FMT_GET_SIGN(fmt_) - N_;
    const ai_float max_ = (ai_float)(0x1U << M_);

    /* Perform nb_rub inferences (batch = 1) */
    while (--nb_run) {

        /* ---------------------------------------- */
        /* Data generation and Pre-Process          */
        /* ---------------------------------------- */
        /* - fill the input buffer with random data */
        for (ai_size i=0;  i < AI_NETWORK_HAR_IN_1_SIZE; i++ ) {

            /* Generate random data in the range [-1, 1] */
            ai_float val = 2.0f * (ai_float)rand() / (ai_float)RAND_MAX - 1.0f;

            /* Convert the data if necessary */
            if (type_ == AI_BUFFER_FMT_TYPE_FLOAT) {
                ((ai_float *)in_data)[i] = val;
            } else { /* AI_BUFFER_FMT_TYPE_Q */
                /* Scale the values in the range [-2^M, 2^M] */
                val *= max_;
                /* Convert float to Qmn format */
                const ai_i32 tmp_ = AI_ROUND(val * scale_, ai_i32);
                in_data[i] =  AI_CLAMP(tmp_, -128, 127, ai_i8);
            }
        }

        /* Perform the inference */
        res = aiRun(in_data, out_data);
        if (res) {
            // ...
            return;
        }

        /* Post-Process - process the output buffer */
        // ...
    }
    /* USER CODE END 1 */
}
#ifdef __cplusplus
}
#endif
