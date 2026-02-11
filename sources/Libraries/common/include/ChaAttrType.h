//=============================================================================
// FileName: ChaAttrType.h
// Creater: ZhangXuedong
// Date: 2004.12.13
// Comment: Character Attribute type
//=============================================================================

#ifndef ATTRTYPE_H
#define ATTRTYPE_H

const long	ATTR_COUNT_BASE0    = 0;
const long	ATTR_LV             = ATTR_COUNT_BASE0 + 0; // 角色等级
const long	ATTR_HP             = ATTR_COUNT_BASE0 + 1; // 当前HP（对于船只，表示耐久）
const long	ATTR_SP             = ATTR_COUNT_BASE0 + 2; // 当前SP（对于船只，表示补给）
const long	ATTR_TITLE          = ATTR_COUNT_BASE0 + 3; // 角色称号
const long	ATTR_JOB            = ATTR_COUNT_BASE0 + 4; // 角色职业
const long	ATTR_FAME           = ATTR_COUNT_BASE0 + 5; // 名声
const long	ATTR_AP             = ATTR_COUNT_BASE0 + 6; // 属性点
const long	ATTR_TP             = ATTR_COUNT_BASE0 + 7; // 技能点
const long	ATTR_GD             = ATTR_COUNT_BASE0 + 8; // 金钱
const long	ATTR_SPRI           = ATTR_COUNT_BASE0 + 9; // 炮弹飞行速度
const long	ATTR_CHATYPE        = ATTR_COUNT_BASE0 + 10; // 角色控制类型（玩家，NPC，怪物等）
const long	ATTR_SAILLV         = ATTR_COUNT_BASE0 + 11; // 航海等级
const long	ATTR_LIFELV         = ATTR_COUNT_BASE0 + 12; // 生活等级
const long	ATTR_LIFETP         = ATTR_COUNT_BASE0 + 13; // 生活技能点
const long	ATTR_BOAT_BERTH     = ATTR_COUNT_BASE0 + 14; // 船只停泊港口
//			// 以下范围内的各属性的顺序不可改变
const long	ATTR_CEXP           = ATTR_COUNT_BASE0 + 15; // 当前经验（对于船只，表示熟练度）
const long	ATTR_NLEXP          = ATTR_COUNT_BASE0 + 16; // 下一级所需经验
const long	ATTR_CLEXP          = ATTR_COUNT_BASE0 + 17; // 当前等级的经验
const long	ATTR_CLEFT_SAILEXP  = ATTR_COUNT_BASE0 + 18; // 当前剩余航海经验
const long	ATTR_CSAILEXP       = ATTR_COUNT_BASE0 + 19; // 累积航海经验（当前航海经验）
const long	ATTR_CLV_SAILEXP    = ATTR_COUNT_BASE0 + 20; // 当前等级航海经验
const long	ATTR_NLV_SAILEXP    = ATTR_COUNT_BASE0 + 21; // 下一等级等级航海经验
const long	ATTR_CLIFEEXP       = ATTR_COUNT_BASE0 + 22; // 当前生活经验
const long	ATTR_CLV_LIFEEXP    = ATTR_COUNT_BASE0 + 23; // 当前等级生活经验
const long	ATTR_NLV_LIFEEXP    = ATTR_COUNT_BASE0 + 24; // 下一等级生活经验
//			//

