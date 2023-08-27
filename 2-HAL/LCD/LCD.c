
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
static GPIO_RegDef_t* GPIO_Arr[6]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF};

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
    GPIO_Init(Config, 7);

    delay_ms(40);
    CLR_BIT(GPIO_Arr[LCD_DATA_PORT]->DATA[4],LCD_RW);

    LCD_vidSendNibbleCMD(0x2); /*2 Line, 5*7 matrix in 4-bit mode*/
    LCD_vidSendNibbleCMD(0x8);

    LCD_vidSendNibbleCMD(0x0); /*Display on cursor off*/
    LCD_vidSendNibbleCMD(0xF); /*Increment cursor */

    LCD_vidSendNibbleCMD(0x0);
    LCD_vidSendNibbleCMD(0x6);

    LCD_vidSendNibbleCMD(0x0);/*clear the lcd*/
    LCD_vidSendNibbleCMD(0x1);
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
    CLR_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[8],LCD_RS);
    SET_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2],LCD_EN);
    /*send the nibble to the LCD through data pins*/
    INSERT_VALUE(GPIO_Arr[LCD_DATA_PORT]->DATA[15],0,4,u8NibbleCopy);
    delay_ms(1);

    CLR_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2],LCD_EN);
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
    SET_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[8],LCD_RS);
    SET_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2],LCD_EN);
    /*send the nibble to the LCD through data pins*/
    INSERT_VALUE(GPIO_Arr[LCD_DATA_PORT]->DATA[15],0,4,u8NibbleCopy);
    delay_ms(1);

    CLR_BIT(GPIO_Arr[LCD_CONTROL_PORT]->DATA[2],LCD_EN);

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
    if(counter >= 16){
        LCD_move_cusor(1,0);
        counter=0;

    }
}

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteSteing (u8 u8DataCopy)
 * \Description     : print a full string in the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : (u8 pu8StringCopy) the string that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteString (u8* pu8StringCopy)
{
    u8 i;
    /*loop into string till reach null*/
    for(i=0;pu8StringCopy[i];i++)
    {
        /*if counter = 16 (row 1 full) go to new line*/
        if(i==16)
        {
            //Go to New Line
            LCD_move_cusor(1,0);
        }
        /*print the each single character individually*/
        LCD_vidWriteChar(pu8StringCopy[i]);
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
    if(colu==0){
        /*add the row number to the address of the first row in the first column*/
        local_positon=0x80+row;
        /*check if the column is the second column*/
    }else if(colu==1){
        /*add the row number to the address of the first row in the second column*/
        local_positon=row+0xC0;
    }
    /*send the position of the cursor*/
    LCD_vidSendNibbleCMD(local_positon>>4);
    LCD_vidSendNibbleCMD(local_positon);
    delay_ms(5);


}
/***************************************************************************************************************************
 * \Syntax          : void clear(void)
 * \Description     : clear the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : NONE
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_clear(void){
    LCD_vidSendNibbleCMD(0x0);
    LCD_vidSendNibbleCMD(0x1);
    counter=0;
}

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteNumber (u16 num )
 * \Description     : convert number to string to print it on the lcd
 *                    num can be negative,float with precision 3 digits
 * \Sync\Async      : Synchronous
 * \Parameters      : (u16 num) the number that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteNumber (f32 num ){

    u32 intNum;         /* To Carry the integer part in variable num */
    u8 arr[5]={0};      /* arr is an array To Carry the integer part as a single digits */
    u8 fraction[5]={0}; /* fraction is an array To Carry the fraction part as a single digits */
    u8 i=5;             /* i represents index to start from last element */

    /*Check If num is Neg */
    if(num < 0)
    {

        num = -1*num;           /*Convert it to Positive  */
        LCD_vidWriteChar('-');  /*Print negative symbol - */
    }

    intNum = (u32)num;          /*cast num to integer and store integer part in intNum*/

    /*check in num = 0 print it*/
    if(num==0){
        LCD_vidWriteChar('0');
    }else{

        /*while intNum not equal 0 that means there is a value to be truncated and stored in the array to b print*/
        while(intNum!=0)
        {
            i--;
            arr[i]=intNum%10;   /*store least digit in last index if num = 12345
                                    it stores as {1,2,3,4,5};
                                    start from last digit which is 5 to 1
                                 */

            intNum=intNum/10;   /*Remove taken digit*/
        }

        for(;i<5;i++){

            /* i now has (Left digit->most digit) start printing from it till last digit (right digit ->least digit)  */
            LCD_vidWriteChar(arr[i]+'0');

        }

        i=5;
        intNum = (u32)num;
        /*Check if there is a fraction(3 digits right . ) then print it*/
        if(((num - intNum)*1000) > 1)
        {
            /* First print dot */
            LCD_vidWriteChar('.');

            intNum = (float)(num - intNum)*1000.0; /* store 3 digit of fraction in intNum to print them */

            /*while intNum not equal 0 that means there is a value to be truncated and stored in the array to b print*/
            while(intNum!=0){
                i--;
                fraction[i]=intNum%10;/*store least digit in last index if num = 12345
                                        it stores as {1,2,3,4,5};
                                        start from last digit which is 5 to 1
                                      */

                intNum=intNum/10;     /*Remove taken digit*/
            }

            for(;i<5;i++){
                /* i now has (Left digit->most digit) start printing from it till last digit (right digit ->least digit)  */
                LCD_vidWriteChar(fraction[i]+'0');
            }

        }

    }

}
