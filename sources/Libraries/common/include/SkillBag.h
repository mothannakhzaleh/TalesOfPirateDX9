//=============================================================================
// FileName: SkillBag.h
// Creater: ZhangXuedong
// Date: 2005.02.17
// Comment: Skill Bag — компактное хранение (deque/map вместо массивов)
//=============================================================================

#pragma once

#include "CompCommand.h"

#include <cstdint>
#include <cstring>
#include <deque>
#include <format>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define defMAX_SKILL_NO		500
#define defMAX_SKILL_LV		20

// Тип синхронизации скилл-бэга — см. CompCommand.h, ESynSkillBagType

#define defSKILL_RANGE_PARAM_NUM	4

struct SSkillGrid
{
	char	chState;	// состояние использования — CompCommand.h, ESkillUseState
	char	chLv;		// уровень навыка
	short	sID;		// id навыка

	uint32_t	lColdDownT;	// момент начала восстановления

	short	sReverseID;	// индекс в упорядоченном списке активных навыков
};

struct SSkillGridEx // расширенная сетка для клиента / UI
{
	char	chState;	// состояние использования — CompCommand.h, ESkillUseState
	char	chLv;		// уровень навыка
	short	sID;		// id навыка
	short	sUseSP;		// расход SP
	short	sUseEndure;	// расход выносливости
	short	sUseEnergy;	// расход энергии
	long	lResumeTime;// время восстановления
	short	sRange[defSKILL_RANGE_PARAM_NUM]; // параметры радиуса
};

class CSkillBag
{
public:
	CSkillBag()
	{
		Init();
	}

	CSkillBag(const CSkillBag&) = delete;
	CSkillBag& operator=(const CSkillBag&) = delete;
	CSkillBag(CSkillBag&&) = delete;
	CSkillBag& operator=(CSkillBag&&) = delete;

	void		Init(void);
	bool		Add(SSkillGrid *pGridCont);
	bool		Del(short sSkillID);
	SSkillGrid*	GetSkillContByID(short sSkillID);
	SSkillGrid*	GetSkillContByNum(short sNum);
	short		GetSkillNum();
	bool		HasSkill(short sID);
	bool		SetState(short sID, char chState);
	char		GetState(short sID);

	void		SetChangeFlag(bool bChange = true);
	short		GetChangeSkillNum();
	SSkillGrid*	GetChangeSkill(short sChangeID);

private:
	void		MarkChanged(SSkillGrid* pGrid);

	// Владение гридами; указатели стабильны при push_back (не erase из середины)
	std::deque<SSkillGrid> _skills;
	// O(1) lookup по skill id (включая tombstone с chLv==0 после Del)
	std::unordered_map<short, SSkillGrid*> _byId;
	// Порядок для GetSkillContByNum / sync / DB-строки
	std::vector<SSkillGrid*> _ordered;
	// Change-tracking для SynSkillBag
	std::vector<SSkillGrid*> _changed;
	std::unordered_set<short> _changedIds;
};

inline void CSkillBag::MarkChanged(SSkillGrid* pGrid)
{
	if (!pGrid) {
		return;
	}
	if (_changedIds.insert(pGrid->sID).second) {
		_changed.push_back(pGrid);
	}
}

inline void CSkillBag::Init(void)
{
	_skills.clear();
	_byId.clear();
	_ordered.clear();
	SetChangeFlag(false);
}

inline void CSkillBag::SetChangeFlag(bool bChange)
{
	_changed.clear();
	_changedIds.clear();
	if (bChange) {
		for (SSkillGrid* pGrid : _ordered) {
			MarkChanged(pGrid);
		}
	}
}

inline bool CSkillBag::Add(SSkillGrid *pGridCont)
{
	if (!pGridCont) {
		return false;
	}
	if (pGridCont->sID < 0 || pGridCont->sID > defMAX_SKILL_NO) {
		return false;
	}

	if (pGridCont->chLv == 0) {
		return false;
	}

	if (static_cast<std::uint8_t>(pGridCont->chLv) > defMAX_SKILL_LV) {
		pGridCont->chLv = defMAX_SKILL_LV;
	}

	SSkillGrid* pExisting = nullptr;
	const auto it = _byId.find(pGridCont->sID);
	if (it != _byId.end()) {
		pExisting = it->second;
	}

	if (!pExisting) {
		SSkillGrid grid{};
		grid.sID = pGridCont->sID;
		grid.chLv = 0;
		grid.chState = enumSUSTATE_INACTIVE;
		grid.lColdDownT = 0;
		grid.sReverseID = 0;
		_skills.push_back(grid);
		pExisting = &_skills.back();
		_byId[pGridCont->sID] = pExisting;
	}

	if (pExisting->chLv == 0) {
		pExisting->sReverseID = static_cast<short>(_ordered.size());
		_ordered.push_back(pExisting);
	}

	if (pExisting->chState != pGridCont->chState || pExisting->chLv != pGridCont->chLv) {
		MarkChanged(pExisting);
	}

	pExisting->chState = pGridCont->chState;
	pExisting->chLv = pGridCont->chLv;
	pExisting->lColdDownT = 0;

	return true;
}

