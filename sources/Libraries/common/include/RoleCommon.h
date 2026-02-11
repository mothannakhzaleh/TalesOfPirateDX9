// RoleCommon.h Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#pragma once

#ifndef _ROLECOMMON_H_
#define _ROLECOMMON_H_

//---------------------------------------------------------

// 
#define ROLE_MAXNUM_DESCPAGE				12		// 描述信息最大页数
#define ROLE_MAXNUM_FUNCPAGE				4		// 功能描述信息最大页数
#define ROLE_MAXNUM_FLAGSIZE				16		// 标记尺寸（单位1字节)可执行128步
#define ROLE_MAXNUM_RECORDSIZE				256     // 历史任务标记记录尺寸（单位1字节）共计1024个
#define ROLE_MAXSIZE_DBMISSION				2048 	// 数据库保存任务数据字节空间
#define ROLE_MAXSIZE_DBTRIGGER				2048	// 数据库保存任务触发器数据字节尺寸
#define ROLE_MAXSIZE_DBMISCOUNT				512		// 数据库保存任务完成计数字节尺寸
#define ROLE_MAXSIZE_DBRECORD				1024	// 数据库保存任务历史记录数据字节尺寸
#define ROLE_MAXNUM_INDEXSIZE				8		// 每个字节建立8个任务记录索引
#define ROLE_MAXNUM_FUNCITEM				8		// 每个功能页最大有8个功能项
#define ROLE_MAXNUM_DESPSIZE				1024    // 最大对话页描述信息字节长度
#define ROLE_MAXNUM_NEEDDESPSIZE			256     // 最大对话页需求描述信息字节长度
#define ROLE_MAXNUM_FUNCITEMSIZE			64      // 最大功能页每个项目描述信息字节长度
#define ROLE_MAXNUM_CAPACITY				64		// 每个npc最大携带任务数据索引尺寸
#define ROLE_MAXNUM_TRADEITEM				120     // 每个npc最大携带单个类型物品索引尺寸
#define ROLE_MAXNUM_TRADEDATA				18      // 角色交易时每个角色可以放置的物品最大栏位
#define ROLE_MAXNUM_ITEMTRADE				99		// 每次允许的一个交易堆叠物品最大计数
#define ROLE_MAXNUM_CHARTRIGGER				64      // 每个角色最大携带的任务触发器数量
#define ROLE_MAXNUM_NPCTRIGGER				4		// 每个NPC最大携带的任务触发器数量
#define ROLE_MAXNUM_FLAG					16		// 每个角色可以携带20个任务
#define ROLE_MAXNUM_MISSIONSTATE			32		// 每个NPC角色可带的最大任务数量
#define ROLE_MAXNUM_MISSION_STEP			16		// 每个NPC带的每个任务的最大步骤
#define ROLE_MAXNUM_RANDMISSION				10		// 每个角色最大可以携带4个随机任务
#define ROLE_MAXNUM_INSIDE_NPCCOUNT			24		// 角色同步视野内最大任务NPCE数量
#define ROLE_MAXSIZE_TRADEDATA				2000	// 每个服务器同时最大可运行最大数量的交易
#define ROLE_MAXSIZE_STALLDATA				2000	// 每个服务器同时最大可运行最大数量的摆摊交易
#define ROLE_MAXSIZE_MSGPROC				16		// npc消息处理函数名称长度
#define ROLE_MAXVALUE_PARAM					16      // 每个标记携带的最大参数值
#define ROLE_MAXNUM_MISNEED					8		// 任务最大的需求数量
#define ROLE_MAXNUM_MISPRIZE				8		// 任务最大的奖励数量
#define ROLE_MAXSIZE_MISNAME				32		// 任务名称最大尺寸
#define ROLE_MAXNUM_FORGE					12		// 最大精练数据信息数量
#define ROLE_MAXNUM_MISSION					10		// 每个角色可以接受最大10个任务
#define ROLE_MAXNUM_RAND_DATA				4		// 随机任务最大四个执行内容信息
#define ROLE_MAXNUM_MISSIONCOUNT			32		// 随机任务完成次数记录最大数量
#define ROLE_MAXNUM_EUDEMON					4		// 游戏世界中最大守护神数量
#define ROLE_MAXNUM_MAPNPC					512		// 每个地图最大NPC数量
#define ROLE_MAXNUM_STALL_GOODS				24		// 最大摆摊交易货物
#define ROLE_MAXNUM_STALL_NUM				64		// 最大摆摊名称字节长度
#define ROLE_MAXSIZE_PASSWORD2				32		// 二次密码最大长度

