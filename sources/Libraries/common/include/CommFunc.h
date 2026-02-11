//=============================================================================
// FileName: CommFunc.h
// Creater: ZhangXuedong
// Date: 2005.01.06
// Comment: 
//	2005.4.28	Arcol	add the text filter manager class: CTextFilter
//=============================================================================

#ifndef COMMFUNC_H
#define COMMFUNC_H

#include "CompCommand.h"
#include "SkillRecord.h"
#include "CharacterRecord.h"
#include "ItemRecord.h"
#include "ItemAttrType.h"
#include "JobType.h"
#include "NetRetCode.h"
#include <regex>
#include <string_view>
#include <algorithm>
#include <bitset>
#include "i18n.h"


extern bool KitbagStringConv(short sKbCapacity, std::string &strData);

//=============================================================================
/*---------------------------------------------------------------
* ��;:���ڼ�ⴴ���Ľ�ɫ��������Ƿ�Ϸ�
* nPart - ��Ӧ���ID,nValue - ��۵�ֵ
* ����ֵ����������Ƿ�Ϸ���
*/
extern bool g_IsValidLook( int nType, int nPart, long nValue );

/*---------------------------------------------------------------
* ulAreaMask ��������
* ����ֵ��true ����false ½��
*/
inline bool g_IsSea(unsigned short usAreaMask)
{
	return !(usAreaMask & enumAREA_TYPE_LAND);
}

inline bool g_IsLand(unsigned short usAreaMask)
{
	return (usAreaMask & enumAREA_TYPE_LAND) || (usAreaMask & enumAREA_TYPE_BRIDGE);
}

// ���ݴ���������ֵ���ID
// ���ؿ���ʹ�õ�Ĭ�ϼ���,����-1,û�м���
extern int g_GetItemSkill( int nLeftItemID, int nRightItemID );

extern BOOL IsDist( int x1, int y1, int x2, int y2, DWORD dwDist );

// �Ƿ���ȷ�ļ���Ŀ��
extern int	g_IsRightSkillTar(int nTChaCtrlType, bool bTIsDie, bool bTChaBeSkilled, int nTChaArea,
					  int nSChaCtrlType, int nSSkillObjType, int nSSkillObjHabitat, int nSSkillEffType,
					  bool bIsTeammate, bool bIsFriend, bool bIsSelf);

/*---------------------------------------------------------------
* ����:���֣����֣�����ĵ���ID�����ܱ�š�
* ����ֵ:1-��ʹ��,0-����ʹ��,-1-���ܲ�����
*/
extern int g_IsUseSkill( stNetChangeChaPart *pSEquip, int nSkillID );
extern bool g_IsRealItemID(int nItemID);

inline int g_IsUseSkill( stNetChangeChaPart *pSEquip, CSkillRecord *p )
{
	if( !p )  return -1;

	return g_IsUseSkill( pSEquip, p->nID );
}

inline int g_IsUseSeaLiveSkill(long lFitNo, CSkillRecord *p)
{
	if( !p )  return -1;

	for( int i=0; i<defSKILL_ITEM_NEED_NUM; i++ )
	{	
		if( p->sItemNeed[0][i][0] == cchSkillRecordKeyValue )
			break;

		if (p->sItemNeed[0][i][0] == enumSKILL_ITEM_NEED_ID)
		{
			if (p->sItemNeed[0][i][1] == lFitNo)
				return 1;
		}
	}

	return 0;
}

inline bool g_IsPlyCtrlCha(int nChaCtrlType)
{
	if (nChaCtrlType == enumCHACTRL_PLAYER || nChaCtrlType == enumCHACTRL_PLAYER_PET)
		return true;
	return false;
}

inline bool g_IsMonsCtrlCha(int nChaCtrlType)
{
	if (nChaCtrlType == enumCHACTRL_MONS
		|| nChaCtrlType == enumCHACTRL_MONS_TREE
		|| nChaCtrlType == enumCHACTRL_MONS_MINE
		|| nChaCtrlType == enumCHACTRL_MONS_FISH
		|| nChaCtrlType == enumCHACTRL_MONS_DBOAT
		|| nChaCtrlType == enumCHACTRL_MONS_REPAIRABLE
		)
		return true;
	return false;
}