const long	ATTR_COUNT_BASE1    = 25;
const long	ATTR_STR            = ATTR_COUNT_BASE1 + 0; // 力量
const long	ATTR_DEX            = ATTR_COUNT_BASE1 + 1; // 专注
const long	ATTR_AGI            = ATTR_COUNT_BASE1 + 2; // 敏捷
const long	ATTR_CON            = ATTR_COUNT_BASE1 + 3; // 体质
const long	ATTR_STA            = ATTR_COUNT_BASE1 + 4; // 精力
const long	ATTR_LUK            = ATTR_COUNT_BASE1 + 5; // 幸运
const long	ATTR_MXHP           = ATTR_COUNT_BASE1 + 6; // 最大HP
const long	ATTR_MXSP           = ATTR_COUNT_BASE1 + 7; // 最大SP
const long	ATTR_MNATK          = ATTR_COUNT_BASE1 + 8; // 最小攻击力
const long	ATTR_MXATK          = ATTR_COUNT_BASE1 + 9; // 最大攻击力
const long	ATTR_DEF            = ATTR_COUNT_BASE1 + 10; // 防御力
const long	ATTR_HIT            = ATTR_COUNT_BASE1 + 11; // 命中率
const long	ATTR_FLEE           = ATTR_COUNT_BASE1 + 12; // 闪避率
const long	ATTR_MF             = ATTR_COUNT_BASE1 + 13; // 寻宝率
const long	ATTR_CRT            = ATTR_COUNT_BASE1 + 14; // 爆击率
const long	ATTR_HREC           = ATTR_COUNT_BASE1 + 15; // hp恢复速度
const long	ATTR_SREC           = ATTR_COUNT_BASE1 + 16; // sp恢复速度
const long	ATTR_ASPD           = ATTR_COUNT_BASE1 + 17; // 攻击间隔
const long	ATTR_ADIS           = ATTR_COUNT_BASE1 + 18; // 攻击距离
const long	ATTR_MSPD           = ATTR_COUNT_BASE1 + 19; // 移动速度
const long	ATTR_COL            = ATTR_COUNT_BASE1 + 20; // 资源采集速度
const long	ATTR_PDEF           = ATTR_COUNT_BASE1 + 21; // 物理抵抗
const long  ATTR_BOAT_CRANGE    = ATTR_COUNT_BASE1 + 22; // 船只炮弹爆炸范围
const long  ATTR_BOAT_CSPD      = ATTR_COUNT_BASE1 + 23; // 船只炮弹飞行速度
const long  ATTR_BOAT_PRICE     = ATTR_COUNT_BASE1 + 24; // 船的价值

const long	ATTR_COUNT_BASE2    = 50;
const long	ATTR_BSTR           = ATTR_COUNT_BASE2 + 0; // 基本力量
const long	ATTR_BDEX           = ATTR_COUNT_BASE2 + 1; // 基本专注
const long	ATTR_BAGI           = ATTR_COUNT_BASE2 + 2; // 基本敏捷
const long	ATTR_BCON           = ATTR_COUNT_BASE2 + 3; // 基本体质
const long	ATTR_BSTA           = ATTR_COUNT_BASE2 + 4; // 基本精力
const long	ATTR_BLUK           = ATTR_COUNT_BASE2 + 5; // 基本幸运
const long	ATTR_BMXHP          = ATTR_COUNT_BASE2 + 6; // 基本最大HP
const long	ATTR_BMXSP          = ATTR_COUNT_BASE2 + 7; // 基本最大SP
const long	ATTR_BMNATK         = ATTR_COUNT_BASE2 + 8; // 基本最小攻击力
const long	ATTR_BMXATK         = ATTR_COUNT_BASE2 + 9; // 基本最大攻击力
const long	ATTR_BDEF           = ATTR_COUNT_BASE2 + 10; // 基本防御力
const long	ATTR_BHIT           = ATTR_COUNT_BASE2 + 11; // 基本命中率
const long	ATTR_BFLEE          = ATTR_COUNT_BASE2 + 12; // 基本闪避率
const long	ATTR_BMF            = ATTR_COUNT_BASE2 + 13; // 基本寻宝率
const long	ATTR_BCRT           = ATTR_COUNT_BASE2 + 14; // 基本爆击率
const long	ATTR_BHREC          = ATTR_COUNT_BASE2 + 15; // 基本hp恢复速度
const long	ATTR_BSREC          = ATTR_COUNT_BASE2 + 16; // 基本(角色sp恢复速度)(船只为消耗速度)
const long	ATTR_BASPD          = ATTR_COUNT_BASE2 + 17; // 基本攻击间隔
const long	ATTR_BADIS          = ATTR_COUNT_BASE2 + 18; // 基本攻击距离
const long	ATTR_BMSPD          = ATTR_COUNT_BASE2 + 19; // 基本移动速度
const long	ATTR_BCOL           = ATTR_COUNT_BASE2 + 20; // 基本资源采集速度
const long	ATTR_BPDEF          = ATTR_COUNT_BASE2 + 21; // 基本物理抵抗
const long  ATTR_BOAT_BCRANGE   = ATTR_COUNT_BASE2 + 22; // 船只基本炮弹爆炸范围
const long  ATTR_BOAT_BCSPD     = ATTR_COUNT_BASE2 + 23; // 船只基本炮弹飞行速度