// npc对话信息
#define ROLE_CLOSE_DESC						"您现在没有符合条件可以接受的任务！"

// 对话信息命令
#define ROLE_FIRSTPAGE						0		// 请求初次对话信息
#define ROLE_CLOSEPAGE						-1		// 请求关闭对话页命令

// npc交易信息
#define ROLE_TRADE_SALE						0		// 请求出售物品
#define ROLE_TRADE_BUY						1		// 请求购买物品
#define ROLE_TRADE_SALE_GOODS				2		// 请求出售货舱物品
#define ROLE_TRADE_BUY_GOODS				3		// 请求购买货舱物品
#define ROLE_TRADE_SELECT_BOAT				4		// 选择交易的船只

// 交易时间信息定义
#define ROLE_TRADE_START					1		// 交易开始
#define ROLE_MAXNUM_TRADETIME				50000	// 最大交易时间
#define ROLE_MAXSIZE_TRADEDIST				80*80	// 最大交易距离

//npc任务状态信息定义
#define ROLE_MIS_ACCEPT						1<<0	// 有符合条件可以接的任务
#define ROLE_MIS_DELIVERY					1<<1	// 已完成可交付的任务
#define ROLE_MIS_PENDING					1<<2	// 未完成不可交付的任务
#define ROLE_MIS_IGNORE						1<<3	// 不符合条件可以接的任务

// 任务对话信息命令
#define ROLE_MIS_PREV						0		// 请求任务列表的上一页信息
#define ROLE_MIS_NEXT						1		// 请求任务列表的下一页信息
#define ROLE_MIS_PREV_END					2		// 任务列表没有上一页信息
#define ROLE_MIS_NEXT_END					3		// 任务列表没有下一页信息
#define ROLE_MIS_SEL						4		// 请求选择任务列表项目
#define ROLE_MIS_TALK						5		// 请求任务对话信息
#define ROLE_MIS_BTNACCEPT					6		// 请求接受任务
#define ROLE_MIS_BTNDELIVERY				7		// 请求交付任务
#define ROLE_MIS_BTNPENDING					8		// 未决任务请求（客户端禁止按钮）
#define ROLE_MIS_LOG						9		// 请求任务日志信息

#define ROLE_MIS_PENDING_FLAG				0		// 任务正在进行中
#define ROLE_MIS_COMPLETE_FLAG				1		// 任务已经处于完成状态标记
#define ROLE_MIS_FAILURE_FALG				2		// 任务失败

#define	EM_OK								0		// 正确
#define EM_FAILER							-1		// 错误

// 角色任务信息存储头标识和版本号，用于识别分析存储信息
#define ROLE_MIS_TRIGGER_HEADER				DWORD(*(LPDWORD)"trig")
#define ROLE_MIS_MISINFO_HEADER				DWORD(*(LPDWORD)"misn")

#define ROLE_MIS_RECORD_EDITION				0x0003	// 任务系统历史标记记录	
#define ROLE_MIS_TRIGGER_EDITION			0x0003	// 任务系统当前触发器存储版本号
#define ROLE_MIS_MISINFO_EDITION			0x0003	// 任务系统当前任务信息存储版本号
#define ROLE_MIS_MISCOUNT_EDITION			0x0003  // 随机任务完成计数信息存储版本号

namespace mission
{
	// 交易操作类型
	enum TRADE_OPTYPE	
	{ 
		TRADE_SALE				= 0,     // 出售物品操作
		TRADE_BUY				= 1,     // 购买物品操作
		TRADE_GOODS				= 2,	 // 交易船舱货物
		TRADE_DRAGTO_ITEM		= 3,     // 拖动物品从道具栏到交易栏
		TRADE_DRAGTO_TRADE		= 4,     // 拖动物品从交易栏到物品栏
		TRADE_DRAGMONEY_ITEM	= 5,	 // 拖动金钱从道具栏到交易栏
		TRADE_DRAGMONEY_TRADE	= 6,	 // 拖动金钱从交易栏到物品栏
		TRADE_SUCCESS			= 7,	 // 当前交易成功
		TRADE_FAILER			= 8,	 // 当前交易失败
	};

