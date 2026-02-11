// CharForge.cpp Created by knight-gongjian 2004.12.7.
//---------------------------------------------------------
#include "stdafx.h"
#include "CharForge.h"
#include <ForgeRecord.h>
#include "GameAppNet.h"
#include "Player.h"

//---------------------------------------------------------

mission::CForgeSystem g_ForgeSystem;

namespace mission
{

	CForgeSystem::CForgeSystem()
	{
		m_pRecordSet = NULL;
	}

	CForgeSystem::~CForgeSystem()
	{
		Clear();
	}

	void CForgeSystem::Clear()
	{
		SAFE_DELETE( m_pRecordSet );
	}

	BOOL CForgeSystem::LoadForgeData( char szName[] )
	{
		Clear();
		
		extern BOOL LoadTable(CRawDataSet *pTable, const char*);
		m_pRecordSet = new CForgeRecordSet( 1, ROLE_MAXNUM_FORGE );
		return LoadTable(m_pRecordSet, szName);
	}

	void CForgeSystem::ForgeItem( CCharacter& character, BYTE byIndex )
	{
		// 判断是否在交易状态
		if( character.m_CKitbag.IsLock() )
		{
			character.SystemNotice( "你的背包已被锁定，不可以精练物品！" );
			return;
		}

        //密码锁定
        if( character.m_CKitbag.IsPwdLocked() )
        {
            character.SystemNotice( "你的背包已被密码锁定，不可以精练物品！" );
			return;
        }
		//add by ALLEN 2007-10-16
				if( character.IsReadBook() )
        {
            character.SystemNotice( "正在读书，不可以精练物品！" );
			return;
        }
		SItemGrid *pItemData;
		if( !(pItemData = character.m_CKitbag.GetGridContByID( byIndex )) )
		{
			character.SystemNotice( "ForgeItem:错误的背包栏位索引！ ID = %d", byIndex );
			return;
		}
		
		CItemRecord* pItem = GetItemRecordInfo( pItemData->sID );
		if( pItem == NULL )
		{
			character.SystemNotice( "ForgeItem:错误的精练物品！ ID = %d", pItemData->sID );
			return;
		}

		if( pItem->chForgeLv == 0 )
		{
			character.SystemNotice( "物品《%s》不可以精练！", pItem->szName );
			return;
		}

		BYTE byLevel = pItemData->chForgeLv;
		if( byLevel >= ROLE_MAXNUM_FORGE )
		{
			character.SystemNotice( "你的《%s》已经是精练顶级装备！", pItem->szName );
			return;
		}
		
		// 精练到下一级
		byLevel++;

		CForgeRecord* pRecord = (CForgeRecord*)m_pRecordSet->GetRawDataInfo( byLevel );
		if( !pRecord )
		{
			character.SystemNotice( "ForgeItem:无效的精练等级！Level = %d", byLevel );
			return;
		}

		if( !character.HasMoney( pRecord->dwMoney ) )
		{
			character.SystemNotice( "精练道具所需金钱不足，精练失败！" );
			return;
		}

		for( int i = 0; i < FORGE_MAXNUM_ITEM; i++ )
		{
			if( pRecord->ForgeItem[i].sItem == 0 )
				break;
			if( !character.HasItem( pRecord->ForgeItem[i].sItem, pRecord->ForgeItem[i].byNum ) )
			{
				char szForgeItem[64] = "未知";
				CItemRecord* pForgeItem = (CItemRecord*)GetItemRecordInfo( pRecord->ForgeItem[i].sItem );
				if( pForgeItem )
				{
					strcpy( szForgeItem, pForgeItem->szName );
				}
				character.SystemNotice( "缺少精练需求物品《%s》，共计%d个，精练失败！", szForgeItem, pRecord->ForgeItem[i].byNum );
				return;
			}
		}

		BOOL bSuccess = TRUE;
		// 判断是否超出
		if( byLevel > pItem->chForgeLv )
		{
			// 超过精练物品允许精练等级失败，惩罚
			bSuccess = FALSE;
		}
		else
		{
			// 判断是否 超过精练安定值
			if( byLevel > pItem->chForgeSteady )
			{
				// 
				if( rand()%100 >= pRecord->byRate )
				{
					// 精练物品运气差失败，惩罚
					bSuccess = FALSE;
				}
			}
		}

		for( int i = 0; i < FORGE_MAXNUM_ITEM; i++ )
		{
			if( pRecord->ForgeItem[i].sItem == 0 )
				break;
			if( !character.TakeItem( pRecord->ForgeItem[i].sItem, pRecord->ForgeItem[i].byNum, "系统" ) )
			{
				char szForgeItem[64] = "未知";
				CItemRecord* pForgeItem = (CItemRecord*)GetItemRecordInfo( pRecord->ForgeItem[i].sItem );
				if( pForgeItem )
				{
					strcpy( szForgeItem, pForgeItem->szName );
				}
				character.SystemNotice( "取走精练需求物品《%s》，共计%d个失败！", szForgeItem, pRecord->ForgeItem[i].byNum );
				return;
			}
		}

		if( bSuccess )
		{
			// 设置成功后精练等级
			character.m_CKitbag.SetChangeFlag(false);
			SItemGrid* pGrid = character.m_CKitbag.GetGridContByID( byIndex );
			if( pGrid == NULL || !character.ItemForge( pGrid, byLevel ) )
			{
				character.SystemNotice( "错误：精练成功，设置物品《%s》精练等级(%d)失败！", pItem->szName, byLevel );
				return;
			}

			character.SystemNotice( "精练物品《%s》成功，当前精练等级(%d)！", pItem->szName, byLevel );
			character.SynKitbagNew( enumSYN_KITBAG_FORGES );
		}
		else
		{
			if( pRecord->byFailure == BYTE(-1) )
			{
				character.m_CKitbag.SetChangeFlag(false);
				character.KbClearItem( true, true, byIndex );
				character.SystemNotice( "精练物品《%s》不幸失败，导致物品损毁！", pItem->szName );
				character.SynKitbagNew( enumSYN_KITBAG_FORGEF );
			}
			else
			{
				// 设置成功后精练等级
				character.m_CKitbag.SetChangeFlag(false);
				byLevel = pRecord->byFailure;
				SItemGrid* pGrid = character.m_CKitbag.GetGridContByID( byIndex );
				if( pGrid == NULL ||  !character.ItemForge( pGrid, byLevel ) )
				{
					character.SystemNotice( "精练失败，设置物品《%s》精练等级(%d)失败！", pItem->szName, byLevel );
					return;
				}

				character.SystemNotice( "精练物品《%s》失败，退回到当前精练等级(%d)！", pItem->szName, byLevel );
				character.SynKitbagNew( enumSYN_KITBAG_FORGEF );
			}
		}
	}

}
