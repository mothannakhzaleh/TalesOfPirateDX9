#pragma once

// Raw Data : 原始数据
// Raw Data Set : 适用于实际数据只有一份, 使用该数据的实例却可以创建多个的应用场合
// 例如 : Mesh模型数据, 贴图数据, 骨架数据等等,  也可以用在非游戏的各种软件里

// RawDataSet类的主要功能
//1. 原始数据的资源描述读入(文本,二进制)
//2. 通过ID索引数据
//3. 动态释放
//4  资源打包和包数据存取

// 目前的应用还只能为一些资源描述格式较为简单的数据做基类接口
// 例如:  ID  数据来源(文件名) 简单参数集合 这类的格式

// 索引方式:
// 类型ID = 数组下标
// 通过类型ID索引到原始数据


// 子类在使用前, 必须继承以下的方法
// virtual int				_GetRawDataInfoSize()										      // 每个子类的RawDataInfo都有不同, 取得RawDataInfo的数据尺寸
// virtual void*			_CreateNewRawData(CRawDataInfo *pRawInfo)		    		      // 取得新的RawData数据, 可能模型数据指针，也可以是贴图表面指针
// virtual void				_ReadRawDataInfo(CRawDataInfo *pRawInfo, list<string> &ParamList) // 资源文件每读到新的一行, 子类可以得到的回调处理
// virtual void				_DeleteRawData(void *pData);								      // 删除资源, 资源的删除方式可能子类各有不同	

// 另外, 子类必须在自己的构造里调用_Init()函数


#include <fstream>
#include <cryptlib.h>
#include <aes.h>
#include <filters.h>
#include <gcm.h>
#include <base64.h>
#include <util2.h>
#include <log.h>

class CRawDataInfo
{
public:
	BOOL	bExist{ false };				// 资源是否存在
	int		nIndex{ 0 };				// 在Array中的位置				
	char	szDataName[72]{ "" };		// 数据来源(通常是数据文件名)
	DWORD	dwLastUseTick{ 0 };		// 上次使用的时间
	BOOL	bEnable{ true };			// 是否有效, 可以动态设置
	void* pData{ nullptr };				// 实际数据
	DWORD   dwPackOffset{ 0 };		// 在包文件中的数据偏移
	DWORD   dwDataSize{ 0 };			// 原始数据尺寸(文件尺寸)
	int     nID{ 0 };				// ID
	DWORD   dwLoadCnt{ 0 };          // 资源读取次数
};



class CRawDataSet
{

protected:

	CRawDataSet(int nIDStart, int nIDCnt, int nFieldCnt = DEFAULT_FIELD_CNT) // 一定要继承使用
	:_nIDStart(nIDStart),
	_nIDCnt(nIDCnt),
    _nIDLast(nIDCnt)
	{
		_nMaxFieldCnt = std::max<decltype(_nMaxFieldCnt)>(nFieldCnt, DEFAULT_FIELD_CNT);
   	}

public:

    void*			GetRawData(int nID, BOOL bRequest = FALSE);
	void*			GetRawData(const char* pszDataName, int *pnID);
	CRawDataInfo*	GetRawDataInfo(int nID);
    CRawDataInfo*   GetRawDataInfo(const char *pszDataName);
    int             GetRawDataID(const char *pszDataName);
	
	BOOL			LoadRawDataInfo(const char *pszFileName, BOOL bBinary = FALSE);
	BOOL			LoadRawDataInfoEx(const char *pszFileName, BOOL bBinary = FALSE);
	void			WriteBinFile(const char *pszFileName);
	
	BOOL			IsValidID(int nID);
    int             GetLastID() const {return _nIDLast;}
	
	// 对于动态释放的参数设置
    void			SetReleaseInterval(DWORD dwInterval)	{ _dwReleaseInterval = dwInterval;	}
	void			SetMaxRawData(int nDataCnt)				{ _nMaxRawDataCnt	 = nDataCnt;	}
	
