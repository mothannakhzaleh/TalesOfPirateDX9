#include "stdafx.h"
#include "caLua.h"
#include "script.h"
#include "scene.h"
#include "GameApp.h"
#include "ChaClientAttr.h"
#include "WorldScene.h"

SClientAttr g_ClientAttr[2000];

SCameraMode CameraMode[4];

//---------------------------------------------------------------------------
// Scene_Script
//---------------------------------------------------------------------------
int SN_CreateScene( int type, char* name, char* map_name, int ui, int max_cha, int max_obj, int max_item, int max_eff )
{
	if( type>=0 && type<enumSceneEnd )
	{
		stSceneInitParam param;
		param.nTypeID = type;
		param.strName = name;
		param.strMapFile = map_name;
		param.nUITemplete = ui;
		param.nMaxCha = max_cha;
		param.nMaxObj = max_obj;
		param.nMaxItem = max_item;
		param.nMaxEff = max_eff;

		CGameScene* s = g_pGameApp->CreateScene( &param );
		
		if( s ) return s->GetScriptID();
	}
	return R_FAIL;
}

int SN_SetIsShow3DCursor( BOOL isShow )
{
	g_pGameApp->GetCursor()->SetIsVisible( isShow==TRUE );
	return R_OK;
}

int SN_SetTerrainShowCenter( int sceneid, int x, int y )
{
	CGameScene * p =  dynamic_cast<CGameScene*>(CScript::GetScriptObj(sceneid));
	if( !p ) return R_FAIL;
	
    MPTerrain *t = p->GetTerrain();
	if( t ) 
	{
		t->SetShowCenter( (float)x, (float)y );
		return R_OK;
	}

	return R_FAIL;
}

int SN_SetIsShowMinimap( int isShow )
{
	CGameScene::ShowMinimap( isShow );
	return R_OK;
}

int SN_SetAttackChaColor( int r, int g, int b )
{
	CWorldScene::SetAttackChaColor( r, g, b );
	return R_OK;
}

int CHA_SetClientAttr(int nScriptID, int nAngle, float fDis, float fHei)
{
	SClientAttr *pAttr = &g_ClientAttr[nScriptID];
	pAttr->sTeamAngle  = (short)nAngle;
	pAttr->fTeamDis    = fDis;
	pAttr->fTeamHei    = fHei;
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//C_NORMAL    = 0  --正常模式   
//C_NEAR      = 1  --偏近模式  距离比正常模式偏近
//C_HIGHSPEED = 2  --高速模式  固定镜头, 不可旋转
//C_SHIP      = 3  --海上模式  在船上, 距离很远
//
//CameraRangeXY(C_NORMAL, 40, 45)
//CameraRangeZ(C_NORMAL, 25, 35)
//CameraRangeFOV(C_NORMAL, 17, 20)
//CameraEnableRotate(C_NORMAL, 0)
//CameraShowSize(38, 38)
//
//CameraRangeXY(C_NEAR, 40, 45)
//CameraRangeZ(C_NEAR, 25, 35)
//CameraRangeFOV(C_NEAR, 17, 20)
//CameraEnableRotate(C_NEAR, 0)
//CameraShowSize(34, 34)
//
//CameraRangeXY(C_HIGHSPEED, 40, 45)
//CameraRangeZ(C_HIGHSPEED, 25, 35)
//CameraRangeFOV(C_HIGHSPEED, 17, 20)
//CameraEnableRotate(C_HIGHSPEED, 1)
//CameraShowSize(30, 30)
//
//CameraRangeXY(C_SHIP, 40, 45)
//CameraRangeZ(C_SHIP, 25, 35)
//CameraRangeFOV(C_SHIP, 17, 20)
//CameraEnableRotate(C_SHIP, 0)
//CameraShowSize(45, 45)

//scripts/cameraconf.clu
//对应c的全局变量 CameraMode[].xxxx

int CameraRangeXY(int nMode, float fMin, float fMax)
{
	CameraMode[nMode].m_fminxy = fMin;
	CameraMode[nMode].m_fmaxxy = fMax;
	return 0;

}

int CameraRangeZ(int nMode, float fMin, float fMax)
{
	CameraMode[nMode].m_fminHei = fMin;
	CameraMode[nMode].m_fmaxHei = fMax;
	return 0;

}

int CameraRangeFOV(int nMode, float fMin, float fMax)
{
	CameraMode[nMode].m_fminfov = fMin;
	CameraMode[nMode].m_fmaxfov = fMax;
	return 0;

}

int CameraEnableRotate(int nMode, int nEnable)
{
	CameraMode[nMode].m_bRotate = nEnable;
	return 0;

}

int CameraShowSize(int nMode, int w, int h)
{
	CameraMode[nMode].m_nShowWidth  = w;
	CameraMode[nMode].m_nShowHeight = h;
	return 0;

}

int CameraShowSize1024(int nMode, int w, int h)
{
	CameraMode[nMode].m_nShowWidth  = w;
	CameraMode[nMode].m_nShowHeight = h;
	return 0;
}

//---------------------------------------------------------------------------
// ScriptRegedit
//---------------------------------------------------------------------------
void MPInitLua_Scene()
{
	CLU_RegisterFunction("SN_CreateScene", "int", "int,char*,char*,int,int,int,int,int", CLU_CDECL, CLU_CAST(SN_CreateScene));	
	CLU_RegisterFunction("SN_SetTerrainShowCenter", "int", "int,int,int", CLU_CDECL, CLU_CAST(SN_SetTerrainShowCenter));	
	CLU_RegisterFunction("SN_SetIsShow3DCursor", "int", "int", CLU_CDECL, CLU_CAST(SN_SetIsShow3DCursor));
	CLU_RegisterFunction("SN_SetIsShowMinimap", "int", "int", CLU_CDECL, CLU_CAST(SN_SetIsShowMinimap));
	CLU_RegisterFunction("SN_SetAttackChaColor", "int", "int,int,int", CLU_CDECL, CLU_CAST(SN_SetAttackChaColor));

	CLU_RegisterFunction("CHA_SetClientAttr", "int", "int,int,float,float", CLU_CDECL, CLU_CAST(CHA_SetClientAttr));
	CLU_RegisterFunction("CameraRangeXY", "int", "int,float,float", CLU_CDECL, CLU_CAST(CameraRangeXY));
	CLU_RegisterFunction("CameraRangeZ",  "int", "int,float,float", CLU_CDECL, CLU_CAST(CameraRangeZ));
	CLU_RegisterFunction("CameraRangeFOV","int", "int,float,float", CLU_CDECL, CLU_CAST(CameraRangeFOV));
	CLU_RegisterFunction("CameraEnableRotate", "int", "int,int", CLU_CDECL, CLU_CAST(CameraEnableRotate));
	CLU_RegisterFunction("CameraShowSize", "int", "int,int,int", CLU_CDECL, CLU_CAST(CameraShowSize));
}