
/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Ahmed Abdulazeem & Ahmed Ragab
 *         File:  LCD.c
 *        Layer:  HAL
 *       Module:  LCD 16x2
 *      Version:  1.00
 *
 *  Description:  Implementation of LCD driver
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../4.LIB/Bit_Math.h"
#include "../../4.LIB/Platform_Types.h"
#include "../../4.LIB/Std_Types.h"
#include "../../4.LIB/TM4C123xx.h"
#include "../../4.LIB/delay.h"

#include "../../1.MCAL/GPIO/INC/GPIO_Interface.h"

#include "LCD.h"

/*array of GPIO_RegDef_t and each element represents a GPIO base address*/
static GPIO_RegDef_t * GPIO_Arr[6]={(GPIO_RegDef_t *)GPIOA, (GPIO_RegDef_t *)GPIOB, (GPIO_RegDef_t *)GPIOC, (GPIO_RegDef_t *)GPIOD,(GPIO_RegDef_t *)GPIOE,(GPIO_RegDef_t *)GPIOF};

/*To count number of printing to go new line after first Line is completed*/
u8 counter;
/**********************************************************************************************************************
 * Functions Implementation
 *********************************************************************************************************************/

/***************************************************************************************************************************
 * \Syntax          : void LCD_vidInit(void)
 * \Description     : initialize the lcd
 * \Sync\Async      : Synchronous
 * \Parameters      : NONE
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidInit(void)
{
    /*SET THE CONFIGRATION FOR THE GPIO PINS THAN WILL BE CONNECTED TO THE LCD*/
    const GPIO_Config_t Config[7]={
                                   /*Set 4 DIO PINS as Data PINS*/
                                   {LCD_DATA_PORT,LCD_D0,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {LCD_DATA_PORT,LCD_D1,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {LCD_DATA_PORT,LCD_D2,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {LCD_DATA_PORT,LCD_D3,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},


                                   /*Set 3 DIO PINS as Control PINS*/
                                   {LCD_CONTROL_PORT,LCD_EN,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {LCD_CONTROL_PORT,LCD_RW,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0},
                                   {LCD_CONTROL_PORT,LCD_RS,GPIO_PIN_OUTPUT,GPIO_PIN_DIGITAL,GPIO_PIN_PUSHPULL,GPIO_PIN_FLOATING,GPIO_2MA_DRIVE,GPIO_ALTFUNC0}
    };

    /*Initialize KeyPad Pins */
    GPIO_Init(Config, 7U);

    delay_ms(40U);
    CLR_BIT(GPIO_Arr[LCD_DATA_PORT]->DATA[4U],LCD_RW);

    LCD_vidSendNibbleCMD(0x2U); /*2 Line, 5*7 matrix in 4-bit mode*/
    LCD_vidSendNibbleCMD(0x8U);

    LCD_vidSendNibbleCMD(0x0U); /*Display on cursor off*/
    LCD_vidSendNibbleCMD(0xFU); /*Increment cursor */

    LCD_vidSendNibbleCMD(0x0U);
    LCD_vidSendNibbleCMD(0x6U);

    LCD_vidSendNibbleCMD(0x0U);/*clear the lcd*/
    LCD_vidSendNibbleCMD(0x1U);
}


/***************************************************************************************************************************
 * \Syntax          : void LCD_vidSendNibbleCMD(u8 u8NibbleCopy)

 * \Description     : send nibble in the command mode
 * \Sync\Async      : Synchronous
 * \Parameters      : u8 u8NibbleCopy
 * \Return value:   : None
 ***************************************************************************************************************************/

void LCD_vidSendNibbleCMD(u8 u8NibbleCopy)
{

    /*clear the RS pin and set the EN pin to enter command mode*/
    CLR_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[8U],LCD_RS);
    SET_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2U],LCD_EN);
    /*send the nibble to the LCD through data pins*/
    INSERT_VALUE(GPIO_Arr[LCD_DATA_PORT]->DATA[15U],0,4,u8NibbleCopy);
    delay_ms(1U);

    CLR_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2U],LCD_EN);
}

