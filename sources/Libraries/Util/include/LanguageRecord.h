
//
//	多语言字符串记录类
//
//	add by Philip.Wu  2006-07-18
//

#pragma once

#include <fstream>
#include <string>
#include <map>

#include "enclib.h"


class CLanguageRecord
{
public:
	CLanguageRecord(void);
	CLanguageRecord(const char* szBinFile, const char* szTxtFile);

	~CLanguageRecord(void);

	// 获得字符串
	const char* GetString(int nID);

	// 从文件中读取
	bool LoadFromBinFile(const char* szBinFile);

	// 从文件中读取
	bool LoadFromTxtFile(const char* szTxtFile);

	// 获得字符串个数
	int GetRecordCount(void);

	// 加密并生成二进制
	bool MadeBinFile(const char* szBinFile, const char* szTxtFile);

private:

	// 字符串 map
	std::map<int, std::string> m_mapString;

	// 添加一行
	void Add(char* szLine);

	// 替换
	int ReplaceString(char* _str, const char* _old, const char* _new);

	// 查找
	int Find(const char* _str, const char* _find);
};

