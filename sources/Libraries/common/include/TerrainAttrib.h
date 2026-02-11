
//
// TerrainAttrib.h
//
//  by claude at 2004-9-13
//

#pragma once

#include <stdio.h>
#include <memory.h>
#include <io.h>


#define ATTRIB_FILE_DEFAULT_WIDTH 4096
#define ATTRIB_FILE_DEFAULT_HEIGHT 4096

// 区域属性值域定义
#define TILE_ATTRIB_MIN_BITS 1
#define TILE_ATTRIB_MAX_BITS 16
#define TILE_ATTRIB_DEFAULT_VALUE 0

// 海岛值域定义
#define TILE_ISLAND_MIN_VALUE 0
#define TILE_ISLAND_MAX_VALUE 200
#define TILE_ISLAND_DEFAULT_VALUE TILE_ISLAND_MIN_VALUE


// 开放给客户端使用，其中filename不带扩展名
bool createAttribFile(char const* filename, int width = ATTRIB_FILE_DEFAULT_WIDTH,
					  int height = ATTRIB_FILE_DEFAULT_HEIGHT,
					  int option = 0);
void setTileAttrib(int nX, int nY, unsigned char btAttrib, bool bAdd = true);
void setTileIsland(int nX, int nY, unsigned char btIsland);
bool openAttribFile(char const* filename);
bool getAttribFileInfo(int& width, int& height);
bool getTileAttrib(int x, int y, unsigned short& attrib);
bool hasTileAttrib(int x, int y, unsigned char attrib);
bool delTileAttrib(int x, int y, unsigned char attrib);
bool getTileIsland(int x, int y, unsigned char& index);


// 服务端只使用以下接口，客户端切勿使用！
int s_openAttribFile(char const* filename); // 返回 -1 则错误
bool s_getAttribFileInfo(int id, unsigned int& width, unsigned int& height);
bool s_getTileAttrib(int id, unsigned int x, unsigned int y, unsigned short& attrib);
bool s_hasTileAttrib(int id, unsigned int x, unsigned int y, unsigned char attrib_mask);
bool s_getTileIsland(int id, unsigned int x, unsigned int y, unsigned char& island_index);

// 设置对齐方式为1字节，因为要写入文件中
#pragma pack(push)
#pragma pack(1)

// 区域属性文件文件头定义
struct SAttribFileHeader
	{
	unsigned int width;
	unsigned int height;
	};
typedef SAttribFileHeader terrain_attr_hdr;

// 区域属性文件单位数据定义
typedef struct _Tile_Attrib
	{
	unsigned short attrib; // tile属性
	unsigned char island; // 海岛编号

	} STILE_ATTRIB;
typedef _Tile_Attrib terrain_attr_dat;

#pragma pack(pop)
