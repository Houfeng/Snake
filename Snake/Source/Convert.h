/*
描述:转换
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#ifndef __Convert_h_
#define __Convert_h_

#include "System.h"


class Convert
{
public:
	static string ToString(long num)
	{
		std::string str;
		std::stringstream sstr;
		sstr<<num; 
		sstr>>str;
		sstr.clear();
		return str;
	}
	static string ToString(double num)
	{
		std::string str;
		std::stringstream sstr;
		sstr.unsetf(ios::scientific);
		int prec=18;
		sstr.precision(prec);
		sstr<<num;
		sstr>>str;
		sstr.clear();
		return str;
	}
	static string ToString(char ch)
	{
		std::string str;
		std::stringstream sstr;
		sstr<<ch;
		sstr>>str;
		sstr.clear();
		return str;
	}
	static string ToString(char* ch)
	{
		std::string str(ch);
		return str;
	}
	static string ToString(const char* ch)
	{
		std::string str(ch);
		return str;
	}
	static long ToLong(string str)
	{
		long num;
		std::stringstream sstr;
		sstr<<str;
		sstr>>num;
		sstr.clear();	
		return num;
	}
	static double ToDouble(string str)
	{
		double num;
		std::stringstream sstr;
		sstr<<str;
		sstr>>num;
		sstr.clear();	
		return num;
	}
	static char* ToCharPtr(const char* str)
	{
		char* pc=new char[strlen(str)+1];
		strcpy(pc,str);
		return pc;
	}
};

#endif