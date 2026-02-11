// CharForge.h Created by knight-gongjian 2005.1.24.
//---------------------------------------------------------
#pragma once

#ifndef _CHARFORGE_H_
#define _CHARFORGE_H_


#include "Character.h"

//---------------------------------------------------------
_DBC_USING

class CForgeRecordSet;

namespace mission
{
	class CForgeSystem
	{
	public:
		CForgeSystem();
		virtual ~CForgeSystem();

		void	Clear();

		// 装载精练数据信息
		BOOL	LoadForgeData( char szName[] );
		
		// 精练物品
		void	ForgeItem( CCharacter& character, BYTE byIndex );

	private:

		// 精练数据集
		CForgeRecordSet* m_pRecordSet;
	};

}

extern mission::CForgeSystem g_ForgeSystem;

//---------------------------------------------------------

#endif // _CHARFORGE_H_