	// 交易的物品类型
	enum TRADE_ITEMTYPE 
	{ 
		TI_WEAPON 					= 0,	// 武器类型
		TI_DEFENCE 					= 1,	// 防具类型
		TI_OTHER    				= 2,	// 杂项类型
		TI_SYNTHESIS 				= 3,	// 合成物品类型
		MAXTRADE_ITEMTYPE			= 4,
	};

	// 角色交易类型
	enum TRADE_CHAR_TYPE
	{
		TRADE_CHAR					= 0,	// 角色之间交易
		TRADE_BOAT					= 1,	// 船只之间交易
	};

	// 时间触发器时间类型
	enum TRIGGER_TIME_TYPE
	{
		TT_CYCLETIME	= 0,			// 时间循环
		TT_MULTITIME	= 1,			// 循环运行n次
	};

	// 触发器事件类型
	enum TRIGGER_EVENT 
	{
		TE_MAP_INIT		= 0,			// 角色出生		（游戏服务器公共触发器存储）
		TE_NPC			= 1,			// npc携带		（不需要触发器存储）
		TE_KILL			= 2,			// 摧毁物件		（角色触发器存储）参数1，摧毁数量， 参数2，指定物件类型ID, 参数3，摧毁物件数量计数
		TE_GAME_TIME	= 3,			// 游戏时间		（角色触发器存储）参数1，时间类型， 参数2，运行次数， 参数3，时间间隔，参数4，时间计数
		TE_CHAT			= 4,			// 聊天关键字	（角色触发器存储）参数1，角色输入聊天关键字次数计数，参数2，聊天关键字ID， 参数3，有效聊天次数计数
		TE_GET_ITEM		= 5,			// 获得物品		（角色触发器存储）参数1，物品数量， 参数2，物品ID， 参数3，拾取物品计数
		TE_EQUIP_ITEM	= 6,			// 装备物品		（角色触发器存储）参数1，物品ID， 参数2，保留， 参数3，保留，
		TE_GOTO_MAP     = 7,			// 到达目标点	（角色触发器存储）参数1，地图ID， 参数2，地图坐标x，参数3, 地图坐标y, 参数4，坐标范围，参数5，是否主动关闭触发器
		TE_LEVEL_UP     = 8,			// 升级			（角色触发器存储）参数1，是否主动关闭（非主动关闭类型表明所有等级都触发执行动作，主动形触发器动作后关闭），参数2，指定等级触发
		TE_MAXNUM_TYPE,					// 最大事件类型数
	};

	// 任务类型
	enum MIS_TYPE
	{
		MIS_TYPE_NOMAL  = 0,			// 普通任务类型
		MIS_TYPE_RAND	= 1,			// 随机任务类型
		MIS_TYPE_WORLD	= 2,			// 世界任务类型
	};

	// 任务显示操作类型
	enum MIS_SHOW_TYPE
	{
		MIS_ALLWAYS_SHOW	= 0, 	 	// 总是显示任务状态
		MIS_COMPLETE_SHOW	= 1, 		// 完成任务条件时才显示任务状态，（只有角色满足完成条件，角色才会看到任务）
	};

	// 任务需求类型
	enum MIS_NEED_TYPE
	{
		MIS_NEED_ITEM		= 0,		// 需要获得物品 (参数1，物品ID，参数2，数量)
		MIS_NEED_KILL		= 1,		// 需要摧毁物件 (参数1，物件类型ID，参数2，数量)
		MIS_NEED_SEND		= 2,        // 需要送给某人 (参数1，npcid)
		MIS_NEED_CONVOY		= 3, 		// 需要护送到某的 (参数1，地图ID，参数2，x坐标，参数3，y坐标)
		MIS_NEED_EXPLORE	= 4,		// 需要探索某的 (参数1，地图ID，参数2，x坐标，参数3，y坐标)
		MIS_NEED_DESP		= 5,		// 文字表述的任务目标
	};

	// 任务奖励类型
	enum MIS_PRIZE_TYPE
	{
		MIS_PRIZE_ITEM		= 0,		// 奖励物品 (参数1，物品ID，参数2，数量)
		MIS_PRIZE_MONEY		= 1,		// 奖励金钱 (参数1，数量)
		MIS_PRIZE_FAME		= 2,		// 奖励声望 (参数1，声望值)
		MIS_PRIZE_CESS		= 3,		// 奖励贸易税率 (参数1，税率值)
	};

