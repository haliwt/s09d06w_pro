/*
 * bsp_cmd_link.c
 *
 *  Created on: 2025年3月4日
 *      Author: Administrator
 */
#include "bsp.h"

#define MAX_BUFFER_SIZE  12

#define USART1_IT_FLAG   0

#define FRAME_HEADER 0x5A
#define FRAME_END 0xFE


//uint8_t  inputBuf[4];
uint8_t  inputCmd[30];
uint8_t  wifiInputBuf[1];


uint8_t rx_wifi_data[7];

uint8_t txComplete;

uint8_t rxBuffer[RX_DATA_SIZE];  // DMA接收缓冲区


static uint8_t transferSize;
uint8_t outputBuf[MAX_BUFFER_SIZE];
volatile uint8_t dataReceived = 0;  // 接收完成标志

#if 0
//volatile uint8_t transOngoingFlag;
//volatile uint8_t usart2_transOngoingFlag;

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
{

	//crc=0x55;
	outputBuf[0]=0x5A; //head : mainboard Board = 0x5A
	outputBuf[1]=0x10; //main board device No: 0x10
	outputBuf[2]=0x1A; //command : temperature of value 
	outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
	outputBuf[4]= 0x02; //data of length: 0x01 - 2 byte.
	outputBuf[5] =hum;
    outputBuf[6] =temp;

    outputBuf[7] = 0xFE;
    outputBuf[8] = bcc_check(outputBuf,8);
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;

	#if USART1_IT_FLAG 
	
		transferSize=9;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	#else 
		
	      HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);

    #endif 



}
/********************************************************************************
    **
    *Function Name:void SendWifiData_To_PanelTime(uint8_t hours,uint8_t minutes,uint8_t seconds)
    *Function :
    *Input Ref: hours,minutes,seconds of beijing time 
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_PanelTime(uint8_t hours,uint8_t minutes,uint8_t seconds)
{
    outputBuf[0]=0x5A; //mainboard head : displayBoard = 0xA5
	outputBuf[1]=0x10; //mainboard device No: 01
	outputBuf[2]=0x1C; //command : is data of hours and minutes and seconds.
	outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
	outputBuf[4]= 0x03; //data of length: 0x01 - 3 byte.  有3个数据
	outputBuf[5]= hours; //	
	outputBuf[6]= minutes; //	
	outputBuf[7]= seconds; //	

    outputBuf[8] = 0xFE;
    outputBuf[9] = bcc_check(outputBuf,9);

	#if USART1_IT_FLAG 

	transferSize=10;
	if(transferSize)
	{
	while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
	transOngoingFlag=1;
	HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #else 
		
	   HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);

    #endif 
}

/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Set_Command(uint8_t cmd,uint8_t data)
{
    outputBuf[0]=0x5A; //display board head = 0xA5
	outputBuf[1]= 0x10; //display device Number:is 0x01
	outputBuf[2]=cmd; // command type = 0x06 ->buzzer sound open or not
	outputBuf[3]= data; // command order -> 01 - buzzer sound done, 00- don't buzzer sound 
	outputBuf[4]=0x00; // data is length: 00 ->don't data 
	outputBuf[5]=0xFE; // frame of end code -> 0xFE.
    outputBuf[6] = bcc_check(outputBuf,6);

     #if USART1_IT_FLAG 
		transferSize=7;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	#else 
		
	      HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);


		#endif 
	
}
/********************************************************************************
    **
    *Function Name:void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
    *Function :
    *Input Ref: dat1- fan of grade value 
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
{
   
	    outputBuf[0]=0x5A; //head : displayBoard = 0xA5
        outputBuf[1]=0x10; //device No: 01
        outputBuf[2]=0x1E; //command type: fan speed of value 
        outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
        outputBuf[4]= 0x01; //data of length: 0x01 - 2 byte.
        outputBuf[5] =dat1;
    
    
        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf,7);
		
         #if USART1_IT_FLAG 
        transferSize=8;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
		#else 
		
	      HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);


		#endif 

}

/********************************************************************************
    **
    *Function Name:void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
{
        outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=cmd; //command type: fan speed of value 
        outputBuf[3]=data; // 0x0F : is data ,if don;t is 0x0F ->command order.
        outputBuf[4]= 0x0; // don't data ,onlay is command order,recieve data is 1byte .
       
        outputBuf[5] = 0xFE; //frame is end of byte.
        outputBuf[6] = bcc_check(outputBuf,6);
		
         #if USART1_IT_FLAG 
        transferSize=7;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
		#else 
		
	      HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);


		#endif 
		
	
}

/********************************************************************************
    **
    *Function Name:void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_One_Data(uint8_t cmd,uint8_t data)
{
        outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=cmd; //command type: fan speed of value 
        outputBuf[3]=0x0F; // 0x0F : is data ,don't command order.
        outputBuf[4]= 0x01; // don't data ,onlay is command order,recieve data is 1byte .
        outputBuf[5]= data; // don't data 
        
        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf,7);
		
        #if USART1_IT_FLAG 
        transferSize=8;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
		#else 
		
	      HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);


		#endif 
	
}

/********************************************************************************
    **
    *Function Name
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_Answer_Cmd(uint8_t cmd ,uint8_t data)
{
        outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=0xFF; // answer cmd :
        outputBuf[3]= cmd; // 0x0F : is data ,don't command order.
        outputBuf[4]= data; // don't data ,onlay is command order,recieve data is 1byte .
       
        outputBuf[5] = 0xFE; //frame is end of byte.
        outputBuf[6] = bcc_check(outputBuf,6);

		#if USART1_IT_FLAG 
        
        transferSize=7;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
	  #else 

     	HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);

	  #endif 
		
	
}
#else 
// 公共函数：填充帧数据
void FillFrame(uint8_t *buf, uint8_t cmd, uint8_t *data, uint8_t dataLen) 
{
    buf[0] = FRAME_HEADER;
    buf[1] = 0x10; // Mainboard device number
    buf[2] = cmd;
    buf[3] = (dataLen > 0) ? 0x0F : 0x00; // Data or command

	if(buf[3] ==0){
       buf[4] = data[0];
       buf[5] = FRAME_END;
	   buf[6] = bcc_check(buf, 6 );
	}
	else{
	      buf[4] = dataLen;
	
		   for (uint8_t i = 0; i < dataLen; i++) {
			   buf[5 + i] = data[i];
		   }
	
		   buf[5 + dataLen] = FRAME_END;
		   buf[6 + dataLen] = bcc_check(buf, 6 + dataLen);


	}
}

void FillFrame_Response(uint8_t *buf, uint8_t cmd, uint8_t *data, uint8_t dataLen) 
{
    buf[0] = FRAME_HEADER;          // 帧头
    buf[1] = 0x10;                  // 主板设备号
    buf[2] = 0xFF;                  // 应答信号标志
    buf[3] = cmd;                   // 命令类型
    buf[4] = (dataLen > 0) ? 0x0F : 0x00; // 数据标志：0x0F 表示有数据，0x00 表示无数据

    if (buf[4] == 0x00) {           // 无数据的情况
        buf[5] = data[0];           // 具体指令
        buf[6] = FRAME_END;         // 帧尾
        buf[7] = bcc_check(buf, 7); // 校验码
    } else {                        // 有数据的情况
        buf[5] = dataLen;           // 数据长度
        if (data != NULL) {         // 检查数据指针是否有效
            for (uint8_t i = 0; i < dataLen; i++) {
                buf[6 + i] = data[i]; // 填充数据
            }
        }
        buf[6 + dataLen] = FRAME_END;         // 帧尾
        buf[7 + dataLen] = bcc_check(buf, 7 + dataLen); // 校验码
    }
}



// 公共函数：发送数据
void TransmitData(uint8_t *buf, uint8_t size) 
{
    transferSize = size;

    #if USART1_IT_FLAG
    if (transferSize) {
        while (transOngoingFlag); // 等待传输完成
        transOngoingFlag = 1;
        HAL_UART_Transmit_IT(&huart1, buf, transferSize);
    }
    #else
    	HAL_UART_Transmit_DMA(&huart1, buf, transferSize);
    #endif
}

// 发送实时温湿度数据
void sendData_Real_TimeHum(uint8_t hum, uint8_t temp) 
{
    uint8_t data[2] = {hum, temp};
    FillFrame(outputBuf, 0x1A, data, 2);
    TransmitData(outputBuf, 9);
}

// 发送时间数据
void SendWifiData_To_PanelTime(uint8_t hours, uint8_t minutes, uint8_t seconds) 
{
    uint8_t data[3] = {hours, minutes, seconds};
    FillFrame(outputBuf, 0x1C, data, 3);
    TransmitData(outputBuf, 10);
}

// 发送命令数据
void SendData_Set_Command(uint8_t cmd, uint8_t data) 
{
    uint8_t cmdData[1] = {data};
    FillFrame(outputBuf, cmd, cmdData, 0);
    TransmitData(outputBuf, 7);
}

// 发送风速数据
void SendWifiData_To_PanelWindSpeed(uint8_t speed) {
    uint8_t data[1] = {speed};
    FillFrame(outputBuf, 0x1E, data, 1);
    TransmitData(outputBuf, 8);
}

// 发送命令响应
void SendWifiData_Answer_Cmd(uint8_t cmd, uint8_t cmdata) 
{
    uint8_t cmdData[1] = {cmdata};
    FillFrame_Response(outputBuf, cmd,cmdData ,0);
    TransmitData(outputBuf,8);
}
// smart phone send command
void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
{
	 uint8_t cmdData[1] = {data};
    FillFrame(outputBuf, cmd, cmdData, 0);
    TransmitData(outputBuf, 7);

}


/***********************************************************************
	*
	*Function Name:void SendWifiData_One_Data(uint8_t cmd,uint8_t data)
	*Function:
	*Input Ref: 1-cmd   2. data.
	*Return Ref:NO
	*
************************************************************************/
void SendWifiData_One_Data(uint8_t cmd,uint8_t data)
{
      #if 0
		outputBuf[0]=0x5A; //head : main board 0x5A
        outputBuf[1]=0x10; //main board device No: 0x10
        outputBuf[2]=cmd; //command type: fan speed of value 
        outputBuf[3]=0x0F; // 0x0F : is data ,don't command order.
        outputBuf[4]= 0x01; // don't data ,onlay is command order,recieve data is 1byte .
        outputBuf[5]= data; // don't data 
        
        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf,7);
		
        #if USART1_IT_FLAG 
        transferSize=8;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
		#else 
		
	      HAL_UART_Transmit_DMA(&huart1,outputBuf,transferSize);


		#endif 
	#endif 

	
	uint8_t cmdata[1] = {data};
	FillFrame(outputBuf, cmd, cmdata, 1);
	TransmitData(outputBuf, 8);

}

