/*
描述:输出消息
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#ifndef __OutMessages_h_
#define __OutMessages_h_

#include "System.h"
#include "Convert.h"
#include "Define.h"


class OutMessage
{
public:
	//向用户输出一个信息
	static void Out(string msg)
	{
		std::cout<<msg<<"\r\n";
		//printf("\r\n{0}\r\n",msg.c_str()); 
	}
	static void Usage()
	{
		string usage="";
		usage+=("Snake v0.0.1 By Houfeng\r\n");
		usage+=("Snake filename		: 执行(.sa文件;.sn文件;.se文件)\r\n");
		usage+=("Snake /c filename.sn	: 编译源文件\r\n");
		usage+=("Snake /console		: 进入控制台模式\r\n");
		OutMessage::Out(usage);
	}
	//向用户输出一个信息,并提示行号
	static void Out(long line,string msg)
	{
		OutMessage::Out("在"+Convert::ToString(line)+"行: "+msg);
	}
	//缺少表达式
	static void NoExpression(long line)
	{
		OutMessage::Out(line,"缺少表达式");
	}
	//不识别的字符
	static void UnknownCharacter(long line,string str)
	{
		OutMessage::Out(line,"不能识别的字符 '"+str+"'");
	}
	//未结束的字符串
	static void StringDoesNotEnd(long line,string str)
	{
		OutMessage::Out(line,"未结束字符串 '"+str+"' ");
	}
	//设置控制台标题
	static void SetSnakeConsoleTitle()
	{
		#if(PLATFORM==WINDOWS)
			system("title Snake控制台"); 
		#endif
	}
	//设置控制台标题
	static void SetSnakeAsmConsoleTitle()
	{
		#if(PLATFORM==WINDOWS)
			system("title Snake控制台-ASM"); 
		#endif
	}
	static void ClearConsole()
	{
		cin.clear();
		cout.clear();
		#if(PLATFORM==WINDOWS)
			system("CLS");
		#else
			system("clear");
		#endif
	}
	//文件结束
	static void IsEof()
	{
		OutMessage::Out("已到文件尾");
	}
		//文件结束
	static void IsEof(long line)
	{
		OutMessage::Out(line,"已到文件尾");
	}
	//符号标记
	static void IsTokenEof()
	{
		OutMessage::Out("已到符号标记尾");
	}
	//符号标记
	static void IsTokenEof(long line)
	{
		OutMessage::Out(line,"已到符号标记尾");
	}
};

#endif