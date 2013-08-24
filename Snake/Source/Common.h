/*
描述:公共方法
作者:Houfeng
电邮:admin@xhou.net
------------------------------------------
修改记录:
*/


#ifndef  __Common_h_
#define  __Common_h_


#include "System.h"
#include "Convert.h"
#include "OutMessage.h"
#include "Define.h"


/*
公共静态功能类
*/
class Common
{
public:
	//从一个文本文件中读取所有字符
	static string ReadAllText(string filePath)
	{
		ifstream file;
		file.open(filePath.c_str());
		if(file)
		{
			ostringstream tmp; 
			tmp <<file.rdbuf(); 
			string textBuffer= tmp.str(); 
			file.close();
			return textBuffer;
		}
		else
		{
			OutMessage::Out("不能打开文件:'"+filePath+"'");
			return "";
		}
	}
	//写一个文本文件
	static int WriteAllText(string filePath,string content)
	{
		ofstream file;
		file.open(filePath.c_str());
		if(file)
		{
			file.write(content.c_str(),content.size());
			file.close();
		}
		else
		{
			OutMessage::Out("不能写入文件:'"+filePath+"'");
			return 1;
		}
		return 0;
	}
	//字符串替换
	static void string_replace(string&s1,const string&s2,const string&s3)
	{
		string::size_type pos=0;
		string::size_type a=s2.size();
		string::size_type b=s3.size();
		while((pos=s1.find(s2,pos))!=string::npos)
		{
			s1.replace(pos,a,s3);
			pos+=b;
		}
	}
	//字符串转小写
	static void string_tolower( string &src )
	{
		use_facet< ctype<char> >( locale() ).tolower(
			&(*src.begin()), &(*src.end()) );
	}
	// 处理转义字符
	static string HandleEscape(string str)
	{
		string newStr = str;
		Common::string_replace( newStr,"\\\\", "\\");
		Common::string_replace(newStr,"\\r", "\r");
		Common::string_replace(newStr,"\\n", "\n");
		Common::string_replace(newStr,"\\t", "\t");
		Common::string_replace(newStr,"\\v", "\v");
		Common::string_replace(newStr,"\\b", "\b");
		Common::string_replace(newStr,"\\f", "\f");
		Common::string_replace(newStr,"\\a", "\a");
		Common::string_replace(newStr,"\\'", "\'");
		Common::string_replace(newStr,"\\\"", "\"");
		return newStr;
	}
	//处理转义字符
	static char* HandleEscape(const char* str)
	{
		string s=Convert::ToString(str);
		string rs=Common::HandleEscape(s);
		return  Convert::ToCharPtr(rs.c_str());
	}
	//从控制台读入一行
	static string ReadLine()
	{
		string str;
		cin.clear();
		cout.clear();
		std::getline(cin,str);
		cin.clear();
		cout.clear();
		return str;
	}
	//取文件扩展名
	static string GetFileExtName(string fullName)
	{
		long start=fullName.find_last_of('.');
		return fullName.substr(start);
		//
		/* char path_buffer[_MAX_PATH];
		    char drive[_MAX_DRIVE];
		    char dir[_MAX_DIR];
		    char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath(fullName.c_str(),NULL,NULL,NULL,ext);
			return string(ext);
		*/
	}
	//取文件名不含扩展名
	static string GetFileShortFileNmae(string fullName)
	{
		long start=fullName.find_last_of('/')+1;
		long end=fullName.find_last_of('.')-start;
		return fullName.substr(start,end);
	}
	//取文件名路径不含文件名及扩展名
	static string GetFilePath(string fullName)
	{
		long end=fullName.find_last_of('/');
		return fullName.substr(0,end);
	}
};

#endif