const long	ATTR_COUNT_BASE3    = 74;
const long	ATTR_ITEMC_STR      = ATTR_COUNT_BASE3 + 0; // 力量道具系数（item coefficient）加成
const long	ATTR_ITEMC_AGI      = ATTR_COUNT_BASE3 + 1; // 敏捷道具系数加成
const long	ATTR_ITEMC_DEX      = ATTR_COUNT_BASE3 + 2; // 专注道具系数加成
const long	ATTR_ITEMC_CON      = ATTR_COUNT_BASE3 + 3; // 体质道具系数加成
const long	ATTR_ITEMC_STA      = ATTR_COUNT_BASE3 + 4; // 精力道具系数加成
const long	ATTR_ITEMC_LUK      = ATTR_COUNT_BASE3 + 5; // 幸运道具系数加成
const long	ATTR_ITEMC_ASPD     = ATTR_COUNT_BASE3 + 6; // 攻击间隔道具系数加成
const long	ATTR_ITEMC_ADIS     = ATTR_COUNT_BASE3 + 7; // 攻击距离道具系数加成
const long	ATTR_ITEMC_MNATK    = ATTR_COUNT_BASE3 + 8; // 最小攻击力道具系数加成
const long	ATTR_ITEMC_MXATK    = ATTR_COUNT_BASE3 + 9; // 最大攻击力道具系数加成
const long	ATTR_ITEMC_DEF      = ATTR_COUNT_BASE3 + 10; // 防御力道具系数加成
const long	ATTR_ITEMC_MXHP     = ATTR_COUNT_BASE3 + 11; // 最大HP道具系数加成
const long	ATTR_ITEMC_MXSP     = ATTR_COUNT_BASE3 + 12; // 最大SP道具系数加成
const long	ATTR_ITEMC_FLEE     = ATTR_COUNT_BASE3 + 13; // 闪避率道具系数加成
const long	ATTR_ITEMC_HIT      = ATTR_COUNT_BASE3 + 14; // 命中率道具系数加成
const long	ATTR_ITEMC_CRT      = ATTR_COUNT_BASE3 + 15; // 爆击率道具系数加成
const long	ATTR_ITEMC_MF       = ATTR_COUNT_BASE3 + 16; // 寻宝率道具系数加成
const long	ATTR_ITEMC_HREC     = ATTR_COUNT_BASE3 + 17; // hp恢复速度道具系数加成
const long	ATTR_ITEMC_SREC     = ATTR_COUNT_BASE3 + 18; // sp恢复速度道具系数加成
const long	ATTR_ITEMC_MSPD     = ATTR_COUNT_BASE3 + 19; // 移动速度道具系数加成
const long	ATTR_ITEMC_COL      = ATTR_COUNT_BASE3 + 20; // 资源采集速度道具系数加成
const long	ATTR_ITEMC_PDEF     = ATTR_COUNT_BASE3 + 21; // 物理抵抗道具系数加成

