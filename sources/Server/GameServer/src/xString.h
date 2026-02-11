#ifndef XSTRING_H
#define XSTRING_H

#include "TChar.h"

// 从给定字符窜(_TCHAR *in)的给定位置(long *in_from)获取字符窜，
// 直到遇到字符窜列表(_TCHAR *end_list)中的任一字符时结束
long StringGet(_TCHAR *out, long out_max, _TCHAR *in, long *in_from, _TCHAR *end_list, long end_len);


// 从给定字符窜(_TCHAR *in)的给定位置(long *in_from)剔除字符窜列表(_TCHAR *end_list)中的任一字符
void StringSkipCompartment(_TCHAR *in, long *in_from, _TCHAR *skip_list, long skip_len);





















#endif