inline bool g_IsNPCCtrlCha(int nChaCtrlType)
{
	if (nChaCtrlType == enumCHACTRL_NPC || nChaCtrlType == enumCHACTRL_NPC_EVENT)
		return true;
	return false;
}

inline bool g_IsChaEnemyCtrlSide(int nSCtrlType, int nTCtrlType)
{
	if (g_IsPlyCtrlCha(nSCtrlType) && g_IsPlyCtrlCha(nTCtrlType))
		return false;
	if (g_IsMonsCtrlCha(nSCtrlType) && g_IsMonsCtrlCha(nTCtrlType))
		return false;
	return true;
}

inline bool g_IsFileExist(const char *szFileName)
{
    FILE	*fp = NULL;
    if (NULL == (fp = fopen(szFileName,"rb")))
        return false;
    if (fp) fclose(fp);
    return true;
}

extern void String2Item(const char* pszData, SItemGrid* SGridCont);
extern char* LookData2String(const stNetChangeChaPart *pLook, char *szLookBuf, int nLen, bool bNewLook = true);
extern bool Strin2LookData(stNetChangeChaPart *pLook, std::string &strData);
extern char* ShortcutData2String(const stNetShortCut *pShortcut, char *szShortcutBuf, int nLen);
extern bool String2ShortcutData(stNetShortCut *pShortcut, std::string &strData);


inline long g_ConvItemAttrTypeToCha(long lItemAttrType)
{
	if (lItemAttrType >= ITEMATTR_COE_STR && lItemAttrType <= ITEMATTR_COE_PDEF)
		return lItemAttrType + (ATTR_ITEMC_STR - ITEMATTR_COE_STR);
	else if (lItemAttrType >= ITEMATTR_VAL_STR && lItemAttrType <= ITEMATTR_VAL_PDEF)
		return lItemAttrType + (ATTR_ITEMV_STR - ITEMATTR_VAL_STR);
	else
		return 0;
}

// ��Ӧ�������͵Ĳ�������
inline short g_GetRangeParamNum(char RangeType)
{
	short	sParamNum = 0;
	switch (RangeType)
	{
	case	enumRANGE_TYPE_STICK:
		sParamNum = 2;
		break;
	case	enumRANGE_TYPE_FAN:
		sParamNum = 2;
		break;
	case	enumRANGE_TYPE_SQUARE:
		sParamNum = 1;
		break;
	case	enumRANGE_TYPE_CIRCLE:
		sParamNum = 1;
		break;
	}

	return sParamNum + 1;
}

//=============================================================================
// chChaType ��ɫ����
// chChaTerrType ��ɫ��ռ������
// bIsBlock �Ƿ��ϰ�
// ulAreaMask ��������
// ����ֵ��true ���ڸõ�Ԫ���ƶ���false �����ƶ�
//=============================================================================
inline bool	g_IsMoveAble(char chChaCtrlType, char chChaTerrType, unsigned short usAreaMask)
{
	bool	bRet1 = false;
	if (chChaTerrType == defCHA_TERRITORY_DISCRETIONAL)
		bRet1 = true;
	else if (chChaTerrType == defCHA_TERRITORY_LAND)
	{
		if (usAreaMask & enumAREA_TYPE_LAND || usAreaMask & enumAREA_TYPE_BRIDGE)
			bRet1 = true;
	}
	else if (chChaTerrType == defCHA_TERRITORY_SEA)
	{
		if (!(usAreaMask & enumAREA_TYPE_LAND))
			bRet1 = true;
	}

	bool	bRet2 = true;
	if (usAreaMask & enumAREA_TYPE_NOT_FIGHT) // ��ս������
	{
		if (g_IsMonsCtrlCha(chChaCtrlType))
			bRet2 = false;
	}

	return bRet1 && bRet2;
}

inline const char* g_GetJobName(short sJobID)
{
	if (sJobID < 0 || sJobID >= MAX_JOB_TYPE)
		return g_szJobName[0];

	return g_szJobName[sJobID];
}

