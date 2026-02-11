#pragma once

#include "NetIF.h"
#include "PacketCmd.h"

/****************************************************************
		发送Packet打包函数声明段
		添加:服务器开发人员
		修改:服务器开发人员
		调用:客户端开发人员
		代码体请填入文件"NetPkXXX.cpp"(注意:有Pk,意为网络包处理的cpp文件)
*****************************************************************/
extern void	CM_GUILD_PUTNAME(bool confirm,cChar *guildname,cChar *passwd);	//发送创建公会的名字
extern void CM_GUILD_TRYFOR(uLong	guildid);	//申请加入公会
extern void CM_GUILD_TRYFORCFM(bool confirm);	//确认加入confirm =true;
extern void CM_GUILD_LISTTRYPLAYER();			//管理CMD_CM_GUILD_LISTTRYMEMBER
extern void CM_GUILD_APPROVE(uLong	chaid);		//管理人员批准申请人加入
extern void CM_GUILD_REJECT(uLong	chaid);
extern void CM_GUILD_KICK(uLong		chaid);
extern void CM_GUILD_LEAVE();
extern void CM_GUILD_DISBAND(cChar *passwd);
extern void CM_GUILD_MOTTO(cChar *motto);
extern void CM_GUILD_CHALL( BYTE byLevel, DWORD dwMoney );
extern void CM_GUILD_LEIZHU( BYTE byLevel, DWORD dwMoney );



/****************************************************************
		接收Packet后调用的本地接口声明段
		添加:服务器开发人员
		修改:客户端开发人员
		调用:“接收的Packet解析函数声明段”对应函数
		代码体请填入文件"NetXXX.cpp"(注意:没有Pk,相对于NetPkXXX,没有包处理)
*****************************************************************/
extern void NetMC_GUILD_GETNAME();
extern void NetMC_LISTGUILD_BEGIN();
extern void NetMC_LISTGUILD_END();
extern void NetMC_LISTGUILD(uLong id,cChar *name,cChar *motto,cChar *leadername,uShort memtotal,LLong exp);
extern void NetMC_GUILD_TRYFORCFM(cChar *oldgldname);
extern void NetMC_LISTTRYPLAYER_BEGIN(uLong gldid,cChar *gldname,cChar *motto,char stat,cChar *ldrname,uShort memnum,uShort maxmem,LLong exp,uLong remain);
extern void NetMC_LISTTRYPLAYER_END();
extern void NetMC_LISTTRYPLAYER(uLong chaid,cChar *chaname,cChar *job,uShort degree);

extern void NetPC_GUILD_ONLINE(uLong chaid);
extern void NetPC_GUILD_OFFLINE(uLong chaid);
extern void NetPC_GUILD_UPDATEPERM(uLong chaid,uLong perm);
extern void NetPC_GUILD_START_BEGIN(uLong guildid,cChar *guildname,uLong leaderid);
extern void NetPC_GUILD_START_END();
extern void NetPC_GUILD_START(bool online,uLong chaid,cChar *chaname,cChar *motto,cChar *job,uShort degree,uShort icon,uLong permission);
extern void NetPC_GUILD_STOP();

extern void NetPC_GUILD_ADD(bool online,uLong chaid,cChar *chaname,cChar *motto,cChar *job,uShort degree,uShort icon,uLong permission);
extern void NetPC_GUILD_DEL(uLong chaid);

extern void NetMC_GUILD_MOTTO(cChar *motto);
extern void NetMC_GUILD_INFO( DWORD dwCharID, DWORD dwGuildID, const char szGuildName[], const char szGuildMotto[],uLong chGuildPermission);
extern void NetMC_GUILD_CHALLINFO( const NET_GUILD_CHALLINFO& Info );





/****************************************************************
		接收的Packet解析函数声明段
		添加:服务器开发人员
		修改:服务器开发人员
		调用:NetIF::HandlePacketMessage
		代码体请填入文件"NetPkXXX.cpp"(注意:有Pk,意为网络包处理的cpp文件)
*****************************************************************/
extern BOOL	MC_GUILD_GETNAME(LPRPACKET pk);
extern bool	g_listguild_begin;
extern BOOL MC_LISTGUILD(LPRPACKET pk);
extern BOOL MC_GUILD_TRYFORCFM(LPRPACKET pk);
extern BOOL MC_GUILD_LISTTRYPLAYER(LPRPACKET pk);
extern bool g_guild_start_begin;
extern BOOL PC_GUILD(LPRPACKET pk);
extern BOOL PC_GUILD_PERM(LPRPACKET pk);
extern BOOL MC_GUILD_MOTTO(LPRPACKET pk);
extern BOOL MC_GUILD_LEAVE(LPRPACKET pk);
extern BOOL MC_GUILD_KICK(LPRPACKET pk);
extern BOOL	MC_GUILD_INFO(LPRPACKET pk);
extern BOOL MC_GUILD_LISTCHALL(LPRPACKET pk);



