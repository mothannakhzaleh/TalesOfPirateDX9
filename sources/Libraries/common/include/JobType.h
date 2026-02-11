//=============================================================================
// FileName: JobType.h
// Creater: ZhangXuedong
// Date: 2005.03.18
// Comment: Job Type
//=============================================================================

#ifndef JOBTYPE_H
#define JOBTYPE_H

const long	JOB_TYPE_XINSHOU		= 0;	// "新手"
const long	JOB_TYPE_JIANSHI		= 1;	// "剑士"
const long	JOB_TYPE_LIEREN			= 2;	// "猎人"
const long	JOB_TYPE_SHUISHOU		= 3;	// "水手"
const long	JOB_TYPE_MAOXIANZHE		= 4;	// "冒险者"
const long	JOB_TYPE_QIYUANSHI		= 5;	// "祈愿使"
const long	JOB_TYPE_JISHI			= 6;	// "技师"
const long	JOB_TYPE_SHANGREN		= 7;	// "商人"
const long	JOB_TYPE_JUJS			= 8;	// "巨剑士"
const long	JOB_TYPE_SHUANGJS		= 9;	// "双剑士"
const long	JOB_TYPE_JIANDUNSHI		= 10;	// "剑盾士"
const long	JOB_TYPE_XUNSHOUSHI		= 11;	// "驯兽师"
const long	JOB_TYPE_JUJISHOU		= 12;	// "狙击手"
const long	JOB_TYPE_SHENGZHIZHE	= 13;	// "圣职者"
const long	JOB_TYPE_FENGYINSHI		= 14;	// "封印师"
const long	JOB_TYPE_CHUANZHANG		= 15;	// "船长"
const long	JOB_TYPE_HANGHAISHI		= 16;	// "航海士"
const long	JOB_TYPE_BAOFAHU		= 17;	// "暴发户"
const long	JOB_TYPE_GONGCHENGSHI	= 18;	// "工程师"

const long	MAX_JOB_TYPE			= 19;	// 最大职业类型

extern const char	*g_szJobName[MAX_JOB_TYPE];

#endif // JOBTYPE_H