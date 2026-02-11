//======================================================================================================================
// FileName: CharacterRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaRecordSet class
//======================================================================================================================

#ifndef CHARACTERRECORD_H
#define CHARACTERRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

// �����Ϣ-ģ������
enum EChaModalType
{
	enumMODAL_MAIN_CHA		= 1,
	enumMODAL_BOAT,				
	enumMODAL_EMPL,
	enumMODAL_OTHER,
};

// ��������
enum EChaCtrlType
{
	enumCHACTRL_NONE		= 0, // δ�����

	enumCHACTRL_PLAYER		= 1, // ���

	enumCHACTRL_NPC			= 2, // ��ͨNPC
	enumCHACTRL_NPC_EVENT	= 3, // �¼�NPC

	enumCHACTRL_MONS		    = 5, // ��ͨ����
	enumCHACTRL_MONS_TREE	    = 6, // ������
	enumCHACTRL_MONS_MINE	    = 7, // ��ʯ����
	enumCHACTRL_MONS_FISH	    = 8, // �����
	enumCHACTRL_MONS_DBOAT	    = 9, // ��������

	enumCHACTRL_PLAYER_PET      = 10, // ��ҳ���

	enumCHACTRL_MONS_REPAIRABLE = 17, // ���޲�����
};

#define defCHA_AI_NONE				0 // ����
#define defCHA_AI_ATTACK_PASSIVE	1 // ��������
#define defCHA_AI_ATTACK_ACTIVE		2 // ��������

const char cchChaRecordKeyValue = (char)0xff;

#define defCHA_NAME_LEN			32
#define defCHA_ICON_NAME_LEN	17

#define defCHA_SKIN_NUM			8
#define defCHA_INIT_SKILL_NUM	11		// 0����,1-10Ϊ��������
#define defCHA_INIT_ITEM_NUM	15		// Edit by Mdrst
#define defCHA_GUILD_NAME_LEN	33
#define defCHA_TITLE_NAME_LEN	33
#define defCHA_JOB_NAME_LEN		17
#define defCHA_ITEM_KIND_NUM	20
#define defCHA_DIE_EFFECT_NUM	3		// �ͻ��������������
#define defCHA_BIRTH_EFFECT_NUM	3		// �ͻ�������������
#define defCHA_HP_EFFECT_NUM    3

class CChaRecord : public CRawDataInfo
{
public:
	//CChaRecord();

	long	lID;								// ���
	_TCHAR	szName[defCHA_NAME_LEN];			// ����
	_TCHAR	szIconName[defCHA_ICON_NAME_LEN];	// ͷ��ͼ������
	char	chModalType;						// ģ������
	char	chCtrlType;							// ��������
	short	sModel;								// �Ǽܺ�
	short	sSuitID;							// �׺�
	short	sSuitNum;							// ��װ����
	short	sSkinInfo[defCHA_SKIN_NUM];			// Ƥ��
	short	sFeffID[4];							// FeffID
	short	sEeffID;							// EeffID
	short   sEffectActionID[3];					// ��Ч�������,0-�������,1-��Ч,2-dummy
	short	sShadow;							// Ӱ��
    short   sActionID;                          // �������
    char    chDiaphaneity;                      // ͸����
	short	sFootfall;							// ������Ч
	short	sWhoop;								// ��Ϣ��Ч
	short	sDirge;								// ������Ч
	char	chControlAble;						// �Ƿ�ɿ�
	//char	chMoveAble;							// �ɷ��ƶ�
	char	chTerritory;						// ��������
	short   sSeaHeight;							// �߶�ƫ��
	short	sItemType[defCHA_ITEM_KIND_NUM];	// ��װ������
	float	fLengh;								// �����ף�
	float	fWidth;								// �����ף�
	float	fHeight;							// �ߣ��ף�
	short	sRadii;								// �뾶
	char    nBirthBehave[defCHA_BIRTH_EFFECT_NUM];// ��������
	char    nDiedBehave[defCHA_DIE_EFFECT_NUM];	// ��������
    short   sBornEff;                           // ������Ч
    short   sDieEff;                            // ������Ч
	short	sDormancy;							// ���߶���
    char    chDieAction;                        // ����ʱ�Ķ���
	int		_nHPEffect[defCHA_HP_EFFECT_NUM];	// ʣ��hp��Ч����
	bool	_IsFace;							// �Ƿ����ת
	bool	_IsCyclone;							// �Ƿ�ɱ�쫷紵��
	long	lScript;							// �ű��ļ�ID
	long	lWeapon;							// ���߱��е�����ID
	long	lSkill[defCHA_INIT_SKILL_NUM][2];	// ���ܼ�ʹ��Ƶ��
	long	lItem[defCHA_INIT_ITEM_NUM][2];		// ��Ʒ�����ϼ���
	long	lTaskItem[defCHA_INIT_ITEM_NUM][2];	// ������Ʒ�����ϼ���
	long	lMaxShowItem;						// ÿ����౬����Ʒ����
	float	fAllShow;							// �򱬼���
	long	lPrefix;							// ǰ׺�ȼ�
	long	lAiNo;								// AI���
	char	chCanTurn;							// ����ʱ�Ƿ�ת��
	long	lVision;							// ��Ұ��Χ�����ף�
	long	lNoise;								// ���з�Χ�����ף�
	long	lGetEXP;							// ֱ�ӻ�õ�EXP
	bool	bLight;								// �Ƿ��ܹ���Ӱ��

	long	lMobexp;// ��ͨexp
	long	lLv;	// ��ɫ�ȼ�
	long	lMxHp;	// ���HP
	long	lHp;	// ��ǰHP
	long	lMxSp;	// ���SP
	long	lSp;	// ��ǰSP
	long	lMnAtk;	// ��С������
	long	lMxAtk;	// ��󹥻���
	long	lPDef;	// �����ֿ�
	long	lDef;	// ������
	long	lHit;	// ������
	long	lFlee;	// ������
	long	lCrt;	// ������
	long	lMf;	// Ѱ����
	long	lHRec;	// hp�ָ��ٶ�
	long	lSRec;	// sp�ָ��ٶ�
	long	lASpd;	// �������
	long	lADis;	// ��������
	long	lCDis;	// ׷����Χ
	long	lMSpd;	// �ƶ��ٶ�
	long	lCol;	// ��Դ�ɼ��ٶ�
	long	lStr;	// ������strength��		Ӱ�칥�����������ٶ�
	long	lAgi;	// ���ݣ�agility��		Ӱ��������
	long	lDex;	// רע��dexterity��	Ӱ��������
	long	lCon;	// ���ʣ�constitution��	Ӱ���������hp
	long	lSta;	// ������stamina��		Ӱ��sp��Ӱ�켼�ܹ�����
	long	lLuk;	// ���ˣ�luck��			Ӱ�����һ�����ʡ���Ʒ���伸��
	long	lLHandVal;	// ������������
	_TCHAR	szGuild[defCHA_GUILD_NAME_LEN];	// �л�����
	_TCHAR	szTitle[defCHA_TITLE_NAME_LEN];	// ��ɫ�ƺ�
	_TCHAR	szJob[defCHA_JOB_NAME_LEN];		// ��ɫְҵ
	LONG32	lCExp;	// ��ǰ����
	LONG32	lNExp;	// ��һ�����辭��
	long	lFame;	// ����
	long	lAp;	// ���Ե�
	long	lTp;	// ���ܵ㣬ÿ��������ã����ܵ㣬���ɵ�technique point��
	long	lGd;	// ��Ǯ
	long	lSpri;	// �ڵ������ٶ�
	long	lStor;	// ����(����)����
	long	lMxSail;	// ��Ա����
	long	lSail;	// ���д�Ա������
	long	lStasa;	// ��׼��Ա��������
	long	lScsm;	// ��������

	long	lTStr;	// ��ʱ����				�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTAgi;	// ��ʱ����				�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTDex;	// ��ʱרע				�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTCon;	// ��ʱ����				�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTSta;	// ��ʱ����				�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTLuk;	// ��ʱ����				�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTMxHp;	// ��ʱ���Ѫ��			�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTMxSp;	// ��ʱ�����ֵ		�ɵ��߻��߼��ܼӳɵģ��õ����߻���ʱ�䵽���Զ�����
	long	lTAtk;	// ��ʱ������			��ʱ���ӵĹ�����
	long	lTDef;	// ��ʱ������			��ʱ���ӵķ�����
	long	lTHit;	// ��ʱ������			��ʱ���ӵ�������
	long	lTFlee;	// ��ʱ������			��ʱ���ӵ�������
	long	lTMf;	// ��ʱѰ����			��ʱ���ӵ�Ѱ����
	long	lTCrt;	// ��ʱ������			��ʱ���ӵı�����
	long	lTHRec;	// ��ʱhp�ָ��ٶ�		��ʱ���ӵ�ÿ���ӻָ�hp����ֵ
	long	lTSRec;	// ��ʱsp�ָ��ٶ�		��ʱ���ӵ�ÿ���ӻָ�sp����ֵ
	long	lTASpd;	// ��ʱ�������			��ʱ������ҹ������
	long	lTADis;	// ��ʱ��������			��ʱ������ҵĹ�������
	long	lTSpd;	// ��ʱ�ƶ��ٶ�			��ʱ���ӵ��ƶ��ٶ�
	long	lTSpri;	// ��ʱ�����ٶ�			��ʱ�ܵ�ÿ���ӷ��еľ���
	long	lTScsm;	// ��ʱ��������			��ʱ�ı�ĵ�λʱ���ڵ�����

	// added by clp ������Ϣ
	float	scaling[3];

public:
	// ����������������Ѫ���ʱ,Ҫ��ʾ����Ч
	bool	HaveEffectFog()				{ return _HaveEffectFog;	}
	int		GetEffectFog( int i )		{ return _nHPEffect[i];		}

	bool	IsFace()					{ return _IsFace;			}			// �Ƿ����ת
	bool	IsCyclone()					{ return _IsCyclone;		}			// �Ƿ�ɱ�쫷�ȴ�����

	void	RefreshPrivateData();				// ˢ���ڲ�����

private:
	bool	_HaveEffectFog;

};

class CChaRecordSet : public CRawDataSet
{
public:
	static CChaRecordSet* I() { return _Instance; }

	CChaRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CChaRecordSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CChaRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CChaRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CChaRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);

};

inline CChaRecord* GetChaRecordInfo( int nTypeID )
{
	return (CChaRecord*)CChaRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // CHARACTERRECORD_H