/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FLASH_PAGE_F1.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

/* USER CODE BEGIN PV */
CAN_RxHeaderTypeDef rxHeader; //CAN Bus Transmit Header
CAN_TxHeaderTypeDef txHeader; //CAN Bus Receive Header
uint8_t canRX[8] = {0,0,0,0,0,0,0,0};  //CAN Bus Receive Buffer
CAN_FilterTypeDef canfil; //CAN Bus Filter
uint32_t canMailbox; //CAN Bus Mail box variable



int
rUp_rel1,
rUp_rel2,
rUp_rel3,
rUp_rel4,
rUp_rel5,
rUp_rel6,
rUp_cod = 0;

int state1,
state2,
state3,
state4,
state5,
state6 =0;

uint32_t coding[] = {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


uint32_t def_code[] = {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
void  checkInConfigMode(void);
void updateState(void);
void checkInput(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  Flash_Read_Data(0x0801FC00, coding, 21);

  if (coding[2] != 0) {

	  Flash_Write_Data(0x0801FC00, (uint32_t *)def_code, 22);

	  Flash_Read_Data(0x0801FC00, coding, 21);

 }

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
  canfil.FilterBank = 0;
  canfil.FilterMode = CAN_FILTERMODE_IDLIST;
  canfil.FilterScale = CAN_FILTERSCALE_16BIT;
  canfil.FilterFIFOAssignment = CAN_RX_FIFO0;
  canfil.FilterIdHigh = coding[1] << 5u;
  canfil.FilterIdLow = 0;
  canfil.FilterMaskIdHigh = 0;
  canfil.FilterMaskIdLow = 0;
  canfil.FilterActivation = ENABLE;


  txHeader.DLC = 8;
  txHeader.IDE = CAN_ID_STD;
  txHeader.RTR = CAN_RTR_DATA;
  txHeader.StdId = 0x030;
  txHeader.ExtId = 0x02;
  txHeader.TransmitGlobalTime = DISABLE;

  HAL_CAN_ConfigFilter(&hcan,&canfil);
  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);


 if (coding[0] == 1) {
	txHeader.DLC = 4;
	txHeader.StdId = coding[1];
	uint8_t csend[] = {0x05,0x02,0x06, coding[0]};
	HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
 }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  checkInConfigMode();
	  updateState();
	  checkInput();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 2;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(sysled_GPIO_Port, sysled_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, relay1_Pin, !coding[10]);
  HAL_GPIO_WritePin(GPIOA, relay2_Pin, !coding[11]);
  HAL_GPIO_WritePin(GPIOA, relay3_Pin, !coding[12]);
  HAL_GPIO_WritePin(GPIOA, relay4_Pin, !coding[13]);
  HAL_GPIO_WritePin(GPIOA, relay5_Pin, !coding[14]);
  HAL_GPIO_WritePin(GPIOA, relay6_Pin, !coding[15]);

  /*Configure GPIO pin : sysled_Pin */
  GPIO_InitStruct.Pin = sysled_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(sysled_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : inConf_Pin input1_Pin input2_Pin */
  GPIO_InitStruct.Pin = inConf_Pin|input1_Pin|input2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : relay1_Pin relay2_Pin relay3_Pin relay4_Pin
                           relay5_Pin relay6_Pin */
  GPIO_InitStruct.Pin = relay1_Pin|relay2_Pin|relay3_Pin|relay4_Pin
                          |relay5_Pin|relay6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : input3_Pin input4_Pin input5_Pin input6_Pin */
  GPIO_InitStruct.Pin = input3_Pin|input4_Pin|input5_Pin|input6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void checkInConfigMode() {
	  HAL_Delay(1);
	  if (coding[0] != 0) {
		  //in config mode
		  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
		  HAL_Delay(500);
		  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
		  HAL_Delay(500);
		  if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0) {
			  coding[0] = 0;
			  Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
			  while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0) {
				  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
				  HAL_Delay(100);
				  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
				  HAL_Delay(100);
			  }
			  canfil.FilterIdHigh = coding[1] << 5u;
			  HAL_CAN_ConfigFilter(&hcan,&canfil);
		  }
	  }else{
		  if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0) {
			  coding[0] = 1;
			  Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
			  txHeader.DLC = 3;
			  txHeader.StdId = coding[1];
			  uint8_t csend[] = {0x05,0x02,0x06};
			  HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			  while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0) {
				  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
				  HAL_Delay(100);
				  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
				  HAL_Delay(100);
			  }
		  }
	  }




}