/***********************************************************************
	*
	*Function Name:void SendWifiData_One_Data(uint8_t cmd,uint8_t data)
	*Function:
	*Input Ref: 1-cmd   2. data.
	*Return Ref:NO
	*
************************************************************************/
void SendWifidata_Two_Data(uint8_t cmd,uint8_t datacmd)
{
   
    uint8_t cmdata[1] = {datacmd};
	FillFrame(outputBuf, cmd, cmdata, 2);
	TransmitData(outputBuf, 9);

}



#endif 

//void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void))
//{
//    EUSART_TxDefaultInterruptHandler = interruptHandler;
//}

/********************************************************************************
	**
	*Function Name:
	*Function :
	*Input Ref: 
	*Return Ref:NO
	*
*******************************************************************************/
void Start_DMA_Receive(void) 
{
    // 清空缓冲区
    memset(rxBuffer, 0, MAX_BUFFER_SIZE);
    dataReceived = 0;
    
    // 启动DMA接收
    HAL_UART_Receive_DMA(&huart1, rxBuffer,sizeof(rxBuffer));
}

/********************************************************************************
	**
	*Function Name:
	*Function :
	*Input Ref: 
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
        #if USART1_IT_FLAG 
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
		#else

		g_pro.DMA_txComplete = 1;//uartTxComplete = 1; // 标记发送完成

		#endif 
	}

//	if(huart== &huart2){
//
//       usart2_transOngoingFlag =0;
//
//	}

}

/**
  * @brief  UART错误回调函数，处理USART1通信错误
  * @param  huart: UART句柄指针
  */
