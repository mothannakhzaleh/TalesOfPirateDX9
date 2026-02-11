//=============================================================================
// FileName: SkillTemp.h
// Creater: ZhangXuedong
// Date: 2005.03.03
// Comment: Skill Temp Data
//=============================================================================

#ifndef SKILLTEMP_H
#define SKILLTEMP_H

#include "PreAlloc.h"

#define defSKILL_RANGE_BASEP_NUM	3	// 区域基本参数个数（范围中心坐标，方向）
#define defSKILL_RANGE_EXTEP_NUM	4	// 区域扩展参数个数（范围类型，类型参数）
#define defSKILL_STATE_PARAM_NUM	3	// 区域状态参数个数（编号，等级，持续时间）

class CSkillTempData : public dbc::PreAllocStru
{
public:
	CSkillTempData(unsigned long ulSize = 1) : PreAllocStru(1)
	{
		sRange[0] = enumRANGE_TYPE_NONE;
		sStateParam[0] = SSTATE_NONE;
	}

	short	sUseEndure{};	// 对“耐久度“的消耗
	short	sUseEnergy{};	// 对“能量“的消耗
	short	sUseSP{};		// 对“SP“的消耗

	long	lResumeTime{};// 再释放需要的时间（毫秒）
	short	sRange[defSKILL_RANGE_EXTEP_NUM];	// 区域描述（类型+参数）
	short	sStateParam[defSKILL_STATE_PARAM_NUM];	// 状态编号，等级，持续时间

protected:

private:
	void	Initially();
	void	Finally();

};

inline void CSkillTempData::Initially()
{
	sUseEndure = 0;
	sUseEnergy = 0;
	sUseSP = 0;
	lResumeTime = 0;
	sRange[0] = enumRANGE_TYPE_NONE;
	sStateParam[0] = SSTATE_NONE;
}

inline void CSkillTempData::Finally()
{
}

#endif // SKILLTEMP_H
