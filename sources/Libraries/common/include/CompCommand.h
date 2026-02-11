//=============================================================================
// FileName: CompCommand.h
// Creater: ZhangXuedong
// Date: 2004.11.22
// Comment: compositive command
//=============================================================================

#pragma once

#include "util.h"
#include "ItemAttrType.h"
#include "ChaAttrType.h"
#include "ItemContent.h"
#include <algorithm>

#define defPROTOCOL_HAVE_PACKETID // ����Э�����Ƿ��������ID

enum EActionType // server,client ֮����ж�����
{
	enumACTION_NONE = 0,
	enumACTION_MOVE,		// �ƶ�
	enumACTION_SKILL,		// ����
	enumACTION_SKILL_SRC,	// ʹ�ü���
	enumACTION_SKILL_TAR,	// ��ʹ�ü���
	enumACTION_LOOK,		// ���½�ɫ���
	enumACTION_KITBAG,		// ���½�ɫ�ĵ�����
	enumACTION_SKILLBAG,	// ���¼�����
	enumACTION_ITEM_PICK,	// �����
	enumACTION_ITEM_THROW,	// ������
	enumACTION_ITEM_UNFIX,	// ����жװ
	enumACTION_ITEM_USE,	// ����ʹ��
	enumACTION_ITEM_POS,	// ���߸ı�λ��
	enumACTION_ITEM_DELETE,	// ����ɾ��
	enumACTION_ITEM_INFO,	// ������Ϣ
	enumACTION_ITEM_FAILED,	// ���߲���ʧ��
	enumACTION_LEAN,		// �п�
	enumACTION_CHANGE_CHA,	// ������ɫ
	enumACTION_EVENT,		// �����¼�
    enumACTION_FACE,        // �ͻ��������鶯��,Ŀǰ����������Ҫת���������ͻ���
	enumACTION_STOP_STATE,	// ֹͣ����״̬
	enumACTION_SKILL_POSE,	// ����Pose
	enumACTION_PK_CTRL,		// PK����
	enumACTION_LOOK_ENERGY,	// ���½�ɫ�������

	enumACTION_TEMP,		// ��ʱЭ��

    enumACTION_SHORTCUT,    // �ͻ��˷��Ϳ���������������̣��������֪ͨ�ͻ��˿��������,ע:����Ҫ������,��������ʼ��ɺ���ܷ���
	enumACTION_BANK,		// ����������Ϣ
	enumACTION_CLOSE_BANK,	// �ر�����

	enumACTION_KITBAGTMP,		//������ʱ����
	enumACTION_KITBAGTMP_DRAG,	//�Ϸ���ʱ�����еĵ���
	
	enumACTION_GUILDBANK,
	enumACTION_REQUESTGUILDBANK,
	enumACTION_REQUESTGUILDLOGS,
	enumACTION_UPDATEGUILDLOGS,

	enumMAX_ACTION_NUM		// ����ж�����
};

enum EAttrSynType // server->client ����ͬ������
{
	enumATTRSYN_INIT,			// ��ʼ��
	enumATTRSYN_ITEM_EQUIP,		// ��װ����жװ����,���Է����ı�
	enumATTRSYN_ITEM_MEDICINE,	// ��ʹ����Ʒ��ҩ�,���Է����ı�
    enumATTRSYN_ATTACK,			// ��Ϊ�������ܻ�,���Է����ı�,������
	enumATTRSYN_TASK,			// ���������Ըı�
	enumATTRSYN_TRADE,			// ���ף����Ըı�
	enumATTRSYN_REASSIGN,		// �������Ե㣬�����������Ըı�
	enumATTRSYN_SKILL_STATE,	// ����״̬�ı���������Ըı�
	enumATTRSYN_AUTO_RESUME,	// �Զ��ָ���HP,SP��
	enumATTRSYN_CHANGE_JOB,		// תְ
	enumATTRSYN_RECTIFY,		// ��������
};

