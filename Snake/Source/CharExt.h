/*
描述:字符功能扩展
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/
#ifndef  __CharExt_h_
#define  __CharExt_h_

#include "System.h"

class CharExt
{
public:
	//是否是空白字符
	static bool IsWhiteSpace(char ch)
	{
	   return (bool)isspace(ch);
	}
	//是否是字母
	static bool IsAlpha(char ch)
	{ 
		return (bool)isalpha(ch);
	}
	//是否是数字
	static bool IsDigit(char ch)
	{
		return (bool)isdigit(ch);
	}
	//如果是符号
	static bool IsPunct(char ch)
	{
		return (bool)ispunct(ch);
	}
	//是否是Unicode字符
	static bool IsUnicode(char ch)
	{
		return (bool)(ch<0);
	}
	//是否是数字或字母
	static bool IsAlnum(char ch)
	{
		return (bool)isalnum(ch);
	}
};

#endif