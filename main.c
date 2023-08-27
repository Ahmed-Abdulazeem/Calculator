


/**********************************************************************************************************************
 * Include Libraries
 *********************************************************************************************************************/
#include "4.LIB/Bit_Math.h"
#include "4.LIB/Platform_Types.h"
#include "4.LIB/Std_Types.h"
#include "4.LIB/TM4C123xx.h"
#include "4.LIB/hw_gpio.h"

#include "1.MCAL/GPIO/INC/GPIO_Interface.h"
#include "1.MCAL/EXTI/INC/EXTI_Interface.h"

#include "2-HAL/LCD/LCD.h"
#include "2-HAL/KeyPad/KeyPad.h"
#include "2-HAL/KeyPad/KeyPad.h"

#include "3-APP/Calculator/Calculator.h"

int main(void)
{

    LCD_vidInit();            /*Initialize LCD*/
    KeyPad_vidInit();         /*Initialize KEYPAD*/
    Calculator_vidProgram();  /*RUN Calculator*/


    return 0;
}
