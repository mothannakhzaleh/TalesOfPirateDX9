//=============================================================================
// FileName: Kitbag.h
// Creater: ZhangXuedong
// Date: 2004.12.17
// Comment: Kitbag — слоты в vector по capacity (не фиксированный массив)
//=============================================================================

#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include "ItemRecord.h"
#include "CompCommand.h"
//#include "KitbagEnCode.h"



extern char g_key[9];
extern int Decrypt(char* buf, int len, const char* enc, int elen);

extern int Encrypt(char* buf, int len, const char* pwd, int plen);


enum EKitbagItemType
{
	enumKBITEM_TYPE_ORD,	// обычные предметы

	enumKBITEM_TYPE_NUM		// число типов
};

enum ITEM_STATE 
{	
	ITEM_DISENABLE			= 1<<0,	 // слот/предмет запрещён для операций
};

enum EKbActRet
{
	enumKBACT_SUCCESS				= 0,	// успех
	// коды ошибок операций с инвентарём
	enumKBACT_ERROR_LOCK			= -1,	// инвентарь заблокирован
	enumKBACT_ERROR_RANGE			= -2,	// выход за границы
	enumKBACT_ERROR_PUSHITEMID		= -3,	// некорректный id предмета
	enumKBACT_ERROR_FULL			= -4,	// нет места
	enumKBACT_ERROR_NULLGRID		= -5,	// пустой слот
	enumKBACT_ERROR_POPNUM			= -6,	// некорректное количество
};

#define defKITBAG_DEFPUSH_POS	-1 // автопоиск свободного слота при push

class CKitbag
{
public:
	CKitbag();

	struct SItemUnit
	{
		BYTE		byState;	// состояние слота (ITEM_DISENABLE и т.п.)
		SItemGrid	SContent;

		short		sPosID;		// индекс слота
		short		sReverseID;	// индекс в упорядоченном списке m_pSItem
	};

	void		Init(short sCapacity = defMAX_KBITEM_NUM_PER_TYPE);
	void		Reset(void);
	void		SetCapacity(short sCapacity);
	short		GetCapacity();
	bool		AddCapacity(short sAddVal);
	short		GetUseGridNum(short sType = 0);
	
	short		CanPush(SItemGrid *pGrid, short &sPosID, short sType = 0);
	short		CanPop(SItemGrid *pGrid, short sPosID, short sType = 0);

	short		Push(SItemGrid *pGrid, short &sPosID, short sType = 0, bool bCommit = true, bool bSureOpr = false);	
	short		Pop(SItemGrid *pGrid, short sPosID, short sType = 0, bool bCommit = true);
	short		Clear(short sPosID, short sType = 0);
	short		Clear(SItemGrid *pGrid, short sNum = 0, short *psPosID = NULL);
	short		Switch(short sSrcPosID, short sTarPosID, short sType = 0);
	short		Regroup(short sSrcPosID, short sSrcNum, short sTarPosID, short sType = 0);
	short		Refresh(short sPosID, short sType = 0);
	bool		IsFull(short sType = 0);
	SItemGrid*	GetGridContByID(short sPosID, short sType = 0);
	SItemGrid*	GetGridContByNum(short sPosNum, short sType = 0);
	short		GetPosIDByNum(short sPosNum, short sType = 0);
	bool		GetPosIDByGrid(SItemGrid *pGrid, short *psPosID = 0, short *psType = 0);
	short		GetID(short sPosID, short sType = 0);
	short		GetNum(short sPosID, short sType = 0);
	long		GetDBParam(short sParamID, short sPosID, short sType = 0);
	short		SetDBParam(short sParamID, long lParamVal, short sPosID, short sType = 0);
	short		GetEnergy(bool bMax, short sPosID, short sType = 0);
	short		SetEnergy(bool bMax, short sEnergy, short sPosID, short sType = 0);
	BOOL		HasItem( short sPosID, short sType = 0 );
	BOOL		IsEnable( short sPosID, short sType = 0 );
	void		Enable( short sPosID, short sType = 0 );
	void		Disable( short sPosID, short sType = 0 );
	BOOL		IsLock(void);

	void		SetChangeFlag(bool bChange = true, short sType = 0);
	void		SetSingleChangeFlag(short sPosID, short sType = 0);
	bool		IsSingleChange(short sPosID, short sType = 0);
	bool		IsChange(short sType = 0);
	short		GetChangeNum(short sType = 0);

	void		Lock();
	void		UnLock();

	void		PwdLock();
	void		PwdUnlock();
	BOOL		IsPwdLocked();
	BOOL		IsPwdAutoLocked();
	void		PwdAutoLock(char cAuto);
	int			GetPwdLockState();
	void		SetPwdLockState(int nLock);

	void		SetVer(short sVers);
	short		GetVer(void);

	CKitbag&	operator = ( CKitbag& bag );

protected:
	bool		CheckValid(void); // for test

private:
	void		RebuildPointers(short sType);
	void		ResizeStorage(short sCapacity);

	short		sVer = 0;
	BOOL		m_bLock = FALSE;

	// Битовые флаги пароля инвентаря
	int			m_bPwdLocked = 0;

	// Упорядоченный список указателей на слоты (занятые в начале)
	std::vector<SItemUnit*> m_pSItem[enumKBITEM_TYPE_NUM];
	short		m_sCapacity = 0;
	short		m_sUseNum[enumKBITEM_TYPE_NUM] = {};
	// Владение слотами; размер == capacity
	std::vector<SItemUnit> m_SItem[enumKBITEM_TYPE_NUM];

	short		m_sChangeNum[enumKBITEM_TYPE_NUM] = {};
	std::vector<std::uint8_t> m_bChangeFlag[enumKBITEM_TYPE_NUM];

};

//=============================================================================
char* KitbagData2String(CKitbag *pKitbag, char *szStrBuf, int nLen);

bool String2KitbagData(CKitbag *pKitbag, std::string &strData);

//	2008-7-28	yangyinyu	add	begin!

bool	SItemGrid2String(	
					std::string&	r,
					long&			lnCheckSum,
					SItemGrid*		pGridCont,
					int				iOrder
					);

bool String2SItemGrid(	SItemGrid*	pGridCont,	long&	lnCheckSum,	const	std::string&	sData,	int	iVer,	bool	bIsOldVer	);

//	2008-7-28	yangyinyu	add	end!
