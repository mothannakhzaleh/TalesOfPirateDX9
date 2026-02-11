//----------------------------------------------------------------------
// ����:�����
// ����:lh 2004-07-08
// ���˼��:��֧������,ͼ��,ͼ�󲿷�δ���
// ����޸�����:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiCompent.h"
#include "uiForm.h"

namespace GUI
{

class CTextButton;
class CEdit : public CCompent
{
public:
	CEdit(CForm& frmOwn);
	CEdit( const CEdit& rhs );
	CEdit& operator=(const CEdit& rhs);
	virtual ~CEdit(void);
	GUI_CLONE(CEdit)

	virtual void	Init();
	virtual void	Refresh();
	virtual void	SetAlpha( BYTE alpha );
	virtual void	OnActive();
	virtual void	OnLost();

	// ��������Ϣѭ���У����ռ�����Ϣ
	bool OnKeyDown( int key );		
	bool OnChar( char c );

public:	// Get,Set
	static MPTexRect* GetCursorImage()	{ return &_CursorImage;		}

	CGuiPic*		GetImage() { return _pImage; }

	bool			GetIsPassWord()		{ return _bIsPassWord;		}
	bool            GetIsParseText()    { return _bParseText ;     } 
	bool			GetIsDigit()		{ return _bIsDigit;			}
	bool			GetIsMulti()		{ return _bIsMulti;			}
	bool			GetIsWrap()			{ return _bIsWrap;			}

	void			SetIsMulti( bool v ){ _bIsMulti = v;			}
	void			SetIsPassWord( bool v ) { _bIsPassWord = v;		}
	void            SetIsParseText(bool  v ) { _bParseText = v;     } 
	void			SetIsDigit( bool v ){ _bIsDigit = v;			}
	void			SetIsWrap( bool v ) { _bIsWrap = v;				}

	int				GetFontHeight()		{ return _nFontHeight;		}
	void			SetFontHeight( bool v ) { _nFontHeight = v;		}

	int				GetMaxNum()			{ return _nMaxNum;			}
	void			SetMaxNum( int v )	{ _nMaxNum = v;				}

	int             GetMaxVisible()     {return  _nMaxNumVisible;   }
	void            SetMaxNumVisible(int v) { _nMaxNumVisible = v;     }

	int				GetMaxLineNum()		{ return _nMaxLineNum;		}
	void			SetMaxLineNum( int v );
	void			SetCaption( const char * str );
	const char *	GetCaption()		{ return _str.c_str();		}
	void			ReplaceSel( const char * str, BOOL bCanUndo=TRUE );

	virtual void	SetTextColor( DWORD color );
	DWORD			GetTextColor()		{ return _color;			}

	void			SetCursorColor( DWORD  v )	{ _nCursorColor = v;}

	void			SetEnterButton( CTextButton* pButton )	{ _pEnterButton=pButton;	}

public:
	GuiEvent			evtEnter;		// ���ı����س�ʱִ�е��¼�
	GuiKeyDownEvent		evtKeyDown;
	GuiKeyCharEvent		evtKeyChar;
    GuiEvent            evtChange;      // caption�����仯

public:
	void			Render();
	static bool		InitCursor( const char* szFile );

	static int		GetCursorX()	{ return _nCursorX;		}
	static int		GetCursorY()	{ return _nCursorY;		}

	void			RefreshText();
	void			RefreshCursor();
	void			ClearText();

protected:
	bool			_IsCursorInHZ( long l, char * s );		// �жϹ���Ƿ���һ�������м�
	void			ShowFocus();		// ��ʾ���
	void			CorrectCursor();

	// ��������йصĲ���
	void			_Copy();	
	void			_Paste();

	void			_Cut();
	void			_Delete();			// ɾ����ѡ����ַ���

	void			_UpdataLines();		// ���µ�����ʾ���֣�����Ӳ�س������س�

	bool			_isdigit( char c )	{ return (c>='0' && c<='9') || c==VK_BACK || c==VK_RETURN || c==VK_DELETE;	}

private:
	// void		_RefreshCursorPos();	// ���ݹ���������У���������ʾ��λ��
	void        _GetCursorPos(int nCurPos);       
	void		_Copy( const CEdit& rhs );

private:
	static MPTexRect	_CursorImage;	// �����ͼ��Ϣ

	static int			_nCursorFlashCount;
	static bool			_bCursorIsShow;
	static int	_nCursorX, _nCursorY;	// ���Ӧ����ʾ��λ��

	CGuiPic*			_pImage;

	DWORD				_color;

	int					_nLeftMargin;
	int					_nTopMargin;

	bool		        _bParseText;	// �Ƿ���Ҫ����ͼԪ

	CTextButton*		_pEnterButton;	// ��Ӧ�س��İ�ť

protected:
	std::string		_str;
	std::string      _strVisible ; 

	int			_nMaxNum;			// �������
	int         _nMaxNumVisible ;      //�ؼ�һ�ο������ɵ��ַ���Ŀ

	bool		_bIsPassWord;		// �Ƿ�������ʾ
	bool		_bIsMulti;			// �Ƿ��������
	bool		_bIsDigit;			// �Ƿ������������
	bool		_bIsWrap;
	int			_nOffset;

protected:		// �����ڲ�����	
	int			_nFontHeight;		// �иߣ����������еĸ߶�
	int			_nMaxLineNum;		// �������������Ӳ�س������س�

protected:		// ��궨λ
	int			_nCursorRow;		// ��������
	int			_nCursorCol;		// ��������,�ӵ�0�е�length������
	int			_nCursorFirstCol;
	int			_nCursorSecondCol;

	DWORD		_nCursorColor;
	DWORD		_nCursorHeight;

};

// ��������
inline void	CEdit::SetMaxLineNum( int v ) 
{ 
	if( v > 1 ) {
		_nMaxLineNum = v;
		SetIsMulti( true );
	}
}

}
