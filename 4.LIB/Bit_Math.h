/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  -
 *	   	   File:  BIT_MATH.h
 *		  Layer:  LIB
 *       Module:  BIT_MATH
 *		Version:  1.00
 *	
 *  Description:  Bit manipulation Function Like Macros       
 *  
 *********************************************************************************************************************/

#ifndef BIT_MATH_H_
#define BIT_MATH_H_


#include "Platform_Types.h"
#include "Std_Types.h"



#define SET_BIT(REG,BitNo)                   ((REG)|=(1U<<(BitNo)))
#define CLR_BIT(REG,BitNo)                   ((REG)&=(~(1U<<(BitNo))))
#define TOGGLE_BIT(REG,BitNo)                ((REG)^=(1U<<(BitNo)))
#define GET_BIT(REG,BitNo)                   (((REG)>>(BitNo))&1U)
#define GET_VALUE(REG,Offset,count)          ((REG>>Offset)&((1<<count)-1))
#define INSERT_BIT(REG,BIT_NO,VALUE)         (REG =(REG&(~(1<<BIT_NO))|(VALUE<<BIT_NO)))
#define INSERT_VALUE(REG,Offset,count,VALUE) (REG =((REG&(~(((1<<count)-1)<<Offset))))|(VALUE<<Offset))

#define INSERT_LOWNIBBLE(REG,VALUE) (REG = ((REG & 0xFFFFFFF0) | VALUE))

#endif