enum EItemAppearType // server->client ���߳�������
{
	enumITEM_APPE_MONS,		// �������
	enumITEM_APPE_THROW,	// ����ɫ�ӳ�
	enumITEM_APPE_NATURAL,	// ��Ȼ����
};

enum ESynKitbagType // server->client ������ͬ������
{
	enumSYN_KITBAG_INIT,	// ��������ʼ��
	enumSYN_KITBAG_EQUIP,	// װ����������ͬ��������
	enumSYN_KITBAG_UNFIX,	// жװ��������ͬ��������
	enumSYN_KITBAG_PICK,	// �����
	enumSYN_KITBAG_THROW,	// �ӵ���
	enumSYN_KITBAG_SWITCH,	// ��������λ��
	enumSYN_KITBAG_TRADE,	// ����
	enumSYN_KITBAG_FROM_NPC,// NPC����
	enumSYN_KITBAG_TO_NPC,	// ��NPCȡ��
	enumSYN_KITBAG_SYSTEM,	// ϵͳ����
	enumSYN_KITBAG_FORGES,	// �����ɹ�
	enumSYN_KITBAG_FORGEF,	// ����ʧ��
	enumSYN_KITBAG_BANK,	// ����
	enumSYN_KITBAG_ATTR,	// ���Ըı䣨�;���ģ�
};

enum ESynSkillBagType // server->client ������ͬ������
{
	enumSYN_SKILLBAG_INIT,	// ��������ʼ��������ȫ���ļ�����Ϣ��
	enumSYN_SKILLBAG_ADD,	// ���Ӽ��ܣ�ֻ���������ӵļ��ܵ���Ϣ��
	enumSYN_SKILLBAG_MODI,	// �޸ļ��ܣ�ֻ���ͱ��޸ĵļ��ܵ���Ϣ��
};

enum ESynLookType
{
	enumSYN_LOOK_SWITCH,	// ������
	enumSYN_LOOK_CHANGE,	// ��ֵ�ı䡣
};

enum EEquipLinkPos // ���ߵ�װ����λ
{
	enumEQUIP_HEAD		= 0,	// ������岿λ,ͷ,��,����,��,��
	enumEQUIP_FACE		= 1,
	enumEQUIP_BODY		= 2,
	enumEQUIP_GLOVE		= 3,	// ����
	enumEQUIP_SHOES		= 4,	// Ь��

	enumEQUIP_NECK		= 5,	// ����:������,����
	enumEQUIP_LHAND		= 6,	// ����		-- �������ֵĵ���ֵΪ�ͻ���Link��
	enumEQUIP_HAND1		= 7,	// ��������
	enumEQUIP_HAND2		= 8,
	enumEQUIP_RHAND		= 9,	// ����
	enumEQUIP_Jewelry1	= 10,
	enumEQUIP_Jewelry2	= 11,
	enumEQUIP_Jewelry3	= 12,
	enumEQUIP_Jewelry4	= 13,
	enumEQUIP_WING		= 14,
	
	enumEQUIP_CLOAK		= 15,
	enumEQUIP_FAIRY		= 16,
	enumEQUIP_REAR		= 17,
	enumEQUIP_MOUNT		= 18,

	enumEQUIP_HEADAPP = 19,
	enumEQUIP_FACEAPP = 20,
	enumEQUIP_BODYAPP = 21,
	enumEQUIP_GLOVEAPP = 22,
	enumEQUIP_SHOESAPP = 23,
	
	enumEQUIP_FAIRYAPP = 24,
	enumEQUIP_GLOWAPP = 25,
	
