//=============================================================================
// FileName: SkillState.h
// Creater: ZhangXuedong
// Date: 2005.01.13
// Comment: Skill State — компактное хранение (deque/map вместо массивов 255)
//=============================================================================

#pragma once

#include "GameAppNet.h"
#include "SkillStateType.h"
#include "CompCommand.h"
#include "SkillStateRecord.h"
#include <cstdint>
#include <cstring>
#include <deque>
#include <unordered_map>
#include <vector>

struct SSkillStateUnit
{
	unsigned char	uchReverseID;
	unsigned char	uchStateID;
	unsigned char	uchStateLv;

	char			chCenter;
	unsigned char	uchFightID;		// id скилла-источника для клиента
	char			chObjType;
	char			chObjHabitat;
	char			chEffType;
	unsigned long	ulSrcWorldID;
	long			lSrcHandle;

	long			lOnTick;
	unsigned long	ulStartTick;	// мс
	unsigned long	ulLastTick;		// мс

	unsigned char	GetStateID() { return uchStateID; }
	unsigned char	GetStateLv() { return uchStateLv; }
};

class CSkillState
{
public:
	CSkillState(unsigned char uchMaxState = AREA_STATE_MAXID)
	{
		Init(uchMaxState);
	}

	CSkillState(const CSkillState&) = delete;
	CSkillState& operator=(const CSkillState&) = delete;
	CSkillState(CSkillState&&) = delete;
	CSkillState& operator=(CSkillState&&) = delete;

	void	Init(unsigned char uchMaxState = AREA_STATE_MAXID);
	bool	Add(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
			unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter = 0);
	bool	Del(unsigned char uchStateID);
	void	Reset(void);
	bool	NeedResetState(unsigned char uchStateID);
	bool	HasState(unsigned char uchStateID, unsigned char uchStateLv);
	bool	HasState(unsigned char uchStateID);
	SSkillStateUnit*	GetSStateByID(unsigned char uchStateID);
	SSkillStateUnit*	GetSStateByNum(unsigned char uchNum);
	unsigned char		GetStateNum(void) { return static_cast<unsigned char>(_ordered.size()); }
	unsigned char		GetReverseID(unsigned char uchStateID);

	void	SetChangeFlag();
	void	ResetChangeFlag();
	void	SetChangeBitFlag(long lBit);
	bool	GetChangeBitFlag(long lBit);
	unsigned char	GetChangeNum(void) { return m_uchChangeNum; }

	void	BeginGetState(void) { m_uchCurGetNo = GetStateNum(); }
	SSkillStateUnit*	GetNextState(void);

	bool	WriteState(WPACKET& pk);

private:
	void	FillUnit(SSkillStateUnit* pUnit, unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle,
			char chObjType, char chObjHabitat, char chEffType,
			unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chWithCenter);

	unsigned char	m_uchMaxState;

	// Владение юнитами; указатели стабильны при push_back (не erase из середины)
	std::deque<SSkillStateUnit> _states;
	// O(1) lookup по state id (включая tombstone с uchStateLv==0 после Del)
	std::unordered_map<unsigned char, SSkillStateUnit*> _byId;
	// Порядок для GetSStateByNum / WriteState / BeginGetState
	std::vector<SSkillStateUnit*> _ordered;

	char			m_szChangeFlag[SSTATE_SIGN_BYTE_NUM];
	unsigned char	m_uchChangeNum;
	unsigned char	m_uchCurGetNo;
};

inline void CSkillState::FillUnit(SSkillStateUnit* pUnit, unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle,
		char chObjType, char chObjHabitat, char chEffType,
		unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chWithCenter)
{
	pUnit->chCenter = chWithCenter;
	pUnit->uchFightID = uchFightID;
	pUnit->ulSrcWorldID = ulSrcWorldID;
	pUnit->lSrcHandle = lSrcHandle;
	pUnit->chObjType = chObjType;
	pUnit->chObjHabitat = chObjHabitat;
	pUnit->chEffType = chEffType;
	pUnit->uchStateID = uchStateID;
	pUnit->uchStateLv = uchStateLv;
	pUnit->ulStartTick = ulStartTick;
	pUnit->ulLastTick = ulStartTick;
	pUnit->lOnTick = lOnTick;
}

