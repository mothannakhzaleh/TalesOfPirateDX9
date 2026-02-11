#pragma once

#include "TableData.h"
class CSceneItemInfo : public CRawDataInfo
{

public:

	CSceneItemInfo()
	{
	   strcpy(szName, g_oLangRec.GetString(351));
	   nPhotoTexID     = 0;
	   nAttachEffectID = 0;
    }
	char	szName[16];
	int		nPhotoTexID;
	int		nAttachEffectID;
    int     nCharacterID;
    BYTE    btWeaponType;  
    //  武器类型
    //  0   单手近战武器(同徒手共用)	
    //  1   双手近战武器	
    //  2   手持两把近战武器	
    //  3   单手持枪械武器	
    //  4   双手持弓箭类武器

    //int     nShipLinkID;
    //int     nItemLinkID;
};


class CSceneItemSet : public CRawDataSet
{

public:
	
	static CSceneItemSet* I() { return _Instance; }
	
	CSceneItemSet(int nIDStart, int nIDCnt)
	:CRawDataSet(nIDStart, nIDCnt)
	{
	    _Instance = this;
		_Init();
	}

protected:

	static CSceneItemSet* _Instance; // 相当于单键, 把自己记住
   
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSceneItemInfo[nCnt];
	}
	
	virtual void _DeleteRawDataArray()
	{
		delete[] (CSceneItemInfo*)_RawDataArray;
	}
	
	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSceneItemInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}
	
	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList)
	{   
        if(ParamList.size()==0) return FALSE;
		
        CSceneItemInfo *pInfo = (CSceneItemInfo*)pRawDataInfo;
		
		strncpy(pInfo->szName, ParamList[0].c_str(),sizeof(pInfo->szName));

        pInfo->btWeaponType = (BYTE)(Str2Int( ParamList[1]));

        pInfo->nCharacterID = Str2Int( ParamList[2] );
        
        //pInfo->nShipLinkID = Str2Int( ParamList[2] );
        //pInfo->nItemLinkID = Str2Int( ParamList[3] );

        //pInfo->nAttachEffectID = Str2Int(ParamList[2]);
		
		char szPhoto[72]; sprintf(szPhoto, "texture/photo/%s.bmp", pInfo->szName);
		pInfo->nPhotoTexID = GetTextureID(szPhoto);

        LG("sceneobj", "Read ItemInfo Model = [%s], Name = [%s],  Type = %d\n", pInfo->szDataName, pInfo->szName, pInfo->btWeaponType);
        return TRUE;
    }
};

inline CSceneItemInfo* GetSceneItemInfo(int nTypeID)
{
	return (CSceneItemInfo*)CSceneItemSet::I()->GetRawDataInfo(nTypeID);
}

inline int GetSceneItemPhotoTexID(int nTypeID)
{
	CSceneItemInfo *pInfo = GetSceneItemInfo(nTypeID);
	if(pInfo)
	{
		return pInfo->nPhotoTexID;
	}
	return 0;
}

