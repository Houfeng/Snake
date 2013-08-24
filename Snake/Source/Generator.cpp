/*
描述:汇编代码生成器
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-08-20   
修改发出方法,发出的代码关键字改为大写
添加内存清理
*/

#include "Generator.h"

/*析构*/
Generator::~Generator()
{
	VarTable.clear();
	FunTable.clear();
}
/*通过语法树构造生成器*/
Generator::Generator(Statement *statementTree)
{
	StatementTree=statementTree;
	GenerateStmt(StatementTree);
	AsmCode=emitor.GetAsmCode();
}
//###################################################
int Generator::ChkVar(string varFullName)
{
	int vtSize=VarTable.size();
	for(int i=0;i<vtSize;i++)
	{
		if(VarTable[i]==varFullName)
			return 1;
	}
	return 0;
}
int Generator::ChkFun(string funName)
{
	int vfSize=FunTable.size();
	for(int i=0;i<vfSize;i++)
	{
		if(FunTable[i]==funName)
			return 1;
	}
	return 0;
}
//###################################################
/*发出一条指令代码*/
int Generator::Emit(OpCode opcode,long arg,string att)
{
	emitor.GetProcEmitor(CurrEtrName)->Emit(opcode,arg,att);
	return 0;
}
/*定义一个跳转标签，并返回标签名*/
string Generator::DefLabel()
{
	return emitor.GetProcEmitor(CurrEtrName)->DefLabel();
}
/*向代码中发出标签*/
int Generator::MarkLabel(string lbName)
{
	emitor.GetProcEmitor(CurrEtrName)->MarkLabel(lbName);
	return 0;
}
//###################################################
//生成表达式
int Generator::GenerateExpr(Expression *theExpr)
{
	if(theExpr==NULL)
		return 1;
	//如果是一个运算表达式
	if ((*theExpr).type== Et_BinExpr)
	{
		BinExpr *expr =(BinExpr*)theExpr;
		if(expr!=NULL)
			GenerateExpr((*expr).Left);
		if(expr!=NULL)
			GenerateExpr((*expr).Right);
		//
		if((*expr).Op=="+")
			Emit( OP_ADD,0,"");
		else if((*expr).Op=="-")
			Emit( OP_SUB,0,"");
		else if((*expr).Op=="*")
			Emit( OP_MUL,0,"");
		else if((*expr).Op=="/")
			Emit( OP_DIV,0,"");
		//
		else if((*expr).Op=="==")
			Emit( OP_EQ,0,"");
		else if((*expr).Op=="!=")
			Emit( OP_NE,0,"");
		else if((*expr).Op==">")
			Emit( OP_GT,0,"");
		else if((*expr).Op==">=")
			Emit( OP_GE,0,"");
		else if((*expr).Op=="<")
			Emit( OP_LT,0,"");
		else if((*expr).Op=="<=")
			Emit( OP_LE,0,"");
		//
		else if((*expr).Op=="!")
			Emit( OP_NOT,0,"");
		else if((*expr).Op=="&")
			Emit( OP_AND,0,"");
		else if((*expr).Op=="|")
			Emit( OP_OR,0,"");
	}
	else 
	{
		//如果是一个简单表达式
		//__________________________________________
		if ((*theExpr).type== Et_String)//字符串常量
		{
			StringLiteral *expr =(StringLiteral*)theExpr;
			Emit( OP_LDC,0,"\""+(*expr).Value+"\"");
		}
		else if ((*theExpr).type== Et_Number)//数值常量
		{
			NumberLiteral *expr =(NumberLiteral*)theExpr;
			Emit( OP_LDC,0,Convert::ToString((*expr).Value));
		}
		else if ((*theExpr).type== Et_Var)//变量
		{
			Variable *expr =(Variable*)theExpr;
			if(!ChkVar(CurrEtrName+":"+expr->Ident))
			{
				OutMessage::Out(expr->Line,"变量"+expr->Ident+"未定义");
				return 1;
			}
			Emit( OP_LD,0,(*expr).Ident);
		}
		else if ((*theExpr).type== Et_CallFunc)//函数表达式
		{
			CallFunctionExpr *expr =(CallFunctionExpr *)theExpr;
			if(!ChkFun(expr->FunctionFullName))
			{
				OutMessage::Out(expr->Line,"函数"+expr->FunctionFullName+"未定义");
				return 1;
			}
			int argSize =(*expr).ParExprs->size();
			for(int i=0;i<argSize;i++)
			{
				this->GenerateExpr((*(expr->ParExprs))[i]);
			}
			this->Emit( OP_CALL,0,expr->FunctionFullName);
		}
		else if ((*theExpr).type== Et_Parenthesis)//括号表达式
		{
			ParenthesisExpr *expr =(ParenthesisExpr*)theExpr;
			GenerateExpr((*expr).Expr);
		}
		else
		{
			OutMessage::Out("无法生成的表达式");
		}
	}
	delete(theExpr);
	return 0;
}
//生成语句
int Generator::GenerateStmt(Statement* theStmt)
{
	if (theStmt==NULL)
	{
		return 1;
	}
	//函数定义
	else if ((*theStmt).type== St_Function)
	{
		Function *stmt =(Function *)theStmt;
		//*******************************************
		CurrEtrName=stmt->FunctionName;
		emitor.AddProcEmitor(CurrEtrName);
		FunTable.push_back(CurrEtrName);
		//*******************************************
		int argSize =(*stmt).Args->size();
		for(int i=0;i<argSize;i++)
		{
			Emit( OP_ARG,0,(*(*stmt).Args)[i]);
		}
		GenerateStmt((*stmt).Body);
	}
	//顺序
	else if ((*theStmt).type== St_Sequ)
	{
		Sequence *stmt =(Sequence *)theStmt;
		GenerateStmt((*stmt).First);
		GenerateStmt((*stmt).Second);
	}
	//if分支
	else if ((*theStmt).type== St_If)
	{
		string lbElse=DefLabel();
		string lbEnd=DefLabel();
		//
		If *stmt =(If *)theStmt;
		GenerateExpr((*stmt).Condition);
		Emit( OP_JF,0,lbElse);
		GenerateStmt((*stmt).TrueBody);
		Emit( OP_JMP,0,lbEnd);
		MarkLabel(lbElse);
		GenerateStmt((*stmt).FalseBody);
		MarkLabel(lbEnd);//在结构后标记lbEnd
	}
	//while循环
	else if ((*theStmt).type== St_While)
	{
		string lbTest=DefLabel();
		string lbTrue=DefLabel();
		//
		While *stmt =(While *)theStmt;
		Emit( OP_JMP,0,lbTest);
		MarkLabel(lbTrue);
		GenerateStmt(stmt->Body);
		MarkLabel(lbTest);
		GenerateExpr(stmt->Condition);
		Emit( OP_JT,0,lbTrue);
	}
	//如是输出语句
	else if ((*theStmt).type== St_Write)
	{
		Write *stmt =(Write *)theStmt;
		this->GenerateExpr(stmt->Expr);
		this->Emit( OP_OUT,0,"");
	}
	//如是输入语句
	else if ((*theStmt).type== St_Read)
	{
		Read *stmt =(Read *)theStmt;
		this->Emit( OP_IN,0,"");
		this->Emit( OP_ST,0,stmt->Ident);
	}
	//变量定义
	else if ((*theStmt).type== St_Declare)
	{
		Declare *stmt =(Declare *)theStmt; 
		GenerateExpr((*stmt).Expr);
		Emit( OP_ST,0,(*stmt).Ident);
		VarTable.push_back(CurrEtrName+":"+stmt->Ident);
	}
	//赋值
	else if ((*theStmt).type== St_Assign)
	{
		Assign *stmt =(Assign *)theStmt;
		if(!ChkVar(CurrEtrName+":"+stmt->Ident))
		{
			OutMessage::Out(stmt->Line,"变量"+stmt->Ident+"未定义");
			return 1;
		}
		if ((*stmt).op == "+=" || (*stmt).op == "-=" || (*stmt).op == "*="|| (*stmt).op == "/=")
		{
			BinExpr *binExpr=new BinExpr();
			Variable *var=new Variable();
			(*var).Ident = (*stmt).Ident;
			(*var).type= Et_Var;
			(*binExpr).Left = var;
			(*binExpr).Op = (*stmt).op.substr(0,1);
			(*binExpr).Right = (*stmt).Expr;
			(*binExpr).type= Et_BinExpr;
			(*stmt).Expr = binExpr;
		}
		GenerateExpr((*stmt).Expr);
		Emit( OP_ST,0,(*stmt).Ident);
	}
	//返回值
	else if ((*theStmt).type== St_Return)
	{
		Return *stmt =(Return *)theStmt;
		this->GenerateExpr(stmt->Expr);
		this->Emit( OP_RET,0,"");
	}
	//调用函数语句
	else if ((*theStmt).type== St_CallFunc)
	{
		CallFunction *stmt =(CallFunction *)theStmt;
		if(!ChkFun(stmt->FunctionFullName))
		{
			OutMessage::Out(stmt->Line,"函数"+stmt->FunctionFullName+"未定义");
			return 1;
		}
		int argSize =(*stmt).ParExprs->size();
		for(int i=0;i<argSize;i++)
		{
			this->GenerateExpr((*(stmt->ParExprs))[i]);
		}
		this->Emit( OP_CALL,0,stmt->FunctionFullName);
	}
	else
	{
		//OutMessage::Out(typeid(theStmt).name());
		OutMessage::Out("无法生成的语义");
	}
	delete(theStmt);
	return 0;
}

