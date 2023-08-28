/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Authors:  Ahmed Abdulazeem & Ahmed Ragab
 *         File:  Calculator.c
 *        Layer:  HAL
 *       Module:  Calculator
 *      Version:  1.00
 *
 *  Description:  Implementation of Calculator
 *
 *********************************************************************************************************************/

#include "Calculator.h"

#define EquationLength 20 /*EquationLength represents equation size*/


volatile u8 pressedkey = 0xff; /*variable to store Pressed Key in ISR it's volatile because it's accessed in ISR*/
volatile u8 isPressed  = 0;    /*Flag to sense pressing and it's volatile because it's accessed in ISR*/



/*arrElements => Array to carry the equation with variable length */
f32 arrElements[EquationLength]={0};
u8 elemntCount =EquationLength; /*elemntCount represents equation size*/

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
void Calculator_vidProgram(void)
{
    f32 Value=0.0f;       /* Value stores pressed value to be  stored in equation array */
    u8 temp=0;          /* temp is To carry pressed value temporary */
    f32 result =0;      /* result is To carry the final result */
    u8 negFlag=0;       /* negFlag is a flag to detect negative Numbers */
    u8 opFlag=0;        /* opFlag is a flag to detect pressed operation */
    while(1)
    {
        if(cur_col==5U)
        {
            /* to rescan again from column 0 */
            cur_col=col_0;
        }
        else
        {

        }
        /* to set specific column with low and the rest with high */
        scanButton(cur_col);

        /* 10ms delay then change column */
        delay_ms(10U);

        /* increment cur_col to point to next column */
        cur_col++;
        if (isPressed )
        {
            isPressed = 0U;        /*Clear the flag*/
            temp = pressedkey;  /*Store pressed key value in temp*/

            /*check if pressed value is an operation or equality*/
            if ((temp == (u8)'-') || (temp == (u8)'+') || (temp == (u8)'*') || (temp == (u8)'/') || (temp == (u8)'='))
            {

                LCD_vidWriteChar(temp);   /*print it on the LCD as char "Symbol"*/

            }else
            {
                LCD_vidWriteNumber((f32)temp); /*print it on the LCD as Value "by "ASCII"*/
            }

            /*check if pressed key is a Number*/
            if ((temp <= 9U) && (temp >= 0U))
            {
                Value = (f32)((f32)Value * 10.0f) + (f32)temp;   /*multiply previous input with 10 then increase current pressed value
                                                        *Ex
                                                        * 9*10 + 8 => 9 represents previous input while 8 represents current input
                                                        */

                opFlag=1U;                          /*Set flag to predict that the comming input could be an operation
                                                     *Number has entered now you can add an operation as a kind of checking(Verification)
                                                     */

            } /*check if pressed value is an operation*/
            else if ((temp == (u8)'+') || (temp == (u8)'-') || (temp == (u8)'*') || (temp == (u8)'/'))
            {
                /*check if opflag has set (that means an Number has entered previously)*/
                if(opFlag == 1U)
                {
                    /*Check if negFlag is set*/
                    if(negFlag == 1U)
                    {
                        arrElements[elemntCount] = (f32)(-1.0*Value); /*store Valueber as a negative value*/
                    }
                    else
                    {

                        arrElements[elemntCount] = (f32)Value;     /*store Valueber as a positive value*/
                    }

                    elemntCount++;                          /*increment counter*/
                    arrElements[elemntCount] = (f32)temp;        /*Store operation*/
                    elemntCount++;                          /*increment counter*/
                    Value   = (f32)0;                                  /*clear Value to store the next Valueber*/
                    opFlag  = 0U;                               /*clear operation flag*/
                    negFlag = 0U;                            /*clear negative flag*/

                } /*check if opflag has not set (that means previous operation wasn't a Valueber)
                   *last operation could be operation like 5+-
                   *or it could be first input like start with negative -
                   *So it could be a negative Valueber*/
                else if(temp == (u8)'-')
                {
                    /*if it's a negative sign so set Negatice Flag*/
                    negFlag=1U;
                }
                else
                {

                }
            }  /*check if pressed key is equal so check errors and print an error or result*/
            else if (temp == (u8)'=')
            {
                /*Check if there is a divison by zero*/
                if(((u8)arrElements[elemntCount] == 0U) && ((u8)arrElements[elemntCount-1U] == (u8)'/'))
                {
                    deleteEquation();                       /*delete entered equation*/
                    LCD_clear();                            /*Clear the screen*/
                    LCD_vidWriteString("Division By Zero!");/*Print Error Message*/

                } /*Check if opFlag is zero that means last operation was operation like 5-6+=
                    and it should be a Number like 5-6+3= so it prints Syntax Error*/
                else if(opFlag==0U){

                    deleteEquation();                       /*delete entered equation*/
                    LCD_clear();                            /*Clear the screen*/
                    LCD_vidWriteString("Syntax Error!");    /*Print Error Message*/

                }else
                {   /*Check if negFlag is set*/
                    if(negFlag == 1U)
                    {
                        negFlag=0U;                            /*Clear the flag*/
                        arrElements[elemntCount] = (f32)(-1.0*Value);  /*store Valueber as a negative value*/
                    }
                    else
                    {
                        arrElements[elemntCount] = (f32)Value;      /*store Valueber as a positive value*/
                    }
                    result = equationParse();               /*Calculate result and store it in result*/
                    LCD_vidWriteNumber(result);              /*Print the result*/
                }

            }/*Check if pressed key is 'c' which means clear screen and delete last enterd equation*/
            else if(temp == (u8)'c')
            {
                result = (f32)0;             /*Clear result*/
                Value  = (f32)0;                /*Clear Value*/
                negFlag= (u8)0;            /*Clear negFlag*/
                opFlag = (u8)0;             /*Clear opFlag*/
                deleteEquation();     /*delete entered equation*/
                LCD_clear();          /*Clear screen*/
            }
            else
            {

            }
        }
    }
}

/***************************************************************************************************************************
 * \Syntax          : f32 equationParse(void)
 * \Description     : used to Parse Calculator Equation
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
f32  equationParse(void)
{
    u8 i =0;
    /* start iteration from Odd index (as operations are always stored in odd indexes)
     * Ex:
     * equation stored as => {10,'+',2,'*',3,'-',4}
     * So all odd indexes stores operations
     * */
    for(i=1U; i<=elemntCount ;i=i)
    {
        /*Check if the current operation is multiplication or division*/
        if(((u8)arrElements[i] == (u8)'/') || ((u8)arrElements[i] == (u8)'*'))
        {
            /* calculate result and store it in previous index
             * Ex
             * assume equation stored as => {10,'+',2,'*',3,'-',4}
             * at index 3 it will apply operation on next and previous operands => calc 1*-1 and stores result in
             * index 2
             * now equation => {10,'+',6,'*',3,'-',4}
             * */
            arrElements[i-1U] = (f32)calcOperation((f32)arrElements[i-1U],(f32)arrElements[i],(f32)arrElements[i+1U]);

            /* remove current index
             * now equation => {10,'+',6,3,'-',4,4}
             * */
            removeElement(i);
            /* remove current index (which now represents next index as deleted index was i so i+1 beome i now so delete it)
             * now equation => {10,'+',6,'-',4,4,4}
             * */
            removeElement(i);

            /* now zeros last two element
             * now equation => {10,'+',6,'-',4,0,0}
             */
            arrElements[elemntCount]    = (f32)0;
            arrElements[elemntCount-1U] = (f32)0;

            /*decrement array size by 2 (the deleted 2 now out of the array)*/
            elemntCount-=(u8)2;
            /*
             * now index 3 has next operation which is '-' so we shouldn't increase i and we should chheck current operation
             * */

        }
        else
        {
            /*if operation is not '*' or '/' so go next operation*/
            i+=(u8)2;
        }
    }

    for(i=1U;i<=elemntCount;i=i)
    {
        /*Check if the current operation is subtraction or addition it should be one of them*/
        if(((u8)arrElements[i] == (u8)'-') || ((u8)arrElements[i] == (u8)'+'))
        {
            /* calculate result and store it in previous index
             * Ex
             * equation now => {10,'+',6,'-',4}
             * at index 1 it will apply operation on next and previous operands => calc 10+6 and stores result in
             * index 0
             * now equation => {16,'+',6,'-',4}
             * */
            arrElements[i-1U] = (f32)calcOperation(arrElements[i-1U],arrElements[i],arrElements[i+1U]);

            /* remove current index
             * now equation => {16,6,'-',4,4}
             * */
            removeElement(i);
            /* remove current index (which now represents next index as deleted index was i so i+1 beome i now so delete it)
             * now equation => {16,'-',4,4,4}
             * */
            removeElement(i);

            /* now zeros last two element
             * now equation => {16,'-',4,0,0}
             */
            arrElements[elemntCount] = 0.0f;
            arrElements[elemntCount-1U] = 0.0f;
            /*decrement array size by 2 (the deleted 2 now out of the array)*/
            elemntCount-=(u8)2;
            /*
             * now index 1 has next operation which is '-' so we shouldn't increase i and we should check current operation
             * now equation has => {16,'-',4} and i =1 and elemntCount =2
             * for next operation the last the result will be calculated and stored in index 0 and the elemntCount
             * will be zero while i is 1 which breaks the loop
             * */
        }
        else
        {

        }
        /*
         * now equation has only one element and elemntCount is 0
         * */
    }


    return arrElements[0]; /*Return result which stored at index zero*/
}

/***************************************************************************************************************************
 * \Syntax          : f32  calcOperation(f32 Value1,f32 op,f32 Value2)
 * \Description     : used to calculate simple operation
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Reentrant
 * \Parameters (in) : Value1 -> left operand ,
 *                    Value2 -> right operand ,
 *                    op   ->operation to be performed
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
f32  calcOperation(f32 Value1,f32 op,f32 Value2)
{
    f32 res=0;      /*local variable to calculate result*/
    switch((u8)op) /*check input operation*/
    {
    case (u8)'*':
        res = Value1 * Value2;
        break;
    case (u8)'/':
        res = (f32)(1.0f * ((f32)Value1 / (f32)Value2));
        break;
    case (u8)'+':
        res = Value1 + Value2;
        break;
    case (u8)'-':
        res = Value1 - Value2;
        break;
    default:
        break;
    }
    return res;
}

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
void removeElement(u8 index)
{
    /*start from index to the last of the array write the data in the next index to current index*/
    for(;index<elemntCount;index++)
    {
        arrElements[index] = arrElements[index+1U];
    }
}

/***************************************************************************************************************************
 * \Syntax          : void deleteEquation(void)
 * \Description     : used to delete an equation after detecting an error or after the user decision to clear screen
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : index -> the Element's index to be deleted
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void deleteEquation(void)
{
    /*loop the elements from last to first and zeros elements and decrements the arr size*/
    for(;elemntCount>0U;elemntCount--)
    {
        arrElements[elemntCount] = 0.0f;
    }
    arrElements[0] = 0.0f;
}

/***************************************************************************************************************************
 * \Syntax          : void EXTIB0_CallBack(void)
 * \Description     : callback function that checks (scan) first row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB0_CallBack(void)
{
    /*check current column to predict which button pressed because the interrupt indicates the row
     * with known column and row we could know the pressed key
     * */
    switch(cur_col)
    {
    case col_0:
        pressedkey=1U;
        break;
    case col_1:
        pressedkey=2U;
        break;
    case col_2:
        pressedkey=3U;
        break;
    case col_3:
        pressedkey=(u8)'+';
        break;
    default:
        break;
    }
    isPressed=1U; /*Set flag*/
    delay_ms(100U);

}

/***************************************************************************************************************************
 * \Syntax          : void EXTIB1_CallBack(void)
 * \Description     : callback function that checks (scan) second row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB1_CallBack(void)
{
    /*check current column to predict which button pressed because the interrupt indicates the row
     * with known column and row we could know the pressed key
     * */
    switch(cur_col)
    {
    case col_0:
        pressedkey=4U;
        break;
    case col_1:
        pressedkey=5U;
        break;
    case col_2:
        pressedkey=6U;
        break;
    case col_3:
        pressedkey=(u8)'-';
        break;
    default:
        break;
    }
    isPressed=1U;    /*Set flag*/
    delay_ms(100U);  /*delay to avoid bouncing (Software de-bouncing)*/

}

/***************************************************************************************************************************
 * \Syntax          : void EXTIB2_CallBack(void)
 * \Description     : callback function that checks (scan) third row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB2_CallBack(void)
{
        /* check current column to predict which button pressed because the interrupt indicates the row
         * with known column and row we could know the pressed key
         * */
    switch(cur_col)
    {
    case col_0:
        pressedkey=7U;
        break;
    case col_1:
        pressedkey=8U;
        break;
    case col_2:
        pressedkey=9U;
        break;
    case col_3:
        pressedkey=(u8)'/';
        break;
    default:
        break;
    }

    isPressed=1U;   /*Set Flag*/
    delay_ms(100U); /*delay to avoid bouncing (Software de-bouncing)*/
}

/***************************************************************************************************************************
 * \Syntax          : void EXTIB3_CallBack(void)
 * \Description     : callback function that checks (scan) forth row
 * \Sync\Async      : Synchronous
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : the result of equation after parsing
 ***************************************************************************************************************************/
void EXTIB3_CallBack(void)
{
        /* check current column to predict which button pressed because the interrupt indicates the row
         * with known column and row we could know the pressed key
         * */
    switch(cur_col)
    {
    case col_0:
        pressedkey=(u8)'c';
        break;
    case col_1:
        pressedkey=0U;
        break;
    case col_2:
        pressedkey=(u8)'=';
        break;
    case col_3:
        pressedkey = (u8)'*';
        break;
    default:
        break;
    }
    isPressed=1U;   /*Set Flag*/
    delay_ms(100U); /*delay to avoid bouncing (Software de-bouncing)*/
}
