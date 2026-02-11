
////////////////////////////////////////
//
//      加密/解密
//      by jampe
//      2006/5/26
//
////////////////////////////////////////

#if !defined __ENC_LIB__
#define __ENC_LIB__


typedef unsigned char       __byte;


//  设置密钥
//  key为 8位密钥
extern int SetEncKey(const __byte* key);


//  加密
//  buf 输出缓存
//  len 输出缓存长度
//  pwd 待加密密码
//  plen 密码长度
//  返回加密后的字符串长度
extern int Encrypt(__byte* buf, int len, const __byte* pwd, int plen);


//  解密
//  buf 输出缓存
//  len 输出缓存长度
//  enc 待解密密钥
//  elen 密钥长度
//  返回加密后的字符串长度
extern int Decrypt(__byte* buf, int len, const __byte* enc, int elen);



#if NDEBUG
#pragma comment(lib, "EncLib.lib")
#else
#pragma comment(lib, "EncLib_d.lib")
#endif


#endif  /*__ENC_LIB__*/
