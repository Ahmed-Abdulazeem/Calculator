/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Authors:  Ahmed Abdulazeem & Ahmed Ragab
 *         File:  Calculator.h
 *        Layer:  HAL
 *       Module:  Calculator
 *      Version:  1.00
 *
 *  Description:  Interfaces Needed by user
 *
 *********************************************************************************************************************/

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../4.LIB/Bit_Math.h"
#include "../../4.LIB/Platform_Types.h"
#include "../../4.LIB/Std_Types.h"
#include "../../4.LIB/TM4C123xx.h"
#include "../../4.LIB/hw_gpio.h"
#include "../../4.LIB/delay.h"
#include "../../1.MCAL/GPIO/INC/GPIO_Interface.h"
#include "../../1.MCAL/EXTI/INC/EXTI_Interface.h"
#include "../../2-HAL/LCD/LCD.h"
#include "../../2-HAL/KeyPad/KeyPad.h"


/**********************************************************************************************************************
 * Functions Prototype
 *********************************************************************************************************************/

/***************************************************************************************************************************
 * \Syntax          : void KeyPad_vidProgram(void)
 * \Description     : Calculator Program
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : None
 ***************************************************************************************************************************/
void Calculator_vidProgram(void);

/***************************************************************************************************************************
 * \Syntax          : f32 equationParse(void)
 * \Description     : used to Parse Calculator Equation
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
f32  equationParse(void);

/***************************************************************************************************************************
 * \Syntax          : f32  calcOperation(f32 num1,f32 op,f32 num2)
 * \Description     : used to calculate simple operation
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Reentrant
 * \Parameters (in) : num1 -> left operand ,
 *                    num2 -> right operand ,
 *                    op   ->operation to be performed
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
f32  calcOperation(f32 num1,f32 op,f32 num2);

/***************************************************************************************************************************
 * \Syntax          : void removeElement(u8 index)
 * \Description     : used to remove element from equation by it's index
 *                    and send it to last location
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : index -> the Element's index to be deleted
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void removeElement(u8 index);

/***************************************************************************************************************************
 * \Syntax          : void deleteEquation(void)
 * \Description     : used to delete an equation after detecting an error or after the user decision to clear screen
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : index -> the Element's index to be deleted
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void deleteEquation(void);




#endif /* 3_APP_CALCULATOR_CALCULATOR_H_ */
