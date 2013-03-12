/*
描述:汇编器
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释,添加 OP_ARG指令处理,
添加汇编过程及过程参数处理;
*/

#include "Asm.h"

Asm::Asm(std::vector<Token> tokenCollection)
{
	Tokens=tokenCollection;
	size=Tokens.size();
	index=0;
	if(index <size)
	{
		Parse();
		HandleJumpLabel();
	}
	else
	{
		OutMessage::IsTokenEof(0);
	}
}
Asm::~Asm(void)
{
	Tokens.clear();
	Procs.clear();
}
//####################################
//不是符号标尾
bool Asm::NotEof()
{
	return index<size;
}
//不是符号标尾
bool Asm::NotEof(long ix)
{
	return ix<size;
}
//####################################
int Asm::Parse()
{
	string CurrProc="";
	while(NotEof())
	{
		Instruction ins;
		bool OPT=true;
		Common::string_tolower(Tokens[index].String);
		if(Tokens[index].String=="proc")
		{
			//创建一个过程
			CurrProc=Tokens[++index].String;
			Proc proc;
			Procs[CurrProc]=proc;
			OPT=false;
		}
		else if(Tokens[index].String=="end")
		{
			OPT=false;
		}
		//----------------------------------------------------------
		else if(Tokens[index].String=="nop")
		{
			ins.Op= OP_NOP;
		}
		//-------------------------------------------------------
		else if(Tokens[index].String=="new")
		{
			ins.Op= OP_NEW;
		}
		else if(Tokens[index].String=="ldc")
		{
			ins.Op= OP_LDC;
			long ti=++index;
			if(Tokens[ti].Type==TT_String)
			{
				ins.att=(char *)(Tokens[ti].String.c_str());
			}
			else
			{
				ins.arg=Convert::ToDouble(Tokens[ti].String);
				ins.att=NULL;
			}
		}
		else if(Tokens[index].String=="ld")
		{
			ins.Op= OP_LD;
			long var=GetVars(CurrProc,Tokens[++index].String);
			if(var>-1)
				ins.arg=var;
			else
			{
				OutMessage::Out(Tokens[index].Line,"未定义的变量'"+Tokens[index].String+",在ASM代码中");
				return 1;
			}		
		}
		else if(Tokens[index].String=="st")
		{
			ins.Op= OP_ST;
			long var=GetVars(CurrProc,Tokens[++index].String);
			if(var>-1)
			{
				ins.arg=var;
			}
			else
			{
				ins.Op= OP_PUSH;
				Procs[CurrProc].Vars[Tokens[index].String]=Procs[CurrProc].Vars.size();//暂存局部变量位置
			}
		}	
		//else if(Tokens[index].String=="push")
		//{
		//	ins.Op= OP_PUSH;
		//	Procs[CurrProc].Vars[Tokens[++index].String]=Procs[CurrProc].Vars.size();//暂存局部变量位置
		//}
		//----------------------------------------------------------
		else if(Tokens[index].String=="add")
			ins.Op= OP_ADD;
		else if(Tokens[index].String=="sub")
			ins.Op= OP_SUB;
		else if(Tokens[index].String=="mul")
			ins.Op= OP_MUL;
		else if(Tokens[index].String=="div")
			ins.Op= OP_DIV;
		//------------------------------------------
		else if(Tokens[index].String=="eq")
			ins.Op= OP_EQ;
		else if(Tokens[index].String=="ne")
			ins.Op= OP_NE;
		else if(Tokens[index].String=="gt")
			ins.Op= OP_GT;
		else if(Tokens[index].String=="lt")
			ins.Op= OP_LT;
		else if(Tokens[index].String=="ge")
			ins.Op= OP_GE;
		else if(Tokens[index].String=="le")
			ins.Op= OP_LE;
		//----------------------------------------------------
		else if(Tokens[index].String=="not")
			ins.Op= OP_NOT;
		else if(Tokens[index].String=="and")
			ins.Op= OP_AND;
		else if(Tokens[index].String=="or")
			ins.Op= OP_OR;
		//------------------------------------------------------
		else if(Tokens[index].String=="jmp")
		{
			ins.Op= OP_JMP;
			ins.att=Convert::ToCharPtr(Tokens[++index].String.c_str());
			Procs[CurrProc].Jumps.push_back(Procs[CurrProc].instructions.size());//暂存跳转语句
		}
		else if(Tokens[index].String=="jt")
		{
			ins.Op= OP_JT;
			ins.att=Convert::ToCharPtr(Tokens[++index].String.c_str());
			Procs[CurrProc].Jumps.push_back(Procs[CurrProc].instructions.size());//暂存跳转语句
		}
		else if(Tokens[index].String=="jf")
		{
			ins.Op= OP_JF;
			ins.att=Convert::ToCharPtr(Tokens[++index].String.c_str());
			Procs[CurrProc].Jumps.push_back(Procs[CurrProc].instructions.size());//暂存跳转语句
		}
		else if(Tokens[index].String=="exit")
			ins.Op= OP_EXIT;
		//------------------------------------------------------------------
		else if(Tokens[index].String=="out")
			ins.Op= OP_OUT;
		else if(Tokens[index].String=="in")
			ins.Op= OP_IN;
		//-------------------------------------------------------------
		else if(Tokens[index].String=="call")
		{
			ins.Op= OP_CALL;
			ins.att=(char *)(Tokens[++index].String.c_str());
		}
		else if(Tokens[index].String=="arg")
		{
			ins.Op= OP_ARG;
			Procs[CurrProc].Vars[Tokens[++index].String]=Procs[CurrProc].Vars.size();//暂存参数变量位置
		}
		else if(Tokens[index].String=="ret")
			ins.Op= OP_RET;
		//-------------------------------------------------------------
		else if(Tokens[index].String=="label")
		{
			Procs[CurrProc].Labels[Tokens[++index].String]=Procs[CurrProc].instructions.size();//暂存跳转标志位置
			OPT=false;
		}
		//-----
		else
		{
			OutMessage::Out(Tokens[index].Line,"无效指令 '"+Tokens[index].String+"'");
			return 1;
		}

		//############################################
		if(OPT)
		{
			Procs[CurrProc].instructions.push_back(ins);
		}
		index++;
	}
	return 0;
}
/*处理跳转标志*/
int Asm::HandleJumpLabel()
{
	std::map<string,Proc>::iterator it=Procs.begin();   
	for(;it!=Procs.end();++it)
	{
		int lsize=it->second.Jumps.size();
		for(int i=0;i<lsize;i++)
		{
			long l=it->second.Jumps[i];
			string lbName=it->second.instructions[l].att;
			it->second.instructions[l].arg=it->second.Labels[lbName];
		}
	}
	return 0;
}
/*取得已定义的变量*/
long Asm::GetVars(string procName,string varName)
{
	std::map<string,long>::iterator it=Procs[procName].Vars.find(varName);
	if(it == Procs[procName].Vars.end()) {
		return -1;
	}
	else {
		return it->second;
	}
}
