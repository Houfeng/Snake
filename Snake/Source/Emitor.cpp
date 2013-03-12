/*
描述: 代码发出器,通过相应指令及操作数,发出实际代码
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/


#include "Emitor.h"
//########################################################

int Emitor::AddProcEmitor(string name)
{
	ProcEmitors[name]=new ProcEmitor();
	return 0;
}
ProcEmitor* Emitor::GetProcEmitor(string name)
{
	return ProcEmitors[name];
}
string Emitor::GetAsmCode()
{
	string str="";
	std::map<string,ProcEmitor*>::iterator it=ProcEmitors.begin();   
	for(;it!=ProcEmitors.end();++it)
	{
		str+="PROC	"+it->first+"\n";
		str+=it->second->AsmCode;
		str+="END\n";
	}
	return str;
}

//#########################################################

ProcEmitor::ProcEmitor()
{
	LabelNumber=0;
}
//将发出代码加入暂存
int ProcEmitor::PushAsmCode(string asmCode)
{
	AsmCode+=asmCode+"\n";
	return 0;
}
/*发出汇编代码*/
int ProcEmitor::Emit(OpCode opcode,long arg,string att)
{
	switch(opcode)
	{
	case OP_NOP:
		break;
		//----------------------------
	case OP_NEW:
		PushAsmCode("NEW");
		break;
	case OP_LDC:
		PushAsmCode("	LDC	"+att);
		break;
	case OP_LD:
		PushAsmCode("	LD	"+att);
		break;
	case OP_ST:
		PushAsmCode("	ST	"+att);
		break;
	case OP_PUSH:
		PushAsmCode("	PUSH	"+att);
		break;
		//--------------------------------------
	case OP_ADD:
		PushAsmCode("	ADD	");
		break;
	case OP_SUB:
		PushAsmCode("	SUB");
		break;
	case OP_MUL:
		PushAsmCode("	MUL");
		break;
	case OP_DIV:
		PushAsmCode("	DIV");
		//------------------------------------
		break;
	case OP_EQ:
		PushAsmCode("	EQ");
		break;
	case OP_GT:
		PushAsmCode("	GT");
		break;
	case OP_LT:
		PushAsmCode("	LT");
		break;
	case OP_GE:
		PushAsmCode("	GE");
		break;
	case OP_LE:
		PushAsmCode("	LE");
		break;
		//------------------------------------
	case OP_NOT:
		PushAsmCode("	NOT	");
		break;
	case OP_AND:
		PushAsmCode("	AND	");
		break;
	case OP_OR:
		PushAsmCode("	OR	");
		//------------------------------------
	case OP_CALL:
		PushAsmCode("	CALL	"+att);
		break;
	case OP_ARG:
		PushAsmCode("	ARG	"+att);
		break;
	case OP_RET:
		PushAsmCode("	RET");
		//---------------------------------------
		break;
	case OP_JMP:
		PushAsmCode("	JMP	"+att);
		break;
	case OP_JT:
		PushAsmCode("	JT	"+att);
		break;
	case OP_JF:
		PushAsmCode("	JF	"+att);
		break;
	case OP_EXIT:
		PushAsmCode("	EXIT	");
		break;
		//-----------------------------------------
	case OP_OUT:
		PushAsmCode("	OUT	");
		break;
	case OP_IN:
		PushAsmCode("	IN	");
		break;
		//---------------------------------------------
	default:
		OutMessage::Out("不能发出无效指令");
		return 1;
		break;
	}
	return 0;
}
string ProcEmitor::DefLabel()
{
	string label= "lb_"+Convert::ToString(++LabelNumber);
	return label;
}
int ProcEmitor::MarkLabel(string labelName)
{
		PushAsmCode("	LABEL	"+labelName);
		return 0;
}