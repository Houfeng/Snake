
/*
描述:扫描器(其中汇编器,及脚本编译器,共用些扫描器)
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#include "Scaner.h"

/*析构*/
Scanner::~Scanner()
{
	TokenCollection.clear();
}
/*通过源码构造一个扫描器*/
Scanner::Scanner(string srcCode )
{
	code=srcCode; 
	line=1;
	size=code.size();
	index=0;
	if(index<size)
		this->Scan(code);
	else
		OutMessage::IsEof(0);
}
//扫描
void Scanner::Scan(string code)
{
	while (NotEof())
	{
		char ch=code[index];
		if(CharExt::IsWhiteSpace(ch))
		{
			HandleWhiteSpace();
		}
		else if(CharExt::IsAlpha(ch) || ch == '_' || ch == '$' || CharExt::IsUnicode(ch))
		{
			HandleAlnum();
		}
		else if(ch == '"')
		{
			HandleString();
		}
		else if(CharExt::IsDigit(ch))
		{
			HandleDigit();
		}
		else if(CharExt::IsPunct(ch))
		{
			HandlePunct();
		}
		else
		{
			OutMessage::UnknownCharacter(line,Convert::ToString(ch));
		}
		++index;
	}
}
//===========================
//不是文件尾
bool Scanner::NotEof()
{
	return index<size;
}
//不是文件尾
bool Scanner::NotEof(long ix)
{
	return ix<size;
}
//===========================
//空白的字符
void Scanner::HandleWhiteSpace()
{
	if(code[index]=='\n')
		line++;
}
//标识符
void Scanner::HandleAlnum()
{
	string buffer="";
	buffer+=code[index];
	while (NotEof(index+1) && ( 
		CharExt::IsAlnum(code[index+1]) 
		|| code[index+1] == '_' 
		|| code[index+1] == '$'
		|| CharExt::IsUnicode(code[index+1])
		))
	{
		buffer+=code[++index];
	}
	/*if(TokenCollection[TokenCollection.size()-1].String==".")
	{
		Token tk(buffer,TT_IdentOrKeyword,line);
		TokenCollection.push_back(tk);
	}
	else*/
	{
		Token tk(buffer,TT_IdentOrKeyword,line);
		TokenCollection.push_back(tk);
	}
}
//数字
void Scanner::HandleDigit()
{
	string buffer="";
	buffer+=code[index];
	while (NotEof(index+1) && ( CharExt::IsDigit(code[index+1]) || code[index+1]=='.') )
	{
		buffer+=code[++index];
	}
	Token tk(buffer,TT_Number,line);
	TokenCollection.push_back(tk);
}
//字符串
void Scanner::HandleString()
{
	string buffer="";
	index++;//跳过开始的引号
	//buffer+=code[index];
	while (NotEof(index) && code[index]!='\n' && code[index]!='"' )
	{
		buffer+=code[index++];
	}
	//index++;//跳到结束的引号
	if(!NotEof(index) || code[index]=='\n')
		OutMessage::StringDoesNotEnd(line,buffer);
	Token tk(buffer,TT_String,line);
	TokenCollection.push_back(tk);
}
//检查一个字串是不是一个操符
bool Scanner::IsOperator(string str)
{
	if(str=="+"
		|| str=="-"
		|| str=="*"
		|| str=="/"
		//
		|| str=="+="
		|| str=="-="
		|| str=="*="
		|| str=="/="
		//
		|| str=="<"
		|| str==">"
		|| str=="=="
		|| str=="!="
		|| str=="<="
		|| str==">="
		//
		|| str=="!"
		|| str=="&"
		|| str=="|"
		//
		|| str=="="
		|| str=="."//取成员
		)
		return true;
	else
		return false;
}
//处理符号
void Scanner::HandlePunct()
{
	if(code[index]!='#')
	{
		string buffer="";
		buffer+=code[index];
		if(NotEof(index+1) && code[index+1]=='=' && (
			code[index]=='+'
			|| code[index]=='-'
			|| code[index]=='*'
			|| code[index]=='/'
			|| code[index]=='!'
			|| code[index]=='='
			|| code[index]=='<'
			|| code[index]=='>'
			))
		{
			buffer+=code[++index];
		}
		if(IsOperator(buffer))
		{
			Token tk(buffer,TT_Operator,line);
			TokenCollection.push_back(tk);
		}
		else
		{
			Token tk(buffer,TT_Other,line);
			TokenCollection.push_back(tk);
		}
	}
	else
	{
		while (NotEof(index+1) && code[index+1]!='\n' )
		{
			index++;
		}
		line++;
		index++;//跳到 \n
	}
}

