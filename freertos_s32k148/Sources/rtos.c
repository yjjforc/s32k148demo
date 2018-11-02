/**
  ******************************************************************************
  * File Name          : CanHandle.c
  * Description        : Code for 3-dragon   rtos 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* SDK includes. */
// #include "interrupt_manager.h"
// #include "clock_manager.h"
// #include "clockMan1.h"
// #include "pin_mux.h"


#include "Cpu.h"
#include "clockMan1.h"
#include "pin_mux.h"
#include "Send.h"
#include "dmaController1.h"
#include "delay.h"
#include "flashread.h"
#include "canCom1.h"
//#include "lpuart_hw_access.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#if CPU_INIT_CONFIG
#include "Init_Config.h"
#endif
//persional include
#include "Flash_Handle.h"
#include "FlexCanTxRx.h"
#include "S32K_Interrupt_Callback.h"
#include "Application.h"
#include "MyPNVIC_Config.h"
#include "CanHandle.h"
#include "ShowModule.h"
//#include "w25qxx.h"
//#include "Application.h"
//volatile int exit_code = 0;

#define welcomeMsg "welicome the JLRX Board!\r\n"
#define testMsg    "test queue!\r\n"
#define timerMsg  "timerstart!\r\n"
#define FPGA_PORT       PTD
#define FPGA_EN            5U



#include "JL_Data_Structure.h"
Head AHead; //实例化管理结构体


uint8_t rdata=0;
uint32_t bytesRemaining;
//FTM 
ftm_state_t LEDftmStateStruct;
//CAN
flexcan_msgbuff_t rxDataBuff;
//SPI
lpspi_state_t sendmasterState;
lpspi_state_t flashreadrState;











/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY +3 )
//#define	mainCircle_Show_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
/* The rate at which data is sent to the queue, specified in milliseconds, and
converted to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 200 / portTICK_PERIOD_MS )

/* The LED will remain on until the button has not been pushed for a full
5000ms. */
#define mainBUTTON_LED_TIMER_PERIOD_MS		( 5000UL / portTICK_PERIOD_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/* The LED toggle by the queue receive task (blue). */
#define mainTASK_CONTROLLED_LED				( 1UL << 0UL )

/* The LED turned on by the button interrupt, and turned off by the LED timer
(green). */
#define mainTIMER_CONTROLLED_LED			( 1UL << 1UL )

/* The vector used by the GPIO port C.  Button SW7 is configured to generate
an interrupt on this port. */
#define mainGPIO_C_VECTOR					( 61 )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK						( 0UL )

/*-----------------------------------------------------------*/

/*
 * Setup the NVIC, LED outputs, and button inputs.
 */
static void prvSetupHardware( void );

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );
static void prvCircleShowTask( void *pvParameters );
/*
 * The LED timer callback function.  This does nothing but switch off the
 * LED defined by the mainTIMER_CONTROLLED_LED constant.
 */
//static void prvButtonLEDTimerCallback( TimerHandle_t xTimer );


/*
 * 初始化底层接口，个人配置
 * 
 */
static void Common_Init(void);
static void Peripherals_Init(void);
static void NVIC_Init(void);

CRece_BUFF CRBuffer;
unsigned long ulValueToSend=0xffffffff;
/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
QueueHandle_t TxQueue = NULL;
QueueHandle_t RxQueue = NULL;
/* The LED software timer.  This uses prvButtonLEDTimerCallback() as its callback
function. */
//static TimerHandle_t xButtonLEDTimer = NULL;

/*-----------------------------------------------------------*/










void rtos_start( void )
{
	/* Configure the NVIC, LED outputs and button inputs. */
	prvSetupHardware();
  

  
	/* Create the queue. */
	TxQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );
  	RxQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );
	if( TxQueue != NULL && RxQueue !=NULL )
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */

		xTaskCreate( prvQueueReceiveTask, "ViewModule", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
		xTaskCreate( prvQueueSendTask, "CANModule", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
//    xTaskCreate( prvCircleShowTask, "CircleShow", configMINIMAL_STACK_SIZE, NULL, mainCircle_Show_TASK_PRIORITY, NULL );

		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; )
  {
    Log("Queue malloc default\r\n");
  }
}



