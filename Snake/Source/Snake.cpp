/*
描述:入口函数
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/

/*
文件格式说明(暂定):
.sn 源文件
.sa Snake汇编源文件
.se 字节文件
.sl  库文件
*/

#include "System.h"
#include "Common.h"
#include "OutMessage.h"
#include "Scaner.h"
#include "Parser.h"
#include "Generator.h"
#include "Asm.h"
#include "VM.h"
   

int ConsoleMode();
int main(int argc, char *argv[], char *env[])
{
	string snakeMode="";//是否是编译
	string filePath="";//文件路径，可是源文件路径，也可是执行文件路径
	//################################################################
	if(argc>1)
		snakeMode=argv[1];
	if(argc>2)
		filePath=argv[2];
	//################################################################
	 if(snakeMode=="/console")//交互
	{
		ConsoleMode();
		return 0;
	}
	else if(snakeMode=="/c" && filePath!="")//编译
	{
		string fname=Common::GetFileShortFileNmae(filePath);
		string ext=Common::GetFileExtName(filePath);
		if(ext==".sn")
		{
			string codeBuffer=Common::ReadAllText(filePath);
			Scanner scanner(codeBuffer);
			Parser parser(scanner.TokenCollection);
			Generator generator(parser.StatementTree);
			Common::WriteAllText(fname+".sa",generator.AsmCode);
		}
		else
		{
			OutMessage::Out("不接受的源文件格式"+ext+"只能是.sn(源文件)");
		}
		return 0;
	}
	else 	if( snakeMode!=""&& filePath=="")//运行
	{
		filePath=snakeMode;
		string codeBuffer=Common::ReadAllText(filePath);
		string ext=Common::GetFileExtName(filePath);
		if(ext==".sn" || ext==".sa")
		{
			if(ext==".sn"){
				Scanner scanner(codeBuffer);
				Parser parser(scanner.TokenCollection);
				Generator generator(parser.StatementTree);
				codeBuffer=generator.AsmCode;
			}
			Scanner asmScanner(codeBuffer);
			Asm pAsm(asmScanner.TokenCollection);
			VM vm;
			vm.LoadByteCode(pAsm.Procs);
			vm.Start(); 
		}
		else
		{
			OutMessage::Out("不接受的文件格式"+string(ext)+"只能是.sn(源文件)或.sa(汇编源文件)");
		}
		return 0;
	}
	else
	{
		OutMessage::Usage();
		return 0;
	}
	//################################################################
	return 0;
}

/*进入控制台模式*/
int ConsoleMode()
{
		bool IsAmsConsoldMode=false;
	    OutMessage::SetSnakeConsoleTitle();
		int lineNum=0;
		string codeBuffer="";
		OutMessage::ClearConsole();
		while (true)
		{
			printf("%03d> ",++lineNum);
			string inp="";
			inp=Common::ReadLine();
			if(inp=="/exit")//退出
			{
				if(IsAmsConsoldMode)//如是是Asm模式则返回普通控制台模式
				{				
					lineNum=0;
					codeBuffer="";
					OutMessage::ClearConsole();
					OutMessage::SetSnakeConsoleTitle();
					IsAmsConsoldMode=false;
				}
				else //退出控制台模式
				{
					OutMessage::ClearConsole();
					exit(0);
				}
			}
			if(inp=="/asm")//进入asm控制台模式
			{				
				lineNum=0;
				codeBuffer="";
				OutMessage::ClearConsole();
				OutMessage::SetSnakeAsmConsoleTitle();
				IsAmsConsoldMode=true;
			}
			else if(inp=="/run")//执行
			{
				cin.clear();
				cout.clear();
				cout<<endl;
				if(IsAmsConsoldMode)//按asm执行
				{
						Scanner scanner(codeBuffer);
						Asm pAsm(scanner.TokenCollection);
						VM vm;
						vm.LoadByteCode(pAsm.Procs);
						vm.Start();
				}
				else//普能执行
				{
					Scanner scanner(codeBuffer);
					Parser parser(scanner.TokenCollection);
					Generator generator(parser.StatementTree);
					Scanner asmScanner(generator.AsmCode);
					Asm pAsm(asmScanner.TokenCollection);
					VM vm;
					vm.LoadByteCode(pAsm.Procs);
					vm.Start();
				}
				lineNum=0;
				codeBuffer="";	
				cout<<endl<<endl;
				cin.clear();
				cout.clear();
			}
			else if(inp=="/cls")//清屏
			{
				lineNum=0;
				codeBuffer="";
				OutMessage::ClearConsole();
			}
			else //暂存输入源码
			{
				codeBuffer+=inp+"\n";
			}
		}
}