inline short g_GetJobID(const char *szJobName)
{
	for (short i = 0; i < MAX_JOB_TYPE; i++)
	{
		if (!strcmp(g_szJobName[i], szJobName))
			return i;
	}

	return 0;
}

inline const char* g_GetCityName(short sCityID)
{
	if (sCityID < 0 || sCityID >= defMAX_CITY_NUM)
		return "";

	return g_szCityName[sCityID];
}

inline short g_GetCityID(const char *szCityName)
{
	for (short i = 0; i < defMAX_CITY_NUM; i++)
	{
		if (!strcmp(g_szCityName[i], szCityName))
			return i;
	}

	return -1;
}

inline bool g_IsSeatPose( int pose )
{
	return 16==pose;
}

// �������ַ�����
inline bool g_IsValidFightState( int nState )
{
	return nState < enumFSTATE_TARGET_NO;
}

inline bool g_ExistStateIsDie(char chState)
{
	if (chState >= enumEXISTS_WITHERING)
		return true;

	return false;
}

inline const char* g_GetItemAttrExplain( int v )
{
    switch( v )
    {
    case ITEMATTR_COE_STR: return "Strength Bonus";// "�����ӳ�"; 
    case ITEMATTR_COE_AGI: return "Agility Bonus";// "���ݼӳ�"; 
    case ITEMATTR_COE_DEX: return "Accuracy Bonus";// "רע�ӳ�"; 
    case ITEMATTR_COE_CON: return "Constitution Bonus";// "���ʼӳ�"; 
    case ITEMATTR_COE_STA: return "Spirit Bonus";// "����ӳ�"; 
    case ITEMATTR_COE_LUK: return "Luck Bonus";// "���˼ӳ�"; 
    case ITEMATTR_COE_ASPD: return "Attack Speed Bonus";// "����Ƶ�ʼӳ�"; 
    case ITEMATTR_COE_ADIS: return "Attack Range Bonus";// "��������ӳ�"; 
    case ITEMATTR_COE_MNATK: return "Minimum Attack Bonus";// "��С�������ӳ�"; 
    case ITEMATTR_COE_MXATK: return "Maximum Attack Bonus";// "��󹥻����ӳ�"; 
    case ITEMATTR_COE_DEF: return "Defense Bonus";// "�����ӳ�"; 
    case ITEMATTR_COE_MXHP: return "Maximum HP Bonus";// "���HP�ӳ�"; 
    case ITEMATTR_COE_MXSP: return "Maximum SP Bonus";// "���SP�ӳ�"; 
    case ITEMATTR_COE_FLEE: return "Dodge Rate Bonus";// "�����ʼӳ�"; 
    case ITEMATTR_COE_HIT: return "Hit Rate Bonus";// "�����ʼӳ�"; 
    case ITEMATTR_COE_CRT: return "Critical Hitrate Bonus";// "�����ʼӳ�"; 
    case ITEMATTR_COE_MF: return "Drop Rate Bonus";// "Ѱ���ʼӳ�"; 
    case ITEMATTR_COE_HREC: return "HP Recovery Speed Bonus";// "HP�ָ��ٶȼӳ�"; 
    case ITEMATTR_COE_SREC: return "SP Recovery Speed Bonus";// "SP�ָ��ٶȼӳ�"; 
    case ITEMATTR_COE_MSPD: return "Movement Speed Bonus";// "�ƶ��ٶȼӳ�"; 
    case ITEMATTR_COE_COL: return "Material Mining Speed Bonus";// "��Դ�ɼ��ٶȼӳ�"; 

    case ITEMATTR_VAL_STR: return "Strength Bonus";// "�����ӳ�"; 
    case ITEMATTR_VAL_AGI: return "Agility Bonus";// "���ݼӳ�"; 
    case ITEMATTR_VAL_DEX: return "Accuracy Bonus";// "רע�ӳ�"; 
    case ITEMATTR_VAL_CON: return "Constitution Bonus";// "���ʼӳ�"; 
    case ITEMATTR_VAL_STA: return "Spirit Bonus";// "����ӳ�"; 
    case ITEMATTR_VAL_LUK: return "Luck Bonus";// "���˼ӳ�"; 
    case ITEMATTR_VAL_ASPD: return "Attack Speed Bonus";// "����Ƶ�ʼӳ�"; 
    case ITEMATTR_VAL_ADIS: return "Attack Range Bonus";// "��������ӳ�"; 
    case ITEMATTR_VAL_MNATK: return "Minimum Attack Bonus";// "��С�������ӳ�"; 
    case ITEMATTR_VAL_MXATK: return "Maximum Attack Bonus";// "��󹥻����ӳ�"; 
    case ITEMATTR_VAL_DEF: return "Defense Bonus";// "�����ӳ�"; 
    case ITEMATTR_VAL_MXHP: return "Maximum HP Bonus";// "���HP�ӳ�"; 
    case ITEMATTR_VAL_MXSP: return "Maximum SP Bonus";// "���SP�ӳ�"; 
    case ITEMATTR_VAL_FLEE: return "Dodge Rate Bonus";// "�����ʼӳ�"; 
    case ITEMATTR_VAL_HIT: return "Hit Rate Bonus";// "�����ʼӳ�"; 
    case ITEMATTR_VAL_CRT: return "Critical Hitrate Bonus";// "�����ʼӳ�"; 
    case ITEMATTR_VAL_MF: return "Drop Rate Bonus";// "Ѱ���ʼӳ�"; 
    case ITEMATTR_VAL_HREC: return "HP Recovery Speed Bonus";// "HP�ָ��ٶȼӳ�"; 
    case ITEMATTR_VAL_SREC: return "SP Recovery Speed Bonus";// "SP�ָ��ٶȼӳ�"; 
    case ITEMATTR_VAL_MSPD: return "Movement Speed Bonus";// "�ƶ��ٶȼӳ�"; 
    case ITEMATTR_VAL_COL: return "Material Mining Speed Bonus";// "��Դ�ɼ��ٶȼӳ�"; 

	case ITEMATTR_VAL_PDEF: return "Physical Resist Bonus";// "�����ֿ��ӳ�";
    case ITEMATTR_MAXURE: return "Max Durability";// "����;ö�"; 
    case ITEMATTR_MAXENERGY: return "Max Energy";// "�������"; 
    default: return "Unknown tools characteristics";// "δ֪��������";
    }
}

