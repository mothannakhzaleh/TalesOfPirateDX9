#ifndef MPStringUtil_H
#define MPStringUtil_H

#include "MPEffPrerequisites.h"
#include "MPStringVector.h"

class MINDPOWER_API MPStringUtil
{
public:
	typedef std::stringstream StrStreamType;

public:
	/** 移去任何空格字符, 标准空格包括空格或Tabsbe等等.
	@remarks
	可以指定是否移去前端或后面的空格，默认都移去.
	*/
	static void trim( String& str, bool left = true, bool right = true );

	/** 返回MPStringVector，包括在分隔符之间的所有子串.
	@param 
	delims 分隔符列表
	@param 
	maxSplits 执行分隔的最大次数。如果为0则无限次分隔，如果分隔次数大于0
	则分隔到的子串数量达到这个数为止.
	*/
	static MPStringVector split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

	/** 将字符串中的字符全部转换成小写.
	*/
	static void toLowerCase( String& str );

	/** 将字符串中的字符全部转换成大写写.
	*/
	static void toUpperCase( String& str );

};
#endif