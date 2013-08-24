/*
描述: 代码发出器,通过相应指令及操作数,发出实际代码
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#pragma once 

#include "System.h"
#include "OpCodes.h"
#include "Common.h"
#include "OutMessage.h"


//代码发出器
class ProcEmitor
{
public:
	ProcEmitor();
	string ProcName;
	//发出代码
	int Emit(OpCode opcode,long arg,string att);
	string DefLabel();
	int MarkLabel(string labelName);
	//存储代码
	string AsmCode;
private:
	long LabelNumber;
	//将发出的代码暂存
	int PushAsmCode(string asmCode);
};

class Emitor
{
public:
	string GetAsmCode();
    int AddProcEmitor(string name);
	ProcEmitor* GetProcEmitor(string name);
private:
	std::map<string,ProcEmitor*> ProcEmitors;
};
