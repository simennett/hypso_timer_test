// GPIO General InoutOutput
#include <stdio.h>
#include "platform.h"
#include "xgpiops.h"
#include "stdbool.h"
#include "xgpio.h"

/*
#ifdef PRE_2_00A_APPLICATION


	//  The following macros are provided to allow an application to compile that
	//  uses an older version of the driver (pre 2.00a) which did not have a channel
	//  parameter. Note that the channel parameter is fixed as channel 1.

#define XGpio_SetDataDirection(InstancePtr, DirectionMask) \
        XGpio_SetDataDirection(InstancePtr, LED_CHANNEL, DirectionMask)

#define XGpio_DiscreteRead(InstancePtr) \
        XGpio_DiscreteRead(InstancePtr, LED_CHANNEL)

#define XGpio_DiscreteWrite(InstancePtr, Mask) \
        XGpio_DiscreteWrite(InstancePtr, LED_CHANNEL, Mask)

#define XGpio_DiscreteSet(InstancePtr, Mask) \
        XGpio_DiscreteSet(InstancePtr, LED_CHANNEL, Mask)

#endif
*/

/*
#define startButton 50  //PB1: pushbutton 1
#define stopButton 51  //PB2: pushbutton 2
#define ledpin 7
*/
#define LED_CHANNEL 1
#define GPIO_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
XGpio Gpio;
//XGpioPs_Config *GPIOConfigPtr;

// Timer
#include "xparameters.h"
#include "xstatus.h"
#include "xtmrctr_l.h"
#include "xil_printf.h"

#define TMRCTR_BASEADDR		XPAR_TMRCTR_0_BASEADDR
#define TIMER_COUNTER_0	 0




int main(void) {

  int Status;

	//GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);

	Status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		print("GPIO INIT FAILED\n\r");
		return XST_FAILURE;
	}
	/*
	XGpioPs_SetDirectionPin(&Gpio, ledpin, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, ledpin, 1);
	XGpioPs_SetDirectionPin(&Gpio, startButton, 0x0);
	XGpioPs_SetDirectionPin(&Gpio, stopButton, 0x0);
	*/
	XGpio_SetDataDirection(&Gpio, LED_CHANNEL, 0x00);

  //bool start;
  //bool stop;


  // Timer
  u32 ControlStatus;
  u32 ticks = 0;

  XTmrCtr_SetControlStatusReg(TMRCTR_BASEADDR, TIMER_COUNTER_0,0x0);

  XTmrCtr_SetLoadReg(TMRCTR_BASEADDR, TIMER_COUNTER_0, 0x0);
	XTmrCtr_LoadTimerCounterReg(TMRCTR_BASEADDR, TIMER_COUNTER_0);

  ControlStatus = XTmrCtr_GetControlStatusReg(TMRCTR_BASEADDR, TIMER_COUNTER_0);
  XTmrCtr_SetControlStatusReg(TMRCTR_BASEADDR, TIMER_COUNTER_0, ControlStatus & (~XTC_CSR_LOAD_MASK));


  //XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x00);
  while(1) {
    //start = XGpioPs_ReadPin(&Gpio,startButton);
    //stop = XGpioPs_ReadPin(&Gpio,stopButton);

	  XTmrCtr_Enable(TMRCTR_BASEADDR, TIMER_COUNTER_0);
	  ticks = XTmrCtr_GetTimerCounterReg(TMRCTR_BASEADDR, TIMER_COUNTER_0);
	  ticks = ticks/100000000;

	  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, ticks);

	  /*
	  if((ticks > 100) && (ticks < 200)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x01);
	  }
	  if((ticks > 200) && (ticks < 300)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x02);
	  }
	  if((ticks > 300) && (ticks < 400)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x03);
	  }
	  if((ticks > 400) && (ticks < 500)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x04);
	  }
	  if((ticks > 500) && (ticks < 600)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x05);
	  }
	  if((ticks > 700) && (ticks < 800)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x06);
	  }
	  if((ticks > 800) && (ticks < 900)){
		  XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, 0x07);
	  }
	  */
  }
  return 0;
}
