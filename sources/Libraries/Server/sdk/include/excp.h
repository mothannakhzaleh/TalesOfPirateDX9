#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include "DBCCommon.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)

//------------------------------------------------------------------------------------------------------------------
//common exception define
class excp :public std::runtime_error			//基异常类
{
public:
	excp(const char* desc) : std::runtime_error(desc) {}
};
//------------------------------------------------------------------------------------------------------------------
class excpMem:public excp				//内存分配或释放异常
{
public:
	excpMem(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpArr:public excp				//数组越界或其他数组相关错误异常
{
public:
	excpArr(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpSync:public excp				//操作系统同步对象操作异常
{
public:
	excpSync(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpThrd:public excp				//操作系统线程操作异常
{
public:
	excpThrd(cChar * desc):excp(desc){};
};
class excpSock:public excp				//操作系统线程操作异常
{
public:
	excpSock(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpCOM:public excp				//COM操作异常
{
public:
	excpCOM(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpDB:public excp				//数据库操作异常
{
public:
	excpDB(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpIniF:public excp				//文件操作异常
{
public:
	excpIniF(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpFile:public excp				//文件操作异常
{
public:
	excpFile(cChar * desc):excp(desc){};
};
//------------------------------------------------------------------------------------------------------------------
class excpXML:public excp				//文件操作异常
{
public:
	excpXML(cChar * desc):excp(desc){};
};

#pragma pack(pop)
_DBC_END

//===================================================================================================================================

//From: https://stackoverflow.com/questions/2670816/how-can-i-use-the-compile-time-constant-line-in-a-string
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

#define THROW_EXCP(EXCP, DESC) throw EXCP((std::string("File:") + __FILE__ + " Line:" + LINE_STRING + " desc:" + DESC).c_str());