/*
描述: 分析器,通过输入扫描器扫描结果,执行分析输入分析结果(语法树)
代码生成器,将通过分析器分析结果,生成中间代码.
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/ 

#include "Parser.h"

Parser::~Parser()
{
	Tokens.clear();
}
//通过扫描器的扫描结果构造分析器
Parser::Parser(std::vector<Token> tokenCollection)
{
	Tokens=tokenCollection;
	size=Tokens.size();
	index=0;
	if(index <size)
	{
		StatementTree=ParseStatement();
		int x=0;
	}
	else
	{
		OutMessage::IsTokenEof(0);
	}
}
//不是符号标尾
bool Parser::NotEof()
{
	return index<size;
}
//不是符号标尾
bool Parser::NotEof(long ix)
{
	return ix<size;
}
//结束分析
int Parser::EndParser()
{
	index=size+2;
	return 0;
}
//###########################################
//分析语句
Statement* Parser::ParseStatement()
{
	//标识集合长度异常
	if (!NotEof())
	{
		OutMessage::IsTokenEof(Tokens[index].Line);
		return NULL;
	}
	Statement *result;
	bool isEnd = false;
	//引入语句
	if (Tokens[index].String==Keyword::Import)
	{
		Import *impt =  new Import();
		(*impt).Line=Tokens[index].Line;
		(*impt).type= St_Import;
		index++;
		if(NotEof() && (Tokens[index].Type==TT_IdentOrKeyword || Tokens[index].Type==TT_String))
			(*impt).Library =Tokens[index].String;
		else
		{
			OutMessage::Out((*impt).Line,"在import后缺少名称;"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		result = impt;
	}
	//定义函数
	else if (Tokens[index].String==Keyword::Function || Tokens[index].String==Keyword::Func)
	{
		Function *fun = new Function();
		(*fun).Line=Tokens[index].Line;
		(*fun).type= St_Function;
		index++;
		if(NotEof() && Tokens[index].Type==TT_IdentOrKeyword)
			(*fun).FunctionName= Tokens[index].String;
		else
		{
			OutMessage::Out((*fun).Line,"在function后缺少名称;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		if(Tokens[index+1].String=="(")
				index++;
		//查找参数开始
		(*fun).Args =new std::vector<string>();
		while ( NotEof() && Tokens[++index].String!=")" )
		{
			if (NotEof() && Tokens[index].Type==TT_IdentOrKeyword)
				(*fun).Args->push_back(Tokens[index].String);
		}
		//查找参数结束
		index++;
		if(NotEof()){
			(*fun).Body = ParseStatement();
		}
		else{
			OutMessage::Out(Tokens[index].Line,"异常文件尾;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		index++;
		if (!NotEof() || Tokens[index].String!=Keyword::End)
		{
			OutMessage::Out(Tokens[index].Line,"在function ... 后缺少预期的end;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		result = fun;
	}
	//如果反回值
	else if (Tokens[index].String==Keyword::Return)
	{
		Return (*rt) =  new Return();
		(*rt).Line=Tokens[index].Line;
		(*rt).type= St_Return;
		index++;
		if(NotEof())
			(*rt).Expr =ParseExpr();	
		else
		{
			OutMessage::Out(rt->Line,"异常的文件尾;在"+Tokens[index].String+"附近");
			EndParser();
			return NULL;
		}
		result = rt;
	}
	//如果输出语句
	else if (Tokens[index].String==Keyword::Write)
	{
		Write (*wt) =  new Write();
		(*wt).Line=Tokens[index].Line;
		(*wt).type= St_Write;
		index++;
		if(NotEof())
			(*wt).Expr =ParseExpr();	
		else
		{
			OutMessage::Out(wt->Line,"异常的文件尾;在"+Tokens[index].String+"附近");
			EndParser();
			return NULL;
		}
		result = wt;
	}
	//如果输入语句
	else if (Tokens[index].String==Keyword::Read)
	{
		Read (*rd) =  new Read();
		(*rd).Line=Tokens[index].Line;
		(*rd).type= St_Read;
		index++;
		if(NotEof() && Tokens[index].Type==TT_IdentOrKeyword)
			(*rd).Ident =Tokens[index].String;
		else
		{
			OutMessage::Out(rd->Line,"在read语句后确少变量名;在"+Tokens[index].String+"附近");
			EndParser();
			return NULL;
		}
		result = rd;
	}
	//如果定义变量
	else if (Tokens[index].String==Keyword::Var)
	{
		Declare *declareVar =  new Declare();
		(*declareVar).Line=Tokens[index].Line;
		(*declareVar).type= St_Declare;
		index++;
		//检查变量名
		if (NotEof() && Tokens[index].Type==TT_IdentOrKeyword){
			(*declareVar).Ident = Tokens[index].String;
		}
		else{
			OutMessage::Out(Tokens[index].Line,"在var后缺少预期变量名;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		index++;
		//检查赋值符
		if (!NotEof() || Tokens[index].String!="="){
			OutMessage::Out(Tokens[index].Line,"在变量名" + (*declareVar).Ident + "后缺少预期'='号;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		index++;
		if(NotEof()){
			//分析当前赋值语句赋给变量的表达式
			(*declareVar).Expr = ParseExpr();
		}
		else{
			(*declareVar).Expr=NULL;
			OutMessage::Out(Tokens[index].Line,"缺少预期表达式;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		result = declareVar;
	}
	//while循环
	else if (Tokens[index].String==Keyword::While)
	{
		While *wh =  new While();
		(*wh).Line=Tokens[index].Line;
		(*wh).type= St_While;
		index++;
		if (NotEof()){
			(*wh).Condition = ParseExpr();
		}
		else{
			(*wh).Condition =NULL;
			OutMessage::Out(Tokens[index].Line,"在关键字while后缺少预期的 表达式;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		index++;
		if (NotEof()){
			(*wh).Body = ParseStatement();
		}
		else{
			(*wh).Body =NULL;
			OutMessage::Out(Tokens[index].Line,"在关键字while后缺少预期的循环主体;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		index++;
		if (!NotEof()|| Tokens[index].String!=Keyword::End)
		{
			OutMessage::Out(Tokens[index].Line,"在while ... 后缺少预期的end;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		result = wh;
	}
	//If分支结构
	else if (Tokens[index].String==Keyword::If)
	{
		If *myIf =  new If();
		(*myIf).Line=Tokens[index].Line;
		(*myIf).type= St_If;
		index++;
		if (NotEof()){
			(*myIf).Condition = ParseExpr();
		}
		else{
			(*myIf).Condition=NULL;
			OutMessage::Out(Tokens[index].Line,"在关键字if后缺少预期的表达式;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		index++;
		if (NotEof()){
			(*myIf).TrueBody = ParseStatement();
		}
		else{
			(*myIf).TrueBody=NULL;
			OutMessage::Out(Tokens[index].Line,"在关键字if后缺少预期的主体;在"+Tokens[index].String+"在附近");
			EndParser();
			return NULL;
		}
		if (Tokens[index+1].String==Keyword::Else)
		{
			index += 2;
			if (NotEof())	{
				(*myIf).FalseBody = ParseStatement();
			}
			else{
				(*myIf).FalseBody=NULL;
				OutMessage::Out(Tokens[index].Line,"在关键字else后缺少预期的主体;在"+Tokens[index].String+"在附近");
				EndParser();
				return NULL;
			}
		}
		else{
			(*myIf).FalseBody =NULL;
		}
		index++;
		if (!NotEof() || Tokens[index].String!=Keyword::End)
		{
			OutMessage::Out(Tokens[index].Line,"在if ... 后缺少预期的end;在"+Tokens[index].String+"附近");
			EndParser();
			return NULL;
		}
		result = myIf;
	}
	else if (Tokens[index].String==Keyword::End || Tokens[index].String==Keyword::Else )
	{
		isEnd = true;
		result = NULL;
	}
	//如果是其它（赋值 , 调用函数）
	else if (Tokens[index].Type==TT_IdentOrKeyword)
	{
		//调用函数-语句
		if (NotEof(size- 1) && Tokens[index + 1].String=="(")
		{
			CallFunction *callFun =  new CallFunction();
			(*callFun).Line=Tokens[index].Line;
			(*callFun).type= St_CallFunc;
			(*callFun).FunctionFullName = Tokens[index].String;
			if(Tokens[index+1].String=="(")
				index++;
			//查找参数开始
			(*callFun).ParExprs =new std::vector<Expression*>();
			while ( NotEof() && Tokens[++index].String!=")" )
			{
				if (NotEof() && Tokens[index].Type==TT_IdentOrKeyword)
					(*callFun).ParExprs->push_back(this->ParseExpr());
			}
			//查找参数结束
			//index++;
			if(!NotEof())
			{
				OutMessage::Out(callFun->Line,"异常文件尾;在"+Tokens[index].String+"附近");
				EndParser();
				return NULL;
			}
			return callFun;
		}
		else if (NotEof(size- 1) && Tokens[index + 1].String==".")//成员赋值语句
		{
			ItemStmt *item=new ItemStmt();
			item->Line=Tokens[index].Line;
			item->type= St_Item;
			item->Ident=Tokens[index].String;
			index++;
			item->KeyExpr=this->ParseExpr();
			//item->Expr=this->ParseExpr();
		}
		//赋值语句
		else
		{
			Assign *assign = new Assign();
			(*assign).Line=Tokens[index].Line;
			(*assign).type= St_Assign;
			(*assign).Ident =Tokens[index].String;
			index++;
			if (NotEof() || NotEof(index+ 1) && (Tokens[index].Type==TT_Operator)){
				(*assign).op =Tokens[index].String;
			}
			else{
				OutMessage::Out(Tokens[index].Line,"在变量名" + (*assign).Ident + "后缺少预期的赋值运算符;在"+Tokens[index].String+"在附近");
				EndParser();
				return NULL;
			}
			index++;
			if(NotEof())			{
				//分析当前赋值语句赋给变量的表达式
				(*assign).Expr = ParseExpr();
			}
			else{
					OutMessage::Out(Tokens[index].Line,"缺少预期表达式;在"+Tokens[index].String+"在附近");
					EndParser();
					return NULL;
			}
			result = assign;
		}
	}
	else
	{
		OutMessage::Out(Tokens[index].Line,"分析器错误;在"+Tokens[index].String+"在附近");
		EndParser();
		return NULL;
	}
	if (!isEnd)
	{
		index++;
		//检查是否分析完毕
		if (NotEof())//如果未完
		{
			//顺序结构分析
			Sequence *sequence =  new Sequence();
			(*sequence).Line=Tokens[index].Line;
			(*sequence).type= St_Sequ;
			(*sequence).First = result;
			(*sequence).Second = ParseStatement();
			result = sequence;
		}
	}
	else
	{
		index--;
	}
	return result;
}
/// 分析无运算符表达式（注意不移动标记指针）
Expression* Parser::ParseSimpleExpr()
{
	if (!NotEof())
	{
		OutMessage::IsTokenEof(Tokens[index].Line);
	}
	if (NotEof()
		&& Tokens[index].Type==TT_Operator
		&& Tokens[index].String=="!")//如果是!号
	{
		BinExpr *binExpr = new BinExpr();
		(*binExpr).Line=Tokens[index].Line;
		(*binExpr).type= Et_BinExpr;
		(*binExpr).Left = NULL;
		(*binExpr).Op = Tokens[index].String;
		index++;
		(*binExpr).Right = ParseSimpleExpr();
		return binExpr;
	}
	else if (NotEof() && Tokens[index].Type==TT_Operator
		&& Tokens[index].String=="-")//如果是-号
	{
		BinExpr *binExpr = new BinExpr();
		(*binExpr).Line=Tokens[index].Line;
		(*binExpr).type= Et_BinExpr;
		NumberLiteral *num1 = new NumberLiteral();
		(*num1).Value = 0;
		(*num1).type= Et_Number;
		(*binExpr).Left = num1;
		(*binExpr).Op =Tokens[index].String;
		index++;
		(*binExpr).Right = ParseSimpleExpr();
		return binExpr;
	}
	else if (NotEof()
		&& Tokens[index].Type==TT_Other
		&&Tokens[index ].String=="(")//如果是括号
	{
		ParenthesisExpr *pExpr = new ParenthesisExpr();
		(*pExpr).type= Et_Parenthesis;
		(*pExpr).Line=Tokens[index].Line;
		index++;
		(*pExpr).Expr = ParseExpr();
		index++;
		return pExpr;
	}
	else if ((index <size - 1)&& Tokens[index + 1].String=="(")//如果是函数调用
	{
		CallFunctionExpr *callFun =new CallFunctionExpr();
		(*callFun).type= Et_CallFunc;
		(*callFun).Line=Tokens[index].Line;
		(*callFun).FunctionFullName = Tokens[index].String;
		if(Tokens[index+1].String=="(")
				index++;
		//查找参数开始
		(*callFun).ParExprs =new std::vector<Expression*>();
		while ( NotEof() && Tokens[++index].String!=")" )
		{
			if (NotEof() && Tokens[index].Type==TT_IdentOrKeyword)
				(*callFun).ParExprs->push_back(this->ParseExpr());
		}
		//查找参数结束
		//index++;
		if(!NotEof())
		{
			OutMessage::Out(callFun->Line,"异常文件尾;在"+Tokens[index].String+"附近");
			EndParser();
			return NULL;
		}
		return callFun;
	}
	else if (Tokens[index].Type==TT_String)//如果是字符
	{
		StringLiteral *stringLiteral = new StringLiteral();
		(*stringLiteral).type= Et_String;
		(*stringLiteral).Line=Tokens[index].Line;
		string value = Tokens[index].String;
		(*stringLiteral).Value = value;
		return stringLiteral;
	}
	else if (Tokens[index].Type==TT_Number)//如果是数值
	{
		double numberValue = Convert::ToDouble(Tokens[index].String);
		NumberLiteral *intLiteral =new NumberLiteral();
		(*intLiteral).type= Et_Number;
		(*intLiteral).Line=Tokens[index].Line;
		(*intLiteral).Value = numberValue;
		return intLiteral;
	}
	else if (Tokens[index].Type ==TT_IdentOrKeyword)//变量表达式
	{
		string ident = Tokens[index].String;
		Variable *var = new Variable();
		(*var).type= Et_Var;
		(*var).Line=Tokens[index].Line;
		(*var).Ident = ident;
		return var;
	}
	else
	{
		OutMessage::NoExpression(Tokens[index].Line);
		EndParser();
		return NULL;
	}
}
//分析表达式
Expression* Parser::ParseExpr()
{
	return ParseExpr(NULL);
}
/// 分析表达式 （注意：不移动标记指针）
Expression* Parser::ParseExpr(Expression* preExpr)
{
	if(!NotEof())
	{
		OutMessage::IsTokenEof(Tokens[index].Line);
	}
	//----
	Expression *expr = preExpr;
	if (preExpr==NULL)
		expr = ParseSimpleExpr();
	//----
	if (index + 1 < size && Tokens[index + 1].Type==TT_Operator)
	{
		BinExpr *binExpr = new BinExpr();
		(*binExpr).Line=Tokens[index].Line;
		(*binExpr).type= Et_BinExpr;
		(*binExpr).Left = expr;
		index++;
		(*binExpr).Op=Tokens[index].String;
		index++;
		(*binExpr).Right = ParseSimpleExpr();
		return ParseExpr(binExpr);
	}
	else
	{
		return expr;
	}
}
//###########################################

