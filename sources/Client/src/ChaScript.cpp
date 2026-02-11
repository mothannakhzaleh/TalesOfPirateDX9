#include "stdafx.h"
#include "caLua.h"
#include "script.h"
#include "scene.h"
#include "GameApp.h"
#include "Character.h"
//#include "SelChaScene.h"
#include "CharacterRecord.h"

int GetChaPhotoTexID(int nTypeID)
{
	CChaRecord *pInfo = GetChaRecordInfo(nTypeID);
    if(pInfo)
	{
    	char szPhoto[80] = { 0 }; 
		sprintf(szPhoto, "texture/photo/%s.bmp", pInfo->szIconName);
		return GetTextureID(szPhoto);
	}
	return 0;
}

int CH_Create(int sceneid, int type)
{
	CGameScene * s =  dynamic_cast<CGameScene*>(CScript::GetScriptObj(sceneid));
	if( !s ) return R_FAIL;

	CCharacter* c = s->AddCharacter(type);
	if( c ) return c->GetScriptID();

	return R_FAIL;
}

int CH_SetPos(int id, int x, int y )
{
	CCharacter* c = dynamic_cast<CCharacter*>( CScript::GetScriptObj(id) );
	if( !c ) return R_FAIL;

	c->setPos( x, y );
	return R_OK;
}

int CH_SetYaw(int id, int yaw )
{
	CCharacter* c = dynamic_cast<CCharacter*>( CScript::GetScriptObj(id) );
	if( !c ) return R_FAIL;

	c->setYaw( yaw );
	return R_OK;
}

int CH_PlayPos( int id, int pose, int posetype )
{
	CCharacter* c = dynamic_cast<CCharacter*>( CScript::GetScriptObj(id) );
	if( !c ) return R_FAIL;

	c->PlayPose( pose, posetype );
	return R_OK;
}
//---------------------------------------------------------------------------
// ScriptRegedit
//---------------------------------------------------------------------------
void MPInitLua_Cha()
{
	CLU_RegisterFunction("CH_Create", "int", "int,int", CLU_CDECL, CLU_CAST(CH_Create));	
	CLU_RegisterFunction("CH_SetPos", "int", "int,int,int", CLU_CDECL, CLU_CAST(CH_SetPos));	
	CLU_RegisterFunction("CH_SetYaw", "int", "int,int", CLU_CDECL, CLU_CAST(CH_SetYaw));	
	CLU_RegisterFunction("CH_PlayPos", "int", "int,int,int", CLU_CDECL, CLU_CAST(CH_PlayPos));	
}
