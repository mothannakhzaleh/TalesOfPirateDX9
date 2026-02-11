#pragma once
#include "MPRender.h"

#define UI_FVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FONTLEVEN	3

struct UI_VERTEX
{
	float x, y, z,w;
	DWORD diffuse;
	float tu, tv;
};



class CUIPanel
{
public:
	CUIPanel();
	~CUIPanel();
	bool			Create(IDirect3DDeviceX*  pDev);

	void			Draw( IDirect3DTextureX*  pSrcTexture, 
		CONST RECT* pSrcRect, CONST D3DXVECTOR2* pScaling, 
		CONST D3DXVECTOR2* pRotationCenter, FLOAT Rotation, 
		CONST D3DXVECTOR2* pTranslation, D3DCOLOR Color);
	void			Begin(){}
	void			End();
public:
	IDirect3DDeviceX*		m_pDev;
protected:
	IDirect3DVertexBufferX* _lpVB;

	int						_w,_h;

	int						_x,_y;
};

class UIRender
{
public:
	UIRender();
	~UIRender()	    { SAFE_RELEASE(_p2DSprite);  }

    bool Init();

	void RenderSprite(LPTEXTURE tex, RECT* rc,VECTOR2* vscale,VECTOR2* vdest, DWORD dwColor);
	void RenderTextureRect(int nX, int nY, MPTexRect *pRect);

	void RenderTextureAbsRect(int nX, int nY, MPTexRect *pRect);

	void RenderTextureRect(int nX, int nY, MPTexRect *pRect, DWORD dwColor);

	void LineFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// 画一个边框
	void SelectChaLineFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// 画一个边框
	
	void LineThinFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// 画一个细框
	void FillFrame(int left, int top, int right, int bottom, DWORD color=0x90000000 );	// 填充边框
	void SetIsConvert( bool v );
	void SetIsChangeResolution(bool v )  {      _bChangeResolution = v ;    }
	bool GetIsChangeResolution()         {      return _bChangeResolution;  }  
		
	void SetClipRect( int x, int y, int w, int h );
	void Reset();

public:	// 三层坐标：屏幕坐标，画图坐标，GUI坐标
	void SetScreen( int w, int h, bool isFull );			// 设置游戏桌面显示的宽高，当初始化或改变屏幕分辨率时调用

	// 将屏幕坐标转换为GUI坐标
	void ScreenConvert( int &x, int &y ) {
		x = (int)((float)(x) / _fScreenScaleX);
		y = (int)((float)(y) / _fScreenScaleY);
	}
    float ScreenConvertX( int x ) {     
        return (float)x / _fScreenScaleX; 
    }
    float ScreenConvertY( int y ) {    
        return (float)y / _fScreenScaleY; 
    }

	// 将画图坐标转换为GUI坐标
	void DrawConvert( int &x, int &y ) {
		x = (int)((float)(x) * _fDrawScaleX);
		y = (int)((float)(y) * _fDrawScaleY);
	}
	float DrawConvertX( float x ) {	
        return x * _fDrawScaleX; 
    }
	float DrawConvertY( float y ) { 
        return y * _fDrawScaleY; 
    }

	float DrawConvertX2( float x ) { //added by billy 
        return x / _fDrawScaleX; 
    } 
	float DrawConvertY2( float y ) { 
        return y / _fDrawScaleY; 
    }
	
	int	  GetGuiWidth()		{ return _nGuiWidth;    }
	int	  GetGuiHeight()	{ return _nGuiHeight;   }

    int   GetScreenWidth()  { return _nScrWidth;    }
    int   GetScreenHeight() { return _nScrHeight;   }

	void			OnLostDevice();
	void			OnResetDevice();

	void			RegisterFunc();

	bool			IsFullScreen()	{ return _IsFullScreen;		}

    static UIRender s_Render;

private:
	LPD3DXSPRITE	_p2DSprite;
	//CUIPanel		_p2DSprite;


	float			_fScreenScaleX, _fScreenScaleY;
	float			_fDrawScaleX, _fDrawScaleY;

	float			_fOldScreenScaleX, _fOldScreenScaleY;
	float			_fOldDrawScaleX, _fOldDrawScaleY;

	int				_nGuiWidth, _nGuiHeight;		// GUI系统宽高,即屏幕大小
	bool			_IsFullScreen;

private:
	//LPTEXTURE		_pTex;				// 纹理
    int             _nTex;				// 纹理
	int             _nOutLine ;         // 私人框
	RECT			_rtFrame;			// 纹理大小
	VECTOR2			_vesLevel;
	VECTOR2			_vesVertical;

	VECTOR2			_vesLevelThin;
	VECTOR2			_vesVerticalThin;

    int             _nScrWidth;
    int             _nScrHeight;
	bool            _bChangeResolution;

};

inline UIRender& GetRender() { return UIRender::s_Render;   }

extern bool UIGetChooseColor( DWORD &c );					// 颜色对话框,c为默认颜色,同时也作为执行成功时的返回值
extern char* UIGetOpenFileName( char* strInitDir=NULL );	// 打开文件对话框,成功返回文件名称,否则返回NULL

