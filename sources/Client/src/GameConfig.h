#pragma once

#include <string>

struct SPlaceCha 
{
	int nTypeID;
	int nX;
	int nY;
};

class CGameConfig
{
public:	
	CGameConfig();
	
	void	Load(const char *pszFileName); // 读配置文件
	void	SetDefault();  //默认配置
	void	SetMoveClient( bool v ); //是否与客户端同步
	
public:

	// 请注意此处不要放置任何非固定内存尺寸的变量, 比如模板, string之类的,
	// 此类将被直接写盘和读盘
	BOOL		m_bAutoLogin;
	BOOL		m_bFullScreen;
	SPlaceCha	m_ChaList[20];
	int			m_nChaCnt;
	float		m_LightDir[3];
	float		m_LightColor[3];
    BOOL        m_bNoObj;
    BOOL        m_bEditor;
    BYTE        m_btScreenMode;

	int			m_iFogR;
	int			m_iFogG;
	int			m_iFogB;
	float		m_fExp2;


    int         m_nMaxChaType;
    int         m_nMaxSceneObjType;
    int         m_nMaxEffectType;
	int			m_nMaxResourceNum;
    int         m_nMaxItemType;
    BOOL        m_bEnableMusic;
	BOOL		m_bCheckOvermax;

	// 网络
    BOOL        m_nSendHeartbeat;       // 多少时间发送一次心跳,单位:s,最小值10s,关闭0
	DWORD		m_nConnectTimeOut;		// 网络连接超时

    BOOL        m_bEnableLG;            // 是否允许输出LG信息
    BOOL        m_bEnableLGMsg;         // 是否允许弹出LG-Box
	BOOL		m_bMThreadRes;			// 多线程资源载入


	int		m_nCreateScene;

	//{lemon add@2004.9.16
	float	m_fCameraVel;
	float	m_fCameraAccl;

	//lemon add@2004.11.26, 登陆专用
	float	eyeX;
	float   eyeY;      
	float	eyeZ;      
	float   refX;      
	float   refY;      
	float   refZ;
	float   fov;
	//lemon add@2005.1.24
	float   fAspect;

	float	fnear;
	float	ffar;
	
    float   m_fLgtFactor;
    DWORD   m_dwLgtBkColor;

    // lh add@2004.12.8用于单机可以拿武器
    int     nLeftHand;
    int     nRightHand;


	BOOL	m_bRenderSceneObj;
	BOOL    m_bRenderCha;
	BOOL    m_bRenderEffect;
	BOOL    m_bRenderTerrain;
	BOOL	m_bRenderUI;
	BOOL    m_bRenderMinimap;
	BOOL	m_bRender;

    DWORD   m_dwFullScreenAntialias;

	// 用于网络切换地图时的参数
	DWORD	m_dwMaxCha;
	DWORD	m_dwMaxEff;
	DWORD	m_dwMaxItem;
	DWORD	m_dwMaxObj;

	char	m_szMD5Pass[48];	// 填写的MD5密码

	bool	m_IsShowConsole;	// 是否可以操作控件台
	bool	m_IsTomServer;		// 登陆Tom服务器

	bool	m_IsMoveClient;		// 是否客户端非同步走路

	char	m_szVerErrorHTTP[256];	// 版本不匹配时，调用的网页

	bool	m_IsBill;

	bool	IsPower()		{ return m_IsShowConsole;	}		// 如果是编辑器模式,自动可以操作控件台

	bool	m_IsDoublePwd;	// 是否有二次密码

	// Add by lark.li 20080429 for res
	char   m_szLocale[256];
	char   m_szResPath[256];

	char   m_szFontName1[50];
	char   m_szFontName2[50];
	// End

	// Add by lark.li 20080825 begin
	int		m_nMovieW;
	int		m_nMovieH;
	// End
};


extern CGameConfig g_Config;

