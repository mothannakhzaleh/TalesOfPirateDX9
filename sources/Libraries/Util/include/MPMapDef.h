#pragma once


// 版本说明
// 当前版本转换为 map 780625版 -> 780626版
//                obj 600版    -> 700版
// 目的在于进一步减少map文件和obj文件的尺寸
// 此版本使用short来表示地表顶点颜色, byte来表示地表顶点高度
// 通过降低精度来减小文件尺寸, 新版本的文件将减小1/3的尺寸


#define MP_MAP_FLAG		 780624   // Map文件版本号起始
#define LAST_VERSION_NO  780626	  // 上一次版本需要支持转化	
#define CUR_VERSION_NO   780627	  // 当前使用的版本号

// 地图文件文件头定义
struct MPMapFileHeader
{
	int		nMapFlag;
	int		nWidth;
	int     nHeight;
	int		nSectionWidth;
	int		nSectionHeight;
};


#pragma pack(push)
#pragma pack(1)

struct SNewFileTile // 总计15个字节
{
	DWORD	dwTileInfo;	// 保存3个layer的tex no和alpha no
	BYTE	btTileInfo; // 最下层的tex no
	short	sColor;		// 32bit的颜色保存为565
	char	cHeight;	// 每10cm为一级来保存高度
	short	sRegion;	
	BYTE	btIsland;
	BYTE	btBlock[4];
	
	SNewFileTile(): sRegion(0), btIsland(0)
    {
        btBlock[0] = 0;
        btBlock[1] = 0;
        btBlock[2] = 0;
        btBlock[3] = 0;
    }
};

struct SFileTile
{
    BYTE  t[8];
    short sHeight;    // 以厘米为单位的地表顶点高度
    DWORD dwColor;
    short sRegion;
    BYTE  btIsland;
    BYTE  btBlock[4]; // 4 x 1的障碍和高度记录

    SFileTile(): sRegion(0), btIsland(0)
    {
        btBlock[0] = 0;
        btBlock[1] = 0;
        btBlock[2] = 0;
        btBlock[3] = 0;
    }
};

#pragma pack(pop)



// 原有的8个字节的Tile信息, 转化为5个字节表示
// 每个贴图编号占6位, 从 1 ~ 63, 最多表示63种, 0除外
// 每个Alpha编号占4位, 0 ~ 15, 最多表示16个样式
inline void TileInfo_8To5(BYTE *pbtTile, DWORD &dwNew, BYTE &btNew)
{
	DWORD dwTex1   = *(pbtTile + 2);
	DWORD dwTex2   = *(pbtTile + 4);
	DWORD dwTex3   = *(pbtTile + 6);
	DWORD dwAlpha1 = *(pbtTile + 3);
	DWORD dwAlpha2 = *(pbtTile + 5);
	DWORD dwAlpha3 = *(pbtTile + 7);

	dwNew = dwTex1<<26 | dwAlpha1<<22 | dwTex2<<16 | dwAlpha2<<12 | dwTex3<<6 | dwAlpha3<<2;
	btNew = *(pbtTile + 0);
}

// 5个字节到8个字节的反向转换
inline void TileInfo_5To8(DWORD dwNew, BYTE btNew, BYTE *pbtTile)
{
	*(pbtTile + 0) = btNew;
	*(pbtTile + 1) = 15;
	*(pbtTile + 2) = (BYTE)(dwNew>>26);
	*(pbtTile + 3) = (BYTE)(dwNew>>22) & 0x000f;
	*(pbtTile + 4) = (BYTE)(dwNew>>16) & 63;
	*(pbtTile + 5) = (BYTE)(dwNew>>12) & 0x000f;
	*(pbtTile + 6) = (BYTE)(dwNew>>6)  & 63;
	*(pbtTile + 7) = (BYTE)(dwNew>>2)  & 0x000f;
}

#define NEW_VERSION  // MindPower地图读入是否使用新版本

