//----------------------------------------------------------------------
// 名称:输入法
// 作者:lh 2004-07-28
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiGuidata.h"

namespace GUI
{
class CImeInput
{
    static const int InputX = 120;     //  输入框宽
    static const int InputY = 170;     //  输入框高
	enum
	{
		MAX_CHAR = 256,
	};

private:	// 禁止拷贝,复制,克隆
	CImeInput& operator=(const CImeInput& rhs);
	CImeInput(const CImeInput& rhs);

	virtual bool		Clone( CGuiData* rhs )			{ return false;			}
	virtual CGuiData*	Clone()							{ return NULL;			}
	virtual void		SetAlpha( BYTE alpha );

public:
	CImeInput(void);
	~CImeInput(void);

	static	CImeInput	s_Ime;

	void		Init();
	void		Clear();
	bool		HandleWindowMsg(DWORD dwMsg, WPARAM wParam, LPARAM lParam);
	void		Render();
	void		SetShowPos( int x, int y );
	void		SetScreen( bool isFull, int w, int h );
	void		OnCreate( HWND hWnd, HINSTANCE hInst );

	void		SetSize( int w, int h )			{ _nWidth = w, _nHeight = h;	}
	int			GetWidth()						{ return _nWidth;				}
	int			GetHeight()						{ return _nHeight;				}

private:
	bool		_GetCompositionString( char* str, DWORD ImeValue );
	bool		_GetCandidateList();
    bool        _GetConversion();

private:
	CGuiPic*	_pImage;
	DWORD		_ImmNameColor;
	int			_nShowX;
	int			_nShowY;
	bool		_bIsShow;

private:
	bool		_bIsFull;
	int			_nScreenWidth;
	int			_nScreenHeight;
    unsigned long _lConversion;

private:
	char		_strImmName[7];
	char		_strComposition[MAX_CHAR];
	char		_strCandidate[MAX_CHAR];
    char        _strInput[5];
    char        _strSBC[5];
    char        _strInterpunction[7];

	HIMC		_hImc;		// 输入句柄
	HIMC		_oldImc;
	LPCANDIDATELIST		_pList;			// 预选字列表

	int			_nWidth, _nHeight;		// 宽高

};

// 内联函数
inline void	CImeInput::OnCreate( HWND hWnd, HINSTANCE hInst )
{
	HIMC hImc = ::ImmCreateContext();
	//::ImmAssociateContextEx( hWnd, hImc, IACE_IGNORENOCONTEXT );
	//::CreateWindow( "Ime", "", WS_POPUP | WS_DISABLED, 0, 0, 0, 0, hWnd, NULL, hInst, NULL );
}

}