	enumEQUIP_DAGGERAPP = 26,
	enumEQUIP_GUNAPP = 27,
	enumEQUIP_SWORD1APP = 28,
	enumEQUIP_GREATSWORDAPP = 29,
	enumEQUIP_STAFFAPP = 30,
	enumEQUIP_BOWAPP = 31,
	enumEQUIP_SWORD2APP = 32,
	enumEQUIP_SHIELDAPP = 33,
	
	
	enumEQUIP_PART_NUM  = 5,	// �ܹ������λ
	enumEQUIP_NUM		= 34,	// װ����λ�ĸ���
    enumEQUIP_BOTH_HAND = 9999,  // �����˫������,��һ�߾�Ϊ���ֵ
    enumEQUIP_TOTEM		= 9998,  // �����ͼ��װ��,������λ��Ϊ���ֵ
};

enum EBoatLinkPos	// ����װ��λ��
{
	enumBOAT_BODY		= 0,	// ����
	enumBOAT_HEADER		= 1,	// ��ͷ	
	enumBOAT_ENGINE		= 2,	// ��ֻ����
	enumBOAT_FLAG		= 3,	// ����־
	enumBOAT_MOTOR0		= 4,	// ���4������
	enumBOAT_MOTOR1		= 5,
	enumBOAT_MOTOR2		= 6,
	enumBOAT_MOTOR3		= 7,
};

enum EMoveState
{
	enumMSTATE_ON		= 0x00,	// �ƶ���
	enumMSTATE_ARRIVE	= 0x01,	// ����Ŀ���ֹͣ
	enumMSTATE_BLOCK	= 0x02,	// �����ϰ�ֹͣ
	enumMSTATE_CANCEL	= 0x04,	// ��Ҫ��ֹͣ
	enumMSTATE_INRANGE	= 0x08, // �Ѿ�����Ҫ��ķ�Χ��ֹͣ
	enumMSTATE_NOTARGET	= 0x10, // Ŀ�겻���ڶ�ֹͣ
	enumMSTATE_CANTMOVE	= 0x20,	// �����ƶ�
};

enum EFightState
{// enumFSTATE_TARGET_NO֮ǰ���������ֵ�״̬�������ǲ��������ֵ�״̬
	enumFSTATE_ON			= 0x0000,	// ս����
	enumFSTATE_STOP			= 0x0001,	// ����ֹͣ

	enumFSTATE_TARGET_NO	= 0x0010,	// ���󲻴��ڶ�ֹͣ��û���ҵ�����ʩ�ö��󣩣�����������
	enumFSTATE_TARGET_OUT	= 0x0020,	// �����뿪ʹ�÷�Χ��ֹͣ������������
	enumFSTATE_TARGET_IMMUNE= 0x0040,	// �����ܱ�����������������
	enumFSTATE_CANCEL		= 0x0080,	// ��Ҫ��ֹͣ������������
	enumFSTATE_DIE			= 0x0100,	// ����������ֹͣ������������
    enumFSTATE_TARGET_DIE   = 0x0200,	// Ŀ�걻��������������ֹͣ������������
	enumFSTATE_OFF			= 0x0400,	// ���ܹ�����ʵ�壬����������
	enumFSTATE_NO_EXPEND	= 0x0800,	// ����Ʒ��MP�����������ߵȣ����㣬����������
};

enum EExistState
{
	// ����״̬
	enumEXISTS_NATALITY,	// ����
	enumEXISTS_WAITING,		// ����
	enumEXISTS_SLEEPING,	// ����
	enumEXISTS_MOVING,		// �ƶ�
	enumEXISTS_FIGHTING,	// ����
	//
	// ����״̬
	enumEXISTS_WITHERING,	// ������
	enumEXISTS_RESUMEING,	// ������
	enumEXISTS_DIE,			// ����
	//
};

enum ESkillStateAdd
{
	enumSSTATE_ADD_UNDEFINED		= 0,	// δ������滻����
	enumSSTATE_ADD_EQUALORLARGER	= 1,	// ���ڵ����滻
	enumSSTATE_ADD_LARGER			= 2,	// �����滻
	enumSSTATE_NOTADD				= 3,	// ���滻
	enumSSTATE_ADD					= 4,	// �滻
};

