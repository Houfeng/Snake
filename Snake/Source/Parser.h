/*
描述: 分析器,通过输入扫描器扫描结果,执行分析输入分析结果(语法树)
代码生成器,将通过分析器分析结果,生成中间代码.
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#pragma once

#include "System.h"
#include "OutMessage.h" 
#include "Token.h"
#include "Ast.h"
//#include "Operator.h"
#include "Keyword.h"


//分析器
class Parser
{
public:
	//分析结果（语法树）
	Statement *StatementTree;
	//通过 扫描器结果（符号标记列表）构造一个分析器
	Parser(std::vector<Token> tokenCollection);
	~Parser();
private:
	//暂存符号标记
	std::vector<Token> Tokens;
	//符号标记个数
	long size;
	//当前符号标记索引
	long index;
	//测试是否到符号标记尾
	bool NotEof();
	//测试指定的索引是否到符号标记尾
	bool NotEof(long ix);
	//结束分析;
	int EndParser();
	//分析语名
	Statement* ParseStatement();
	//分析表达式
	Expression* ParseExpr();
	//分析运算表达式
	Expression* ParseExpr(Expression* preExpr);
	//分析简单无运算的表达式
	Expression* ParseSimpleExpr();
};