#if 0
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) 
{
    

	if (huart->Instance == USART1) {
        // 重新初始化或报警
        #if 0
          __HAL_UART_CLEAR_OREFLAG(&huart1);
          __HAL_UART_CLEAR_NEFLAG(&huart1);
          __HAL_UART_CLEAR_FEFLAG(&huart1);
           
          
          temp=USART1->ISR;
          temp = USART1->RDR;
		  
     
		  UART_Start_Receive_IT(&huart1,inputBuf,1);
		 #endif 
	    /* 1. 清除所有可能出现的错误标志 */
	    // 使用单条语句清除多个标志（更高效）
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

	    /* 2. 读取状态和数据寄存器（清空残留数据）*/
	    // 使用UNUSED宏避免编译器警告（如果不需要实际值）
	    //UNUSED(uint32_t temp_isr = huart->Instance->ISR);  // 读取ISR会清除部分标志
	    //UNUSED(uint32_t temp_rdr = huart->Instance->RDR);  // 清空接收寄存器
	      /* 2. 清空寄存器（简洁写法）*/
		    (void)huart->Instance->ISR;  // 清除状态标志
		    (void)huart->Instance->RDR;  // 清空接收数据

	    /* 3. 重启接收（带错误检查）*/
	    if (HAL_UART_GetState(huart) == HAL_UART_STATE_READY) {
	         Start_DMA_Receive(); // HAL_UART_Receive_IT(huart, inputBuf, 1);  // 重新启动单字节中断接收
	    } else {
	        // 可选：硬件复位USART（严重错误时）
	        __HAL_UART_DISABLE(huart);
	        __HAL_UART_ENABLE(huart);
	        HAL_UART_Receive_IT(huart, inputBuf, 1);
	    }

	    /* 4. 可选：记录错误日志或触发报警 */
	    //Error_Counter++;  // 全局错误计数器
    }
	else if (huart->Instance == USART2){

		 /* 1. 清除所有可能出现的错误标志 */
	    // 使用单条语句清除多个标志（更高效）
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

	    /* 2. 读取状态和数据寄存器（清空残留数据）*/
	    // 使用UNUSED宏避免编译器警告（如果不需要实际值）
	    //UNUSED(uint32_t temp_isr = huart->Instance->ISR);  // 读取ISR会清除部分标志
	    //UNUSED(uint32_t temp_rdr = huart->Instance->RDR);  // 清空接收寄存器
		  /* 2. 清空寄存器（简洁写法）*/
		(void)huart->Instance->ISR;  // 清除状态标志
		(void)huart->Instance->RDR;  // 清空接收数据

		  /* 3. 重启接收（带错误检查）*/
	    if (HAL_UART_GetState(huart) == HAL_UART_STATE_READY) {
	          UART_Start_Receive_IT(&huart2,wifi_rx_inputBuf,1);// 重新启动单字节中断接收
	    }

	}
}
#endif 

