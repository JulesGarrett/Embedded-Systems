//*****************************************************************************
//
// Call assembly language subroutine
//
// Author: Gary J. Minden
// Organization: KU/EECS/EECS 388
// Date: 2014-04-18
// Version: 1.0
//
// Purpose: Call assembly language subroutine and check result
//
// Notes:
//
//*****************************************************************************
//
#include "drivers/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
//
// Make the Task and Assembly language subroutines external (global)
//
extern void Task_AsmCompute( void *pvParameters );
extern int AsmCompute( int I1, int I2, int I3, int I4 );
//*****************************************************************************
//
// Task initialization
void Task_AsmCompute( void *pvParameters ) {
//*****************************************************************************
//
// Constants and Variables
#define MaxRange 10
//
// Variables for testing assembly language subroutine
//
long int A, B, C, D;
long int CValue, AsmValue;
unsigned long int Delay_2000mS;
 Delay_2000mS = (2000 * configTICK_RATE_HZ ) / 1000;
 while ( 1 ) {
for ( A = 0; A < MaxRange; A++ ) {
for ( B = 0; B < MaxRange; B++ ) {
for ( C = 0; C < MaxRange; C++ ) {
for ( D = 0; D < MaxRange; D++ ) {
//
// You will replace the following statement with the
// algebraic expressions assigned to you.
//


CValue = (A+B/C)+(B&C^A+A/C*D);
//
// This is the invocation of your assembly language
// subroutine to compute the same algebraic expression
//
AsmValue = AsmCompute( A, B, C, D );
//
// Compare the C computation to the assembly language
// computation and report discrepancies.
//
if ( CValue != AsmValue ) {
UARTprintf( "Boo! CValue: %d; AsmValue: %d\n", CValue,
AsmValue );
}
}
}
}
//
// Log progress to the serial port
//
UARTprintf( "A: %d\n", A );
}
//
// Report final results to indicate progress and completion.
//
UARTprintf( "Last CValue, AsmValue: %d, %d\n", CValue, AsmValue );
//
// Delay for 2 seconds.
//
vTaskDelay( Delay_2000mS );
}
}