inline bool CSkillBag::Del(short sSkillID)
{
	if (sSkillID < 0 || sSkillID > defMAX_SKILL_NO) {
		return false;
	}

	const auto it = _byId.find(sSkillID);
	if (it == _byId.end() || it->second->chLv == 0) {
		return true;
	}

	SSkillGrid* pGrid = it->second;
	const short revId = pGrid->sReverseID;
	const short last = static_cast<short>(_ordered.size() - 1);

	if (revId >= 0 && revId <= last) {
		if (revId != last) {
			_ordered[static_cast<std::size_t>(revId)] = _ordered[static_cast<std::size_t>(last)];
			_ordered[static_cast<std::size_t>(revId)]->sReverseID = revId;
		}
		_ordered.pop_back();
	}

	pGrid->chLv = 0;
	pGrid->sReverseID = static_cast<short>(_ordered.size());
	MarkChanged(pGrid);

	return true;
}

inline SSkillGrid* CSkillBag::GetSkillContByID(short sSkillID)
{
	if (sSkillID < 0 || sSkillID > defMAX_SKILL_NO) {
		return nullptr;
	}

	const auto it = _byId.find(sSkillID);
	if (it == _byId.end() || it->second->chLv == 0) {
		return nullptr;
	}
	return it->second;
}

inline SSkillGrid* CSkillBag::GetSkillContByNum(short sNum)
{
	if (sNum < 0 || sNum >= static_cast<short>(_ordered.size())) {
		return nullptr;
	}

	SSkillGrid* pGrid = _ordered[static_cast<std::size_t>(sNum)];
	if (!pGrid || pGrid->chLv == 0) {
		return nullptr;
	}
	return pGrid;
}

inline short CSkillBag::GetSkillNum()
{
	return static_cast<short>(_ordered.size());
}

inline bool CSkillBag::HasSkill(short sSkillID)
{
	return GetSkillContByID(sSkillID) != nullptr;
}

inline bool CSkillBag::SetState(short sID, char chState)
{
	if (sID < 0) {
		// Применить состояние ко всем активным навыкам
		for (SSkillGrid* pGrid : _ordered) {
			if (!pGrid || pGrid->chState == chState) {
				continue;
			}
			pGrid->chState = chState;
			MarkChanged(pGrid);
		}
	}
	else {
		if (sID > defMAX_SKILL_NO) {
			return false;
		}

		const auto it = _byId.find(sID);
		if (it == _byId.end() || it->second->chLv == 0) {
			return true;
		}

		SSkillGrid* pGrid = it->second;
		if (pGrid->chState != chState) {
			pGrid->chState = chState;
			MarkChanged(pGrid);
		}
	}

	return true;
}

inline char CSkillBag::GetState(short sID)
{
	SSkillGrid* pGrid = GetSkillContByID(sID);
	if (!pGrid) {
		return enumSUSTATE_INACTIVE;
	}
	return pGrid->chState;
}

inline short CSkillBag::GetChangeSkillNum()
{
	return static_cast<short>(_changed.size());
}

inline SSkillGrid* CSkillBag::GetChangeSkill(short sChangeID)
{
	if (sChangeID < 0 || sChangeID >= static_cast<short>(_changed.size())) {
		return nullptr;
	}
	return _changed[static_cast<std::size_t>(sChangeID)];
}

//=============================================================================
inline char* SkillBagData2String(CSkillBag *pSkillBag, char *szStrBuf, int nLen)
{
	if (!pSkillBag || !szStrBuf) {
		return nullptr;
	}

	szStrBuf[0] = '\0';
	int nBufLen = 0;

	const auto append = [&](const std::string& chunk) -> bool {
		if (nBufLen + static_cast<int>(chunk.size()) >= nLen) {
			return false;
		}
		std::memcpy(szStrBuf + nBufLen, chunk.data(), chunk.size());
		nBufLen += static_cast<int>(chunk.size());
		szStrBuf[nBufLen] = '\0';
		return true;
	};

	const short sSkillNum = pSkillBag->GetSkillNum();
	if (!append(std::format("{};", sSkillNum))) {
		return nullptr;
	}

	for (short j = 0; j < sSkillNum; j++) {
		SSkillGrid* pGridCont = pSkillBag->GetSkillContByNum(j);
		if (!pGridCont) {
			return nullptr;
		}
		// Формат DB: "id,lv;"
		if (!append(std::format("{},{};", pGridCont->sID, static_cast<int>(pGridCont->chLv)))) {
			return nullptr;
		}
	}

	return szStrBuf;
}

inline bool String2SkillBagData(CSkillBag *pSkillBag, std::string &strData)
{
	if (!pSkillBag) {
		return false;
	}

	pSkillBag->Init();

	int nCount = 0;
	std::string strList[defMAX_SKILL_NO + 1];
	std::string strSubList[2];
	Util_ResolveTextLine(strData.c_str(), strList, defMAX_SKILL_NO + 1, ';');
	const auto sSkillNum = Str2Int(strList[nCount++]);
	SSkillGrid SGridCont;
	for (int j = 0; j < sSkillNum; j++) {
		Util_ResolveTextLine(strList[nCount++].c_str(), strSubList, 3, ',');
		SGridCont.sID = Str2Int(strSubList[0]);
		SGridCont.chLv = static_cast<char>(Str2Int(strSubList[1]));

		SGridCont.chState = enumSUSTATE_INACTIVE;
		pSkillBag->Add(&SGridCont);
	}

	return true;
}
