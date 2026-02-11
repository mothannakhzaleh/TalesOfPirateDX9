#pragma once

#include <string>

#define MAX_GATE 5
#define MAX_MAP  100
#define MAX_MAPNAME_LENGTH	32

class CGameCommand
{
public:
	CGameCommand();
	bool	Load(const  char *pszFileName);
	void	SetDefault();

	// commands
	char	m_cMove[32];
	char	m_cMake[32];
	char	m_cNotice[32];
	char	m_cHide[32];
	char	m_cUnhide[32];
	char	m_cGoto[32];
	char	m_cKick[32];
	char	m_cMute[32];
	char	m_cReload[32];
	char	m_cRelive[32];
	char	m_cQcha[32];
	char	m_cQitem[32];
	char	m_cCall[32];
	char	m_cGamesvrstop[32];
	char	m_cUpdateall[32];
	char	m_cMisreload[32];
	char	m_cSummon[32];
	char	m_cSummonex[32];
	char	m_cKill[32];
	char	m_cAddmoney[32];
	char	m_cAddexp[32];
	char	m_cAttr[32];
	char	m_cItemattr[32];
	char	m_cSkill[32];
	char	m_cDelitem[32];
	char	m_cLuaall[32];
	char	m_cAddkb[32];
	char	m_cLua[32];
	char	m_cAddImp[32];
	char	m_cScrollNotice[32];
	char	m_cgenCharBag[32];
	char	m_cDistance[32];
};

class CGameConfig
{

public:	
	
    CGameConfig();
	
	bool	Load(char *pszFileName);
	void	SetDefault();
	bool	Reload(char *pszFileName);
	bool	LoadCmd(char *pszFileName);
	
public:

	// 请注意此处不要放置任何非固定内存尺寸的变量, 比如模板, string之类的,
	// 此类将被直接写盘和读盘
	char    m_szGateIP[MAX_GATE][64];  // GateIP地址列表
    int     m_nGatePort[MAX_GATE];     // Gate Port列表
    int     m_nGateCnt;                // Gate数量
    char    m_szInfoIP[64];            // InfoServer IP
    int     m_nInfoPort;               // InfoServer Port
	char	m_szInfoPwd[33];		   // InfoServer验证码
	int		m_nSection;				   // 小区号
    char    m_szMapList[MAX_MAP][MAX_MAPNAME_LENGTH];
	BYTE	m_btMapOK[MAX_MAP];		   // 地图是否初始化成功
	int     m_nMapCnt;				   // 地图数量
	char	m_szEqument[MAX_MAPNAME_LENGTH];   // 世界守护神
    char    m_szName[64];			   // 服务器名字
	char	m_szDBIP[64];			   // DB IP
	char	m_szDBUsr[32];			   // DB 用户名
	char	m_szDBPass[32];			   // DB 密码

	// Add by lark.li 20080321 begin
	char	m_szTradeLogDBIP[64];			   // DB IP
	char	m_szTradeLogDBName[32];			   // DB IP
	char	m_szTradeLogDBUsr[32];			   // DB 用户名
	char	m_szTradeLogDBPass[32];			   // DB 密码

	BOOL	m_bTradeLogIsConfig;
	// End
	char	m_szDBName[32];            // kong@pkodev.net 09.22.2017

	long	m_lSocketAlive;            // Socket保持活动
	int		m_nMaxPly;                 // 最大玩家数
	int		m_nMaxCha;                 // 最大角色数
	int		m_nMaxItem;                // 最大道具数
	int		m_nMaxTNpc;                // 最大对话NPC
	unsigned long	m_ulBaseID;        // 服务器的ID基数
	long	m_lItemShowTime;           // 道具存在时间
	long	m_lItemProtTime;           // 道具保护时间
	long	m_lSayInterval;            // 喊话间隔
	char	m_szResDir[255];		   // 运行资源所在的目录
	char	m_szLogDir[255];		   // Log所在的目录
	char	m_chMapMask;               // 是否存取大地图
	long	m_lDBSave;                 // 数据库定时存盘时间间隔

	BOOL	m_bLogAI;				   // 是否打开AI的log
	BOOL	m_bLogCha;				   // 是否打开角色的log
	BOOL	m_bLogCal;				   // 是否打开数值计算的log
	BOOL	m_bLogMission;			   // 是否打开Mission的log

	BOOL	m_bSuperCmd;
	
	// Add by lark.li 20080731 begin
	std::vector<int>	m_vGMCmd;
	// End
	short	m_sGuildNum;
	short	m_sGuildTryNum;
	BOOL	m_bOfflineMode;
	BOOL	m_bDiscStall;
	BOOL	m_bBlindChaos;
	char	m_szChaosMap[32];
	DWORD	m_dwStallTime;		
	
	BOOL	m_bLogDB;				   // 是否把玩家行为记录到数据库
	BOOL    m_bInstantIGS;

	long	m_lWeather;

	unsigned char m_cSaveState[32];
};

#define	defCONFIG_FILE_NAME_LEN	512
extern char	szConfigFileN[defCONFIG_FILE_NAME_LEN];

extern CGameConfig g_Config;
extern CGameCommand g_Command;

