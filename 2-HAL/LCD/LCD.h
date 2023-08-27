/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Ahmed Abdulazeem & Ahmed Ragab
 *         File:  LCD.h
 *        Layer:  HAL
 *       Module:  LCD 16x2
 *      Version:  1.00
 *
 *  Description:  Interfaces Needed by user
 *
 *********************************************************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/**********************************************************************************************************************
 * Data PINs Configurations (Port and Pin)
 *********************************************************************************************************************/
#define LCD_DATA_PORT  GPIO_PORTD
#define LCD_D0         GPIO_PIN0
#define LCD_D1         GPIO_PIN1
#define LCD_D2         GPIO_PIN2
#define LCD_D3         GPIO_PIN3

/**********************************************************************************************************************
 * Control PINs Configurations (Port and Pin)
 *********************************************************************************************************************/
#define LCD_CONTROL_PORT GPIO_PORTE
#define LCD_EN           GPIO_PIN1
#define LCD_RW           GPIO_PIN2
#define LCD_RS           GPIO_PIN3



/**********************************************************************************************************************
 * Functions Prototype
 *********************************************************************************************************************/

/***************************************************************************************************************************
* \Syntax          : void LCD_vidInit(void)
* \Description     : initialize the lcd
* \Sync\Async      : Synchronous
* \Parameters      : NONE
* \Return value:   : None
***************************************************************************************************************************/
void LCD_vidInit(void);

/***************************************************************************************************************************
 * \Syntax          : void LCD_vidSendNibbleCMD(u8 u8NibbleCopy)

 * \Description     : send nibble in the command mode
 * \Sync\Async      : Synchronous
 * \Parameters      : u8 u8NibbleCopy
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidSendNibbleData(u8 u8CmdCopy);

/***************************************************************************************************************************
 * \Syntax          : void LCD_vidSendNibbleData(u8 u8NibbleCopy)

 * \Description     : send nibble in the data mode
 * \Sync\Async      : Synchronous
 * \Parameters      : u8 u8NibbleCopy
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidSendNibbleCMD(u8 u8CmdCopy);

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteChar (u8 u8DataCopy)
 * \Description     : print a character in the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : (u8 u8DataCopy) the character that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteChar (u8 u8DataCopy);

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteSteing (u8 u8DataCopy)
 * \Description     : print a full string in the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : (u8 pu8StringCopy) the string that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteString (u8* pu8StringCopy);

/***************************************************************************************************************************
 * \Syntax          : void LCD_move_cusor(u8 colu,u8 row)
 * \Description     : move the cursor to specific location on the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : u8 colu the column to move to
 *                    u8 row the row to move to
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_move_cusor(u8 colu,u8 row);

/***************************************************************************************************************************
 * \Syntax          : void clear(void)
 * \Description     : clear the LCD
 * \Sync\Async      : Synchronous
 * \Parameters      : NONE
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_clear(void);

/***************************************************************************************************************************
 * \Syntax          :void LCD_vidWriteNumber (u16 num )
 * \Description     : convert number to string to print it on the lcd
 *                    num can be negative,float with precision 3 digits
 * \Sync\Async      : Synchronous
 * \Parameters      : (u16 num) the number that will be printed
 * \Return value:   : None
 ***************************************************************************************************************************/
void LCD_vidWriteNumber (f32 num );
#endif /* 2_HAL_LCD_LCD_H_ */
