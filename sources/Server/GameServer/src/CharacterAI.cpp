#include "stdafx.h"
#include "Character.h"
#include "SubMap.h"
#include "NPC.h"
#include "TryUtil.h"

//--------------------------------------------------------
//                   AI控制所需的接口函数
//--------------------------------------------------------

void CCharacter::SrcFightTar(CFightAble *pTar, dbc::Short sSkillID)
{T_B
	Point	Path[2] = {GetShape().centre, pTar->GetShape().centre};
	Cmd_BeginSkillDirect(sSkillID, pTar);
T_E}