static void prvQueueSendTask( void *pvParameters )
{
  unsigned long ulReceivedValue;

// unsigned long ulValueToSend = (uint32_t)&tsturuct.test1;

	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

   #include "LCD_Basic_Elements.h"

  
//  while(1)
//  {
//    //LCD_Draw_Circle(100,100,50,16,0xFF00FF00);
//    LCD_DrawLine(36,155,164,80,16,32,0xff00ff00);
//    LCD_DrawLine(164,80,226,80,16,32,0xff00ff00);
////    LCD_DrawLine(36,180,36,155,8,4,0xff000000);
////    LCD_DrawLine(68,165,68,138,8,4,0xff000000);
//    delay_ms(100);
//  }
  
  AHead.ShowLayer=1;
  show_init(AHead.ShowLayer);//初始化界面1
  


  
  

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, the constant used converts ticks
		to ms.  While in the Blocked state this task will not consume any CPU
		time. */
		xQueueReceive( RxQueue, &ulReceivedValue, portMAX_DELAY );
    

	    //将收到的CAN数据传入实例化缓冲数组
	    if(ulReceivedValue!=0xffffffff)
	    {  
        show_Layer(AHead.ShowLayer,ulReceivedValue);      
	    }
    
	}
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
  unsigned long ulCANRecedValue;
  
	/* Casting pvParameters to void because it is unused */
	(void)pvParameters;

	for( ;; )
	{
		xQueueReceive( TxQueue, &ulCANRecedValue, portMAX_DELAY );

		if( ulCANRecedValue == (uint32_t)&CRBuffer.recemsg1 )
		{
		    Log("the msg_id = %d\r\n",CRBuffer.recemsg1.msgId);
        CANRec_Layer(AHead.ShowLayer);

		}
    	else if( ulCANRecedValue == (uint32_t)&CRBuffer.recemsg2 )
		{
		    Log("recemsg2\r\n");
        CANRec_Layer(AHead.ShowLayer);
		}
    	else if( ulCANRecedValue == (uint32_t)&CRBuffer.recemsg3 )
		{
		    Log("recemsg3\r\n");
        CANRec_Layer(AHead.ShowLayer);
		}
   		else
    	{
        	Log("error\r\n");
    	}
    	ulValueToSend=0xffffffff;
	}
}
static void prvCircleShowTask( void *pvParameters )
{
  //此任务为循环显示任务
  //难点在于 公用一个spi发送函数容易导死机
  //检测队列是否空闲 来执行操作
  for(;;)
  {
    
  }
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	Common_Init();//时钟 及引脚配置初始化
	Peripherals_Init();//基本外设初始化
  	NVIC_Init();
	//串口发送欢迎
	LPUART_DRV_SendData(INST_LPUART1, (uint8_t *)welcomeMsg, strlen(welcomeMsg));
	//等待发送完成
	while(LPUART_DRV_GetTransmitStatus(INST_LPUART1, &bytesRemaining) != STATUS_SUCCESS);
  
	//初始化FLASH 读Flash ID
	Flash_Handle_Init();
	//应用层初始化
	APP_Init();
}

/*-----------------------------------------------------------------*/

/*时钟 及引脚配置初始化*/ 
void Common_Init(void)
{
	//初始化时钟
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
									g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);
	//初始化引脚
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
}
/*-----------------------------------------------------------------*/

/*基本外设初始化*/
void Peripherals_Init(void)
{
	//初始化滴答定时器
	delay_init(112);
	//初始化PWM 背光


	//初始化io口输出 开启FPGA上拉,使能flash CAN收发器拉低
	PINS_DRV_SetPinsDirection(FPGA_PORT, (1 << FPGA_EN) );
	//PINS_DRV_SetPinsDirection(PTB, (1 << 17) );
  	PINS_DRV_SetPinsDirection(PTA, (1 << 6) );
  
	PINS_DRV_SetPinsDirection(PTE, (1 << 10) );
	PINS_DRV_SetPins(FPGA_PORT, 1 << FPGA_EN);
	//PINS_DRV_SetPins(PTB, 1 << 17);
	PINS_DRV_SetPins(PTA, 1 << 6);  
	PINS_DRV_ClearPins(PTE, 1 << 10);
  
  	FTM_DRV_Init(INST_LED_PWM, &LED_PWM_InitConfig, &LEDftmStateStruct);
	FTM_DRV_InitPwm(INST_LED_PWM, &LED_PWM_PwmConfig);
	FTM_DRV_UpdatePwmChannel(INST_LED_PWM, 1U, FTM_PWM_UPDATE_IN_DUTY_CYCLE, 0x1000, 0x5000, true);

	//初始化 UART 发送 接收
  	LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);
  	LPUART_DRV_ReceiveData(INST_LPUART1, &rdata, 1UL);
  	LPUART_DRV_InstallRxCallback(INST_LPUART1,  uart_isr,NULL);
	//初始化DMA 配置为SPI输出DMA
	EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);
	//初始化 send->FPGA spi
	LPSPI_DRV_MasterInit(SEND, &sendmasterState, &Send_MasterConfig0);
	//初始化 读写flash spi  
	LPSPI_DRV_MasterInit(FLASHREAD, &flashreadrState, &flashread_MasterConfig0);
  	LPSPI_DRV_MasterSetDelay(SEND, 1, 1, 1);
  	FlexCan_Init();
}

void NVIC_Init(void)
{

  S32K_NVIC_SetPriority(DMA0_IRQn,6);
  S32K_NVIC_EnableIRQ(DMA0_IRQn);
  S32K_NVIC_SetPriority(LPSPI1_IRQn,6);
  S32K_NVIC_EnableIRQ(LPSPI1_IRQn);  

  S32K_NVIC_SetPriority(CAN0_ORed_0_15_MB_IRQn,6);
  S32K_NVIC_EnableIRQ(CAN0_ORed_0_15_MB_IRQn);     
  
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    volatile size_t xFreeHeapSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	if( xFreeHeapSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}

}
/*-----------------------------------------------------------*/

/* The Blinky build configuration does not include run time stats gathering,
however, the Full and Blinky build configurations share a FreeRTOSConfig.h
file.  Therefore, dummy run time stats functions need to be defined to keep the
linker happy. */
void vMainConfigureTimerForRunTimeStats( void ) {}
unsigned long ulMainGetRunTimeCounterValue( void ) { return 0UL; }

/* A tick hook is used by the "Full" build configuration.  The Full and blinky
build configurations share a FreeRTOSConfig.h header file, so this simple build
configuration also has to define a tick hook - even though it does not actually
use it for anything. */
void vApplicationTickHook( void ) {}

/*-----------------------------------------------------------*/
