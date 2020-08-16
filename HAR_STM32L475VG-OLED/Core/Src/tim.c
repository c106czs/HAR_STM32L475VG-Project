/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#include "app_x-cube-ai.h"
/* USER CODE END 0 */

TIM_HandleTypeDef htim6;

/* TIM6 init function */
void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 39999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 19;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspInit 0 */

  /* USER CODE END TIM6_MspInit 0 */
    /* TIM6 clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* TIM6 interrupt Init */
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN TIM6_MspInit 1 */

  /* USER CODE END TIM6_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspDeInit 0 */

  /* USER CODE END TIM6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM6_CLK_DISABLE();

    /* TIM6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN TIM6_MspDeInit 1 */

  /* USER CODE END TIM6_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
int8_t max_index(float *A,int8_t num)
{
	int8_t max_i=0,i;
	for(i=0;i<num;i++)
	{
		if(A[i]>A[max_i])
		{
			max_i=i;
		}
	}
	return max_i;
}
float pDataXYZ[3] = {0};
float input[200][6];
int16_t count = 0;
float result[12];
int8_t shownum=0;
int16_t total=0;

//定时器中断回调函�?
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == (&htim6))
	{
		// 每隔 1/20 秒采集一次数据，采集�?200个数据才放入神经网络进行运动的预�?
		//读取加�?�度，单位是mg�?1000倍的重力加�?�度g），下面�? 转成m/s^2，国际单位制的加速度单位，与训练用的数据集单位一�?
		BSP_ACCELERO_AccGetXYZ(pDataXYZ);
    input[count][0]=pDataXYZ[0]/1000.0f;
		input[count][1]=pDataXYZ[1]/1000.0f;
		input[count][2]=pDataXYZ[2]/1000.0f;
		//读取�?螺仪的角速度，单位是mdps，下面要除以100转成dps，也就是常见的角速度单位deg/s，与训练用的数据集单位一�?
		BSP_GYRO_GetXYZ(pDataXYZ);
		input[count][3]=pDataXYZ[0]/1000.0f;
		input[count][4]=pDataXYZ[1]/1000.0f;
		input[count][5]=pDataXYZ[2]/1000.0f;
		//printf("Accel=(%f, %f, %f)\r\n",input[count][0],input[count][1],input[count][2]);
		//printf("Gyro=(%f, %f, %f)\r\n",input[count][3],input[count][4],input[count][5]);
		count++;
		if(count>=200)
		{
			aiRun(input,result);
			count = 0;
			/*
			printf("result = ");
			for (int8_t i=0;i<12;i++)
			{
				printf("%f, ",result[i]);
			}
			printf("\r\n");
			*/
			res_act=max_index(result,12);
			printf("%s\r\n",activity[res_act]);
			OLED_ClearRow(shownum);// 
			OLED_ShowNum(0,shownum,total,3,12);
			OLED_ShowString(30,shownum,activity[res_act],12);
			shownum+=2;
			total+=1;
			if(shownum>=8)
			{
				shownum=0;
			}
		}
	}
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