const long	ATTR_COUNT_BASE4    = 96;
const long	ATTR_ITEMV_STR      = ATTR_COUNT_BASE4 + 0; // 力量道具数值（item value）加成
const long	ATTR_ITEMV_AGI      = ATTR_COUNT_BASE4 + 1; // 敏捷道具数值加成
const long	ATTR_ITEMV_DEX      = ATTR_COUNT_BASE4 + 2; // 专注道具数值加成
const long	ATTR_ITEMV_CON      = ATTR_COUNT_BASE4 + 3; // 体质道具数值加成
const long	ATTR_ITEMV_STA      = ATTR_COUNT_BASE4 + 4; // 精力道具数值加成
const long	ATTR_ITEMV_LUK      = ATTR_COUNT_BASE4 + 5; // 幸运道具数值加成
const long	ATTR_ITEMV_ASPD     = ATTR_COUNT_BASE4 + 6; // 攻击间隔道具数值加成
const long	ATTR_ITEMV_ADIS     = ATTR_COUNT_BASE4 + 7; // 攻击距离道具数值加成
const long	ATTR_ITEMV_MNATK    = ATTR_COUNT_BASE4 + 8; // 最小攻击力道具数值加成
const long	ATTR_ITEMV_MXATK    = ATTR_COUNT_BASE4 + 9; // 最大攻击力道具数值加成
const long	ATTR_ITEMV_DEF      = ATTR_COUNT_BASE4 + 10; // 防御力道具数值加成
const long	ATTR_ITEMV_MXHP     = ATTR_COUNT_BASE4 + 11; // 最大HP道具数值加成
const long	ATTR_ITEMV_MXSP     = ATTR_COUNT_BASE4 + 12; // 最大SP道具数值加成
const long	ATTR_ITEMV_FLEE     = ATTR_COUNT_BASE4 + 13; // 闪避率道具数值加成
const long	ATTR_ITEMV_HIT      = ATTR_COUNT_BASE4 + 14; // 命中率道具数值加成
const long	ATTR_ITEMV_CRT      = ATTR_COUNT_BASE4 + 15; // 爆击率道具数值加成
const long	ATTR_ITEMV_MF       = ATTR_COUNT_BASE4 + 16; // 寻宝率道具数值加成
const long	ATTR_ITEMV_HREC     = ATTR_COUNT_BASE4 + 17; // hp恢复速度道具数值加成
const long	ATTR_ITEMV_SREC     = ATTR_COUNT_BASE4 + 18; // sp恢复速度道具数值加成
const long	ATTR_ITEMV_MSPD     = ATTR_COUNT_BASE4 + 19; // 移动速度道具数值加成
const long	ATTR_ITEMV_COL      = ATTR_COUNT_BASE4 + 20; // 资源采集速度道具数值加成
const long	ATTR_ITEMV_PDEF     = ATTR_COUNT_BASE4 + 21; // 物理抵抗道具数值加成

const long	ATTR_COUNT_BASE5    = 118;
const long	ATTR_STATEC_STR     = ATTR_COUNT_BASE5 + 0; // 力量状态系数（state coefficient）加成
const long	ATTR_STATEC_AGI     = ATTR_COUNT_BASE5 + 1; // 敏捷状态系数加成
const long	ATTR_STATEC_DEX     = ATTR_COUNT_BASE5 + 2; // 专注状态系数加成
const long	ATTR_STATEC_CON     = ATTR_COUNT_BASE5 + 3; // 体质状态系数加成
const long	ATTR_STATEC_STA     = ATTR_COUNT_BASE5 + 4; // 精力状态系数加成
const long	ATTR_STATEC_LUK     = ATTR_COUNT_BASE5 + 5; // 幸运状态系数加成
const long	ATTR_STATEC_ASPD    = ATTR_COUNT_BASE5 + 6; // 攻击间隔状态系数加成
const long	ATTR_STATEC_ADIS    = ATTR_COUNT_BASE5 + 7; // 攻击距离状态系数加成
const long	ATTR_STATEC_MNATK   = ATTR_COUNT_BASE5 + 8; // 最小攻击力状态系数加成
const long	ATTR_STATEC_MXATK   = ATTR_COUNT_BASE5 + 9; // 最大攻击力状态系数加成
const long	ATTR_STATEC_DEF     = ATTR_COUNT_BASE5 + 10; // 防御力状态系数加成
const long	ATTR_STATEC_MXHP    = ATTR_COUNT_BASE5 + 11; // 最大HP状态系数加成
const long	ATTR_STATEC_MXSP    = ATTR_COUNT_BASE5 + 12; // 最大SP状态系数加成
const long	ATTR_STATEC_FLEE    = ATTR_COUNT_BASE5 + 13; // 闪避率状态系数加成
const long	ATTR_STATEC_HIT     = ATTR_COUNT_BASE5 + 14; // 命中率状态系数加成
const long	ATTR_STATEC_CRT     = ATTR_COUNT_BASE5 + 15; // 爆击率状态系数加成
const long	ATTR_STATEC_MF      = ATTR_COUNT_BASE5 + 16; // 寻宝率状态系数加成
const long	ATTR_STATEC_HREC    = ATTR_COUNT_BASE5 + 17; // hp恢复速度状态系数加成
const long	ATTR_STATEC_SREC    = ATTR_COUNT_BASE5 + 18; // sp恢复速度状态系数加成
const long	ATTR_STATEC_MSPD    = ATTR_COUNT_BASE5 + 19; // 移动速度状态系数加成
const long	ATTR_STATEC_COL     = ATTR_COUNT_BASE5 + 20; // 资源采集速度状态系数加成
const long	ATTR_STATEC_PDEF    = ATTR_COUNT_BASE5 + 21; // 物理抵抗状态系数加成

