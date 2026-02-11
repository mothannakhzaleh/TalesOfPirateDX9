#pragma once

#include "Util.h"

// MapData 数据管理类

// 支持数据动态读入, 不限制数据类型

// 比如地表, 物件存盘文件都可以使用此类

// 支持数据块之间的复制, 可以方便地图切割, 组合


struct SDataSection
{
    void*		pData;				// 指向实际的数据
    DWORD		dwDataOffset;		// 文件数据指针位置 = 0, 表示没有数据
	DWORD		dwActiveTime;		// 最后一次使用的时间
    DWORD       dwParam;            // 参数
	
	SDataSection()
    :pData(NULL),dwActiveTime(0),dwDataOffset(0),dwParam(0)
	{
	}
};


class CSectionDataMgr;
typedef long (CALLBACK* MAP_LOADING_PROC)(int nFlag, int nSectionX, 
								  int nSectionY, unsigned long dwParam, CSectionDataMgr* pMapData);

class CSectionDataMgr
{
public:

    ~CSectionDataMgr();
    
    // 从文件读入, 并决定是否需要编辑操作
    BOOL    CreateFromFile(const char *pszMapName, BOOL bEdit = FALSE);
    
    // 创建, 会生成文件
    BOOL    Create(int nSectionCntX, int nSectionCntY, const char *pszMapName);
	
    // 读盘, 可以一次性读入, 也可以动态读入
    void	Load(BOOL bFull, DWORD dwTimeParam = 0);
    
    // 读取Section数据的回调函数
    void	SetLoadSectionCallback(MAP_LOADING_PROC pfn)	{	_pfnProc = pfn;				        }
    
    // 复制Section数据
    void    CopySectionData(SDataSection* pDest, SDataSection *pSrc);
    
    // 保存Section数据到文件
    void    SaveSectionData(int nSectionX, int nSectionY);

    // 清除指定Section的数据, 内部会清除文件索引
    void    ClearSectionData(int nSectionX, int nSectionY);

    // 获取Section数据, 如果不在内存里则返回空
    SDataSection* GetSectionData(int nSectionX, int nSectionY);
    
    // 读取指定Section的数据
    SDataSection* LoadSectionData(int nSectionX, int nSectionY);

	
	// 获取map相关描述信息
	int		GetSectionCntX()						        { return _nSectionCntX;			        }
	int		GetSectionCntY()						        { return _nSectionCntY;			        }
    int     GetSectionCnt()                                 { return _nSectionCntX * _nSectionCntY; }
    char*   GetFileName()                                   { return _szFileName;                   }
    char*   GetDataName()                                   { return _szDataName;                    }
    void    SetDataName(const char *pszName)                { strcpy(_szDataName, pszName);          }   

    // 计算有效数据
    // int     CalValidSectionCnt();
    
    // 设置动态读入的中心位置和范围
    void	SetLoadSize(int nWidth, int nHeight)	{ _nLoadWidth  = nWidth;_nLoadHeight = nHeight; }
	int		GetLoadWidth()							{ return _nLoadWidth;					        }
	int		GetLoadHeight()							{ return _nLoadHeight;					        }
	void	SetLoadCenter(int nX, int nY)		    { _nLoadCenterX = nX; _nLoadCenterY = nY;       }
	int 	GetLoadCenterX()						{ return _nLoadCenterX;					        }
	int 	GetLoadCenterY()						{ return _nLoadCenterY;					        }
	BOOL    IsValidLocation(int nX, int nY);        
	int		GetSectionDataSize()					{ return _GetSectionDataSize();					}

protected:

    CSectionDataMgr();

    virtual int     _GetSectionDataSize()   = 0;
    virtual BOOL    _ReadFileHeader()       = 0;
    virtual void    _WriteFileHeader()      = 0;
    virtual DWORD   _ReadSectionIdx(DWORD dwSectionNo) = 0;
    virtual void    _WriteSectionIdx(DWORD dwSectionNo, DWORD dwDataOffset) = 0;
    virtual void    _AfterReadSectionData(SDataSection *pSection, int nSectionX, int nSectionY) {}
    
protected:

    SDataSection*   _GetDataSection(DWORD dwSectionNo)
    {
        return *(_SectionArray + dwSectionNo);
    }

	char                _szFileName[255];   // 包含完整路径的名称
    char                _szDataName[32];     // 地图名称
	int					_nSectionCntX;		// 地图水平方向上Section的块数
	int					_nSectionCntY;		// 地图垂直方向上Section的块数
	int					_nLoadCenterX;		// 动态读入的中心位置
	int 				_nLoadCenterY;
	int					_nLoadWidth;		// 可视范围
	int					_nLoadHeight;
	
    int					_nSectionStartX;
	int					_nSectionStartY;
    
	MAP_LOADING_PROC	_pfnProc;			// 地图动态读入回调通知函数
	
	FILE*				_fp;				// 地图文件句柄
	BOOL				_bEdit;				// 是否可编辑
    BOOL                _bDebug;
	
    std::list<SDataSection*>	_SectionList;
	SDataSection**       	    _SectionArray;
};

inline SDataSection* CSectionDataMgr::GetSectionData(int nSectionX, int nSectionY)
{
    if(!IsValidLocation(nSectionX, nSectionY))
    {
        return NULL;
    }

    DWORD dwLoc = (nSectionY * _nSectionCntX + nSectionX);
    return (SDataSection*)(*(_SectionArray + dwLoc));
}

inline BOOL CSectionDataMgr::IsValidLocation(int nX, int nY)
{
    if(nX < 0 || nX >= _nSectionCntX || nY < 0 || nY >= _nSectionCntY)
    {
        return FALSE;
    }
    return TRUE;
}

