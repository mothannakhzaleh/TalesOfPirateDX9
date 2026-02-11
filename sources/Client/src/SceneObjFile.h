
#ifndef	SCENEOBJFILE_H
#define	SCENEOBJFILE_H

#define	OBJ_FILE_VER100	    100
#define	OBJ_FILE_VER200		200
#define	OBJ_FILE_VER300		300
#define	OBJ_FILE_VER400		400
#define	OBJ_FILE_VER500		500
#define	OBJ_FILE_VER600		600
#define MAX_MAP_SECTION_OBJ 25

struct ReallyBigObjectInfo;

struct SSceneObjInfo
{
    short	sTypeID;	// 高2位是type(0: 场景物件, 1: 特效物件), 其余是ID
	int     nX;			// 相对坐标
	int  	nY;
	short	sHeightOff;
	short	sYawAngle;
    short	sScale;		// 保留未使用

	short GetType()
	{
		return sTypeID >> (sizeof(short) * 8 - 2);
	}
	short GetID()
	{
		return ~(0x0003 << (sizeof(short) * 8 - 2)) & sTypeID;
	}
};

class	CSceneObjFile
{
	
	struct SFileHead
	{
		_TCHAR	tcsTitle[16];	// "HF Object File!"
		int		lVersion;
		long	lFileSize;

		int		iSectionCntX;	// 地图的横向区域数
		int		iSectionCntY;	// 地图的纵向区域数
		int		iSectionWidth;	// 区域的宽度（单位：Tile）
		int		iSectionHeight; // 区域的高度（单位：Tile）
		int		iSectionObjNum;	// 区域允许的最大物件数
	};

	struct SSectionIndex
	{
		// obj
		long	lObjInfoPos;
		int		iObjNum;
	};
	friend class CObjInfluence;

public:
	CSceneObjFile();
	~CSceneObjFile();
	long	Init(const char *ptcsFileName, bool bSilence = true);
	void	Free(void);
	long	CreateFile(const char*ptcsFileName, int iSectionCntX = 512, int iSectionCntY = 512,
			int iSectionWidth = 8, int iSectionHeight = 8, int iSectionObjNum = MAX_MAP_SECTION_OBJ);
	long	ConvertObjFileVer(const char* ptcsFile, bool bBackUp = true); // 从版本300转换到400
	long	ReadSectionObjInfo(int nSectionNO, SSceneObjInfo* SSceneObj, long* lSectionObjNum);
	long	WriteSectionObjInfo(int nSectionNO, SSceneObjInfo* SSceneObj, long lSectionObjNum);
	long	TrimFile(const char* ptcsFileName, bool bBackUp);
	long	TrimDirectory(const char* ptcsDirectory, bool bBackUp);

	long	GetSectionObjNub(int nSectionNO, long *lSectionObjNum)
	{
		if (!m_bInitSuccess)
			return 0;

		if (nSectionNO >= m_SFileHead.iSectionCntX * m_SFileHead.iSectionCntY)
			return 0;

		*lSectionObjNum = m_SSectionIndex[nSectionNO].iObjNum;
		return 1;
	}

	SFileHead *	GetSFileHead()
	{
		if (m_bInitSuccess)
			return (&m_SFileHead);
		else
			return NULL;
	};

protected:
	bool			m_bInitSuccess;
	FILE			*m_fRdWr, *m_fAppend;
	SFileHead		m_SFileHead;
	SSectionIndex	*m_SSectionIndex;

private:
	// Added by CLP
	// RBO == Really Big Object
	std::string filename_RBO;
	std::set < struct ReallyBigObjectInfo > RBOinfoList;
	void _init_RBO( const std::string &filename );
	void _Serialize_RBO();
	void _Serialize_RBO_ToMap();
public:
	void end_RBO();
	std::set < struct ReallyBigObjectInfo > & GetRBOinfoList()
	{
		return RBOinfoList;
	}
};

extern	CSceneObjFile	g_ObjFile;

#endif //SCENEOBJFILE_H