const long	ATTR_COUNT_BASE6    = 140;
const long	ATTR_STATEV_STR     = ATTR_COUNT_BASE6 + 0; // 力量状态数值（state value）加成
const long	ATTR_STATEV_AGI     = ATTR_COUNT_BASE6 + 1; // 敏捷状态数值加成
const long	ATTR_STATEV_DEX     = ATTR_COUNT_BASE6 + 2; // 专注状态数值加成
const long	ATTR_STATEV_CON     = ATTR_COUNT_BASE6 + 3; // 体质状态数值加成
const long	ATTR_STATEV_STA     = ATTR_COUNT_BASE6 + 4; // 精力状态数值加成
const long	ATTR_STATEV_LUK     = ATTR_COUNT_BASE6 + 5; // 幸运状态数值加成
const long	ATTR_STATEV_ASPD    = ATTR_COUNT_BASE6 + 6; // 攻击间隔状态数值加成
const long	ATTR_STATEV_ADIS    = ATTR_COUNT_BASE6 + 7; // 攻击距离状态数值加成
const long	ATTR_STATEV_MNATK   = ATTR_COUNT_BASE6 + 8; // 最小攻击力状态数值加成
const long	ATTR_STATEV_MXATK   = ATTR_COUNT_BASE6 + 9; // 最大攻击力状态数值加成
const long	ATTR_STATEV_DEF     = ATTR_COUNT_BASE6 + 10; // 防御力状态数值加成
const long	ATTR_STATEV_MXHP    = ATTR_COUNT_BASE6 + 11; // 最大HP状态数值加成
const long	ATTR_STATEV_MXSP    = ATTR_COUNT_BASE6 + 12; // 最大SP状态数值加成
const long	ATTR_STATEV_FLEE    = ATTR_COUNT_BASE6 + 13; // 闪避率状态数值加成
const long	ATTR_STATEV_HIT     = ATTR_COUNT_BASE6 + 14; // 命中率状态数值加成
const long	ATTR_STATEV_CRT     = ATTR_COUNT_BASE6 + 15; // 爆击率状态数值加成
const long	ATTR_STATEV_MF      = ATTR_COUNT_BASE6 + 16; // 寻宝率状态数值加成
const long	ATTR_STATEV_HREC    = ATTR_COUNT_BASE6 + 17; // hp恢复速度状态数值加成
const long	ATTR_STATEV_SREC    = ATTR_COUNT_BASE6 + 18; // sp恢复速度状态数值加成
const long	ATTR_STATEV_MSPD    = ATTR_COUNT_BASE6 + 19; // 移动速度状态数值加成
const long	ATTR_STATEV_COL     = ATTR_COUNT_BASE6 + 20; // 资源采集速度状态数值加成
const long	ATTR_STATEV_PDEF    = ATTR_COUNT_BASE6 + 21; // 物理抵抗状态数值加成