/***************************************************************************************************************************
 * \Syntax          : void LCD_vidSendNibbleData(u8 u8NibbleCopy)

 * \Description     : send nibble in the data mode
 * \Sync\Async      : Synchronous
 * \Parameters      : u8 u8NibbleCopy
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidSendNibbleData(u8 u8NibbleCopy)
{

    /*clear the RS pin and set the EN pin to enter command mode*/
    SET_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[8U],LCD_RS);
    SET_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2U],LCD_EN);
    /*send the nibble to the LCD through data pins*/
    INSERT_VALUE(GPIO_Arr[LCD_DATA_PORT]->DATA[15U],0U,4U,u8NibbleCopy);
    delay_ms(1U);

    CLR_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2U],LCD_EN);

}

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteChar (u8 u8DataCopy)
 * \Description     : print a character in the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : (u8 u8DataCopy) the character that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteChar (u8 u8DataCopy)
{
    /*store the last 4 bits of the character*/
    u8 upper=u8DataCopy>>4;
    /*send the last 4bits*/
    LCD_vidSendNibbleData(upper);
    /*send the first 4bits*/

    LCD_vidSendNibbleData(u8DataCopy);
    counter++;

    /*if counter = 16 (row 1 full) go to new line*/
    if(counter >= 16U){
        LCD_move_cusor(1U,0U);
        counter=0U;

    }
}

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteSteing (u8 u8DataCopy)
 * \Description     : print a full string in the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : (u8 pu8StringCopy) the string that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteString (const u8* pu8StringCopy)
{
    u8 i;
    /*loop into string till reach null*/
    for (i = 0U; pu8StringCopy[i] != 0U; i++)
    {
        /*if counter = 16 (row 1 full) go to new line*/
        if(i==16U)
        {
            /* Go to New Line */
            LCD_move_cusor(1U,0U);
        }
        /*print the each single character individually*/
        LCD_vidWriteChar((u8)pu8StringCopy[i]);
    }
}

/***************************************************************************************************************************
 * \Syntax          : void LCD_move_cusor(u8 colu,u8 row)
 * \Description     : move the cursor to specific location on the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : u8 colu the colum to move to
 *                    u8 row the row to move to
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_move_cusor(u8 colu,u8 row){

    /*initialize a variable to store the position*/
    u8 local_positon ;
    /*check if the column is the first column*/
    if(colu == 0U){
        /*add the row number to the address of the first row in the first column*/
        local_positon=0x80U+row;
        /*check if the column is the second column*/
    }else if(colu == 1U){
        /*add the row number to the address of the first row in the second column*/
        local_positon=row+0xC0U;
    }
    else
    {

    }
    /*send the position of the cursor*/
    LCD_vidSendNibbleCMD(local_positon>>4);
    LCD_vidSendNibbleCMD(local_positon);
    delay_ms(5U);


}
/***************************************************************************************************************************
 * \Syntax          : void clear(void)
 * \Description     : clear the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : NONE
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_clear(void){
    LCD_vidSendNibbleCMD(0x0U);
    LCD_vidSendNibbleCMD(0x1U);
    counter=0U;
}

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteNumber (u16 num )
 * \Description     : convert number to string to print it on the lcd
 *                    num can be negative,float with precision 3 digits
 * \Sync\Async      : Synchronous
 * \Parameters      : (u16 num) the number that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteNumber (f32 Value ){

    u32 intNum;         /* To Carry the integer part in variable num */
    u8 arr[5]={0};      /* arr is an array To Carry the integer part as a single digits */
    u8 fraction[5]={0}; /* fraction is an array To Carry the fraction part as a single digits */
    u8 iterator=5U;             /* i represents index to start from last element */

    /*Check If num is Neg */
    if(Value < 0.0f)
    {

        Value = (f32)(-1.0*Value);      /*Convert it to Positive  */
        LCD_vidWriteChar('-');          /*Print negative symbol - */
    }

    intNum = (u32)Value;          /*cast num to integer and store integer part in intNum*/

    /*check in num = 0 print it*/
    if(Value == 0.0f){

        LCD_vidWriteChar('0');

    }else{

        /*while intNum not equal 0 that means there is a value to be truncated and stored in the array to b print*/
        while(intNum!=0U)
        {
            iterator--;
            arr[iterator]=(u8)(intNum%10U);   /*store least digit in last index if num = 12345
                                    it stores as {1,2,3,4,5};
                                    start from last digit which is 5 to 1
                                 */

            intNum=((u32)intNum/10U);   /*Remove taken digit*/
        }

        for(iterator=iterator;iterator<5U;iterator++){

            /* i now has (Left digit->most digit) start printing from it till last digit (right digit ->least digit)  */
            LCD_vidWriteChar(arr[iterator]+(u8)'0');

        }

        iterator=5U;
        intNum = (u32)Value;
        /*Check if there is a fraction(3 digits right . ) then print it*/
        if(((Value - intNum)*1000U) > 1)
        {
            /* First print dot */
            LCD_vidWriteChar('.');

            intNum = (f32)((f32)Value - (f32)intNum)*1000.0; /* store 3 digit of fraction in intNum to print them */
            /*while intNum not equal 0 that means there is a value to be truncated and stored in the array to b print*/

            while(intNum!=0U){
                iterator--;
                fraction[iterator]=(u8)(intNum%10U);/*store least digit in last index if num = 12345
                                                    it stores as {1,2,3,4,5};
                                                    start from last digit which is 5 to 1
                                                      */

                intNum=((u32)intNum/10U);     /*Remove taken digit*/
            }

            for(iterator=iterator ; iterator<5U ; iterator++){
                /* i now has (Left digit->most digit) start printing from it till last digit (right digit ->least digit)  */
                LCD_vidWriteChar((fraction[iterator]+(u8)'0'));
            }

        }

    }

}
