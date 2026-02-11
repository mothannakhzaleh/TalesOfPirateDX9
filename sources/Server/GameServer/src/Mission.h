// Mission.h Created by knight-gongjian 2004.12.13.
//---------------------------------------------------------
#pragma once

#ifndef _MISSION_H_
#define _MISSION_H_

#include "RoleData.h"
//---------------------------------------------------------
_DBC_USING

class CCharacter;
namespace mission
{
	class CCharMission
	{
	public:		
		CCharMission();
		~CCharMission();

		// 任务记录标签保存读取
		BOOL	MisInit( char* pszBuf );
		BOOL	MisGetData( char* pszBuf, DWORD dwSize );

		// 任务历史记录标签存储
		BOOL	MisInitRecord( char* pszBuf );
		BOOL	MisGetRecord( char* pszBuf, DWORD dwSize );

		// 任务触发器保存和读取
		BOOL	MisInitTrigger( char* pszBuf );
		BOOL	MisGetTrigger( char* pszBuf, DWORD dwSize );

		// 随机任务完成计数
		BOOL	MisInitMissionCount( char* pszBuf );
		BOOL	MisGetMissionCount( char* pszBuf, DWORD dwSize );

		void	MisClear();
		void	SetMisChar( CCharacter& character ) { m_pRoleChar = &character; }

		// 触发器事件处理
		BOOL	MisEventProc( TRIGGER_EVENT e, WPARAM wParam, LPARAM lParam );

		// 记录信息
		BOOL	MisAddTrigger( const TRIGGER_DATA& Data );
		BOOL	MisClearTrigger( WORD wTriggerID );
		BOOL	MisDelTrigger( WORD wTriggerID );

		// 任务操作
		BOOL	MisAddRole( WORD wRoleID, WORD wScriptID );
		BOOL	MisHasRole( WORD wRoleID );
		BOOL	MisClearRole( WORD wRoleID );
		BOOL	MisCancelRole( WORD wRoleID );
		BOOL	MisIsRoleFull() { return m_byNumMission >= ROLE_MAXNUM_MISSION; }
		BOOL	MisGetMisScript( WORD wRoleID, WORD& wScriptID );

		// 设置任务完成标记
		BOOL	MisSetMissionComplete( WORD wRoleID );
		BOOL	MisSetMissionPending( WORD wRoleID );
		BOOL	MisSetMissionFailure( WORD wRoleID );
		BOOL	MisHasMissionFailure( WORD wRoleID );

		// 任务标签
		BOOL	MisSetFlag( WORD wRoleID, WORD wFlag );
		BOOL	MisClearFlag( WORD wRoleID, WORD wFlag );
		BOOL	MisIsSet( WORD wRoleID, WORD wFlag );
		BOOL	MisIsValid( WORD wFlag );
		
		BOOL	MisSetRecord( WORD wRec );
		BOOL	MisClearRecord( WORD wRec );
		BOOL	MisIsRecord( WORD wRec );
		BOOL	MisIsValidRecord( WORD wRec );

		// 记录视野内NPC的针对该角色的任务状态信息
		BOOL	MisAddMissionState( DWORD dwNpcID, BYTE byID, BYTE byState );
		BOOL	MisGetMissionState( DWORD dwNpcID, BYTE& byState );
		BOOL    MisClearMissionState( DWORD dwNpcID );
		BOOL	MisGetNumMission( DWORD dwNpcID, BYTE& byNum );
		BOOL	MisGetMissionInfo( DWORD dwNpcID, BYTE byIndex, BYTE& byID, BYTE& byState );
		BOOL	MisGetCharMission( DWORD dwNpcID, BYTE byID, BYTE& byState );
		BOOL	MisGetNextMission( DWORD dwNpcID, BYTE& byIndex, BYTE& byID, BYTE& byState );

		// 任务快捷临时数据信息操作
		void	MisSetMissionPage( DWORD dwNpcID, BYTE byPrev, BYTE byNext, BYTE byState );
		BOOL	MisGetMissionPage( DWORD dwNpcID, BYTE& byPrev, BYTE& byNext, BYTE& byState );
		void	MisSetTempData( DWORD dwNpcID, WORD wID, BYTE byState, BYTE byMisType );
		BOOL	MisGetTempData( DWORD dwNpcID, WORD& wID, BYTE& byState, BYTE& byMisType );
		
		// 查询任务日志信息
		void	MisGetMisLog();
		void	MisGetMisLogInfo( WORD wMisID );
		void	MisLogClear( WORD wMisID );
		void	MisLogAdd( WORD wMisID, BYTE byState );

		// 随机任务接口操作信息
		BOOL	MisHasRandMission( WORD wRoleID );
		BOOL	MisAddRandMission( WORD wRoleID, WORD wScriptID, BYTE byType, BYTE byLevel, DWORD dwExp, DWORD dwMoney, USHORT sPrizeData, USHORT sPrizeType, BYTE byNumData );
		BOOL	MisSetRandMissionData( WORD wRoleID, BYTE byIndex, const mission::MISSION_DATA& RandData );
		BOOL	MisGetRandMission( WORD wRoleID, BYTE& byType, BYTE& byLevel, DWORD& dwExp, DWORD& dwMoney, USHORT& sPrizeData, USHORT& sPrizeType, BYTE& byNumData );
		BOOL	MisGetRandMissionData( WORD wRoleID, BYTE byIndex, mission::MISSION_DATA& RandData );

