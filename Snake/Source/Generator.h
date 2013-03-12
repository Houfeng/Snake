/*
描述:汇编代码生成器,输入分析器结果(语法树),生成汇编代码
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/
#pragma once 

#include "Common.h"
#include "Scaner.h"
#include "Ast.h"
#include "Parser.h"
#include "OpCodes.h"
#include "OutMessage.h"
#include "Emitor.h"



//代码生成器
class Generator
{
private:
	//暂存分析器结果(语法树)
	Statement *StatementTree;
	//生成语句
	int GenerateStmt(Statement* theStmt);
	//生成表达式
	int GenerateExpr(Expression* theExpr);
	//当前正发生成的函数名
	string CurrEtrName;
	//发出者
	Emitor emitor;
	//发出代码
	int Emit(OpCode opcode,long arg,string att);
	//定义一个跳转标志
	string DefLabel();
	//标记(发出)跳转标志
	int MarkLabel(string lbName);
	//
	//变量表
	std::vector<string> VarTable;
	//检查变量是定义
	int ChkVar(string varFullName);
	//函数表
	std::vector<string>	FunTable;
	//检查函数是否定义
	int ChkFun(string funName);
public:
	Generator(Statement *statementTree);
	~Generator();
	string AsmCode;
};