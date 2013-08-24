/*
描述:定义栈结构
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
2010-8-19 添加注释;
*/

#include "Stack.h"


Stack::Stack(){}
Stack::~Stack(){
	_Stack.clear();
}
//-------------------------------------
int Stack::Clear()
{
	_Stack.clear();
	return 0;
}
int Stack::Init()
{
	Size=sizeof(_Stack);
	Pos=-1;
	return 0;
}
int Stack::IsEmpty()
{
	if(_Stack.empty())
		return 1;
	else
		return 0;
}
Object Stack::Push( Object  obj )
{
	if(obj.type==T_STRING)
		obj.value.str=Common::HandleEscape(obj.value.str);
	_Stack.push_back(obj);
	Size=_Stack.size();
	Pos=Size-1;
	return obj;
}
Object Stack::Pop()
{
	if( Pos<0 || Pos >= Size )
	{
		OutMessage::Out("栈溢出");
		return Null;
	}
	else if (IsEmpty())
	{
		return Null;
	}
	return _Stack[Pos--];
}
Object Stack::Top()
{
	if(Pos<0 || Pos >= Size )
	{
		OutMessage::Out("栈溢出");
		return Null;
	}
	else if (IsEmpty())
	{
		return Null;
	}
	return _Stack[Pos];
}
Object Stack::Get(int pos)
{
	if(pos<0 || pos >= Size )
	{
		OutMessage::Out("不可访问的栈空间");
		return Null;
	}
	else if (IsEmpty())
	{
		return Null;
	}
	return _Stack[pos];
}
Object Stack::Set(int pos, Object  obj)
{
	if(pos<0 || pos >= Size )
	{
		OutMessage::Out("不可访问的栈空间");
		return Null;
	}
	else if (IsEmpty())
	{
		return Null;
	}
	_Stack[pos]=obj;
	return obj;
}
