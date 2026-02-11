
// pch.h
//  created by claude fan at 2004-8-31
//  for precompiled-header

#if !defined(LOGUTIL_PRECOMPILED_HEADER_FILE)
#define LOGUTIL_PRECOMPILED_HEADER_FILE

// 平台无关宏定义
#define LINE_COMMENT /##/
#define LC LINE_COMMENT
#define $ LINE_COMMENT

// C标准库
#include <stdio.h>
#include <time.h>

// C++标准库
#include <list>
#include <map>
#include <list>
#include <string>

// 平台相关定义
#if defined(WIN32)
	// Win32平台所需定义和包含
    #pragma warning(disable : 4251)
    #pragma warning(disable : 4786)

	#include <windows.h>
	#include <io.h>
	#include <direct.h>

#elif defined(LINUX)
	// Linux平台所需定义和包含
    #include <unistd.h>
    #include <string.h>
    
    #include <sys/io.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <sys/stat.h>
    #include <sys/dir.h>
    
    #define VOID    void
    #define BYTE    unsigned char
    #define LPBYTE  unsigned char*
    #define BOOL    bool
    #define DWORD   unsigned long
    #define LPCSTR const char*
    #define TRUE true
    #define FALSE false
    #define HANDLE void*

	#define _LOG
	#define _LOG_CONSOLE
#elif defined(FREEBSD)
	// FreeBSD平台所需定义和包含

#endif


#endif