inline void CSkillState::Init(unsigned char uchMaxState)
{T_B
	if (uchMaxState > SKILL_STATE_MAXID) {
		uchMaxState = SKILL_STATE_MAXID;
	}
	m_uchMaxState = uchMaxState;
	_states.clear();
	_byId.clear();
	_ordered.clear();
	ResetChangeFlag();
T_E}

inline void CSkillState::Reset(void)
{
	for (SSkillStateUnit* pUnit : _ordered) {
		pUnit->uchStateLv = 0;
	}
	_ordered.clear();
	ResetChangeFlag();
}

inline bool CSkillState::Add(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chObjType, char chObjHabitat, char chEffType,
							 unsigned char uchStateID, unsigned char uchStateLv, unsigned long ulStartTick, long lOnTick, char chType, char chWithCenter)
{T_B
	if (uchStateID < 1 || uchStateID > m_uchMaxState) {
		return false;
	}
	if (uchStateLv <= 0) {
		return false;
	}

	SSkillStateUnit* pExisting = nullptr;
	const auto it = _byId.find(uchStateID);
	if (it != _byId.end()) {
		pExisting = it->second;
	}

	if (!pExisting || pExisting->uchStateLv == 0) {
		if (_ordered.size() >= m_uchMaxState) {
			return false;
		}

		if (!pExisting) {
			SSkillStateUnit unit{};
			unit.uchStateID = uchStateID;
			unit.uchStateLv = 0;
			_states.push_back(unit);
			pExisting = &_states.back();
			_byId[uchStateID] = pExisting;
		}

		FillUnit(pExisting, uchFightID, ulSrcWorldID, lSrcHandle, chObjType, chObjHabitat, chEffType,
				uchStateID, uchStateLv, ulStartTick, lOnTick, chWithCenter);
		pExisting->uchReverseID = static_cast<unsigned char>(_ordered.size());
		_ordered.push_back(pExisting);

		SetChangeBitFlag(uchStateID);
	} else if (chType == enumSSTATE_ADD_EQUALORLARGER) {
		if (uchStateLv < pExisting->uchStateLv) {
			return false;
		}

		FillUnit(pExisting, uchFightID, ulSrcWorldID, lSrcHandle, chObjType, chObjHabitat, chEffType,
				uchStateID, uchStateLv, ulStartTick, lOnTick, chWithCenter);
		SetChangeBitFlag(uchStateID);
	} else if (chType == enumSSTATE_ADD_LARGER) {
		if (uchStateLv <= pExisting->uchStateLv) {
			return false;
		}

		FillUnit(pExisting, uchFightID, ulSrcWorldID, lSrcHandle, chObjType, chObjHabitat, chEffType,
				uchStateID, uchStateLv, ulStartTick, lOnTick, chWithCenter);
		SetChangeBitFlag(uchStateID);
	} else if (chType == enumSSTATE_NOTADD) {
		return false;
	} else if (chType == enumSSTATE_ADD) {
		FillUnit(pExisting, uchFightID, ulSrcWorldID, lSrcHandle, chObjType, chObjHabitat, chEffType,
				uchStateID, uchStateLv, ulStartTick, lOnTick, chWithCenter);
		SetChangeBitFlag(uchStateID);
	}

	return true;
T_E}

inline bool CSkillState::Del(unsigned char uchStateID)
{T_B
	if (uchStateID < 1 || uchStateID > m_uchMaxState || _ordered.empty()) {
		return false;
	}

	const auto it = _byId.find(uchStateID);
	if (it == _byId.end() || it->second->uchStateLv == 0) {
		return true;
	}

	SSkillStateUnit* pUnit = it->second;
	const unsigned char revId = pUnit->uchReverseID;
	const unsigned char stateNum = GetStateNum();

	if (m_uchCurGetNo == stateNum) {
		m_uchCurGetNo--;
	}

	const unsigned char last = static_cast<unsigned char>(stateNum - 1);
	if (revId < last) {
		_ordered[revId] = _ordered[last];
		_ordered[revId]->uchReverseID = revId;
	}
	_ordered.pop_back();
	pUnit->uchStateLv = 0;
	pUnit->uchReverseID = static_cast<unsigned char>(_ordered.size());

	SetChangeBitFlag(uchStateID);
	return true;
T_E}

inline SSkillStateUnit* CSkillState::GetSStateByID(unsigned char uchStateID)
{T_B
	if (uchStateID < 1 || uchStateID > m_uchMaxState) {
		return nullptr;
	}

	const auto it = _byId.find(uchStateID);
	if (it == _byId.end() || it->second->uchStateLv == 0) {
		return nullptr;
	}
	return it->second;
T_E}

