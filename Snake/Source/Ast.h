#ifndef __Ast_h_
#define __Ast_h_
 
#include "System.h"
//#include "Operator.h"
//###############################################
typedef enum{
	St_Base,
	St_Declare,
	St_Assign,
	St_Write,
	St_Read,
	St_Import,
	St_For,
	St_While,
	St_If,
	St_Sequ,
	St_Return,
	St_Function,
	St_CallFunc,
	St_Item
} StmtType;
typedef enum{
	Et_Base,
	Et_String,
	Et_Number,
	Et_Var,
	Et_BinExpr,
	Et_CallFunc,
	Et_Parenthesis,
	Et_Item
} ExprType;
//################################################
//语句基类
class Statement{
public:
	long Line;
	StmtType type;
	virtual void foo(){}
};
//表达式
class Expression{
public:
	long Line;
	ExprType type;
	virtual void foo(){}
};
//################################################
// 输出语句
class Write : public Statement
{
public:
	Expression *Expr;
};
// 输入语句
class Read : public Statement
{
public:
	string Ident;
};
// 声明变量
class Declare : public Statement
{
public:
	string Ident;
	Expression *Expr;
};
// 赋值
class Assign : public Statement
{
public:
	string op;
	string Ident;
	Expression *Expr;
};
// 对象成员操作
class ItemStmt : public Statement
{
public:
	string Ident;
	Expression *KeyExpr;
	long ExprIdent;
};
//引入
class Import : public  Statement
{
public: string Library;
};
//动态执行
class Eval : public Statement
{
public: string Code;
};
//##
// for 
class For : public Statement
{
public:
	Statement *Dim;
	Expression *Condition;
	Statement *Setup;
	Statement *Body;
};
/// While结构
class While : public Statement
{
public:
	Expression *Condition;
	Statement *Body;
};
/// if分支结构
class If : public  Statement
{
public :
	Expression *Condition;
	Statement *TrueBody;
	Statement *FalseBody;
};
/// 序列结构
class Sequence : public Statement
{
public: Statement *First;
		Statement *Second;
};
//##
//返回
class Return : public Statement
{
public: Expression *Expr;
};
//定义函数
class Function : public Statement
{
public: 
	string FunctionName;
	std::vector<string>  *Args;
	Statement *Body;
};
//调用函数
class CallFunction : public Statement
{
public: 
	string FunctionFullName;
	std::vector<Expression*> *ParExprs;
};
//################################################
/// 字符串(表达式)
class StringLiteral : public Expression
{
public:
	string Value;
};
/// 数值(表达式)
class NumberLiteral :  public Expression
{
public: double Value;
};
/// 变量(表达式)
class Variable : public Expression
{
public: string Ident;
};
/// 运算表达式
class BinExpr : public  Expression
{
public:
	Expression *Left;
	Expression *Right;
	string Op;
	string TempIdent;
};
/// 函数调用表达式
class CallFunctionExpr : public Expression
{
public: string FunctionFullName;
		std::vector<Expression*> *ParExprs;
};
/// 括号改变优先级表达式
class ParenthesisExpr : public Expression
{
public:
	Expression *Expr;
};
//################################################

#endif