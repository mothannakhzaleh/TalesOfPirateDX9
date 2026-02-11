#pragma once
#include "UIGlobalVar.h"


#define bpHP 10
#define bpSP 10
#define bpMN 10
#define bpMX 10
#define bpASPD 10
#define bpDEF 10
#define bpPR 10
#define bpHR 10
#define bpFLEE 10
#define bpMSPD 10




namespace GUI
{
// 玩家属性
class CStateMgr : public CUIInterface
{
public:
	

	void			RefreshStateFrm();

protected:
	virtual bool	Init();
    virtual void	End();
	virtual void	FrameMove(DWORD dwTime);

private:
	static void _evtMainShow(CGuiData *pSender);
	static void MainMouseDown(CGuiData *pSender, int x, int y, DWORD key);

private:
	//frmState界面的控件
	CForm*			frmState;
	CLabelEx*		labName;                    //创建角色名称
	CLabelEx*		labGuildName;               //创建公会名称
	CLabelEx*		labStateLevel;              //创建等级
	CLabelEx*		labStatePoint;              //创建属性点数 
	CLabelEx*  		labSkillPoint;              //创建技能点数
	CLabelEx*		labJobShow;					//职业
	CLabelEx *      labFameShow;				//创建名声

	//6个基本属性
	CLabelEx*		labStrshow;
	CLabelEx*		labDexshow;
	CLabelEx*		labAgishow;
	CLabelEx*		labConshow;
	CLabelEx*		labStashow;
	CLabelEx*		labLukshow;
	CLabelEx*		labSailLevel;
	CLabelEx*		labSailEXP;

	//8个附加属性
	CLabelEx*		labMinAtackShow;
	CLabelEx*		labMaxAtackShow;
	CLabelEx*		labFleeShow;
	CLabelEx*		labAspeedShow;
	CLabelEx*		labMspeedShow;
	CLabelEx*		labHitShow;
	CLabelEx*		labDefenceShow;
	//CLabelEx*		labCriticalShow;
	//CLabelEx*		labMfShow;
	CLabelEx*		labPhysDefineShow;

	//6 个基本属性按钮
	CTextButton*   btnStr;						//创建力量栏
	CTextButton*   btnAgi;						//创建敏捷栏
	CTextButton*   btnCon;						//创建体质栏
	CTextButton*   btnSta;						//创建精神栏
	//CTextButton*   btnLuk;						//创建幸运栏
	CTextButton*   btnDex;						//创建专注栏     

	CLabelEx*		labStateEXP;
	CLabelEx*		labStateHP;
	CLabelEx*		labStateSP;

	CLabelEx*		labFameSho;              //创建名声
	CLabelEx*		labStateName;
	CLabelEx*		labStateJob;


};

}