	int				GetLoadedRawDataCnt()					{ return _nLoadedRawDataCnt;		}
	void			DynamicRelease(BOOL bClearAll = FALSE);
	void			Release();
    void            FrameLoad(int nFrameLoad = 2);

	// 打包有关
    void			EnablePack(const char *pszPackName);	// 仅在读入二进制资源描述文件后有效
	void            Pack(const char *pszPackName, const char *pszBinName);
    void			PackFromDirectory(std::list<std::string> &DirList, const char *pszPackName, const char *pszBinName);
    BOOL            IsEnablePack()              { return _bEnablePack; } 
	
    // 资源读取
    LPBYTE			LoadRawFileData(CRawDataInfo *pInfo);
	
    void            EnableRequest(BOOL bEnable)   { _bEnableRequest = bEnable; }
	
protected:

	virtual CRawDataInfo*	_CreateRawDataArray(int nIDCnt)								    = 0;	
	virtual void			_DeleteRawDataArray()										    = 0;	
	virtual int             _GetRawDataInfoSize()										    = 0;
	virtual void*			_CreateNewRawData(CRawDataInfo *pInfo)					  	    = 0;
	virtual BOOL			_ReadRawDataInfo(CRawDataInfo *pInfo, std::vector<std::string> &ParamList)= 0;
	virtual void			_ProcessRawDataInfo(CRawDataInfo *pInfo)                        {}
	virtual void			_DeleteRawData(CRawDataInfo *pInfo)							    = 0;
	virtual BOOL			_IsFull()
	{
		if(_nLoadedRawDataCnt <= _nMaxRawDataCnt) return FALSE;
		return TRUE;
	}
	virtual void            _AfterLoad(){}

	BOOL		_LoadRawDataInfo_Bin(const char *pszFileName);
	BOOL		_LoadRawDataInfo_Txt(const char *pszFileName, int nSep = '\t');
	void		_WriteRawDataInfo_Bin(const char *pszFileName);
    void        _Init();
    CRawDataInfo*	_GetRawDataInfo(int nID); // 不作索引范围检测

protected:
	
	int						    _nIDStart;
	int						    _nIDCnt;
	int						    _nUnusedIndex{ 0 };
	DWORD					    _dwReleaseInterval{1000 * 60 * 1};
	int						    _nMaxRawDataCnt{ 50 };
	int						    _nLoadedRawDataCnt{ 0 };
	DWORD                       _dwMaxFrameRawDataSize{ 0 };
	BOOL				    	_bEnablePack{ false };
	char                        _szPackName[64];
	BOOL						_bBinary{ false };
	CRawDataInfo* _RawDataArray{ nullptr };
    std::map<std::string, CRawDataInfo*>	_IDIdx;
    std::list<int>                   _RequestList;
	BOOL                        _bEnableRequest{ false };
    int                         _nIDLast;

	// add by claude
	enum {DEFAULT_FIELD_CNT = 80};
	int _nMaxFieldCnt;
};

inline void CRawDataSet::_Init()
{
    _DeleteRawDataArray();

	_RawDataArray = _CreateRawDataArray(_nIDCnt);

	LPBYTE pbtData = (LPBYTE)_RawDataArray;
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * i);
		pInfo->nIndex = i;
		pInfo->nID    = _nIDStart + i;
	}
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(int nID)
{
	if (!IsValidID(nID)) return NULL;

    CRawDataInfo* pInfo = _GetRawDataInfo(nID);
	if (pInfo->bExist) return pInfo;
    else return NULL;
}

// 不作索引范围检测，不要单独调用
inline CRawDataInfo* CRawDataSet::_GetRawDataInfo(int nID)
{
    LPBYTE pbtData = (LPBYTE)_RawDataArray;

    CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * (nID - _nIDStart));
    return pInfo;
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(const char *pszDataName)
{
	std::map<std::string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // 此ID已经存在
	{
		return (*it).second;
    }
    return NULL;
}