	// 任务奖励分配类型
	enum MIS_PRIZE_SELTYPE
	{
		PRIZE_SELONE        = 0,		// 多选一
		PRZIE_SELALL        = 1,		// 全选
	};

	// 随机任务类型
	enum MIS_RAND_TYPE
	{
		MIS_RAND_KILL		  = 0,   	// 摧毁物件 (参数1，物件类型ID，参数2，数量)
		MIS_RAND_GET		  = 1,		// 获取物品 (参数1，物品ID，参数2，数量)
		MIS_RAND_SEND		  = 2,		// 送物品   (参数1，物品ID，参数2，数量)
		MIS_RAND_CONVOY		  = 3,		// 护送NPC  (参数1，角色类型ID，参数2，数量)
		MIS_RAND_EXPLORE	  = 4,		// 探索地图 (参数1，地图ID，参数2，x坐标，参数3，y坐标)
	};

	// 经验类型
	enum MIS_EXP_TYPE
	{
		MIS_EXP_NOMAL 			= 0,	// 奖励普通经验
		MIS_EXP_SAIL			= 1,	// 奖励航海经验
		MIS_EXP_LIFE			= 2,	// 奖励生活经验
	};

	// 新手帮助信息类型
	enum MIS_HELP_TYPE
	{
		MIS_HELP_DESP			= 0,	// 文字帮助信息
		MIS_HELP_IMAGE			= 1,	// 图形帮助信息
		MIS_HELP_SOUND			= 2,	// 声音帮助信息
		MIS_HELP_BICKER			= 3,	// 闪动字体信息
	};

	// 任务界面显示类型定义
	enum MIS_TREENODE_TYPE
	{
		MIS_TREENODE_INVALID		= 0,	// 无效类型
		MIS_TREENODE_NORMAL			= 1,	// 普通任务
		MIS_TREENODE_HISTORY		= 2,	// 历史任务
		MIS_TREENODE_GUILD			= 3,	// 公会任务
	};

	enum ENTITY_TYPE
	{
		BASE_ENTITY					= 0,	// 基本实体
		RESOURCE_ENTITY				= 1,	// 资源实体
		TRANSIT_ENTITY				= 2,	// 传送实体
		BERTH_ENTITY				= 3,	// 停泊实体
	};

	enum ENTITY_STATE
	{
		ENTITY_DISABLE				= 0,	// 实体未激活状态
		ENTITY_ENABLE				= 1,	// 实体激活状态
	};

	enum ENTITY_ACTION
	{
		ENTITY_START_ACTION			= 0,	// 实体开始动作
		ENTITY_END_ACTION			= 1,	// 实体结束动作
		ENTITY_INTERMIT_ACTION		= 2,	// 实体中断动作
	};

	enum GOODS_TYPE
	{
		RES_WOOD					= 0,	// 木材资源
		RES_MINE					= 1,	// 矿石资源
	};

	enum  BOAT_LIST_TYPE
	{
		BERTH_TRADE_LIST			= 0,	// 交易船只列表
		BERTH_LUANCH_LIST			= 1,	// 出海船只列表
		BERTH_BAG_LIST				= 2,	// 打包船只列表
		BERTH_REPAIR_LIST			= 3,	// 船只修理列表
		BERTH_SALVAGE_LIST			= 4,	// 船只打捞列表
		BERTH_SUPPLY_LIST			= 5,	// 船只补给列表
		BERTH_BOATLEVEL_LIST		= 6		// 船只升级列表
	};

	enum VIEW_ITEM_TYPE
	{
		VIEW_CHAR_BAG				= 0,	// 查看自身道具栏物品信息
		VIEW_CHARTRADE_SELF			= 1,	// 查看自身交易道具栏物品信息
		VIEW_CHARTRADE_OTHER		= 2,	// 查看对方交易道具栏物品信息
	};

	// 摆摊货物交易物品信息结构
	typedef struct _NET_STALL_GOODS
	{
		DWORD dwMoney;
		BYTE  byCount;
		BYTE  byIndex;
		BYTE  byGrid;

	} NET_STALL_GOODS, *PNET_STALL_GOODS;

	typedef struct _NET_STALL_ALLDATA
	{
		BYTE byNum;
		NET_STALL_GOODS Info[ROLE_MAXNUM_STALL_GOODS];

	} NET_STALL_ALLDATA, *PNET_STALL_ALLDATA;
}

#endif // _ROLECOMMON_H_

//---------------------------------------------------------