#pragma once

class CGameScene;
class CSceneItem;
class CCharacter;
class CShadeEff;

// 场景的用户标志
class CSceneSign
{
public:
	CSceneSign();
	~CSceneSign();

	bool	Init( CGameScene* pScene );
	void	Show( int x, int y, int h=0 );
	void	Show( CCharacter* pCha );
	void	Hide();
	void	FrameMove( DWORD dwTime );
	void	ChaDisappear( CCharacter* pCha );

	bool	IsShowMove()				{ return _eStyle==enumMove;	}

private:
	void	Reset( CSceneItem* pItem );

private:
	enum eStyle
	{
		enumNone,
		enumAttack,
		enumMove,
	};
	eStyle			_eStyle;

	CGameScene*		_pScene;

	CSceneItem*		_pAttack;		// 攻击时显示的光环
	CShadeEff*		_pAttackShade;
	CSceneItem*		_pMove;         // 移动或拾取时显示的光环
	CCharacter*		_pCha;			// 被攻击的角色

	int	_nX, nY, _nH;	

};

inline void	CSceneSign::ChaDisappear( CCharacter* pCha )	
{ 
	if( _eStyle==enumAttack && pCha==_pCha ) 
	{
		Hide();
	}
}
