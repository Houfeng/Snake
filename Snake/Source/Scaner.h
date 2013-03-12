
/*
描述:扫描器(其中汇编器,及脚本编译器,共用些扫描器)
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

#pragma once

#include "System.h"
#include "Common.h"
#include "OutMessage.h"
#include "CharExt.h"
#include "Convert.h"
#include "Token.h" 
//#include "Operator.h"


class Scanner
{
public:
	std::vector<Token> TokenCollection;
	Scanner(string srcCode );
	~Scanner();
	void Scan(string code);
private:
	//源
	string code;
	//源长
	long size;
	//源当前游标位置
	long index;
	//行
	long line;
	bool NotEof();
	bool NotEof(long ix);
	void HandleWhiteSpace();
	void HandleAlnum();
	void HandleDigit();
	void HandleString();
	bool IsOperator(string str);
	void HandlePunct();
};