inline void* CRawDataSet::GetRawData(int nID, BOOL bRequest)
{
	CRawDataInfo *pInfo =  GetRawDataInfo(nID);
	if(pInfo==NULL) return NULL;
	
	pInfo->dwLastUseTick = GetTickCount();
	if(!pInfo->pData)
	{
	    if(bRequest && _bEnableRequest)
        {
            LG2("debug", "Push Request RawData!\n");
            _RequestList.push_back(nID);
            return NULL;
        }
        
        pInfo->pData = _CreateNewRawData(pInfo);
        pInfo->dwLoadCnt++;
		if(pInfo->pData==NULL)
		{
			LG2("error", "Load Raw Data [%s] Failed! (ID = %d)\n", pInfo->szDataName, nID);
		}
		else
		{
			_nLoadedRawDataCnt++;
		}
	}
	return pInfo->pData;
}

inline int CRawDataSet::GetRawDataID(const char *pszDataName) // 从名字获取ID, 如果没有则分配一个
{
	CRawDataInfo *pInfo;

    std::map<std::string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // 此ID已经存在
	{
		pInfo = (*it).second;
	}
	else
	{
		if(_nUnusedIndex >= _nIDCnt)
		{
			LG2("error", "RawDataSet OverMax Dynamic ID, MaxIDCnt = %d, Index = %d\n", _nIDCnt, _nUnusedIndex);
			return -1;
		}
		
		LPBYTE pbtData = (LPBYTE)_RawDataArray;
		pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * _nUnusedIndex);
		strcpy(pInfo->szDataName, pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;
		_nUnusedIndex++;
	}
	return pInfo->nIndex + _nIDStart;
}


inline void* CRawDataSet::GetRawData(const char* pszDataName, int *pnID)
{
	int nID = GetRawDataID(pszDataName);
    if(pnID) *pnID = nID;
    if(nID==-1)
    {
        return NULL;
    }
    return GetRawData(nID);
}

inline BOOL CRawDataSet::IsValidID(int nID)
{
	if(nID < _nIDStart || nID >= (_nIDStart + _nIDCnt)) return FALSE;
	return TRUE;
}

extern BOOL  g_bBinaryTable;

inline BOOL CRawDataSet::LoadRawDataInfo(const char *pszFile, BOOL bBinary)
{
	char szTxtName[255], szBinName[255];

	if(g_bBinaryTable) bBinary = TRUE;
	
	_bBinary  = bBinary;

	sprintf(szTxtName, "%s.txt", pszFile);
	sprintf(szBinName, "%s.bin", pszFile);

    BOOL bRet = FALSE;
	if(bBinary) 
	{
		bRet = _LoadRawDataInfo_Bin(szBinName);
	}
	else
	{
		bRet = _LoadRawDataInfo_Txt(szTxtName);
		if(bRet)
		{
			_WriteRawDataInfo_Bin(szBinName);
		}
	}

    try {
        _AfterLoad();
    } catch (...) {}

    return bRet;
}

inline BOOL CRawDataSet::LoadRawDataInfoEx(const char *pszFile, BOOL bBinary)
{
	char szTxtName[255], szBinName[255];

	_bBinary  = bBinary;

	sprintf(szTxtName, "%s.txt", pszFile);
	sprintf(szBinName, "%s.bin", pszFile);
	if(bBinary) 
	{
		return _LoadRawDataInfo_Bin(szBinName);
	}
	else
	{
		BOOL bLoad = _LoadRawDataInfo_Txt(szTxtName);
		if(bLoad)
		{
			_WriteRawDataInfo_Bin(szBinName);
		}
		return bLoad;
	}
	return TRUE;
}

inline void CRawDataSet::FrameLoad(int nFrameLoad)
{
    int nMaxLoadPerFrame = nFrameLoad;

    std::list<int>::iterator it;
    std::list<int> FinishList;
    int n = 0;
    for(it = _RequestList.begin(); it!=_RequestList.end(); it++)
    {
        int nID = (*it);
        GetRawData(nID, FALSE);
        FinishList.push_back(nID);
        n++;
        if(n > nFrameLoad) break;
    }
    
    for(it = FinishList.begin(); it!=FinishList.end(); it++)
    {
        int nID = (*it);
        _RequestList.remove(nID);
    }
}

