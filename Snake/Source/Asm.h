/*
描述:汇编器头文件
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释,添加 OP_ARG指令处理,
		  添加汇编过程及过程参数处理;
*/
#pragma once

#include "System.h"
#include "Common.h"
#include "OpCodes.h"
#include "Token.h"
#include "OutMessage.h"  
#include "VM.h"

 
class Asm
{
private:
	std::vector<Token> Tokens;
	long size;
	long index;
	bool NotEof();
	bool NotEof(long i);
	//-----------------------------
	long GetVars(string procName,string varName);
public:
	std::map<string,Proc> Procs;
	Asm(std::vector<Token> tokenCollection);
	~Asm(void);
	int Parse();
	int HandleJumpLabel();
};