inline const char* g_GetServerError( int error_code )  // ����������
{
	switch( error_code )
	{
	case ERR_AP_INVALIDUSER:return "Invalid Account";// "��Ч�˺�";
	case ERR_AP_INVALIDPWD:	return "Password incorrect";// "�������";
	case ERR_AP_ACTIVEUSER: return "Account activation failed";// "�����û�ʧ��";
    case ERR_AP_DISABLELOGIN: return "Your cha is currently in logout save mode, please try logging in again later.";// "Ŀǰ���Ľ�ɫ���������ߴ��̹����У����Ժ��ٳ��Ե�¼��";
    case ERR_AP_LOGGED: return "This account is already online";// "���ʺ��Ѵ��ڵ�¼״̬";
    case ERR_AP_BANUSER: return "Account has been banned";// "�ʺ��ѷ�ͣ";
	case ERR_AP_GPSLOGGED: return "This GroupServer has login";// "��GroupServer�ѵ�¼";
	case ERR_AP_GPSAUTHFAIL: return "This GroupServer Verification failed";// "��GroupServer��֤ʧ��";
	case ERR_AP_SAVING:	return "Saving your character, please try again in 15 seconds...";// "���ڱ�����Ľ�ɫ����15�������...";
    case ERR_AP_LOGINTWICE: return "Your account is logged on far away";// "����˺���Զ���ٴε�¼";
    case ERR_AP_ONLINE: return "Your account is already online";// "����˺�������";
	case ERR_AP_DISCONN: return "GroupServer disconnected";// "GroupServer�ѶϿ�";
	case ERR_AP_UNKNOWNCMD: return "unknown agreement, don\'t deal with";// "δ֪Э�飬������";
	case ERR_AP_TLSWRONG: return "local saving error";// "���ش洢����";
	case ERR_AP_NOBILL:	return "This account has expired, please topup!";// "���˺��ѹ��ڣ����ֵ��";


	case ERR_PT_LOGFAIL:	return "GateServer to GroupServer login failed";// "GateServer��GroupServer�ĵ�¼ʧ��";
	case ERR_PT_SAMEGATENAME:return "GateServer and login GateServer have similar name";// "GateServer���ѵ�¼GateServer����";

	case ERR_PT_INVALIDDAT:return "Ineffective data model";// "��Ч�����ݸ�ʽ";
	case ERR_PT_INERR: return "server link operation integrality error ";// "������֮��Ĳ��������Դ���";
	case ERR_PT_NETEXCP: return "Account server has encountered a malfunction";// "�ʺŷ���������";	// GroupServer���ֵĵ�AccuntServer���������
	case ERR_PT_DBEXCP: return "database server malfunction";// "���ݿ����������";	// GroupServer���ֵĵ�Database�Ĺ���
	case ERR_PT_INVALIDCHA: return "Current account does not have a request (Select/Delete) to character";// "��ǰ�ʺŲ�ӵ������(ѡ��/ɾ��)�Ľ�ɫ";
	case ERR_PT_TOMAXCHA: return "reached the maximum number of characters you can create";// "�Ѿ��ﵽ����ܴ����Ľ�ɫ����";
	case ERR_PT_BONUSCHARS: return "You don't have the necessary levels to create another character";
	case ERR_PT_SAMECHANAME: return "Character name already exist";// "�ظ��Ľ�ɫ��";
	case ERR_PT_INVALIDBIRTH: return "illegal birth place";// "�Ƿ��ĳ�����";
	case ERR_PT_TOOBIGCHANM:return "Character name is too long";// "��ɫ��̫��";
	case ERR_PT_ISGLDLEADER:return "Guild must have a leader, please disband your guild first then delete your character";// "���᲻��һ���޳������Ƚ�ɢ������ɾ����ɫ";
	case ERR_PT_ERRCHANAME:return "Illegal character name";// "�Ƿ��Ľ�ɫ����";
	case ERR_PT_SERVERBUSY: return "System is busy, please try again later";// "ϵͳæ�����Ժ�����";
	case ERR_PT_TOOBIGPW2: return "second code length illegal";// "�������볤�ȷǷ�";
	case ERR_PT_INVALID_PW2: return "Cha second password not created";// "δ������ɫ������������";
	case ERR_PT_BADBOY:	return "My child, you are very bold. You have been reported to the authority. Please do not commit the offense again!";// "���ӣ����BT���Ѿ��Զ�����������ͨ��������Ҫ����Ϊ�䣬�����ٷ���";
	case ERR_PT_BANUSER: return RES_STRING(CO_COMMFUNC_H_00031);// "ÕÊºÅÒÑ·âÍ£";
	case ERR_PT_PBANUSER: return RES_STRING(CO_COMMFUNC_H_00108);// "ÒÑ¾­Ê¹ÓÃÁËÃÜ±£¿¨";
	case ERR_MC_NETEXCP: return "Discovered exceptional line error on GateServer";// "GateServer���ֵ������쳣";
	case ERR_MC_NOTSELCHA: return "current not yet handled character state";// "��ǰδ����ѡ���ɫ״̬";
	case ERR_MC_NOTPLAY: return "Currently not in gameplay, unable to send ENDPLAY command";// "��ǰ����������Ϸ״̬�����ܷ���ENDPLAY����";
	case ERR_MC_NOTARRIVE: return "target map cannot be reached";// "Ŀ���ͼ���ɵ���";
	case ERR_MC_TOOMANYPLY: return "This server is currently full, please select another server!";// "������������������, ��ѡ�������������Ϸ!";
	case ERR_MC_NOTLOGIN: return "Youa re not login";// "����δ��½";
	case ERR_MC_VER_ERROR: return "Client version error, server refused connection!";// "�ͻ��˵İ汾�Ŵ���,�������ܾ���¼��";
	case ERR_MC_ENTER_ERROR: return "failed to enter map!";// "�����ͼʧ�ܣ�";
	case ERR_MC_ENTER_POS: return "Map position illegal, you\'ll be sent back to your birth city, please relog!";// "��ͼλ�÷Ƿ����������ͻس������У������µ�½��";
	case ERR_MC_BANUSER: return RES_STRING(CO_COMMFUNC_H_00031);// "ÕÊºÅÒÑ·âÍ£";
	case ERR_MC_PBANUSER: return RES_STRING(CO_COMMFUNC_H_00108);// "ÒÑ¾­Ê¹ÓÃÁËÃÜ±£¿¨";
	case ERR_TM_OVERNAME: return "GameServer name repeated";// "GameServer���ظ�";
	case ERR_TM_OVERMAP: return "GameServerMapNameRepeated";// "GameServer�ϵ�ͼ���ظ�";
	case ERR_TM_MAPERR: return "GameServer map assign language error";// "GameServer��ͼ�����﷨����";

	case ERR_SUCCESS:return "Jack is too BT, correct also will ask me if anything is wrong!";// "Jack̫BT�ˣ���ȷҲ������ʲô����";
	default:
		{
			int l_error_code	=error_code;
			l_error_code	/=500;
			l_error_code	*=500;
			static char l_buffer[500];
			char		l_convt[20];
			switch ( l_error_code )
			{
			case ERR_MC_BASE:return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(GameServer/GateServer->Client return error code space 1-500)"); //"(GameServer/GateServer->Client���صĴ�����ռ�1��500)");
			case ERR_PT_BASE: return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(GroupServer->GateServer return error code range 501-1000)"); //"(GroupServer->GateServer���صĴ�����ռ�501��1000)"); 
			case ERR_AP_BASE: return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(AccountServer->GroupServe return error code from 1001-1500)"); //"(AccountServer->GroupServer���صĴ�����ռ�1001��1500)");
			case ERR_MT_BASE: return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(GameServer->GateServer return error code range 1501-2000)"); //"(GameServer->GateServer���صĴ�����ռ�1501��2000)");
			default:return strcat(strcpy(l_buffer,itoa(error_code,l_convt,10)),"(Jack is too insane, he made a mistake that I don\'t even know.)"); //"(Jack̫BT�ˣ�Ū���������Ҷ�����ʶ��)");
			}
		}
	}
}