void checkInput() {
	//INPUT 1 --------------------------------------------------------------------------------
	if (coding[16] == 1) {
	if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) {
		coding[10] = !coding[10];
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,!coding[10]);

		rUp_rel1=1;
		while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)) {

		}
	}
	}else if (coding[16]==2) {
		if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) {
			coding[10] = !coding[10];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,!coding[10]);

			rUp_rel1=1;
			coding[16]= 3;

			}
	}else if (coding[16]==3) {
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) {
			coding[10] = !coding[10];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,!coding[10]);

			rUp_rel1=1;

			coding[16]= 2;

			}
	}


	//INPUT 2 --------------------------------------------------------------------------------
	if (coding[17] == 1) {
	if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) {
		coding[11] = !coding[11];
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,!coding[11]);

		rUp_rel2=1;

		while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)) {

		}
	}
	}else if (coding[17]==2) {
		if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) {
			coding[11] = !coding[11];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,!coding[11]);

			rUp_rel2=1;
			coding[17]= 3;

			}
	}else if (coding[17]==3) {
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) {
			coding[11] = !coding[11];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,!coding[11]);

			rUp_rel2=1;
			coding[17]= 2;

			}
	}



	//INPUT 3 --------------------------------------------------------------------------------
	if (coding[18] == 1) {
	if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
		coding[12] = !coding[12];
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,!coding[12]);

		rUp_rel2=1;

		while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)) {

		}
	}
	}else if (coding[18]==2) {
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
			coding[12] = !coding[12];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,!coding[12]);

			rUp_rel3=1;
			coding[18]= 3;

			}
	}else if (coding[18]==3) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) {
			coding[12] = !coding[12];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,!coding[12]);

			rUp_rel3=1;

			coding[18]= 2;

			}
	}

	//INPUT 4 --------------------------------------------------------------------------------
	if (coding[19] == 1) {
	if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) {
		coding[13] = !coding[13];
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,!coding[13]);

		rUp_rel4=1;


		while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)) {

		}
	}
	}else if (coding[19]==2) {
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) {
			coding[13] = !coding[13];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,!coding[13]);

			rUp_rel4=1;

			coding[19]= 3;

			}
	}else if (coding[19]==3) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) {
			coding[13] = !coding[13];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,!coding[13]);

			rUp_rel4=1;
			coding[19]= 2;

			}
	}



	//INPUT 5 --------------------------------------------------------------------------------
	if (coding[20] == 1) {
	if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)) {
		coding[14] = !coding[14];
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,!coding[14]);

		rUp_rel5=1;

		while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)) {

		}
	}
	}else if (coding[20]==2) {
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)) {
			coding[14] = !coding[14];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,!coding[14]);

			rUp_rel5=1;
			coding[20]= 3;

			}
	}else if (coding[20]==3) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)) {
			coding[14] = !coding[14];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,!coding[14]);

			rUp_rel5=1;
			coding[20]= 2;

			}
	}


	//INPUT 6 --------------------------------------------------------------------------------
	if (coding[21] == 1) {
	if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) {
		coding[15] = !coding[15];
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,!coding[15]);

		rUp_rel6=1;

		while(!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)) {

		}
	}
	}else if (coding[21]==2) {
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) {
			coding[15] = !coding[15];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,!coding[15]);

			rUp_rel6=1;
			coding[21]= 3;

			}
	}else if (coding[21]==3) {
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)) {
			coding[15] = !coding[15];
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,!coding[15]);

			rUp_rel6=1;

			coding[21]= 2;

			}
	}


}


void updateState() {

	if (rUp_rel1) {
			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01,0x01,coding[10],0x00};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			rUp_rel1=0;
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
		}
		if (rUp_rel2) {
			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01,0x02,coding[11],0x00};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			rUp_rel2=0;
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
		}

		if (rUp_rel3) {
			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01,0x03,coding[12],0x00};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			rUp_rel3=0;
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
		}

		if (rUp_rel4) {
			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01,0x04,coding[13],0x00};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			rUp_rel4=0;
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
		}

		if (rUp_rel5) {
			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01,0x05,coding[14],0x00};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			rUp_rel5=0;
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
		}

		if (rUp_rel6) {
			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01,0x06,coding[15],0x00};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			rUp_rel6=0;
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
		}
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)

{

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,0);
	HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &rxHeader, canRX);
