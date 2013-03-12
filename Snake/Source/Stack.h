/*
描述:定义栈结构
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释;
*/

#ifndef __Stack_h_
#define __Stack_h_

#include "Common.h"
#include "OutMessage.h"




typedef unsigned char  Byte;

//typedef void (*Cfunction) (void);
//typedef void (*Function) (void);
 
typedef enum
{
 T_NULL,
 T_OBJECT,
 T_NUMBER,
 T_STRING,
 T_BOOL,
 T_FUNCTION,
 T_CFUNCTION,
 T_USERDATA
} Type; 

typedef union
{
 //Cfunction 	 cfn;
 //Function 	 fun;
 double    	 num;
 char*           str;
 Byte      	byt;
 void           *nul;
} Value;

typedef struct Object
{
 Type  type;
 Value value;
} Object;

typedef struct
{
 char   *name;
 Object  object;
} Symbol;

//==========================================
//定义栈
class Stack
{
private :
	int Size;
	int Pos;
	std::vector<Object> _Stack;
public:	
	Object Null;
	//-------------------------------------------
	Stack();
	~Stack();
	//-------------------------------------------
	int Init();
	Object Push(Object obj);
	Object Pop();
	Object Top();
	Object Get(int pos);
	Object Set(int pos,Object obj);
	int IsFull();
	int IsEmpty();
	int Clear();
};

#endif 