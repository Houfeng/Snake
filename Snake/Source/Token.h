/* 
描述:符号标记定义(扫描器用到)
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#ifndef  __Token_h_
#define __Token_h_

#include "System.h"
#include "Common.h"


//符号标记枚举
enum TokenType{
	TT_IdentOrKeyword,
	TT_Number,
	TT_String,
	TT_Operator,
	TT_Other
};

//符号标记
class Token
{
public:
	string String;
	TokenType Type;
	long Line;
	Token(string tokenString,TokenType tokenType,long line)
	{
		String=tokenString; 
		Type=tokenType;
		Line=line; 
	}
};

#endif