inline bool isAlphanumeric(std::string_view text)
{
	return std::all_of(text.begin(), text.end(), ::isalnum);

}

inline bool isNumeric(std::string_view text)
{
	return std::all_of(text.begin(), text.end(), ::isdigit);
}

inline bool isEmail(const char* email) {
	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	//const std::regex pattern("([\\w\\.\\_\\-] + )@([\\w\\.\\_\\-] + )(\\.(\\w + )) +");
	return std::regex_match(email, pattern);
}


//���������ܰ�������ַ�����GBK˫�ֽں����ַ��������ԡ���������ַ����������Եȡ�
//nameΪֻ�����д�Сд��ĸ���ֺͺ��֣�ȥ��ȫ�ǿո񣩲ŷ���true;
//len����Ϊ�ַ���name�ĳ���=strlen(name),��������βNULL�ַ���
inline bool IsValidName(const char *name,unsigned short len)
{
	const unsigned char *l_name =reinterpret_cast<const unsigned char *>(name);
	bool l_ishan	=false;
	//if (len == 0)
	//	return 0;
	for(unsigned short i=0;i<len;i++)
	{
		if(!l_name[i])
		{
			return false;
		}else if(l_ishan)
		{
			if(l_name[i-1] ==0xA1 && l_name[i] ==0xA1)	//����ȫ�ǿո�
			{
				return false;
			}
			if(l_name[i] >0x3F && l_name[i] <0xFF && l_name[i] !=0x7F)
			{
				l_ishan =false;
			}else
			{
				return false;
			}
		}else if(l_name[i]>0x80 && l_name[i] <0xFF)
		{
			l_ishan	=true;
		}else if((l_name[i] >='A' && l_name[i] <='Z') ||(l_name[i] >='a' && l_name[i] <='z') ||(l_name[i] >='0' && l_name[i] <='9'))
		{
			
		}else
		{
			return false;
		}
	}
	return !l_ishan;
}

