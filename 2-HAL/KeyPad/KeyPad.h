/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Authors:  Ahmed Abdulazeem & Ahmed Ragab
 *         File:  KeyPad.h
 *        Layer:  HAL
 *       Module:  KeyPad
 *      Version:  1.00
 *
 *  Description:  Interfaces Needed by user
 *
 *********************************************************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../4.LIB/Bit_Math.h"
#include "../../4.LIB/Platform_Types.h"
#include "../../4.LIB/Std_Types.h"
#include "../../4.LIB/TM4C123xx.h"
#include "../../4.LIB/delay.h"
#include "../../1.MCAL/GPIO/INC/GPIO_Interface.h"
#include "../../1.MCAL/EXTI/INC/EXTI_Interface.h"
#include "../LCD/LCD.h"

/*column is an enum to indicate which column is active at the moment */
typedef enum
{
    col_0,col_1,col_2,col_3

}column;

/**********************************************************************************************************************
 * ROWS PINs Configurations (Port and Pin)
 *********************************************************************************************************************/
#define ROWS_PORT   GPIO_PORTB

#define ROW_0       GPIO_PIN0
#define ROW_1       GPIO_PIN1
#define ROW_2       GPIO_PIN2
#define ROW_3       GPIO_PIN3

/**********************************************************************************************************************
 * COLUMNS PINs Configurations (Port and Pin)
 *********************************************************************************************************************/
#define COLS_PORT   GPIO_PORTA

#define COL_0       GPIO_PIN2
#define COL_1       GPIO_PIN3
#define COL_2       GPIO_PIN4
#define COL_3       GPIO_PIN5
#define No_pressed_key  0xFF

/**********************************************************************************************************************
 *   DATA
 *********************************************************************************************************************/
column cur_col;



/***************************************************************************************************************************
 * \Syntax          : void KeyPad_vidInit(void)
 * \Description     : Initialize the KeyPad Pins
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : None
 ***************************************************************************************************************************/
void KeyPad_vidInit(void);

/***************************************************************************************************************************
 * \Syntax          : void scanButton(u8 colIndex)
 * \Description     : it sets current column index to LOW while reset columns are set to HIGH to detect falling Edge
 *                    by a specific column on a specific row
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : colIndex : which represents column to set it LOW while the rest of columns is HIGH
 * \Parameters (out): None
 * \Return value:   : None
 ***************************************************************************************************************************/
void scanButton(u8 colIndex);
/**********************************************************************************************************************
 *   CallBack Functions
 *********************************************************************************************************************/

/***************************************************************************************************************************
 * \Syntax          : void EXTIB0_CallBack(void)
 * \Description     : callback function that checks (scan) first row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB0_CallBack(void);

/***************************************************************************************************************************
 * \Syntax          : void EXTIB1_CallBack(void)
 * \Description     : callback function that checks (scan) second row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB1_CallBack(void);

/***************************************************************************************************************************
 * \Syntax          : void EXTIB2_CallBack(void)
 * \Description     : callback function that checks (scan) third row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB2_CallBack(void);

/***************************************************************************************************************************
 * \Syntax          : void EXTIB3_CallBack(void)
 * \Description     : callback function that checks (scan) forth row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB3_CallBack(void);

#endif /* 2_HAL_KEYPAD_KEYPAD_H_ */