inline void CRawDataSet::DynamicRelease(BOOL bClearAll)
{
	if(bClearAll)
    {
        for(int i = 0; i < _nIDCnt; i++)
	    {
		    CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		    if(pInfo->pData==NULL)  continue;
            _DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
			    LG2("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
			}
      	}
        return;
    }
    
    if(_IsFull()==FALSE) return;
	
	DWORD dwCurTick = GetTickCount();

	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		if(pInfo->pData==NULL)  continue;
		
		if((dwCurTick - pInfo->dwLastUseTick) > _dwReleaseInterval)
		{
			_DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
				LG2("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
			}
			// LG2("debug", "Dynamic Release Raw Data [%s]\n", pInfo->szDataName);
		}
	}

    
}

inline void CRawDataSet::Release()
{
	if( _nLoadedRawDataCnt > 0 ) //安全释放内存 by Waiting 2009-06-18
	{
		for(int i = 0; i < _nIDCnt; i++)
		{
			CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
			if( NULL==pInfo || NULL==pInfo->pData )  //安全释放内存 by Waiting 2009-06-18
				continue;

			_DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
				LG2("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
			}
		}
	}
    //安全释放内存 by Waiting 2009-06-18
	if( _RawDataArray )
	{
		_DeleteRawDataArray();
		_RawDataArray = NULL;
	}
	_nUnusedIndex = 0;
}
/*
inline BOOL CRawDataSet::_LoadRawDataInfo_Bin(const char *pszFileName)
{

	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[] = { 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };
	
	FILE* fp = fopen(pszFileName, "rb");
	char szMsg[MAX_PATH] = {0};

	if(fp==NULL) 
	{
		LG2("error", "Load Raw Data Info Bin File [%s] Failed!\n", pszFileName);
		//sprintf(szMsg, "打开表格文件失败：%s\n程序即将退出!\n", pszFileName);
		//MessageBox(NULL, szMsg, "错误", MB_OK | MB_ICONERROR);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
    int nSize     = Util_GetFileSize(fp);
    int nInfoSize = _GetRawDataInfoSize();
    int nResCnt   = (nSize-12) / nInfoSize;

    LPBYTE pbtResInfo = new BYTE[nSize];
    
	//DWORD dwInfoSize = 0; 
	//fread(&dwInfoSize, 4, 1, fp);
	/*
	if(dwInfoSize!=_GetRawDataInfoSize())
	{
		//sprintf(szMsg, "dwInfoSize: %d\n_GetRawDataInfoSize: %d!\n", dwInfoSize, _GetRawDataInfoSize());
		//MessageBox(NULL, szMsg, "Error2", MB_OK | MB_ICONERROR);
		//LG2("table", "msg读取表格文件[%s]时, 发现版本不一致!\n", pszFileName);
		LG2("table", "msg read table file [%s], version can't match!\n", pszFileName);
		fclose(fp);
		//sprintf(szMsg, "读取表格文件错误：%s\n程序即将退出!\n", pszFileName);
		//MessageBox(NULL, szMsg, "错误", MB_OK | MB_ICONERROR);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		exit(0);
		return FALSE;
	}
	
	int test = fread(pbtResInfo, nSize, 1, fp);

	FILE* fp2 = fopen("test2.bin", "wb");
	fwrite(pbtResInfo, nSize, 1, fp2);

	std::string base64;
	std::string sink;
	CryptoPP::GCM<CryptoPP::AES>::Decryption d;
	d.SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);

	CryptoPP::StringSource(pbtResInfo, nSize, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(base64)));
	CryptoPP::AuthenticatedDecryptionFilter df(d, new CryptoPP::StringSink(sink), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS, 12);
	CryptoPP::StringSource(base64,true, new CryptoPP::Redirector(df));

	//if (sink.size() != nSize - 12) {
	//	sprintf(szMsg, "Size:%d\n, expected:%d\n", sink.size(), (nInfoSize * _nIDCnt) + 12);
	//	MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	//}
	memset(pbtResInfo, 0, nSize);
	memcpy(pbtResInfo, sink.c_str(), sink.size());

    for(int i = 0; i < nResCnt; i++)
    {
        CRawDataInfo *pInfo = (CRawDataInfo*)(pbtResInfo + i * _GetRawDataInfoSize());
		// modify by lark.li 20080424 begin
		//strcpy(pInfo->szDataName, ConvertResString(pInfo->szDataName));
		// End

        if(!pInfo->bExist) continue;
		if(IsValidID(i)==FALSE) continue;
		CRawDataInfo* pCurInfo = _GetRawDataInfo(pInfo->nID);
		memcpy(pCurInfo, pInfo, nInfoSize); // 替代原有的信息
         _IDIdx[pCurInfo->szDataName] = pCurInfo;
        //vector<string> ParamList; _ReadRawDataInfo(pCurInfo, ParamList);
        _ProcessRawDataInfo(pCurInfo);
		LG2("debug", "Load Bin RawData [%s] = %d\n", pCurInfo->szDataName, pCurInfo->nID);
    }
    
    delete pbtResInfo;
    
    fclose(fp);
	return TRUE;
}

inline void CRawDataSet::_WriteRawDataInfo_Bin(const char *pszFileName)
{

	const unsigned char cluTableKey[] = { 0x32, 0x72, 0x35, 0x75, 0x38, 0x78, 0x2f, 0x41, 0x3f, 0x44, 0x28, 0x47, 0x2b, 0x4b, 0x62, 0x50 };
	const unsigned char cluTableIV[] =	{ 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };
	FILE* fp = fopen(pszFileName, "wb");
	if(fp==NULL) return;
	char szMsg[MAX_PATH] = { 0 };
	DWORD dwInfoSize = _GetRawDataInfoSize();

	//fwrite(&dwInfoSize, 4, 1, fp);
	LPBYTE buffer = (LPBYTE)malloc((dwInfoSize * _nIDCnt) + 12);
	for (int i = 0; i < _nIDCnt; i++) {
		CRawDataInfo* pInfo = (CRawDataInfo*)((LPBYTE)_RawDataArray + i * _GetRawDataInfoSize());
		if (pInfo->bExist)
		{
			memcpy(buffer + (i * dwInfoSize), pInfo, dwInfoSize);
		}

	}
		
	CryptoPP::GCM<CryptoPP::AES>::Encryption e;
	std::string sink;
	std::string base64;
	e.SetKeyWithIV(cluTableKey, 16, cluTableIV, 16);
	CryptoPP::StringSource(buffer, (dwInfoSize * _nIDCnt), true, new CryptoPP::AuthenticatedEncryptionFilter(e, new CryptoPP::StringSink(sink), false, 12));
	CryptoPP::StringSource(sink, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64), false));
    fwrite(base64.c_str(), base64.size(), 1, fp);

	//if (sink.size() != (dwInfoSize * _nIDCnt) + 12) {
	//	sprintf(szMsg, "Size:%d\n, expected:%d\n", sink.size(), (dwInfoSize * _nIDCnt) + 12);
	//	MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
	//}
	fclose(fp);
}
*/

