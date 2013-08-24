/*
描述:定义VM机器指令
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释,添加 OP_ARG指令;
*/

#ifndef __OpCodes_h_
#define __OpCodes_h_

#include  "Common.h"
#include "Stack.h"


//操作数
typedef enum 
{
	//指令
	OP_NOP,		//空指令
	//==============================栈指令 
	OP_LDC,		//加载常量入求值栈
	OP_LD,			//加载内存入求值栈	
	OP_NEW,		//创建一个表(对象)入求值栈
	OP_GetItem,	//取表(对象)成员
	OP_SetItem,	//设置表(对象)成员
	//------------
	OP_PUSH,		//压入内存
	OP_ST,			//存入内存指定地址	
	//==============================算术指令
	OP_ADD,		//加
	OP_SUB,		//减
	OP_MUL,		//乘
	OP_DIV,		//除
	//==============================关系指令
	OP_EQ,			//等于
	OP_NE,			//不等于
	OP_GT,			//大于
	OP_GE,			//大于等于
	OP_LT,			//小于
	OP_LE,			//小于等于
	//==============================逻辑指令
	OP_NOT,		//非
	OP_AND,		//与
	OP_OR,			//或
	//==============================跳转指令
	OP_JMP,		//无条件跳转
	OP_JT,			//栈顶为真跳转
	OP_JF,			//栈顶为假跳转
	OP_EXIT,		//结束过程
	//==============================过程调用指令
	OP_CALL,		//调用过程
	OP_RET,		//返回过程结果
	OP_ARG,		//定义参数
	//==============================标准输入输出指令
	OP_OUT,		//标准输出
	OP_IN			//标准输入
	//
} OpCode;

//指令结构
typedef struct{
	int Op;
	double arg;
	char* att;
} Instruction ;

#endif

