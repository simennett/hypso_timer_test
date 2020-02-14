// GPIO General InoutOutput
#include <stdio.h>
#include "platform.h"
#include "xgpiops.h"
#include "stdbool.h"

#define startButton 50  //PB1: pushbutton 1
#define stopButton 51  //PB2: pushbutton 2
#define ledpin 7

XGpioPs Gpio;
XGpioPs_Config *GPIOConfigPtr;

// Timer
#include "xparameters.h"
#include "xstatus.h"
#include "xtmrctr_l.h"
#include "xil_printf.h"

#define TMRCTR_BASEADDR		XPAR_TMRCTR_0_BASEADDR
#define TIMER_COUNTER_0	 0


int main(void) {

  int Status;

	GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);

	Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);
	if (Status != XST_SUCCESS) {
		print("GPIO INIT FAILED\n\r");
		return XST_FAILURE;
	}

	XGpioPs_SetDirectionPin(&Gpio, ledpin, 1);
  XGpioPs_SetOutputEnablePin(&Gpio, ledpin, 1);
	XGpioPs_SetDirectionPin(&Gpio, startButton, 0x0);
	XGpioPs_SetDirectionPin(&Gpio, stopButton, 0x0);

  bool start;
  bool stop;


  // Timer
  u32 ControlStatus;
  u32 ticks = 0;

  XTmrCtr_SetControlStatusReg(TMRCTR_BASEADDR, TIMER_COUNTER_0,0x0);

  XTmrCtr_SetLoadReg(TMRCTR_BASEADDR, TIMER_COUNTER_0, 0x0);
  XTmrCtr_LoadTimerCounterReg(TMRCTR_BASEADDR, TIMER_COUNTER_0);

  ControlStatus = XTmrCtr_GetControlStatusReg(TMRCTR_BASEADDR, TIMER_COUNTER_0);
  XTmrCtr_SetControlStatusReg(TMRCTR_BASEADDR, TIMER_COUNTER_0, ControlStatus & (~XTC_CSR_LOAD_MASK));

  while(1) {
    start = XGpioPs_ReadPin(&Gpio,startButton);
    stop = XGpioPs_ReadPin(&Gpio,stopButton);

    if(start) {
      XTmrCtr_Enable(TMRCTR_BASEADDR, TIMER_COUNTER_0);

      XGpioPs_WritePin(&Gpio, ledpin, 1);
    }
    if(stop) {
      ticks = XTmrCtr_GetTimerCounterReg(TMRCTR_BASEADDR, TIMER_COUNTER_0);
      XTmrCtr_Disable(TMRCTR_BASEADDR, TIMER_COUNTER_0);

      if((ticks > 50000000) && (ticks < 150000000)) {
        XGpioPs_WritePin(&Gpio, ledpin, 0);

      }
      xil_printf("Ticks: %d\r\n", ticks);

    }
  }
  return 0;
}