inline BOOL CRawDataSet::_LoadRawDataInfo_Txt(const char *pszFileName, int nSep)
{
	BOOL bRet = TRUE;
	std::ifstream in(pszFileName);
    if(in.is_open()==0)
    {
       // LG2("error", "msgLoad Raw Data Info Txt File [%s] Fail!\n", pszFileName);
        return FALSE;
    }
	
const int LINE_SIZE = 2048;
	char szLine[LINE_SIZE];
	std::string* pstrList = new std::string[_nMaxFieldCnt + 1];
	std::string strComment;

	std::vector<std::string> ParamList;

	// add by claude at 2004-9-1
	BOOL bSaveFieldCnt = FALSE;
	int nFieldCnt = 0;

    
    while(!in.eof())
    {
		in.getline(szLine, LINE_SIZE);
		std::string strLine = szLine;
		
		int p = (int)strLine.find("//");
		if(p!=-1)
		{
			std::string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), pstrList, _nMaxFieldCnt + 1, nSep);
		if (n < 2) continue;
		if (n > _nMaxFieldCnt)
			{
			//LG2("error", "msg在资源文件[%s]中，实际字段数大于预定义字段数\n", pszFileName);
				LG2("error", "msg in resource [%s], the field num is greater than predefine count \n", pszFileName);
			bRet = FALSE;
            break;}

		// 记录下首行字段数目
		if (!bSaveFieldCnt)
			{
			nFieldCnt = n;
            bSaveFieldCnt = TRUE;}
		else {
			// 比较此行字段数目与第一行字段数目是否相同
			if (nFieldCnt != n)
				{
				// 如果不同，说明此资源文件存在数据错误
				//LG2("error", "msg解析资源文件[%s]失败,序号[%s], 请检查格式和版本!\n",
					LG2("error", "msg parse resource file [%s] failed ,No [%s], please chech format and version!\n",
                    pszFileName, pstrList[0].c_str());

				bRet = FALSE;
                break;}
			}

        int	nID = Str2Int(pstrList[0]);
        if (!IsValidID(nID))
            {
            //LG2("error", "msg索引[%d]超出预定范围，请检查资源文件[%s]\n", nID, pszFileName);
				LG2("error", "msg index [%d] overflow,please check resource file [%s]\n", nID, pszFileName);
            bRet = FALSE;
            break;}

        _nIDLast = nID;

        CRawDataInfo *pInfo = _GetRawDataInfo(nID);
		pInfo->bExist = TRUE;
		
        ParamList.clear();
		int i;
        for(i = 0; i < n - 2; i++)
        {
			ParamList.push_back(pstrList[i + 2]);
		}
		for(i = 0; i < 15; i++)
			ParamList.push_back(""); // 放置空串,如果被后面代码读到, 表示格式不对

		//Util_TrimString(pstrList[1]);
		Util_TrimTabString(pstrList[1]); // 修正英文 MAKEBIN 空格丢失问题  modify by Philip.Wu  2006-07-31

		strcpy(pInfo->szDataName, pstrList[1].c_str());
		// char *pszDataName = _strupr( _strdup( pInfo->szDataName ) );
		
		char *pszDataName = ( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
        free(pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;

        BOOL bRet = FALSE;
        // try 
		{
			bRet = _ReadRawDataInfo(pInfo, ParamList);
			_ProcessRawDataInfo(pInfo);
		}
        //catch (...)
        {
		//	LG2("error", "msg解析资源文件[%s]发生未知的异常,导致失败，序号[%s], 请检查格式!\n", pszFileName, pstrList[0].c_str());
		//	bRet = FALSE; break;
    	}        
        if(!bRet)
        {
            //LG2("error", "msg解析资源文件[%s]失败,序号[%s], 请检查格式和版本!\n",
			LG2("error", "msg parse resource file [%s] failed ,No [%s], please chech format and version!\n",
                pszFileName, pstrList[0].c_str());
			bRet = FALSE; break;
        }
	}

	delete[] pstrList;
	in.close();
	return bRet;
}


//----------------------------------------------------------------------------------------------------------
//												打包相关处理
//----------------------------------------------------------------------------------------------------------
inline LPBYTE Util_LoadFile(const char *pszFileName, DWORD* pdwFileSize)
{
	if(strlen(pszFileName)==0) return NULL;
	FILE *fp = fopen(pszFileName, "rb");
	if(fp==NULL)
	{
		pdwFileSize = 0;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	LPBYTE pbtBuf = new BYTE[lSize];
	fread(pbtBuf, lSize, 1, fp);
	fclose(fp);
	*pdwFileSize = lSize;
	return pbtBuf;
}

inline LPBYTE Util_LoadFilePart(const char *pszFileName, DWORD dwStart, DWORD dwSize)
{
	FILE *fp = fopen(pszFileName, "rb");
	if(fp==NULL)
	{
		return NULL;
	}
	LPBYTE pbtBuf = new BYTE[dwSize];
	fseek(fp, dwStart, SEEK_SET);
	fread(pbtBuf, dwSize, 1, fp);
	fclose(fp);
	return pbtBuf;
}


inline void CRawDataSet::Pack(const char *pszPackName, const char *pszBinName)
{
	FILE *fp = fopen(pszPackName, "wb");
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(i);
		if(!pInfo->bExist)  continue;
		DWORD  dwFileSize = 0;
		LPBYTE pbtFileContent = Util_LoadFile(pInfo->szDataName, &dwFileSize);
		if(pbtFileContent)
		{
            pInfo->dwPackOffset = ftell(fp);
			pInfo->dwDataSize   = dwFileSize;
			fwrite(pbtFileContent, dwFileSize, 1, fp);
			delete pbtFileContent;
		}
	}
	fclose(fp);
	
	_WriteRawDataInfo_Bin(pszBinName); // 打包之后重写RawDataSet Bin文件
}


//--------------------------------------------
//  从目录中读取文件, 每个文件作为一个资源, 并
//  生成资源描述信息文件 xxx.bin
//--------------------------------------------
inline void	CRawDataSet::PackFromDirectory(std::list<std::string> &DirList, const char *pszPackName, const char *pszBinName)
{
	std::list<std::string> FileList;
	for(std::list<std::string>::iterator itD = DirList.begin(); itD!=DirList.end(); itD++)
	{
		std::string strDirName = (*itD);
		ProcessDirectory(strDirName.c_str(), &FileList, DIRECTORY_OP_QUERY);
	}
	
	int nFileCnt = (int)(FileList.size());
	
	FILE *fp = fopen(pszPackName, "wb");
		
	int i = 0;
	for(std::list<std::string>::iterator it = FileList.begin(); it!=FileList.end(); it++,i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(i + _nIDStart);
		
		strcpy(pInfo->szDataName, (*it).c_str());
		
		char *pszDataName = _strlwr( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
		free(pszDataName);
		
		DWORD  dwFileSize = 0;
		LPBYTE pbtFileContent = Util_LoadFile(pInfo->szDataName, &dwFileSize);
		if(pbtFileContent)
		{
		    pInfo->bExist       = TRUE;
            pInfo->dwPackOffset = ftell(fp);
			pInfo->dwDataSize   = dwFileSize;
			fwrite(pbtFileContent, dwFileSize, 1, fp);
			delete pbtFileContent;
		}
        LG2("debug", "Pack File (index = %d) ID = %d [%s]\n", pInfo->nIndex, pInfo->nID, pInfo->szDataName);
	}
	
	fclose(fp);
	
	_WriteRawDataInfo_Bin(pszBinName); // 打包之后重写RawDataSet Bin文件
}	

inline void CRawDataSet::EnablePack(const char *pszPackName)
{
	if(pszPackName)
    {
        _bEnablePack = TRUE;
        strcpy(_szPackName, pszPackName);
    }
    else
    {
        _bEnablePack = FALSE;
    }
}
	


//-----------------------------------------------------------------------------
// 读取RawData数据到内存(一般用在把整个文件读入, 或从包中取出完整文件内容的场合
// 子类视自身需要来使用此函数, 也可以完全不用)
//-----------------------------------------------------------------------------
inline LPBYTE CRawDataSet::LoadRawFileData(CRawDataInfo *pInfo)
{
	LPBYTE pbtBuf    = NULL;
	DWORD  dwBufSize = 0;
	if(_bEnablePack) // 从包中读取
	{
		pbtBuf    = Util_LoadFilePart(_szPackName, pInfo->dwPackOffset, pInfo->dwDataSize);
		dwBufSize = pInfo->dwDataSize;
	}
	else
	{
		pbtBuf    = Util_LoadFile(pInfo->szDataName, &dwBufSize);
		pInfo->dwDataSize = dwBufSize;
	}
	return pbtBuf;
}