#include "StdAfx.h"
#include "EventHandler.h"
#include "Character.h"
#include "Player.h"
#include "Parser.h"
#include "lua_gamectrl.h"



//-------------------------------------
// 事件 : 角色死亡
// 处理角色死亡, 攻击者经验分配, 升级等
//-------------------------------------
void CEventHandler::Event_ChaDie(CCharacter *pDead, CCharacter *pAtk)
{
	BOOL bTeam  = FALSE;
	
	// 记录相关的所有角色, 包括曾经攻击过怪物的人和他们的队友
	CCharacter *pValidCha[25] = { NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL};
	
	int nValidCha = 0;
	
	CPlayer *pPlayer = pAtk->GetPlayer();
	if(pPlayer==NULL) // 非玩家角色
	{
		MPTimer t;
		t.Begin();
		// 死亡后计算经验扣除
		extern lua_State *g_pLuaState;
		lua_getglobal(g_pLuaState, "GetExp_New");
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			LG( "lua_invalidfunc", "GetExp_New" );
			return;
		}

		lua_pushlightuserdata(g_pLuaState, (void *)pDead);
		lua_pushlightuserdata(g_pLuaState, (void *)pAtk);
		int r = lua_pcall(g_pLuaState, 2, 0, 0); 
		if(r!=0) // 执行出错
		{
			//LG("lua_err", "GetExp_New执行出错, 攻击者(怪)[%s], 被打死的人[%s]!\n", pAtk->GetName(), pDead->GetName());
			LG("lua_err", "GetExp_New transact error, attacker(bugbear)[%s], people was bring down[%s]!\n", pAtk->GetName(), pDead->GetName());
			lua_callalert(g_pLuaState, r); 	
		}
		lua_settop(g_pLuaState, 0);
		pDead->ItemCount(pAtk); // 掉料

		DWORD dwEndTime = t.End();
		if(dwEndTime > 10)
		{
			//LG("script_time", "玩家死亡调用经验分配脚本, 计算时间过长! time = %d\n", dwEndTime);
			LG("script_time", "when player dead transfer experience assign script,account time too long! time = %d\n", dwEndTime);
		}
		return; // 怪物攻击玩家, 不往下执行
	}
	
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = pDead->m_pHate->GetHarmRec(i);
		if(pHarm->btValid > 0 && pHarm->IsChaValid())
		{
			BOOL bAdd = TRUE;
			for(int j = 0; j < nValidCha; j++)
			{
				if(pHarm->pAtk==pValidCha[j])
				{
					bAdd = FALSE;
					break;
				}
			}
			if(bAdd)
			{
				pValidCha[nValidCha] = pHarm->pAtk;
				nValidCha++;
			}
			
			// 把当前角色的所有队友也添加进来
			pPlayer = pHarm->pAtk->GetPlayer();
			if(pPlayer==NULL)
			{
				//LG("team_error", "检查队友分经验时出现特别错误, player指针为空!, 角色名[%s]\n", pHarm->pAtk->GetName());
				LG("team_error", "it appear especially error when check teammate experience assign, player finger is null!, character name[%s]\n", pHarm->pAtk->GetName());
				break;
			}
			
			for(int i = 0; i < pPlayer->GetTeamMemberCnt(); i++) // 队伍里每一个人都需要计算升级
			{
				CCharacter *pOther = pPlayer->GetTeamMemberCha(i);
				if (!pOther)			 continue;
				if(!pOther->IsLiveing()) continue;

				BOOL bAdd = TRUE;
				for(int j = 0; j < nValidCha; j++)
				{
					if(pOther==pValidCha[j])
					{
						bAdd = FALSE;
						break;
					}
				}
				if(bAdd)
				{
					pValidCha[nValidCha] = pOther;
					nValidCha++;
				}
			}
		}
	}
	
	// 为这些角色增加属性更新标记
	for(int i =0; i < nValidCha; i++)
	{
		CCharacter *pCur = pValidCha[i];
		if(pCur!=pAtk)	pCur->GetPlyMainCha()->m_CChaAttr.ResetChangeFlag();
	}
	
	MPTimer t;
	t.Begin();
	
	// 死亡后计算经验
	extern lua_State *g_pLuaState;
	lua_getglobal(g_pLuaState, "GetExp_New");
	if( !lua_isfunction( g_pLuaState, -1 ) )
	{
		lua_pop( g_pLuaState, 1 );
		LG( "lua_invalidfunc", "GetExp_New" );
		return;
	}

	MPTimer tLua; tLua.Begin();
	lua_pushlightuserdata(g_pLuaState, (void *)pDead);
	lua_pushlightuserdata(g_pLuaState, (void *)pAtk);
	int r = lua_pcall(g_pLuaState, 2, 0, 0); 
	if(r!=0) // 执行出错
	{
		//LG("lua_err", "GetExp_New执行出错, 攻击者[%s], 被打死的怪[%s]!\n", pAtk->GetName(), pDead->GetName());
		LG("lua_err", "GetExp_New transact error, attacker[%s], bugbear wsa bring down [%s]!\n", pAtk->GetName(), pDead->GetName());
		lua_callalert(g_pLuaState, r); 	
	}
	lua_settop(g_pLuaState, 0);
	tLua.End();
	
	// 为这些角色增加属性变更通知
	for(int i =0; i < nValidCha; i++)
	{
		CCharacter *pCur = pValidCha[i];
		if(pCur!=pAtk) pCur->GetPlyMainCha()->SynAttr(enumATTRSYN_ATTACK);
	}

	MPTimer tMission; tMission.Begin();
	// 攻击者和他的队友得到任务通知
	pPlayer = pAtk->GetPlayer();
	pAtk->AfterObjDie(pAtk, pDead);
	for(int i = 0; i < pPlayer->GetTeamMemberCnt(); i++) // 队伍里每一个人都需要计算升级
	{
		CCharacter *pOther = pPlayer->GetTeamMemberCha(i);
		if (!pOther)			 continue;
		if(!pOther->IsLiveing()) continue;
		pOther->AfterObjDie(pAtk, pDead);
	}
	tMission.End();
		
	MPTimer tItem; tItem.Begin();
	// 掉料
	pDead->ItemCount(pAtk);

	tItem.End();
	
	DWORD dwEndTime = t.End();
	if(dwEndTime > 10)
	{
		//LG("script_time", "怪物死亡经验分配流程, 计算时间过长, time = %d, exp = %d, upgrade = %d, item = %d!\n", dwEndTime, tLua.GetTimeCount(), tMission.GetTimeCount(), tItem.GetTimeCount());
		LG("script_time", "the flow of assign experience when bugbear dead, calculate time too long, time = %d, exp = %d, upgrade = %d, item = %d!\n", dwEndTime, tLua.GetTimeCount(), tMission.GetTimeCount(), tItem.GetTimeCount());
	}
}
