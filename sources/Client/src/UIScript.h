//----------------------------------------------------------------------
// 名称:UI脚本类
// 作者:lh 2004-10-26
// 用途:辅助类,用于支持其它对象能够用于外部脚本
//   注:目前不能在程序中动态删除
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once
#include "uiguidata.h"

namespace GUI
{

template <class T>
class UIScript
{
public:
	T*		GetObj( unsigned int nIndex )	{ if(nIndex>=list.size()) return NULL;	return list[nIndex];	}
	int		AddObj( T* p )					{ list.push_back(p); return (int)(list.size()-1);				}

private:
	typedef std::vector<T*> vt;
	vt		list;

};

}
