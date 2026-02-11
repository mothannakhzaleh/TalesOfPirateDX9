
#pragma once


#include <windows.h>
#include <dshow.h>
#pragma comment (lib,"Strmiids.lib")

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			\
	{							\
		if(p)					\
		{						\
			(p)->Release();		\
			(p) = 0;			\
		}						\
	}							
#endif


#define WM_GRAPHNOTIFY WM_USER + 1024


// 简单视频播放
class CGameMovie
{
public:
	CGameMovie(void);
	virtual ~CGameMovie(void);

public:

	// 播放
	bool Play(HWND hWnd, const char* pszFileName);

	// 停止播放
	void Stop(void);

	// 是否正在播放
	bool IsPlaying(void);

	// 清除
	void Cleanup(void);

	// 获得最后的错误码
	HRESULT GetLastError(void) { return _hResult; }

	IMediaEventEx* GetEvent() { return _pEvent; }

	void HandleGraphEvent();

private:

	HRESULT _hResult;
	HWND	_hWnd;

	IGraphBuilder*	_pGraph;
	IMediaControl*	_pControl;
	IVideoWindow*	_pVidWin;
	IMediaPosition*	_pPosition;

	IMediaEventEx* _pEvent;
};

