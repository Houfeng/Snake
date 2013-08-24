/*
描述:定义VM机器结构
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释,定义过程指令 类型 Proc,
*/

#pragma once

#include "Common.h"
#include "OpCodes.h"
#include "Stack.h"


 
/*过程指令*/
typedef struct
{
	std::map<string,long> Labels;
	std::vector<long> Jumps; 
	std::map<string,long> Vars;
	std::vector<Instruction> instructions;
} Proc;
 
/*
基于栈的虚拟机
*/
class VM
{ 
public:
	//
	VM();
	~VM();
	//
	std::map<string,Proc> Procs; //指令表
	int LoadByteCode(std::map<string,Proc> procs);//加载指令
	int Start(); //运行
};
