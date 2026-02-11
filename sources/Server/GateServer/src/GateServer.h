#pragma once

#include <time.h>
#include <dstring.h>
#include <datasocket.h>
#include <threadpool.h>
#include <commrpc.h>
#include <point.h>
#include <inifile.h>
#include <gamecommon.h>
#include <prealloc.h>
#include <LogStream.h>
#include <algo.h>

#include "i18n.h"		//Add by lark.li 20080130
#include "cryptlib.h"
#include "rsa.h"
#include "osrng.h"
#include "base64.h"
#include "gcm.h"
#include "filters.h"
#include "aes.h"
#include "pi_Memory.h"
#include "pi_Alloc.h"

#include <iostream>
#include <map>
#include <mutex>
#include <vector>

// 关于 Client 连接部分＝＝＝＝＝＝＝＝＝＝
class  ClientConnection;
class ToClient : public dbc::TcpServerApp, public dbc::RPCMGR
{
	friend class TransmitCall;
public:
	ToClient(char const* fname, dbc::ThreadPool* proc, dbc::ThreadPool* comm);
	~ToClient();

	void post_mapcrash_msg(ClientConnection* ply);
	std::string GetDisconnectErrText(int reason) const;
	void SetMaxCon(dbc::uShort maxcon) { m_maxcon = maxcon; }
	dbc::uShort GetMaxCon() { return m_maxcon; }
	void CM_LOGIN(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	dbc::WPacket CM_LOGOUT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_BGNPLAY(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_ENDPLAY(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_NEWCHA(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_REGISTER(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CP_CHANGEPASS(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_DELCHA(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_CREATE_PASSWORD2(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_UPDATE_PASSWORD2(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void CM_OPERGUILDBANK(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);

	void TC_DISCONNECT(dbc::DataSocket* datasock, int reason = DS_DISCONN, int remain = 4000);
	dbc::uShort GetVersion() { return m_version; }
	int GetCallTotal() { return m_calltotal; }

	dbc::uShort GetCheckSpan() { return m_checkSpan; }
	dbc::uShort GetCheckWaring() { return m_checkWaring; }
	dbc::uShort GetCheckError() { return m_checkError; }

	void	SetCheckSpan(dbc::uShort checkSpan);
	void	SetCheckWaring(dbc::uShort checkWaring);
	void	SetCheckError(dbc::uShort checkError);
  
private:
	bool		 DoCommand(dbc::DataSocket* datasock, dbc::cChar* cmdline);
	virtual bool OnConnect(dbc::DataSocket* datasock); // 返回值:true-允许连接,false-不允许连接
	virtual void OnConnected(dbc::DataSocket* datasock);
	virtual void OnDisconnect(dbc::DataSocket* datasock, int reason);
	virtual void OnProcessData(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	virtual dbc::WPacket OnServeCall(dbc::DataSocket* datasock, dbc::RPacket& in_para);
	virtual bool OnSendBlock(dbc::DataSocket* datasock) { return false; }

	// communication encryption
	virtual bool OnEncrypt(dbc::DataSocket* datasock, char* ciphertext, dbc::uLong ciphertext_len, const char* text, dbc::uLong& len);
	virtual bool OnDecrypt(dbc::DataSocket* datasock, char* ciphertext, dbc::uLong& len);

	dbc::InterLockedLong	m_atexit, m_calltotal;
	volatile dbc::uShort m_maxcon;
	dbc::uShort m_version;
	int _comm_enc; // 加密算法索引

	volatile dbc::uShort	m_checkSpan;
	volatile dbc::uShort	m_checkWaring;
	volatile dbc::uShort	m_checkError;
	volatile dbc::cChar*	    m_lastSockIP;
	volatile dbc::uLong		m_cmdNum;
	volatile dbc::uLong		m_Warning;
	volatile dbc::uLong     m_lastConnect;
	std::vector<std::string>		blacklistedIP;

	IMPLEMENT_CDELETE(ToClient)
};

// 关于 GameServer 连接部分＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
class ToGameServer;

#define MAX_MAP 100
#define MAX_GAM 100
struct GameServer : public dbc::PreAllocStru
{
	friend class dbc::PreAllocHeap<GameServer>;

private:
	GameServer(dbc::uLong Size) : dbc::PreAllocStru(Size) {}
	~GameServer();
	GameServer(GameServer const&) : dbc::PreAllocStru(1) {}
	GameServer& operator =(GameServer const&) {}
	void Initially();
	void Finally();
public:
	void EnterMap(ClientConnection* ply, dbc::uLong actid, dbc::uLong dbid, dbc::uLong worldid, dbc::cChar* map, dbc::Long lMapCpyNO, dbc::uLong x, dbc::uLong y, char entertype, short swiner);	//角色chaid进入本服务器的地图map中的位置(x,y) winer指定角色是否是乱斗之王。
public:
	dbc::InterLockedLong m_plynum{};
	std::string gamename;
	std::string ip;
	dbc::uShort port{};
	dbc::DataSocket* m_datasock{};
	GameServer* next{};
	std::string maplist[MAX_MAP];
	dbc::uShort mapcnt{};
};

class ToGameServer : public dbc::TcpServerApp, public dbc::RPCMGR
{
	friend class ToGroupServer;
public:
	ToGameServer(char const* fname, dbc::ThreadPool* proc, dbc::ThreadPool* comm);
	~ToGameServer();

	GameServer* find(dbc::cChar* mapname);
	GameServer* GetGameList() { return _game_list; }

private:
	virtual bool OnConnect(dbc::DataSocket* datasock); //返回值:true-允许连接,false-不允许连接
	virtual void OnDisconnect(dbc::DataSocket* datasock, int reason);
	virtual	dbc::WPacket OnServeCall(dbc::DataSocket* datasock, dbc::RPacket& in_para);
	virtual void OnProcessData(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);

	dbc::PreAllocHeap<GameServer> _game_heap; // GameServer 描述对象堆
	void MT_LOGIN(dbc::DataSocket* datasock, dbc::RPacket& recvbuf); // GameServer 登录 GateServer
	void MT_SWITCHMAP(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void MT_MAPENTRY(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void MC_ENTERMAP(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void MC_STARTEXIT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void MC_CANCELEXIT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void MT_PALYEREXIT(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	void MT_KICKUSER(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);

	GameServer* _game_list; // 存储 GameServer 描述对象的链表
	short _game_num;
	void _add_game(GameServer* game);
	bool _exist_game(char const* game);
	void _del_game(GameServer* game);
	std::map<std::string, GameServer*> _map_game; // ´ÓµØÍ¼Ãû¶ÔÓ¦ GameServer ÃèÊö¶ÔÏó
	std::recursive_mutex _mut_game;

	IMPLEMENT_CDELETE(ToGameServer)
};

// 关于 GroupServer 连接部分＝＝＝＝＝＝＝＝＝＝
class ToGroupServer;
class ConnectGroupServer : public dbc::Task
{
public:
	ConnectGroupServer(ToGroupServer* tgts) { _tgps = tgts; _timeout = 3000; }
private:
	virtual long Process();
	virtual dbc::Task* Lastly();

	ToGroupServer* _tgps;
	DWORD _timeout;
};

struct GroupServer
{
	GroupServer() : datasock(NULL), next(NULL) {}
	std::string ip; 
	dbc::uShort port;
	dbc::DataSocket* datasock;
	GroupServer* next;
};

class ToGroupServer : public dbc::TcpClientApp, public dbc::RPCMGR
{
	friend class ConnectGroupServer;
	friend void ToGameServer::MT_LOGIN(dbc::DataSocket* datasock, dbc::RPacket& rpk);
public:
	ToGroupServer(char const* fname, dbc::ThreadPool* proc, dbc::ThreadPool* comm);
	~ToGroupServer();

	dbc::DataSocket* get_datasock() const { return _gs.datasock; }
	dbc::RPacket get_playerlist();

	int GetCallTotal() { return m_calltotal; }

	// Add by lark.li 20081119 begin
	bool	IsSync() { return m_bSync; }
	void	SetSync(bool sync = true) { m_bSync = sync; }
	// End

	// 准备好
	bool	IsReady() { return (!m_bSync && _connected); }

private:
	virtual bool OnConnect(dbc::DataSocket* datasock); // 返回值:true-允许连接,false-不允许连接
	virtual void OnDisconnect(dbc::DataSocket* datasock, int reason);
	virtual void OnProcessData(dbc::DataSocket* datasock, dbc::RPacket& recvbuf);
	virtual dbc::WPacket OnServeCall(dbc::DataSocket* datasock, dbc::RPacket& in_para);

	dbc::InterLockedLong	m_atexit, m_calltotal;

	std::string _myself; // GateServer自己的名字
	GroupServer _gs;
	bool volatile _connected{ false };

	// Add by lark.li 20081119 begin
	bool volatile m_bSync{ false };
	// End

	IMPLEMENT_CDELETE(ToGroupServer)
};

class ClientConnection : public dbc::PreAllocStru, public dbc::RunBiDirectItem<ClientConnection>
{
	friend class dbc::PreAllocHeap<ClientConnection>;
	friend class DelayLogout;
public:
	enum class Status
	{
		Invalid,
		CharacterSelection,
		Playing,
		None
	};

	bool InitReference(dbc::DataSocket* datasock);
	void Free() { PreAllocStru::Free(); }

	// Add by lark.li 20081119 begin
	bool BeginRun();
	bool EndRun();
	//End


	void SendSysInfo(std::string_view message) const;

	//add by Billy
	void SendPacketToClient(dbc::WPacket pkt);
	void SendPacketToGameServer(dbc::WPacket pkt);
	void SendPacketToGroupServer(dbc::WPacket pkt);
	// End

	char	m_password[ROLE_MAXSIZE_PASSWORD2 + 1]{};

	bool  EncryptAES(char* ciphertext, dbc::uLong ciphertext_len, dbc::cChar* plaintext, dbc::uLong& plainsize);
	bool  DecryptAES(dbc::cChar* ciphertext, char* plaintext, dbc::uLong& ciphersize);
	// RSA-AES Network Encryption
	CryptoPP::AutoSeededRandomPool rng{};
	CryptoPP::RSA::PrivateKey	srvPrivateKey{};
	CryptoPP::SecByteBlock cliPrivateKey{ CryptoPP::AES::MIN_KEYLENGTH };
	bool handshakeDone{ false };

	dbc::uLong	volatile	m_actid{};
	dbc::uLong   volatile    m_loginID{};
	dbc::uLong	volatile	m_dbid{};		// 当前角色的数据库ID
	dbc::uLong	volatile	m_worldid{};	// 当前角色的内存唯一ID
	dbc::uLong	volatile	m_pingtime{};
	dbc::InterLockedLong gm_addr{}; // GameServer 上 Player 对象的指针
	dbc::InterLockedLong gp_addr{}; // GroupServer 上 Player 对象的指针
	dbc::DataSocket* volatile m_datasock{}; // 此 Player 的 GateServer <-> Client 连接
	GameServer* volatile game{}; // 此 Player 当前所在的 GameServer 描述对象
	volatile bool enc{}; // 是否加密通信数据

	// 是否禁言
	dbc::uLong	volatile m_lestoptick{};
	bool	volatile m_estop{};
	short	volatile m_sGarnerWiner{};

	struct
	{
		std::recursive_mutex				m_mtxstat;					//0:锁定m_status;
		Status		m_status{Status::Invalid};					//0:无效;1.选角色期间;2.玩游戏中.
		Status		m_exit{Status::Invalid };
	};

	dbc::dstring m_chamap;
	void		SetMapName(dbc::cChar* cszName) { m_chamap = cszName; }
	const char* GetMapName(void) { return m_chamap.c_str(); }

private:
	ClientConnection(dbc::uLong Size);
	~ClientConnection();
	ClientConnection(ClientConnection const&) : dbc::PreAllocStru(1) {}
	ClientConnection& operator =(ClientConnection const&) {}
	void Initially(); void Finally();

};
class TransmitCall :public dbc::PreAllocTask
{
public:
	TransmitCall(dbc::uLong size) :dbc::PreAllocTask(size) {};
	void Init(dbc::DataSocket* datasock, dbc::RPacket& recvbuf)
	{
		m_datasock = datasock;
		m_recvbuf = recvbuf;
	}
	long Process();

	virtual void Finally()
	{
		m_recvbuf = nullptr;
	}


	dbc::DataSocket* m_datasock;
	dbc::RPacket		m_recvbuf;
};
class GateServer
{
public:
	GateServer(char const* fname);
	~GateServer();
	void RunLoop(); // 主循环
	dbc::ThreadPool* m_clproc, * m_clcomm, * m_gpcomm, * m_gpproc, * m_gmcomm;
	ToGroupServer* gp_conn; // 同GroupServer的连接对象（主动重连机制）
	ToGameServer* gm_conn; // 同GameServer的连接对象（被动）
	ToClient* cli_conn; // 同Client的连接对象（被动）
	std::recursive_mutex	_mtxother;

	dbc::PreAllocHeap<ClientConnection>		client_heap; // 玩家对象堆

	// Add by lark.li 20081119 begin
	dbc::RunBiDirectChain<ClientConnection> 	m_plylst;	// 玩家连表
	// End

	dbc::PreAllocHeap<TransmitCall>	m_tch;

	IMPLEMENT_CDELETE(GateServer)
};
extern GateServer* g_gtsvr;
extern bool volatile g_appexit;

class CUdpManage;
class CUdpServer;
class GateServerApp
{
public:
	GateServerApp();

	void ServiceStart();
	void ServiceStop();
	virtual bool CanPaused()const { return true; };	//缺省支持暂停和继续操作

	int		GetShowMin() { return _nShowMin; }
	int		GetShowMax() { return _nShowMax; }
	void	SetShowRange(int min, int max) { _nShowMin = min; _nShowMax = max; }


private:	// 用于在未连接前，客户端通过udp获得游戏统计信息Jerry 2006-1-10
	CUdpManage* _pUdpManage;
	static void _NotifySocketNumEvent(CUdpManage* pManage, CUdpServer* pUdpServer, const char* szClientIP, unsigned int nClientPort, const char* pData, int len);
	static int	_nShowMin;
	static int	_nShowMax;
};

extern dbc::LogStream g_gateerr;
extern dbc::LogStream g_gatelog;
extern dbc::LogStream g_chkattack;
extern dbc::LogStream g_gateconnect;
//extern LogStream g_gatepacket;
extern GateServerApp* g_app;
extern BYTE g_wpe;
extern BYTE g_ddos;

extern BYTE g_rsaaes;
extern dbc::uShort g_wpeversion;

extern HANDLE hConsole;
extern dbc::InterLockedLong g_exit;

#define C_PRINT(s, ...) \
	SetConsoleTextAttribute(hConsole, 14); \
	printf(s, __VA_ARGS__); \
	SetConsoleTextAttribute(hConsole, 10);

#define C_TITLE(s) \
	char szPID[32]; \
	_snprintf_s(szPID,sizeof(szPID),_TRUNCATE, "%d", GetCurrentProcessId()); \
	std::string strConsoleT; \
	strConsoleT += "[PID:"; \
	strConsoleT += szPID; \
	strConsoleT += "]"; \
	strConsoleT += s; \
	SetConsoleTitle(strConsoleT.c_str());

extern CResourceBundleManage g_ResourceBundleManage; //Add by lark.li 20080130