enum ERangeType
{
	enumRANGE_TYPE_NONE				= 0,	// ��
	enumRANGE_TYPE_STICK			= 1,	// ���Σ����ȣ����ȣ�
	enumRANGE_TYPE_FAN				= 2,	// ���Σ��뾶���Ƕȣ�
	enumRANGE_TYPE_SQUARE			= 3,	// �������뾶��
	enumRANGE_TYPE_CIRCLE			= 4,	// Բ�Σ��뾶��
};

// �ж�����ʧ�ܵ�ԭ��
enum EFailedActionReason
{
	enumFACTION_ACTFORBID,		// ���ڽ�ֹ�ж���״̬
	enumFACTION_EXISTACT,		// ֮ǰ���ж�û�н���
	enumFACTION_MOVEPATH,		// �ƶ�·������
	enumFACTION_CANTMOVE,		// �����ƶ�
	enumFACTION_NOSKILL,		// ���ܲ�����
	enumFACTION_NOOBJECT,		// Ŀ�겻����
	enumFACTION_ITEM_INEXIST,	// ���߲�����
	enumFACTION_SKILL_NOTMATCH,	// ����ʩ���߲�ƥ��
};

enum ESkillUseState
{
	enumSUSTATE_INACTIVE,		// ����δ�����
	enumSUSTATE_ACTIVE,			// ���ܼ����
};

enum EItemOperateResult
{
	enumITEMOPT_SUCCESS,		// ���߲����ɹ�
	enumITEMOPT_ERROR_NONE,		// ���߲�����
	enumITEMOPT_ERROR_KBFULL,	// ����������
	enumITEMOPT_ERROR_UNUSE,	// ����ʹ�õĵ���
	enumITEMOPT_ERROR_UNPICKUP,	// ���߲���ʰȡ
	enumITEMOPT_ERROR_UNTHROW,	// ���߲��ܶ���
	enumITEMOPT_ERROR_UNDEL,	// ���߲�������
	enumITEMOPT_ERROR_KBLOCK,	// ��������������״̬
	enumITEMOPT_ERROR_DISTANCE,	// ����̫Զ
	enumITEMOPT_ERROR_EQUIPLV,	// װ���ȼ�������
	enumITEMOPT_ERROR_EQUIPJOB,	// װ��ְҵ������
	enumITEMOPT_ERROR_STATE,	// ���ڲ���ʹ�õ��ߵ�״̬
	enumITEMOPT_ERROR_PROTECT,	// ���߱�����
	enumITEMOPT_ERROR_AREA,		// ��ͬ����������
	enumITEMOPT_ERROR_BODY,		// ���Ͳ�ƥ��
	enumITEMOPT_ERROR_TYPE,		// �˵����޷����
	enumITEMOPT_ERROR_INVALID,	// ��Ч�ĵ���
	enumITEMOPT_ERROR_KBRANGE,	// ������������Χ
	enumITEMOPT_ERROR_EXPIRATION	// Item expiration by Mdr
};

enum EEntitySeenType // ���ڿͻ����жϸ�ʵ���Ƿ��Ѿ����ڣ��������ǵĴ�ֻ�����Ѿ����ڣ�
{
	enumENTITY_SEEN_NEW,		// �½�ʵ��
	enumENTITY_SEEN_SWITCH,		// �л�ʵ��
};

enum EPlayerReliveType
{
	enumEPLAYER_RELIVE_NONE,	// ��ѡ��
	enumEPLAYER_RELIVE_CITY,	// �سǸ���
	enumEPLAYER_RELIVE_ORIGIN,	// ԭ�㸴��
	enumEPLAYER_RELIVE_NORIGIN,	// �ܾ�ԭ�㸴��
	enumEPLAYER_RELIVE_MAP,		// ԭ��ͼ����
	enumEPLAYER_RELIVE_NOMAP,	// �ܾ�ԭ��ͼ����
};

