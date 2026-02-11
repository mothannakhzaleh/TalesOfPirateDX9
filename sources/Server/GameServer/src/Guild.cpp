#include "stdafx.h"
#include "Guild.h"
#include "GameDB.h"
#include "util.h"
#include "dstring.h"
#include "parser.h"
#include "GameCommon.h"
#include "GameApp.h"


BOOL Guild::lua_CreateGuild(CCharacter* pCha)//创建公会,1-海军,2-海盗
{
	if(pCha->GetPlayer()->m_GuildState.IsFalse(emGuildGetName))//请求用户输入公会名和密码
	{
		pCha->GetPlayer()->m_GuildState.SetBit(emGuildGetName);

		WPACKET	l_wpk =GETWPACKET();
		WRITE_CMD(l_wpk, CMD_MC_GUILD_GETNAME);
		pCha->ReflectINFof(pCha,l_wpk);
	}
	return TRUE;
}
void Guild::cmd_CreateGuild(CCharacter* pCha, bool confirm, cChar *guildname, cChar *passwd)
{
	if(pCha->GetPlayer()->m_GuildState.IsFalse(emGuildGetName))
	{
		return;
	}else
	{        
		pCha->GetPlayer()->m_GuildState.ClearBit(emGuildGetName);
		if(!guildname || !passwd || !confirm)
		{
			return;
		}

        if(pCha->m_CKitbag.IsPwdLocked())
        {
            //pCha->SystemNotice("道具栏已锁定,无法建立工会");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00001));
			return;
        }

		//add by ALLEN 2007-10-16
		if(pCha->IsReadBook())
        {
            //pCha->SystemNotice("读书状态,无法建立工会");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00002));
			return;
        }
        
		short l_len	=short(strlen(guildname));
		if(l_len <1)
		{
			//pCha->SystemNotice("公会名太短");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00003));
			return;
		}else if(l_len >16)
		{
			//pCha->SystemNotice("公会名太长");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00004));
			return;
		}else if(strlen(passwd) >16)
		{
			//pCha->SystemNotice("公会口令太长");
			pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00005));
			return;
		}
		//else if(!IsValidGuildName(guildname,l_len))
		//{
		//	pCha->SystemNotice("Guild name includes illegal character");
		//	return;
		//}
	}
	if (g_CParser.DoString("AskGuildItem", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END))
	{
		if (!g_CParser.GetReturnNumber(0))	//条件不满足
			return;
	}
	else
	{
		return;
	}
	char l_guildname[32];
	strcpy(l_guildname,guildname);
	long l_guildid =game_db.CreateGuild(pCha,l_guildname,passwd);
	if(!l_guildid)
	{
		return;
	}

	//调用同步公会名字函数
	pCha->SetGuildName( l_guildname );//设置公会名字
	pCha->SetGuildID( l_guildid	);		//设置公会ID
	pCha->SetGuildState( 0 );
	pCha->guildPermission = emGldPermMax;
	pCha->SyncGuildInfo();

	//pCha->setAttr(ATTR_GUILD,l_guildid);		
	//pCha->setAttr(ATTR_GUILD_TYPE,pCha->GetPlayer()->m_cGuildType);//设置公会类型
	//pCha->setAttr(ATTR_GUILD_STATE,0);			//设置公会状态

	//调用lua脚本扣除物品Begin
	g_CParser.DoString("DeductGuildItem", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END);
	//调用lua脚本扣除物品End
	
	//pCha->SystemNotice("公会创建成功.");
	pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00006));

	char l_str[512];
	sprintf(l_str, "Player [%s] has founded [%s] guild. All Pirates may now apply!", 
			pCha->GetName(), 
			l_guildname);
	
	g_pGameApp->ScrollNotice(l_str, 2);
}
BOOL Guild::lua_ListAllGuild(CCharacter* pCha)			//开始给客户端传递列表，向客户端发生一个开始列表命令，由NPC对话触发一次
{
	cmd_ListAllGuild(pCha);
	return TRUE;
}
void Guild::cmd_ListAllGuild(CCharacter* pCha)			//调用一次返回20行，由客户端命令分次调用，直到返回所有行
{
	game_db.ListAllGuild(pCha,7);
}
void Guild::cmd_GuildTryFor(CCharacter* pCha, uLong guildid)			//申请加入公会
{
	if(!guildid)
	{
		pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00007));
	}else if(guildid >199)
	{
		pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00008));
	}else
	{
		if (g_CParser.DoString("AskJoinGuild", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCha, DOSTRING_PARAM_END))
		{
			if (!g_CParser.GetReturnNumber(0))	//条件不满足
				return;
		}
		else
		{
			return;
		}
		game_db.GuildTryFor(pCha,guildid);
	}
}
void Guild::cmd_GuildTryForComfirm(CCharacter* pCha, char IsReplace)
{
	if(pCha->GetPlayer()->m_GuildState.IsTrue(emGuildReplaceOldTry))
	{
		if(IsReplace ==1)
		{
			game_db.GuildTryForConfirm(pCha,pCha->GetPlayer()->m_lTempGuildID);
		}
		pCha->GetPlayer()->m_GuildState.ClearBit(emGuildReplaceOldTry);
	}
}
void Guild::cmd_GuildListTryPlayer(CCharacter* pCha)
{

	game_db.GuildListTryPlayer(pCha,7);
}
void Guild::cmd_GuildApprove(CCharacter* pCha,uLong chaid)
{

	game_db.GuildApprove(pCha,chaid);
}
void Guild::cmd_GuildReject(CCharacter* pCha,uLong chaid)
{

	game_db.GuildReject(pCha,chaid);
}
void Guild::cmd_GuildKick(CCharacter* pCha,uLong chaid)
{

	game_db.GuildKick(pCha,chaid);
}
void Guild::cmd_GuildLeave(CCharacter* pCha)
{

	game_db.GuildLeave(pCha);
}
void Guild::cmd_GuildDisband(CCharacter* pCha,cChar *passwd)
{

	game_db.GuildDisband(pCha,passwd);
}
void Guild::cmd_GuildMotto(CCharacter* pCha,cChar *motto)
{

	game_db.GuildMotto(pCha,motto);
}
void Guild::cmd_GuildChallenge( CCharacter* pCha, BYTE byLevel, DWORD dwMoney )
{
    if(pCha->m_CKitbag.IsPwdLocked())
    {
        //pCha->SystemNotice("道具栏已锁定,无法挑战工会");
		pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00009));
        return;
    }

	//add by ALLEN 2007-10-16
	if(pCha->IsReadBook())
    {
       // pCha->SystemNotice("读书状态,无法挑战工会");
		 pCha->SystemNotice(RES_STRING(GM_GUILD_CPP_00010));
        return;
    }
    
	game_db.Challenge( pCha, byLevel, dwMoney );
}
void Guild::cmd_GuildLeizhu( CCharacter* pCha, BYTE byLevel, DWORD dwMoney )
{
	game_db.Leizhu( pCha, byLevel, dwMoney );	
}
void Guild::cmd_PMDisband(CCharacter *pCha)
{
	//pCha->m_CChaAttr.ResetChangeFlag();

	//pCha->setAttr(ATTR_GUILD,0);			//设置公会ID
	//pCha->setAttr(ATTR_GUILD_STATE,0);		//设置公会状态

	//pCha->SynAttr(enumATTRSYN_TRADE);

	//pCha->SetGuildName("");
}