inline const char* g_GetUseItemFailedInfo(short sErrorID)
{
	switch (sErrorID)
	{
	case	enumITEMOPT_SUCCESS:
		return "Item operation succesful";// "���߲����ɹ�";
		break;
	case	enumITEMOPT_ERROR_NONE:
		return "Equipment does not exist";// "���߲�����";
		break;
	case	enumITEMOPT_ERROR_KBFULL:
		return "Inventory is full";// "����������";
		break;
	case	enumITEMOPT_ERROR_UNUSE:
		return "Failed to use item";// "����ʹ��ʧ��";
		break;
	case	enumITEMOPT_ERROR_UNPICKUP:
		return "�Rl??���E�C?";// "���߲���ʰȡ";
		break;
	case	enumITEMOPT_ERROR_UNTHROW:
		return "Item cannot be thrown";// "���߲��ܶ���";
		break;
	case	enumITEMOPT_ERROR_UNDEL:
		return "Item cannot be destroyed";// "���߲�������";
		break;
	case	enumITEMOPT_ERROR_KBLOCK:
		return "inventory is currently locked";// "��������������״̬";
		break;
	case	enumITEMOPT_ERROR_DISTANCE:
		return "Distance too far";// "����̫Զ";
		break;
	case	enumITEMOPT_ERROR_EQUIPLV:
		return "Equipment level mismatch";// "װ���ȼ�������";
		break;
	case	enumITEMOPT_ERROR_EQUIPJOB:
		return "Does not meet the class requirement for the equipment";// "װ��ְҵ������";
		break;
	case	enumITEMOPT_ERROR_STATE:
		return "Unable to operate items under the current condition";// "��״̬�²��ܲ�������";
		break;
	case	enumITEMOPT_ERROR_PROTECT:
		return "Item is being protected";// "���߱�����";
		break;
	case	enumITEMOPT_ERROR_AREA:
		return "different region type";// "��ͬ����������";
		break;
	case	enumITEMOPT_ERROR_BODY:
		return "type of build does not match";// "���Ͳ�ƥ��";
		break;
	case	enumITEMOPT_ERROR_TYPE:
		return "Unable to store this item";// "�˵����޷����";
		break;
	case	enumITEMOPT_ERROR_INVALID:
		return "Item not in used";// "��Ч�ĵ���";
		break;
	case	enumITEMOPT_ERROR_KBRANGE:
		return "out of inventory range";// "������������Χ";
		break;
	case	enumITEMOPT_ERROR_EXPIRATION:
		return "This item is expired.";
		break;
	default:
		return "Unknown item usage failure code";// "δ֪�ĵ��߲���ʧ����";
		break;
	}
}

class CTextFilter
{
public:
#define eTableMax 5
	enum eFilterTable{NAME_TABLE=0, DIALOG_TABLE=1, MAX_TABLE=eTableMax};
	/*
	* Warning : Do not use MAX_TABLE enum value, it just use for the maximum limit definition,
	*			If you want to expand this enum table value more than the default number eTableMax(5),
	*			please increase the eTableMax definition
	*/

	CTextFilter();
	~CTextFilter();
	static bool Add(const eFilterTable eTable, const char *szFilterText);
	static bool IsLegalText(const eFilterTable eTable, const std::string strText);
	static bool Filter(const eFilterTable eTable, std::string &strText);
	static bool LoadFile(const char *szFileName, const eFilterTable eTable=NAME_TABLE);
	static BYTE* GetNowSign(const eFilterTable eTable);

private:
	static bool ReplaceText(std::string &strText, const std::string *pstrFilterText);
	static bool bCheckLegalText(const std::string &strText, const std::string *pstrIllegalText);

	static std::vector<std::string> m_FilterTable[eTableMax];
	static BYTE m_NowSign[eTableMax][8];
};

#endif // COMMFUNC_H