enum EUseSkill
{
	enumESKILL_SUCCESS,				// ����ʹ�óɹ�
	enumESKILL_FAILD_NPC,			// Ŀ����NPC
	enumESKILL_FAILD_NOT_SKILLED,	// Ŀ�겻�ܱ�ʹ�ü���
	enumESKILL_FAILD_SAFETY_BELT,	// Ŀ���ڰ�ȫ��
	enumESKILL_FAILD_NOT_LAND,		// ���ܲ�������½��
	enumESKILL_FAILD_NOT_SEA,		// ���ܲ������ں���
	enumESKILL_FAILD_ONLY_SELF,		// ����ֻ����������
	enumESKILL_FAILD_ONLY_DIEPLY,	// ����ֻ���������ʬ��
	enumESKILL_FAILD_FRIEND,		// ���ܲ��������ѷ�
	enumESKILL_FAILD_NOT_FRIEND,	// ����ֻ�������ѷ�
	enumESKILL_FAILD_NOT_PALYER,	// ����ֻ���������
	enumESKILL_FAILD_NOT_MONS,		// ����ֻ�����ڹ���
	enumESKILL_FAILD_ESP_MONS,		// ����ֻ�������������
	enumESKILL_FAILD_SELF,			// ���ܲ�����������
};

enum EEnterMapType // ���ڿͻ����ж��Ƿ���ʾLoading����
{
	enumENTER_MAP_EDGE,				// �߽����
	enumENTER_MAP_CARRY,			// ���ͽ���
};

enum EFightType // ��ս���ͣ�������Ҳ���ڽű��У�����Ķ�
{
	enumFIGHT_NONE		= 0, // ��
	enumFIGHT_TEAM		= 1, // ������ս
	enumFIGHT_MONOMER	= 2, // ������ս
	enumFIGHT_GUILD		= 3, // ������ս
};

/*	2008-7-28	yangyinyu	close!		//	Ϊ������ΨһID���������汾���������İ汾��ÿ��������ǰ������ΨһID�ţ������0�����ʾ��û��ΨһID�ĵ��ߡ�
#define defKITBAG_CUR_VER					113	// ��ʾ�Ƿ�����������
*/
#define defKITBAG_CUR_VER111				111	// ��֮ǰ�İ汾����ǰ����뱳��������Ԫ�����@���ţ�
#define defLOOK_CUR_VER						111	// ��ʾ�Ƿ�����������
#define defLOOK_CUR_VER110					110
#define defITEM_INSTANCE_ATTR_NUM_VER110	10

#define defMAP_GARNER_WIDTH					4096
#define defMAP_GARNER_HEIGHT				4096
#define defMAP_DARKBLUE_WIDTH				4096
#define defMAP_DARKBLUE_HEIGHT				4096
#define defMAP_MAGICSEA_WIDTH				4096
#define defMAP_MAGICSEA_HEIGHT				4096
#define defMAP_EASTGOAF_WIDTH				1024
#define defMAP_EASTGOAF_HEIGHT				1024
#define defMAP_LONETOWER_WIDTH				1024
#define defMAP_LONETOWER_HEIGHT				1024

#define defMAX_KBITEM_NUM_PER_TYPE		48 // ÿ����Ʒ�ռ����Ʒ����
#define defDEF_KBITEM_NUM_PER_TYPE		24 // ÿ����Ʒ�ռ����Ʒ����
#define defESPE_KBGRID_NUM				4  // ������߸���

//#pragma pack(push)
//#pragma pack(1)

typedef struct stNetChangeChaPart		// �ı��ɫ���,ע:�ͻ���-��������Ĭ��Ƥ����Լ��:�ͻ��˽��յ�0��Ƥ��ʱ,�Զ�����CharacterRecord�е�Ƥ��
{
	stNetChangeChaPart() {sVer = 0; sTypeID = 0;}

	short	sVer;
	short	sTypeID;
	union
	{
		struct
		{
			std::array<SItemGrid, enumEQUIP_NUM> SLink;
			short       sHairID;        // Ĭ�ϵ�ͷ��������
		};

		struct
		{
			USHORT sPosID;		// ������ID
			USHORT sBoatID;				// ����Ϣ��ID
			USHORT sHeader;				// ��ͷ
			USHORT sBody;				// ����
			USHORT sEngine;				// ��ֻ����
			USHORT sCannon;				// ��ֻ����
			USHORT sEquipment;			// ��ֻװ��
		};
	};
} LOOK;