inline SSkillStateUnit* CSkillState::GetSStateByNum(unsigned char uchNum)
{T_B
	if (uchNum >= _ordered.size()) {
		return nullptr;
	}

	SSkillStateUnit* pUnit = _ordered[uchNum];
	if (pUnit && pUnit->uchStateLv > 0) {
		return pUnit;
	}
	return nullptr;
T_E}

inline unsigned char CSkillState::GetReverseID(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) {
		return SKILL_STATE_MAXID;
	}

	const auto it = _byId.find(uchStateID);
	if (it == _byId.end() || it->second->uchStateLv <= 0) {
		return SKILL_STATE_MAXID;
	}

	return it->second->uchReverseID;
}

inline bool CSkillState::NeedResetState(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) {
		return false;
	}

	const auto it = _byId.find(uchStateID);
	if (it == _byId.end() || it->second->uchStateLv <= 0) {
		return false;
	}

	if (it->second->lOnTick == -1) {
		return true;
	}

	return false;
}

inline bool CSkillState::HasState(unsigned char uchStateID, unsigned char uchStateLv)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) {
		return false;
	}

	const auto it = _byId.find(uchStateID);
	if (it == _byId.end() || it->second->uchStateLv <= 0 || it->second->uchStateLv != uchStateLv) {
		return false;
	}

	return true;
}

inline bool CSkillState::HasState(unsigned char uchStateID)
{
	if (uchStateID < 1 || uchStateID > m_uchMaxState) {
		return false;
	}

	const auto it = _byId.find(uchStateID);
	if (it == _byId.end() || it->second->uchStateLv <= 0) {
		return false;
	}

	return true;
}

inline void CSkillState::SetChangeFlag()
{T_B
	memset(m_szChangeFlag, 0xff, SSTATE_SIGN_BYTE_NUM);
	m_uchChangeNum = SKILL_STATE_MAXID;
T_E}

inline void CSkillState::ResetChangeFlag()
{T_B
	memset(m_szChangeFlag, 0, SSTATE_SIGN_BYTE_NUM);
	m_uchChangeNum = 0;
T_E}

inline void CSkillState::SetChangeBitFlag(long lBit)
{T_B
	if (lBit > m_uchMaxState) {
		return;
	}

	const short sByteNO = static_cast<short>(lBit / 8);
	const short sBitNO = static_cast<short>(lBit % 8);
	const char chSetFlag = static_cast<char>(0x01 << sBitNO);

	if (!(m_szChangeFlag[sByteNO] & chSetFlag)) {
		m_uchChangeNum++;
	}

	m_szChangeFlag[sByteNO] |= chSetFlag;
T_E}

inline bool CSkillState::GetChangeBitFlag(long lBit)
{T_B
	if (lBit > m_uchMaxState) {
		return false;
	}

	const short sByteNO = static_cast<short>(lBit / 8);
	const short sBitNO = static_cast<short>(lBit % 8);

	return (m_szChangeFlag[sByteNO] & (0x01 << sBitNO)) ? true : false;
T_E}

inline SSkillStateUnit* CSkillState::GetNextState(void)
{
	if (m_uchCurGetNo <= GetStateNum()) {
		return GetSStateByNum(--m_uchCurGetNo);
	}
	return nullptr;
}

inline bool CSkillState::WriteState(WPACKET& pk)
{
	const unsigned char stateNum = GetStateNum();
	WRITE_CHAR(pk, stateNum);

	if (stateNum <= 0) {
		return false;
	}

	for (unsigned char j = 0; j < stateNum; j++) {
		SSkillStateUnit* pUnit = _ordered[j];
		CSkillStateRecord* pCStateRec = GetCSkillStateRecordInfo(pUnit->GetStateID());
		if (pCStateRec->IsShowCenter == 1 && pUnit->chCenter == 0) {
			WRITE_CHAR(pk, 0);
			continue;
		}
		WRITE_CHAR(pk, pUnit->uchStateID);
		WRITE_CHAR(pk, pUnit->uchStateLv);
		WRITE_LONG(pk, pUnit->ulSrcWorldID);
		WRITE_CHAR(pk, pUnit->uchFightID);
	}

	return true;
}