		// 检测送给npc的物品(接受物品的NPC取走物品后，记录一个标记，是否取走用该标记标识)
		BOOL	MisHasSendNpcItemFlag( WORD wRoleID, WORD wNpcID );
		BOOL	MisNoSendNpcItemFlag( WORD wRoleID, WORD wNpcID );
		BOOL	MisTakeRandMissionNpcItem( WORD wRoleID, WORD wNpcID, USHORT& sItemID );
		BOOL	MisTakeAllRandNpcItem( WORD wRoleID );
		BOOL	MisHasRandMissionNpc( WORD wRoleID, WORD wNpcID, WORD wAreaID );		

		// 添加一个护送NPC
		BOOL	MisAddFollowNpc( WORD wRoleID, BYTE byIndex, WORD wNpcCharID, CCharacter* pNpc, BYTE byAiType );
		BOOL	MisClearFollowNpc( WORD wRoleID, BYTE byIndex );
		BOOL	MisClearAllFollowNpc( WORD wRoleID );
		BOOL	MisHasFollowNpc( WORD wRoleID, BYTE byIndex );
		BOOL	MisIsFollowNpc( WORD wRoleID, BYTE byIndex, WORD wNpcCharID );
		BOOL	MisLowDistFollowNpc( WORD wRoleID, BYTE byIndex );

		// 完成一个随机任务
		BOOL	MisCompleteRandMission( WORD wRoleID );
		BOOL	MisFailureRandMission( WORD wRoleID );
		BOOL	MisAddRandMissionNum( WORD wRoleID );

		BOOL	MisResetRandMission( WORD wRoleID );
		BOOL	MisResetRandMissionNum( WORD wRoleID );
		WORD	MisGetRandMissionCount( WORD wRoleID );
		WORD	MisGetRandMissionNum( WORD wRoleID );

		// 检测是否任务需要物品
		BOOL	MisNeedItem( USHORT sItemID );

		// 重新刷新获取物品触发器的计数
		void	MisRefreshItemCount( USHORT sItemID );
		BOOL	MisGetItemCount( WORD wRoleID, USHORT sItemID, USHORT& sCount );

		// 角色离线
		void	MisLogout();
		void	MisLogin();
		void	MisEnterMap();
		void	MisGooutMap();
		void	MisEnterBoat( CCharacter* pBoat );

		// 任务事件实体交互等待时间
		void	SetEntityTime( DWORD dwTime ) { m_dwEntityTime = dwTime; }
		BOOL	GetEntityTime( DWORD& dwTime ) { dwTime = m_dwEntityTime; return dwTime != 0; }

	protected:
		
		// 预分配重用缓冲，初始化信息
		void	Initially();
		void	Finally();

		void	DeleteTrigger();
		void	ClearTrigger( DWORD dwIndex );
		void	ClearRoleTrigger( WORD wRoleID );
		BOOL	CancelRole( WORD wRoleID, WORD wScriptID );

		// 任务事件处理函数
		void	KillWare( USHORT sWareID );
		void	GetItem( USHORT sItemID, USHORT sCount );
		void	TimeOut( USHORT sTime );
		void	GotoMap( BYTE byMapID, WORD wxPos, WORD wyPos );
		void	LevelUp( USHORT sLevel );
		void	CharBorn();
		void	EquipItem( USHORT sItemID, USHORT sTriID );

		// 根据数据库存储的信息版本转换到当前版本信息
		BOOL ConvertMissionInfo( const char* pszBuf, int nEdition );
		BOOL ConvertTriggerInfo( const char* pszBuf, int nEdition );
		BOOL ConvertMisCountInfo( const char* pszBuf, int nEdition );
		BOOL ConvertMissionRecord( const char* pszBuf, int nEdition );

		// 角色任务信息(触发器、标签、历史记录)
		BYTE			m_byNumTrigger;
		TRIGGER_DATA	m_Trigger[ROLE_MAXNUM_CHARTRIGGER];
		BYTE			m_byNumMission;
		MISSION_INFO	m_Mission[ROLE_MAXNUM_MISSION];
		ROLE_RECORDINFO m_RoleRecord;
		BYTE			m_byNumGotoMap; // 切换地图单元事件触发器计数
		RAND_MISSION_COUNT	m_MissionCount[ROLE_MAXNUM_MISSIONCOUNT]; // 完成随机任务计数
		BYTE			m_byNumMisCount;
		BYTE			m_byOnline;		// 该角色是否在线

		// 视野内NPC的针对该角色任务信息
		MISSION_STATE   m_MissionState[ROLE_MAXNUM_INSIDE_NPCCOUNT];
		BYTE			m_byStateIndex; // 任务状态快捷索引
		BYTE			m_byNumState;

		// 任务对话信息临时记录
		DWORD			m_dwTalkNpcID;
		WORD			m_wIndex;
		BYTE			m_byState;
		BYTE			m_byMisType;
		BYTE			m_byStep;
		BYTE			m_byPrev;
		BYTE			m_byNext;

		// 事件实体交互等待时间
		DWORD			m_dwEntityTime;

		// 角色指针
		CCharacter*		m_pRoleChar;

	};
}

//---------------------------------------------------------

#endif // _TRIGGER_EVENT_H_