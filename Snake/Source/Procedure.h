/*
描述:定义过程
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 
	添加注释,添加 OP_ARG指令处理,
	构造函数添加,procName参数;
*/
#pragma once
#include "Stack.h"
#include "VM.h"
#include "OutMessage.h"


class Procedure
{
private :
	VM* TheVM;//虚拟机
	Procedure* ParentProc;//父过程 
	int PState;//过程状态
	long pc;//过程计数器
	Stack PStack; //求值栈	
	string ProcName;
	////////
public:
	Stack PMemory;//过程私有变量栈
	int Start();
	int Stop();
	Procedure(VM* vm,Procedure* proc,string procName,long enter);
	~Procedure(void);
    //##################################
	int P_NOP();
	//----------------------------
	int P_NEW();
	int P_LDC(Instruction ins);
	int P_LD(long I);
	int P_ST(long I);
	int P_PUSH();
	//---------------------
	int P_ADD();
	int P_SUB();
	int P_MUL();
	int P_DIV();
	//-------------------------
	int P_EQ();
	int P_NE();
	int P_GT();
	int P_LT();
	int P_GE();
	int P_LE();
	//----------------------------
	int P_NOT();
	int P_AND();
	int P_OR();
	//-----------------------------
	int P_JMP(long L);
	int P_JT(long L);
	int P_JF(long L);
	int P_EXIT();
	//---------------------
	int P_CALL(Instruction ins);
	int P_ARG();
	int P_RET();
	//-------------------------
	int P_OUT();
	int P_IN();
};