if (!coding[0]) {
	if (!coding[2]) {
	if (rxHeader.StdId == coding[1]) {
		if (canRX[0] == 0) {
			if (canRX[1] == 1) {
				if (canRX[2] == 0) {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
				coding[10] = 0;

				}else{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
				coding[10] = 1;

				}

				rUp_rel1=1;
				}
			else if (canRX[1] == 2) {
				if (canRX[2] == 0) {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
				coding[11] = 0;
				}else{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0);
				coding[11] = 1;
				}
				rUp_rel2=1;
				}
			else if (canRX[1] == 3) {
				if (canRX[2] == 0) {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,1);
				coding[12] = 0;
				}else{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,0);
				coding[12] = 1;
				}
				rUp_rel3=1;
				}

			else if (canRX[1] == 4) {
				if (canRX[2] == 0) {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,1);
				coding[13] = 0;
				}else{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,0);
				coding[13] = 1;
				}
				rUp_rel4=1;
				}
			else if (canRX[1] == 5) {
				if (canRX[2] == 0) {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1);
				coding[14] = 0;
				}else{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
				coding[14] = 1;
				}
				rUp_rel5=1;
				}
			else if (canRX[1] == 6) {
				if (canRX[2] == 0) {
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
				coding[15] = 0;
				}else{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
				coding[15] = 1;
				}
				rUp_rel6=1;
				}
			} else if (canRX[0] == 2) {
				 if (canRX[1] == 1) {
					 txHeader.DLC = 4;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01,0x01,coding[10],0x00};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	 }
				 else if (canRX[1] == 2) {
					 txHeader.DLC = 4;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01,0x02,coding[11],0x00};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	}
				 else if (canRX[1] == 3) {
					 txHeader.DLC = 4;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01,0x03,coding[12],0x00};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	}
				 else if (canRX[1] == 4) {
					 txHeader.DLC = 4;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01,0x04,coding[13],0x00};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	}
				 else if (canRX[1] == 5) {
					 txHeader.DLC = 4;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01,0x05,coding[14],0x00};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	}
				 else if (canRX[1] == 6) {
					 txHeader.DLC = 4;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01,0x06,coding[15],0x00};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	}
				 else if (canRX[1] == 0) {
					  if (canRX[2] == 0) {
					 txHeader.DLC = 8;
					 txHeader.StdId = coding[1];
					 uint8_t csend[] = {0x01, 0x00, coding[10],coding[11],coding[12],coding[13],coding[14], coding[15]};
					 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				 	}

				 }

			}
		}
	}
}else{
	if (rxHeader.StdId == coding[1]) {
		if (canRX[0] == 0) {

					txHeader.DLC = 4;
					txHeader.StdId = coding[1];
					uint8_t csend[] = {0x05,0x02,0x06, coding[0]};
					HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			}
		else if (canRX[0] == 1) {
				txHeader.DLC = 8;
				txHeader.StdId = coding[1];
				uint8_t csend[] = {0x0A,coding[0],coding[1],coding[2],coding[3],coding[4],coding[5], coding[6]};
				HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				// can message support max 8 byte so the coding i sent in pages. the first byte rappresents 1st page

		}else if (canRX[0] == 2) {
			txHeader.DLC = 8;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x0B,coding[7],coding[8],coding[9],coding[10],coding[11],coding[12], coding[13]};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			// can message support max 8 byte so the coding i sent in pages. the first byte rappresents 1st page

		}else if (canRX[0] == 3) {
			txHeader.DLC = 8;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x0C,coding[14],coding[15],coding[16],coding[17],coding[18],coding[19], coding[20]};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			// can message support max 8 byte so the coding i sent in pages. the first byte rappresents 1st page

		}else if (canRX[0] == 4) {
			txHeader.DLC = 2;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x0D,coding[21]};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			// can message support max 8 byte so the coding i sent in pages. the first byte rappresents 1st page

		}


		else if (canRX[0] == 7) {
			for (int i=0; i <7; i++) {
				coding[i] = canRX[i+1];
			}
			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
			 txHeader.DLC = 1;
			 txHeader.StdId = coding[1];
			 uint8_t csend[] = {0x01};
			 HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
			 canfil.FilterIdHigh = coding[1] << 5u;
			 HAL_CAN_ConfigFilter(&hcan,&canfil);
		}
		else if (canRX[0] == 8) {
			for (int i=0; i <8; i++) {
				coding[i+7] = canRX[i+1];
				}

			Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
			txHeader.DLC = 1;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x01};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);

				}
		else if (canRX[0] == 9) {
			for (int i=0; i <8; i++) {
				coding[i+14] = canRX[i+1];
				}

				Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
				txHeader.DLC = 1;
				txHeader.StdId = coding[1];
				uint8_t csend[] = {0x01};
				HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);

				}
		else if (canRX[0] == 16) {
			coding[21] = canRX[1];

				Flash_Write_Data(0x0801FC00 , (uint32_t *)coding, 22);
				txHeader.DLC = 1;
				txHeader.StdId = coding[1];
				uint8_t csend[] = {0x01};
				HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);

				}

	}
}
	if (rxHeader.StdId == 0x0) {
		if (coding[0] | coding[3] | coding[2]) {
		if (canRX[0] == 0) {

			txHeader.DLC = 4;
			txHeader.StdId = coding[1];
			uint8_t csend[] = {0x05,0x02,0x06, coding[0]};
			HAL_CAN_AddTxMessage(&hcan,&txHeader,csend,&canMailbox);
				}
			}
	}
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,1);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  NVIC_SystemReset();
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
