#pragma once

#include <windows.h>
// ------------------------ IMG_HEADER ---------------------------------//
struct IMG_HEADER 
{
	DWORD	m_dwMarks;		// 标志位 'RYO'
	WORD	m_LibType;		// 被打包的图像类型
	WORD	m_wPackedDirectory;	// 图像包中被打包的目录数,　最大允许数量 65535

	WORD	m_wGridX;		// 该图包内所有图像的的重心位置
	WORD	m_wGridY;		// 该图包内所有图像的的重心位置
	WORD	m_wVer;			//	版本号

	BYTE	m_bAlphaInfo;	//	Alpha 信息
	BYTE	m_bShadowInfo;	//	Shadow 信息
};

// ----------------------- IMG_BLOCK ------------------------------------//
struct IMG_BLOCK					//	图像块信息
{	
	BYTE	m_byPixelDepth;			// 像素位深
	BYTE	m_byUseRle;				// 是否使用RLE
	BYTE	m_byUseAlpha;			// 是否有Alpha通道
	BYTE	m_byNone1;

	WORD	m_wPalCount;			// 块中使用到的色盘数量基数
	WORD	m_wWidth;				// 图像宽度
	WORD	m_wHeight;				// 图像高度

	WORD	m_wTotalWidthBlock;		// 宽的数据块数量
	WORD	m_wTotalHeightBlock;	// 高的数据块数量	
	WORD	m_wTailBlockWidth;		// 末尾的块宽
	WORD	m_wTailBlockHeight;		// 末尾的块高
	BYTE	m_byNone2;
	BYTE	m_byNone3;

	RECT	m_rClip;

};

struct _RGB
{
	BYTE	b;
	BYTE	g;
	BYTE	r;
	BYTE	a;
	operator DWORD(){ return *(DWORD*)this;}

	bool operator == (_RGB& v) {return ( (0x00ffffff & this->operator DWORD() ) == (0x00ffffff & v.operator DWORD() ) );}
	_RGB& operator = (DWORD dwRGB){ (*this) = (*(_RGB*)&dwRGB); return *this;}

	//返回两颜色的差值
	unsigned int Difference(_RGB& v){ 
		int dr,dg,db;
		dr=(int)r-(int)v.r;
		dg=(int)g-(int)v.g;
		db=(int)b-(int)v.b;
		
		if(dr<0) dr=-dr;
		if(dg<0) dg=-dg;
		if(db<0) db=-db;
		return (dr+dg+db);
	}
};


#define	MAX_TEXTURE_WIDTH		256



class CBlk
{
public:
	CBlk(void);
	~CBlk(void);
	
	static BOOL Compress_BLK_RLE(const char* filename, DWORD* pARGB, int width, int height,BOOL bAlpha=TRUE);
	static BOOL Compress_BLK(const char* filename, DWORD* pARGB, int width, int height);

	static BOOL CompressPal(const DWORD* pARGB, int width, int height, _RGB* pPal, int & crCount);
	static int SearchPal(_RGB* pPal, int nCount,_RGB color);
	static int GetColorIndex(_RGB* pPal, int nCount, _RGB color);
	static RECT FoundRect(DWORD* pARGB, int width, int height);
	static int UseableSize( int size );
	static void ProcessLine(BYTE* pData, int width, BYTE* pRetData, int& nRetLen);

	void LoadBlk(const char* filename);
	void LoadBlk(BYTE* pBuf);

	typedef struct _stFileData{
		BYTE B;
		BYTE G;
		BYTE R;
		BYTE A;
	}stFileData;

	union {
		stFileData* m_pstFileData;
		DWORD *m_pFileData;
	};

	int m_iWidth;
	int m_iHeight;
	static int DecodeLine(BYTE* pData, int width, BYTE* pRetData,int & nRetLen);
	static void DecodeLine(BYTE* pData, int width, _stFileData* pRetData,int & nRetLen,_RGB* pal);
	static void DecodeAlphaLine(BYTE* pData, int width, _stFileData* pRetData,int & nRetLen);
	HBITMAP GetBitmap(COLORREF ct);
	static void PalTo565(_RGB* pal, int pal_count, WORD* pRet);
	static void _565ToPal(WORD* p565, int pal_count, _RGB* pal);
};
