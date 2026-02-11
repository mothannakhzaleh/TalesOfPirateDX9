#pragma once

enum EActionNumber
{
    POSE_WAITING =       01,//	普通waiting	
    POSE_SHOW =          02,//	摆酷pose	
    POSE_SLEEP =         03,//	休眠
    POSE_WAITING2 =      04,//	戒备waiting	
    POSE_RUN =           05,//	普通奔跑	
    POSE_RUN2 =          06,//	戒备奔跑	
    POSE_ATTACK =        07,//	普通攻击1
    POSE_ATTACK1 =       8, //	普通攻击2
    POSE_ATTACK2 =       9, //	普通攻击3
    POSE_POWER_ATTACK =  10,//  重击
    POSE_SKILL1 =        11,//	技能１
    POSE_SKILL2 =        12,//	技能２
    POSE_SKILL3 =        13,//	技能３
    POSE_SKILL4 =        14,//	技能４	011	
    POSE_SKILL5 =        15,//	技能５	012	
    POSE_SEAT =          16,//	普通坐下	
    POSE_DIE =           17,//	死亡(通用)	
    POSE_WAVE =          18,//	挥手(通用)	
    POSE_CRY =           19,//	大哭(通用)	
    POSE_JUMP =          20,//	雀跃(通用)	
    POSE_ANGER =         21,//	生气(通用)	
    POSE_DARE =          22,//	挑衅(通用)	
    POSE_GAY =           23,//	自恋(男通用)
    POSE_EMOTE7 =        24,//	表情动作7	
    POSE_EMOTE8 =        25,//	表情动作8	
    POSE_EMOTE9 =        26,//	表情动作9	
    POSE_EMOTE10 =       27,//	表情动作10	
    POSE_MINE =          28,//	挖矿(场景互动
    POSE_COLLECT =       29,//	采集(场景互动
    POSE_SEAT2 =         30,//	坐凳子(场景互
    POSE_LEAN =          31,//	靠墙(场景互动
    POSE_LEAN2 =         32,//	靠树(场景互动
    POSE_HAPPY =         33,//	角色互动(场景互动)
    POSE_HAPPY7 =        34,//	场景互动动作7
    POSE_HAPPY8 =        35,//	场景互动动作8
    POSE_HAPPY9 =        36,//	场景互动动作9
    POSE_HAPPY10 =       37,//	场景互动动作10
    POSE_RESERVED1 =     38,//	备用动作1	
    POSE_FALLDOWN =      39,//	备用动作2	
    POSE_SKILL6 =        40,//	技能动作6	
    POSE_SKILL7 =        41,//	技能动作7	
    POSE_FLY_WAITING =   42,//	飞行				//POSE_SKILL8 =        42,//	技能动作8	
    POSE_FLY_RUN =       43,//	飞行移动			//POSE_SKILL9 =        43,//	技能动作9	
    POSE_FLY_SHOW =      44,//	空中摆酷			//POSE_SKILL10 =       44,//	技能动作10	
    POSE_FLY_SEAT =      45,//	悬空坐下			//POSE_SKILL11 =       45,//	技能动作11	
    POSE_SKILL12 =       46,//	技能动作12	
    POSE_SKILL13 =       47,//	技能动作13	
    POSE_SKILL14 =       48,//	技能动作14	
    POSE_SKILL15 =       49,//	技能动作15	
    POSE_SKILL16 =       50,//	技能动作16	
    POSE_SKILL17 =       51,//	技能动作17	
    POSE_SKILL18 =       52,//	技能动作18	
    POSE_SKILL19 =       53,//	技能动作19	
    POSE_SKILL20 =       54,//	技能动作20	
};

struct SActionInfo
{
	short		m_sActionNO;	// 动作编号
	//short		m_sStartFrame;	// 起始帧
	//short		m_sEndFrame;	// 结束帧

	//short		m_sKeyFrameNum;	// 关键帧个数
	//short		*m_sKeyFrame;	// 关键帧集
    lwPoseInfo info;
};

struct SCharacterAction
{
	short		m_iCharacterType;	// 角色类型
	short		m_iMaxActionNum;	// 角色的最大动作数(从文件中读到的角色的最大动作号)
	short		m_iActualActionNum;	// 角色的有效动作数(从文件中读到的角色的动作个数)
	SActionInfo	*m_SActionInfo;		// 角色单个动作信息
};

class	CGameCharacterAction
{
public:
	CGameCharacterAction();
	~CGameCharacterAction();

	bool		Init(const char* ptcsFileName);
	void		Free(void);

	void		GetMaxCharType(int *iMaxType) {*iMaxType = m_iMaxCharacterType;};
	void		GetActualCharType(int *iActualType) {*iActualType = m_iActualCharacterType;};
	bool		GetCharMaxActNum(int iCharType, int *iMaxActNum);
	bool		GetCharActualActNum(int iCharType, int *iActualActNum);
	bool		GetCharFrameInfo(int iCharType, int iActionNO, int *iStartFrame, int *iEndFrame);
	bool		GetCharAction(int iCharType, SCharacterAction *SCharAct);

protected:
	short		m_iMaxCharacterType;		// 最大角色类型数(从文件中读到的最大的角色类型号)
	short		m_iActualCharacterType;		// 有效角色类型数(从文件中读到的角色个数)
	SCharacterAction	*m_SCharacterAction;
};

long StringGet(char* out, long out_max, char* in, long* in_from, const char* end_list, long end_len);
void StringSkipCompartment(char* in, long* in_from, const char* skip_list, long skip_len);


extern CGameCharacterAction g_PoseData;
extern void InitPoseData();
extern bool GetCharacterAction(int nTypeID, SCharacterAction *SCharAct);