//#pragma pack(pop)

//NOTE(Ogge): Intended use is for Selection Scene
struct Look_Minimal
{
	Look_Minimal() = default;
	Look_Minimal(LOOK const& look)
	{
		typeID = look.sTypeID;

		std::transform(std::begin(look.SLink), std::end(look.SLink), std::begin(equip_IDs),
			[](SItemGrid const& item)
			{
				return item.sID;
			}
		);
	}

	uint16_t typeID{};
	std::array<uint16_t, EEquipLinkPos::enumEQUIP_NUM> equip_IDs{};
};


#define defMAX_ITEM_FORGE_GROUP	5

struct SForgeItem
{
	
	struct
	{
		short	sGridNum;
		struct
		{
			short	sGridID;
			short	sItemNum;
		} SGrid[defMAX_KBITEM_NUM_PER_TYPE];
	} SGroup[defMAX_ITEM_FORGE_GROUP];
};

#define defMAX_ITEM_LOTTERY_GROUP	7

struct SLotteryItem
{
	struct
	{
		short	sGridNum;
		struct
		{
			short	sGridID;
			short	sItemNum;
		} SGrid[defMAX_KBITEM_NUM_PER_TYPE];
	} SGroup[defMAX_ITEM_LOTTERY_GROUP];
};

#define defMAX_ITEM_LIFESKILL_GROUP	6
struct SLifeSkillItem
{
		short sGridID[defMAX_ITEM_LIFESKILL_GROUP];
		short sbagCount;
		short sReturn;
};


#define defMAX_CITY_NUM		8
extern const char	*g_szCityName[defMAX_CITY_NUM];

//������ﲿ��ID���õ�������
const int PLAY_NUM = 4;
extern const long g_PartIdRange[PLAY_NUM][enumEQUIP_NUM + 1][2];
//end

#define defMOTTO_LEN		40
#define defGUILD_NAME_LEN	17	// ��������
#define defGUILD_MOTTO_LEN	51	// ����������
#define defPICKUP_DISTANCE	700 // ʰȡ��Χ�����ף� old range: 350
#define defTHROW_DISTANCE	350 // ������Χ�����ף�
#define defBANK_DISTANCE	350 // ���н�����Χ�����ף�
#define defRANGE_TOUCH_DIS	350 // ��Χ�������루���ף�

// �����
const DWORD MAX_FAST_ROW = 3;
const DWORD MAX_FAST_COL = 12;
const DWORD SHORT_CUT_NUM = MAX_FAST_ROW * MAX_FAST_COL;
#define defItemShortCutType       1
#define defSkillFightShortCutType 2
#define defSkillLifeShortCutType  3
#define defSkillSailShortCutType  4

struct stNetShortCut
{
    char     chType[SHORT_CUT_NUM];     // 1-����,2-ս������,3-�����,0-��
    short    byGridID[SHORT_CUT_NUM];   // �ڵ��������ڵڼ���,�������еļ��ܱ��
};
//
inline bool assert_shortcut_range(int index)
{
	return index > 0 && index < SHORT_CUT_NUM;
}
// Returns the shortcut index or -1 if none.
inline int IsItemShortCut(const stNetShortCut& shortcuts, int inventory_index) {
	for (int i = 0; i < SHORT_CUT_NUM; ++i)
	{
		if (shortcuts.chType[i] == defItemShortCutType &&
			shortcuts.byGridID[i] == inventory_index)
		{
			return i;
		}
	}
	return -1;
}

