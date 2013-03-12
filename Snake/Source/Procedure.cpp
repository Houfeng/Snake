/*
描述:VM机器
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 
	添加注释,定义过程指令 类型 Proc,
	构造函数添加,procName参数;
*/
#include "Procedure.h"


Procedure::Procedure(VM *vm,Procedure* proc,string procName,long enter)
{
	TheVM=vm;
	ParentProc=proc;
	PState=0;
	pc=enter;
	ProcName=procName;
}
Procedure::~Procedure(void)
{
	PStack.Clear();
	PMemory.Clear();
}
//################################################

int Procedure::Start()
{
	PState=1;
	long InstSize=TheVM->Procs[ProcName].instructions.size();
	while( PState && pc<InstSize)
	{
		Instruction ins = (Instruction)(TheVM->Procs[ProcName].instructions[pc++]);
		switch(ins.Op)
		{
		case OP_NOP:
			P_NOP();
			break;
		//------------------------
		case OP_NEW:
			P_NEW();
			break;
		case OP_LDC:
			P_LDC(ins);
			break;
		case OP_LD:
			P_LD(ins.arg);
			break;
		case OP_ST:
			P_ST(ins.arg);
			break;
		case OP_PUSH:
			P_PUSH();
			break;
		//------------------------
		case OP_ADD:
			P_ADD();
			break;
		case OP_SUB:
			P_SUB();
			break;
		case OP_MUL:
			P_MUL();
			break;
		case OP_DIV:
			P_DIV();
			break;
		//-----------------------
		case OP_EQ:
			P_EQ();
			break;
		case OP_NE:
			P_NE();
			break;
		case OP_GT:
			P_GT();
			break;
		case OP_LT:
			P_LT();
			break;
		case OP_GE:
			P_GE();
			break;
		case OP_LE:
			P_LE();
			break;
		//---------------------------
		case OP_NOT:
			P_NOT();
			break;
		case OP_AND:
			P_AND();
			break;
		case OP_OR:
			P_OR();
			break;
		//---------------------------------
		case OP_JMP:
			P_JMP(ins.arg);
			break;
		case OP_JT:
			P_JT(ins.arg);
			break;
		case OP_JF:
			P_JF(ins.arg);
			break;
		case OP_EXIT: 
			P_EXIT();
			break;
		//--------------------------------
		case OP_CALL:
			P_CALL(ins);
			break;
		case OP_ARG:
			P_ARG();
			break;
		case OP_RET:
			P_RET();
			break;
		//-------------------------------
		case OP_OUT:
			P_OUT();
			break;
		case OP_IN:
			P_IN();
			break;
		default:
			OutMessage::Out("无效指令");
			PStack.Clear();
			PMemory.Clear();
			PState=0;
			break;
		}
	}
	PStack.Clear();
	PMemory.Clear();
	return 0;
}
int Procedure::Stop()
{
	PState=0;
	PStack.Clear();
	PMemory.Clear();
	return 0;
}

//################################################
int Procedure::P_NOP()
{
	return 0;
}
//---------------------------------------------
int Procedure::P_NEW()
{
	Object obj;
	obj.type= T_OBJECT;
	PStack.Push(obj);
	return 0;
}
int Procedure::P_LDC(Instruction ins)
{
	Object obj;
	if(ins.att!=NULL)
	{
		obj.type= T_STRING;
		obj.value.str=ins.att;
	}
	else
	{
		obj.type= T_NUMBER;
		obj.value.num=ins.arg;
	}
	PStack.Push(obj);
	return 0;
}
int Procedure::P_PUSH()
{
	PMemory.Push(PStack.Pop());
	return 0;
}
int Procedure::P_LD(long I)
{
	PStack.Push(PMemory.Get(I));
	return 0;
}
int Procedure::P_ST(long I)
{
	Object obj=PStack.Pop();
	PMemory.Set(I,obj);
	return 0;
}
//-----------------------------------------------
int Procedure::P_ADD()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	Object rs;
	if(right.type== T_NUMBER && left.type== T_NUMBER)
	{
		rs.type= T_NUMBER;
		rs.value.num=left.value.num+right.value.num;
	}
	else
	{
		string str1;
		string str2;
		//
		if(right.type== T_NUMBER)
			str2=Convert::ToString(right.value.num);
		if(right.type== T_STRING)
			str2=right.value.str;
		//
		if(left.type== T_NUMBER)
			str1=Convert::ToString(left.value.num);
		if(left.type== T_STRING)
			str1=left.value.str;
		rs.type= T_STRING;
		rs.value.str=Convert::ToCharPtr((str1+str2).c_str());
	}
	PStack.Push(rs);
	return 0;
}
int Procedure::P_SUB()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=left.value.num-right.value.num;
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_MUL()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=left.value.num*right.value.num;
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_DIV()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=left.value.num/right.value.num;
		PStack.Push(rs);
	}
	return 0;
}
//-----------------------------------------------
int Procedure::P_EQ()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num==right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_NE()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num!=right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_GT()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num>right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_LT()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num<right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_GE()
{	
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num>=right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_LE()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{

		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num<=right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
//---------------------------------------------------
int Procedure::P_NOT()
{
	Object left=PStack.Pop();
	if(left.type== T_NUMBER )
	{

		Object rs;
		rs.type=left.type;
		rs.value.num=(!left.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_AND()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{

		Object rs;
		rs.type= T_NUMBER;
		rs.value.num=(left.value.num && right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
int Procedure::P_OR()
{
	Object right=PStack.Pop();
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && right.type== T_NUMBER)
	{
		Object rs;
		rs.type=left.type;
		rs.value.num=(left.value.num || right.value.num);
		PStack.Push(rs);
	}
	return 0;
}
//------------------------------------------------
int Procedure::P_JMP(long L)
{
	pc=L;
	return 0;
}
int Procedure::P_JT(long L)
{
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && left.value.num!=0)
	{
		pc=L;
	}
	return 0;
}
int Procedure::P_JF(long L)
{
	Object left=PStack.Pop();
	if(left.type== T_NUMBER && left.value.num==0)
	{
		pc=L;
	}
	return 0;
}
int Procedure::P_EXIT()
{
	PState=0;
	PStack.Clear();
	PMemory.Clear();
	return 0;
}
//--------------------------------------------------
int Procedure::P_OUT()
{
	Object left=PStack.Pop();
	string str;
	if(left.type== T_NUMBER)
		str=Convert::ToString(left.value.num);
	else if(left.type== T_STRING)
		str=left.value.str;
	cout.unsetf(ios::scientific);
	cout<<str;
	return 0;
} 
int Procedure::P_IN()
{
	Object obj;
	obj.type= T_STRING;
	cin>>obj.value.str;
	PStack.Push(obj);
	return 0;
}
//--------------------------------------------
int Procedure::P_CALL(Instruction ins)
{
	if(ins.att!=NULL)
	{
		string str=string(ins.att);
		Procedure subProc=Procedure(TheVM,this,str,0);
		subProc.Start();
	}
	return 0;
}
int Procedure::P_ARG()
{
	Object left=ParentProc->PStack.Pop();
	if(ParentProc!=NULL)
		PMemory.Push(left);
	return 0;
}
int Procedure::P_RET()
{
	Object left=PStack.Pop();
	if(ParentProc!=NULL)
	{
		ParentProc->PStack.Push(left);
	}
	PStack.Clear();
	PMemory.Clear();
	return 0;
}
//##########