const long	ATTR_LHAND_ITEMV    = ATTR_COUNT_BASE6 + 22; // 左手道具加成

const long	ATTR_COUNT_BASE7        = 163;
const long	ATTR_BOAT_SHIP          = ATTR_COUNT_BASE7 + 0; // 船只ID
const long	ATTR_BOAT_HEADER        = ATTR_COUNT_BASE7 + 1; // 船头类型
const long  ATTR_BOAT_BODY          = ATTR_COUNT_BASE7 + 2; // 船身类型
const long  ATTR_BOAT_ENGINE        = ATTR_COUNT_BASE7 + 3; // 船引擎类型
const long  ATTR_BOAT_CANNON        = ATTR_COUNT_BASE7 + 4; // 船火炮类型
const long	ATTR_BOAT_PART          = ATTR_COUNT_BASE7 + 5; // 船配件类型
const long  ATTR_BOAT_DBID          = ATTR_COUNT_BASE7 + 6; // 船的数据库存储ID
const long  ATTR_BOAT_DIECOUNT      = ATTR_COUNT_BASE7 + 7; // 该船死亡计数
const long  ATTR_BOAT_ISDEAD	    = ATTR_COUNT_BASE7 + 8; // 该船目前是否为死亡状态

const long	ATTR_COUNT_BASE8        = 172;
const long	ATTR_BOAT_SKILLC_MNATK  = ATTR_COUNT_BASE8 + 0; // 船只MNATK 技能系数影响
const long	ATTR_BOAT_SKILLC_MXATK  = ATTR_COUNT_BASE8 + 1; // 船只MXATK 技能系数影响
const long	ATTR_BOAT_SKILLC_ADIS   = ATTR_COUNT_BASE8 + 2; // 船只攻击距离atkrange 技能系数影响
const long	ATTR_BOAT_SKILLC_MSPD   = ATTR_COUNT_BASE8 + 3; // 船只移动速度技能系数影响
const long	ATTR_BOAT_SKILLC_CSPD   = ATTR_COUNT_BASE8 + 4; // 炮弹飞行速度 技能系数影响
const long	ATTR_BOAT_SKILLC_ASPD   = ATTR_COUNT_BASE8 + 5; // 船只ASPD 技能系数影响
const long	ATTR_BOAT_SKILLC_CRANGE = ATTR_COUNT_BASE8 + 6; // 炮弹爆炸范围 技能系数影响
const long	ATTR_BOAT_SKILLC_DEF    = ATTR_COUNT_BASE8 + 7; // 船只DEF 技能系数影响
const long	ATTR_BOAT_SKILLC_RESIST = ATTR_COUNT_BASE8 + 8; // 船只RESIST 技能系数影响
const long	ATTR_BOAT_SKILLC_MXUSE  = ATTR_COUNT_BASE8 + 9; // 船只最大耐久 技能系数影响
const long	ATTR_BOAT_SKILLC_USEREC = ATTR_COUNT_BASE8 + 10; // 船只耐久回复速度 技能系数影响
const long	ATTR_BOAT_SKILLC_EXP    = ATTR_COUNT_BASE8 + 11; // 船只消耗率 技能系数影响
const long	ATTR_BOAT_SKILLC_CPT    = ATTR_COUNT_BASE8 + 12; // 船仓容量 技能系数影响
const long	ATTR_BOAT_SKILLC_SPD    = ATTR_COUNT_BASE8 + 13; // 航速 技能系数影响
const long	ATTR_BOAT_SKILLC_MXSPLY = ATTR_COUNT_BASE8 + 14; // 最大补给值 技能系数影响

