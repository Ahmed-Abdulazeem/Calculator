/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Authors:  Ahmed Abdulazeem & Ahmed Ragab
 *         File:  KeyPad.c
 *        Layer:  HAL
 *       Module:  KeyPad
 *      Version:  1.00
 *
 *  Description:  Implementation of Calculator
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  Libraries includes
 *********************************************************************************************************************/

#include "KeyPad.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS Implementations
 *********************************************************************************************************************/

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
void KeyPad_vidInit(void)
{
    const GPIO_Config_t Config[8]={
                                   /*Set 4 DIO PINS as input and pull up (ROWS)*/
                                   {ROWS_PORT,ROW_0,GPIO_PIN_INPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_PULLUP,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {ROWS_PORT,ROW_1,GPIO_PIN_INPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_PULLUP,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {ROWS_PORT,ROW_2,GPIO_PIN_INPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_PULLUP,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {ROWS_PORT,ROW_3,GPIO_PIN_INPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_PULLUP,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},

                                   /*Set 4 DIO PINS as OUTPUT floating (COLS)*/
                                   {COLS_PORT,COL_0,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {COLS_PORT,COL_1,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {COLS_PORT,COL_2,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {COLS_PORT,COL_3,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},

    };

    /*Initialize KeyPad Pins */
    GPIO_Init(Config, 8);

    /*NVIC Enable Port B interrupts*/
    SET_BIT(NVIC_EN0_R,1);

    /*Set 4 DIO INPUT and PULL UP PINS To Allow interrupts with Falling Edge*/
    EXTI_Config_t EXTI_Con[4]={{EXTI_PORTB,EXTI_PIN0,EXTI_EDGE,EXTI_FALLING_EDGE},
                               {EXTI_PORTB,EXTI_PIN1,EXTI_EDGE,EXTI_FALLING_EDGE},
                               {EXTI_PORTB,EXTI_PIN2,EXTI_EDGE,EXTI_FALLING_EDGE},
                               {EXTI_PORTB,EXTI_PIN3,EXTI_EDGE,EXTI_FALLING_EDGE}
    };

    /*Initialize KeyPad Pins(ROWS) TO interact with falling edge and firing interrupts*/
    EXTI_Init(EXTI_Con, 4);

    /*SET High on the 4 Pins*/
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN2, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN3, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN4, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN5, GPIO_PIN_HIGH);

    /*Set callback function to specific PIN on specific PORT*/
    EXTI_SetCallBack(EXTI_PORTB,EXTI_PIN0, EXTIB0_CallBack);
    EXTI_SetCallBack(EXTI_PORTB,EXTI_PIN1, EXTIB1_CallBack);
    EXTI_SetCallBack(EXTI_PORTB,EXTI_PIN2, EXTIB2_CallBack);
    EXTI_SetCallBack(EXTI_PORTB,EXTI_PIN3, EXTIB3_CallBack);
}

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
void scanButton(u8 colIndex)
{
    /*switch columns options from column 1 to column 4 by colIndex*/
    switch(colIndex)
    {
    case col_0:
    /* Set Pin 2 (Column 1) to LOW to allow falling edge on one row to be detected by one column when the key is pressed */
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN2, GPIO_PIN_LOW);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN3, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN4, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN5, GPIO_PIN_HIGH);
    break;

    /* Set Pin 3 (Column 2) to LOW to allow falling edge on one row to be detected by one column when the key is pressed */
    case col_1:
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN2, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN3, GPIO_PIN_LOW);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN4, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN5, GPIO_PIN_HIGH);
    break;

    /* Set Pin 4 (Column 3) to LOW to allow falling edge on one row to be detected by one column when the key is pressed */
    case col_2:
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN2, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN3, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN4, GPIO_PIN_LOW);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN5, GPIO_PIN_HIGH);
    break;

    /* Set Pin 5 (Column 4) to LOW to allow falling edge on one row to be detected by one column when the key is pressed */
    case col_3:
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN2, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN3, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN4, GPIO_PIN_HIGH);
    GPIO_SetPinValue(GPIO_PORTA,GPIO_PIN5, GPIO_PIN_LOW);
    break;
    default:
        break;
    }
}
