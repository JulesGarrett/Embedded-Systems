//*****************************************************************************
//
//	Task_ReportSysTick.c - Display SysTickCount_Nbr.
//
//		Author: 		Gary J. Minden
//		Organization:	KU/EECS/EECS 388
//		Date:			2016-08-16 (B60816)
//		Version:		1.0
//
//		Purpose:		Example task that demonstrates:
//							(1) Formatting a string
//							(2) Write string to OLED
//
//		Notes:			Updated at KU and adapted from the TI LM3S1968 blinky
//						and other examples.
//
//		Date:			2016-08-16 (B60816)
//						Moved task code to separate files.
//						Changed structure of task code. See individual tasks.
//						Updated basic types.
//
//*****************************************************************************
//
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "driverlib/sysctl.h"
#include "Drivers/rit128x96x4.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

#include	"FreeRTOS.h"
#include	"task.h"

#include	<stdio.h>

//*****************************************************************************
//
//	External variables and subroutines.
//	(1) SysTick
//	(2) OLED initialization
//
//*****************************************************************************
//extern uint32_t		SysTickFrequency_Nbr;
extern uint32_t SysTickCount_Nbr;

uint8_t i;

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************

extern unsigned long int ulValue;
enum state {
	low_cycle, high_cycle
};
enum state PWMState=low_cycle;

extern void Task_Servo(void *pvParameters) {
	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD );
	SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM0 );
	GPIOPinTypeGPIOOutput( GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPadConfigSet( GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD );
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_32);
	PWMGenConfigure(PWM_BASE, PWM_GEN_0,
	PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 31250);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 1563);
	PWMOutputState(PWM_BASE, PWM_OUT_1_BIT , true);
	PWMGenEnable(PWM_BASE, PWM_GEN_0);



while(1){

	switch(PWMState){
	case low_cycle:
		PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 3123);
		PWMState=high_cycle;
		break;
	case high_cycle:
		PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 1563);
		PWMState=low_cycle;
		break;

	}
	vTaskDelay(10000);



}

}
