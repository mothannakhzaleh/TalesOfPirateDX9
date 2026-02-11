#pragma once
/************************************************************************/
/*打包文件类:打包一个当前程序下的某个目录及子目录及文件。
/*			以及从打包文件中读取文件数据。
/*create by lemon@2005.6.3
/************************************************************************/
class CPackFile
{
public:
	CPackFile();
	
	~CPackFile();
	

	struct FileData  // 文件数据
	{
		s_string strFileName; // 文件名
		DWORD offset;   //
		DWORD length;  // 文件长度
	};

	struct DirectoryData // 文件路径数据
	{
		s_string  strDirName; // 文件路径
		DWORD dwDirNum;  // 文件目录数
		DWORD dwFileNum; // 文件数
		std::vector<DirectoryData>	vecSubDir; // 函数DirectoryData()自动应用在vecSubDir上
		std::vector<FileData>	vecFile; // 函数FileData()自动应用在vecFile上
	};

	bool	SaveToPack(const char* pszDirName, const char* pszOutFile, const char* pszFilter = "*.bmp"); // 将文件保存打包

	bool	LoadPackFile(char* pszFile); // 读打包文件数据

	void	Clear(); 

	DirectoryData*	GetDir(char* pszDirName) // 获取打包文件的目录
	{
		if(!_pPackFile)  
			return NULL;
		if(_RootDir.strDirName == pszDirName)
			return &_RootDir;
		return FindDir(pszDirName, &_RootDir);
	}
protected:
	bool	PackDirectory(FILE* ptf,DirectoryData* pParentDir,const char* pszDir); // 打包后文件所在目录
	long	AddFileToTempPack(FILE* pf, const char* pszFileName); // 加入文件到打包文件中
	long	GetFileDataSize(FileData* pFileData); // 获取文件数据大小
	void	SaveFileData(FILE* pf, FileData* pFileData, long offset); //保存文件数据
	long	GetDirectorySize(DirectoryData* pDir); // 获得文件目录数据大小
	void	SaveDirData(FILE* pf, DirectoryData* pDir, long offset); // 保存目录数据


	bool	LoadFileData(FileData* pFileData); // 读文件数据				
	bool	LoadDirData(DirectoryData* pDir);  // 读目录数据	

	DirectoryData* FindDir(char* pszDirName, DirectoryData* pDir)  // 查找文件存放目录
	{
		if(pDir->strDirName == pszDirName)
			return pDir;
		WORD n;
		DirectoryData* ptDir;
		for (n = 0; n < pDir->dwDirNum; ++n)
		{
			if(ptDir = FindDir(pszDirName, &pDir->vecSubDir[n]))
				return ptDir;
		}
		return NULL;
	}
public:
	//当前的pack文件名
	s_string strName;
	s_string strFilter;

protected:

	FILE*			_pPackFile; // 要打包的文件
	DirectoryData	_RootDir; // 要到报的文件根目录
}; 


class CMiniPack : public CPackFile // 琐碎打包
{
public:
	CMiniPack();
	~CMiniPack();

	bool 	Init(char* pszMapName);
	void	GetXY( char*pszName, int& x,int& y);
	bool	IsExit(int x,int y)
	{
		if(x >= _imaxX || y >= _imaxY)
			return false;
		if(!_pIdx[x + y * _imaxX])
		{
			return false;
		}
		return true;
	}
	void	GetTex(MPITex**	pTex, int x,int y)
	{
		int idx = x + y * _imaxX;
		if( idx > _imaxX * _imaxY)
		{
			*pTex = NULL;
			return;
		}
		FileData*	   _pFile = _pIdx[idx];
		if(!_pFile)
		{
			*pTex = NULL;
			return;
		}
		if(!_pTData || _lsize != _pFile->length)
		{
			_pTData = std::make_unique<std::byte[]>(_pFile->length);
			_lsize = _pFile->length;
		}
		fseek(_pPackFile,_pFile->offset, SEEK_SET); 
		fread(_pTData.get(), 1,_pFile->length,_pPackFile);

		texInfo.width = _pFile->length;
		texInfo.data = _pTData.get();

		lwLoadTex(pTex,g_Render.GetInterfaceMgr()->res_mgr, &texInfo);
	}
public:
	s_string	strMapName;
	lwTexInfo	texInfo;

protected:
	struct sXY
	{
		int x,y;
	};

	std::unique_ptr<std::byte[]> _pTData{};
	std::size_t _lsize{};
	std::unique_ptr<FileData* []> _pIdx{};
	int			  _imaxX{}, _imaxY{};

};
