/*
描述:定义VM机器结构
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释,定义过程指令 类型 Proc,
		  构造函数添加,procName参数;
*/
#include "VM.h"
#include "Procedure.h"

VM::VM(){}
VM::~VM()
{
	Procs.clear();
}
//------------------------------------------------------------------
/*加载字节指令*/
int VM::LoadByteCode(std::map<string,Proc> procs)
{
	Procs=procs;
	return 0;
}
//启动虚拟机
int VM::Start()
{
	string mainName="main";
	/*if((&Procs["main"])!=NULL){ 
		mainName="main";
	}
	else if((&Procs["Main"])!=NULL){
		mainName="Main";
	}
	else{
		mainName="";
		OutMessage::Out("确少主函数(main或Main)");
		return 1;
	}*/
	Procedure mainProc=Procedure(this,NULL,mainName,0);
	mainProc.Start();
	return 0;
}