const long	ATTR_COUNT_BASE9        = 187;
const long	ATTR_BOAT_SKILLV_MNATK  = ATTR_COUNT_BASE9 + 0; // 船只MNATK 技能常数影响
const long	ATTR_BOAT_SKILLV_MXATK  = ATTR_COUNT_BASE9 + 1; // 船只MXATK 技能常数影响
const long	ATTR_BOAT_SKILLV_ADIS   = ATTR_COUNT_BASE9 + 2; // 船只攻击距离atkrange 技能常数影响
const long	ATTR_BOAT_SKILLV_MSPD   = ATTR_COUNT_BASE9 + 3; // 船只移动速度技能常数影响
const long	ATTR_BOAT_SKILLV_CSPD   = ATTR_COUNT_BASE9 + 4; // 炮弹飞行速度 技能常数影响
const long	ATTR_BOAT_SKILLV_ASPD   = ATTR_COUNT_BASE9 + 5; // 船只ASPD 技能常数影响
const long	ATTR_BOAT_SKILLV_CRANGE = ATTR_COUNT_BASE9 + 6; // 炮弹爆炸范围 技能常数影响
const long	ATTR_BOAT_SKILLV_DEF    = ATTR_COUNT_BASE9 + 7; // 船只DEF 技能常数影响
const long	ATTR_BOAT_SKILLV_RESIST = ATTR_COUNT_BASE9 + 8; // 船只RESIST 技能常数影响
const long	ATTR_BOAT_SKILLV_MXUSE  = ATTR_COUNT_BASE9 + 9; // 船只最大耐久 技能常数影响
const long	ATTR_BOAT_SKILLV_USEREC = ATTR_COUNT_BASE9 + 10; // 船只耐久回复速度 技能常数影响
const long	ATTR_BOAT_SKILLV_EXP    = ATTR_COUNT_BASE9 + 11; // 船只消耗率 技能常数影响
const long	ATTR_BOAT_SKILLV_CPT    = ATTR_COUNT_BASE9 + 12; // 船仓容量 技能常数影响
const long	ATTR_BOAT_SKILLV_SPD    = ATTR_COUNT_BASE9 + 13; // 航速 技能常数影响
const long	ATTR_BOAT_SKILLV_MXSPLY = ATTR_COUNT_BASE9 + 14; // 最大补给值 技能常数影响

// Modify by lark.li 20080723
// 扩展属性
const long	ATTR_COUNT_BASE10        = 202;
const long	ATTR_EXTEND0  = ATTR_COUNT_BASE10 + 0;
const long	ATTR_EXTEND1  = ATTR_COUNT_BASE10 + 1;
const long	ATTR_EXTEND2  = ATTR_COUNT_BASE10 + 2;
const long	ATTR_EXTEND3  = ATTR_COUNT_BASE10 + 3;
const long	ATTR_EXTEND4  = ATTR_COUNT_BASE10 + 4;
const long	ATTR_EXTEND5  = ATTR_COUNT_BASE10 + 5;
const long	ATTR_EXTEND6  = ATTR_COUNT_BASE10 + 6;
const long	ATTR_EXTEND7  = ATTR_COUNT_BASE10 + 7;
const long	ATTR_EXTEND8  = ATTR_COUNT_BASE10 + 8;
const long	ATTR_EXTEND9  = ATTR_COUNT_BASE10 + 9;

const long	ATTR_MAX_NUM                = 202;
//const long	ATTR_MAX_NUM                = 213;

// End

const long	ATTR_CLIENT_MAX             = ATTR_ITEMC_STR;	
const long  MAX_ATTR_CLIENT				= ATTR_ITEMC_STR;				// 可用属性最大值,仅客户端用
const long	ATTR_CLIENT_SIGN_BYTE_NUM   = (ATTR_CLIENT_MAX + sizeof(char) * 8 - 1) / (sizeof(char) * 8);
const long	ATTR_SIGN_BYTE_NUM          = (ATTR_MAX_NUM + sizeof(char) * 8 - 1) / (sizeof(char) * 8);

inline bool	g_IsNosignChaAttr(long lAttrNo)
{
	if (lAttrNo >= ATTR_CEXP && lAttrNo <= ATTR_NLV_LIFEEXP)
		return true;

	return false;
}

#endif // ATTRTYPE_H