inline void SetItemShortCut(stNetShortCut& shortcuts, int shortcut_index, int inventory_index)
{
	if (!assert_shortcut_range(shortcut_index))
	{
		return;
	}

	shortcuts.byGridID[shortcut_index] = inventory_index;
	shortcuts.chType[shortcut_index] = defItemShortCutType;
}

inline void SwapItemShortCut(stNetShortCut& shortcuts, int inventory_index1, int inventory_index2)
{
	const int shortcut1 = IsItemShortCut(shortcuts, inventory_index1);
	const int shortcut2 = IsItemShortCut(shortcuts, inventory_index2);
	if (shortcut1 != -1 && shortcut2 != -1)
	{
		std::swap(shortcuts.byGridID[shortcut1], shortcuts.byGridID[shortcut2]);
		std::swap(shortcuts.chType[shortcut1], shortcuts.chType[shortcut2]);
	}
	else if (shortcut1 != -1)
	{
		SetItemShortCut(shortcuts, shortcut1, inventory_index2);
	}
	else if (shortcut2 != -1)
	{
		SetItemShortCut(shortcuts, shortcut2, inventory_index1);
	}
}

enum EPoseState
{
	enumPoseStand   = 0,
	enumPoseLean    = 1,
	enumPoseSeat	= 2,
};

enum EGuildState		//16λ״̬����λ������
{
	emGldMembStatNormal		=0,			//��ͨ״̬
	emGldMembStatTry		=1,			//����״̬

	//emGldPermMgr			=0x1,		//����
	//emGldPermBank			=0x2,		//����
	//emGldPermBuild			=0x4,		//����
	
	emGldPermSpeak		= 1,		
	emGldPermMgr			= 2,	
	emGldPermViewBank			= 4,	
	emGldPermDepoBank			= 8,	
	emGldPermTakeBank			= 16,
	emGldPermRecruit			= 32,	
	emGldPermKick			= 64,		
	emGldPermMotto		= 128,		
	emGldPermAttr		= 256,	
	emGldPermEnter = 512,
	emGldPermPlace = 1024,
	emGldPermRem = 2048,
	emGldPermDisband = 4096,
	emGldPermLeader = 8192,

	emGldPermMax = 0xFFFFFFFF,//0x7FFFFFF,
	emGldPermDefault = 1,
	emGldPermNum = 12,			// guildhouse objects: 14

	emMaxMemberNum			=80,
	emMaxTryMemberNum		=40,

	emGuildInitVal			=0x0000,	//��ʼֵ
	emGuildGetName			=0x0001,	//�Ƿ��ڻ�ȡ�½���������״̬
	emGuildReplaceOldTry	=0x0002,	//ѯ���Ƿ�����ϵ�����״̬
};

#define defKITBAG_ITEM_NUM	100

//#define defLOOK_DATA_STRING_LEN	2048
#define defLOOK_DATA_STRING_LEN 8192

enum EAreaMask
{
	enumAREA_TYPE_LAND	    = 0x0001,	// ��0λ 1��½�ء�0���Ǻ���
    enumAREA_TYPE_NOT_FIGHT = 0x0002,	// ��1λ 1����ս������0��ս����
    enumAREA_TYPE_PK		= 0x0004,	// ��2λ 1��PK����
	enumAREA_TYPE_BRIDGE	= 0x0008,	// ��3λ 1���š�
	enumAREA_TYPE_NOMONSTER	= 0x0010,	// ��4λ 1��������
    enumAREA_TYPE_MINE		= 0x0020,	// ��5λ 1
    enumAREA_TYPE_FIGHT_ASK	= 0x0040,	// ��6λ 1������PK
};

extern const char* g_GetAreaName( int nValue );

#define defCHA_TERRITORY_DISCRETIONAL	2	// ����ռ�
#define defCHA_TERRITORY_LAND			0	// ½��
#define defCHA_TERRITORY_SEA			1	// ����
