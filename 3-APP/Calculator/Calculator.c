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

volatile u8 pressedkey = 0xff; /*variable to store Pressed Key in ISR it's volatile because it's accessed in ISR*/
volatile u8 isPressed  = 0;    /*Flag to sense pressing and it's volatile because it's accessed in ISR*/


#define EquationLength 20 /*EquationLength represents equation size*/

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
    f32 num=0.0f;       /* num stores pressed value to be  stored in equation array */
    u8 temp=0;          /* temp is To carry pressed value temporary */
    f32 result =0;      /* result is To carry the final result */
    u8 negFlag=0;       /* negFlag is a flag to detect negative numbers */
    u8 opFlag=0;        /* opFlag is a flag to detect pressed operation */
    while(1)
    {
        if(cur_col==5)
        {
            /* to rescan again from column 0 */
            cur_col=col_0;
        }

        /* to set specific column with low and the rest with high */
        scanButton(cur_col);

        /* 10ms delay then change column */
        delay_ms(10);

        /* increment cur_col to point to next column */
        cur_col++;
        if (isPressed )
        {
            isPressed=0;        /*Clear the flag*/
            temp = pressedkey;  /*Store pressed key value in temp*/

            /*check if pressed value is an operation or equality*/
            if(temp == '-' || temp == '+' || temp == '*' || temp == '/' || temp =='='){

                LCD_vidWriteChar(temp);   /*print it on the LCD as char "Symbol"*/

            }else
            {
                LCD_vidWriteNumber(temp); /*print it on the LCD as num "by "ASCII"*/
            }

            /*check if pressed key is a number*/
            if ((temp <= 9) && (temp >=0))
            {
                num = num*10 + temp;   /*multiply previous input with 10 then increase current pressed value
                                        *Ex
                                        * 9*10 + 8 => 9 represents previous input while 8 represents current input
                                        */
                opFlag=1;               /*Set flag to predict that the comming input could be an operation
                                         *number has entered now you can add an operation as a kind of checking(Verification)
                                         */

            } /*check if pressed value is an operation*/
            else if ((temp == '+' || temp == '-' || temp == '*' || temp == '/'))
            {
                /*check if opflag has set (that means an number has entered previously)*/
                if(opFlag == 1)
                {
                    /*Check if negFlag is set*/
                    if(negFlag == 1)
                    {
                        arrElements[elemntCount] = -1.0*num; /*store number as a negative value*/
                    }
                    else
                    {

                        arrElements[elemntCount] = num;     /*store number as a positive value*/
                    }

                    elemntCount++;                          /*increment counter*/
                    arrElements[elemntCount] = temp;        /*Store operation*/
                    elemntCount++;                          /*increment counter*/
                    num=0;                                  /*clear num to store the next number*/
                    opFlag=0;                               /*clear operation flag*/
                    negFlag = 0;                            /*clear negative flag*/

                } /*check if opflag has not set (that means previous operation wasn't a number)
                   *last operation could be operation like 5+-
                   *or it could be first input like start with negative -
                   *So it could be a negative number*/
                else if(temp == '-')
                {
                    /*if it's a negative sign so set Negatice Flag*/
                    negFlag=1;
                }
            }  /*check if pressed key is equal so check errors and print an error or result*/
            else if (temp == '=')
            {
                /*Check if there is a divison by zero*/
                if(arrElements[elemntCount] == 0 && arrElements[elemntCount-1] == '/')
                {
                    deleteEquation();                       /*delete entered equation*/
                    LCD_clear();                            /*Clear the screen*/
                    LCD_vidWriteString("Division By Zero!");/*Print Error Message*/

                } /*Check if opFlag is zero that means last operation was operation like 5-6+=
                    and it should be a number like 5-6+3= so it prints Syntax Error*/
                else if(opFlag==0){

                    deleteEquation();                       /*delete entered equation*/
                    LCD_clear();                            /*Clear the screen*/
                    LCD_vidWriteString("Syntax Error!");    /*Print Error Message*/

                }else
                {   /*Check if negFlag is set*/
                    if(negFlag == 1)
                    {
                        negFlag=0;                            /*Clear the flag*/
                        arrElements[elemntCount] = -1.0*num;  /*store number as a negative value*/
                    }
                    else
                    {
                        arrElements[elemntCount] = num;      /*store number as a positive value*/
                    }
                    result = equationParse();               /*Calculate result and store it in result*/
                    LCD_vidWriteNumber(result);              /*Print the result*/
                }

            }/*Check if pressed key is 'c' which means clear screen and delete last enterd equation*/
            else if(temp == 'c')
            {
                result=0;             /*Clear result*/
                num=0;                /*Clear num*/
                negFlag=0;            /*Clear negFlag*/
                opFlag=0;             /*Clear opFlag*/
                deleteEquation();     /*delete entered equation*/
                LCD_clear();          /*Clear screen*/
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
    for(i=1;i<=elemntCount;)
    {
        /*Check if the current operation is multiplication or division*/
        if(arrElements[i] == '/' || arrElements[i] == '*' )
        {
            /* calculate result and store it in previous index
             * Ex
             * assume equation stored as => {10,'+',2,'*',3,'-',4}
             * at index 3 it will apply operation on next and previous operands => calc 1*-1 and stores result in
             * index 2
             * now equation => {10,'+',6,'*',3,'-',4}
             * */
            arrElements[i-1] = calcOperation(arrElements[i-1],arrElements[i],arrElements[i+1]);

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
            arrElements[elemntCount] = 0;
            arrElements[elemntCount-1] = 0;

            /*decrement array size by 2 (the deleted 2 now out of the array)*/
            elemntCount-=2;
            /*
             * now index 3 has next operation which is '-' so we shouldn't increase i and we should chheck current operation
             * */
            continue;

        }
        /*if operation is not '*' or '/' so go next operation*/
        i+=2;
    }

    for(i=1;i<=elemntCount;)
    {
        /*Check if the current operation is subtraction or addition it should be one of them*/
        if(arrElements[i] == '-' || arrElements[i] == '+')
        {
            /* calculate result and store it in previous index
             * Ex
             * equation now => {10,'+',6,'-',4}
             * at index 1 it will apply operation on next and previous operands => calc 10+6 and stores result in
             * index 0
             * now equation => {16,'+',6,'-',4}
             * */
            arrElements[i-1] = calcOperation(arrElements[i-1],arrElements[i],arrElements[i+1]);

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
            arrElements[elemntCount] = 0;
            arrElements[elemntCount-1] = 0;
            /*decrement array size by 2 (the deleted 2 now out of the array)*/
            elemntCount-=2;
            /*
             * now index 1 has next operation which is '-' so we shouldn't increase i and we should check current operation
             * now equation has => {16,'-',4} and i =1 and elemntCount =2
             * for next operation the last the result will be calculated and stored in index 0 and the elemntCount
             * will be zero while i is 1 which breaks the loop
             * */
        }
        /*
         * now equation has only one element and elemntCount is 0
         * */
    }


    return arrElements[0]; /*Return result which stored at index zero*/
}

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
f32  calcOperation(f32 num1,f32 op,f32 num2)
{
    f32 res=0;      /*local variable to calculate result*/
    switch((int)op) /*check input operation*/
    {
    case '*':
        res = num1 * num2;
        break;
    case '/':
        res = (1.0*num1 / num2);
        break;
    case '+':
        res = num1 + num2;
        break;
    case '-':
        res = num1 - num2;
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
        arrElements[index] = arrElements[index+1];
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
    for(;elemntCount>0;elemntCount--)
    {
        arrElements[elemntCount] = 0;
    }
    arrElements[0] = 0;
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
        pressedkey=1;
        break;
    case col_1:
        pressedkey=2;
        break;
    case col_2:
        pressedkey=3;
        break;
    case col_3:
        pressedkey='+';
        break;
    default:
        break;
    }
    isPressed=1; /*Set flag*/
    delay_ms(100);

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
        pressedkey=4;
        break;
    case col_1:
        pressedkey=5;
        break;
    case col_2:
        pressedkey=6;
        break;
    case col_3:
        pressedkey='-';
        break;
    default:
        break;
    }
    isPressed=1;    /*Set flag*/
    delay_ms(100);  /*delay to avoid bouncing (Software de-bouncing)*/

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
        pressedkey=7;
        break;
    case col_1:
        pressedkey=8;
        break;
    case col_2:
        pressedkey=9;
        break;
    case col_3:
        pressedkey='/';
        break;
    default:
        break;
    }

    isPressed=1;   /*Set Flag*/
    delay_ms(100); /*delay to avoid bouncing (Software de-bouncing)*/
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
        pressedkey='c';
        break;
    case col_1:
        pressedkey=0;
        break;
    case col_2:
        pressedkey='=';
        break;
    case col_3:
        pressedkey = '*';
        break;
    default:
        break;
    }
    isPressed=1;   /*Set Flag*/
    delay_ms(100); /*delay to avoid bouncing (Software de-bouncing)*/
}
