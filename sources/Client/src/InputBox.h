
// 与一个隐藏的Window Edit控件绑定来处理文字输入
// 依靠windows控件来完成文字的添加,删除,退格,剪切,粘贴,光标位置计算等繁琐的处理
class CInputBox 
{

public:


	CInputBox()
	:_bAccountMode(FALSE), _hEdit(0)
	{

	}

	HWND    GetEditWindow()						{ return _hEdit;  }
	void	SetEditWindow(HWND hEdit)			{ _hEdit = hEdit; }
	void	HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2);
	
	
	void	SetAccountMode(BOOL bAccount);		// 设置为帐号输入模式, 屏蔽掉空格, 逗号等
	void	SetPasswordMode(BOOL bPassword);	// 设置为密码模式, 得到*号字符串
	void	SetDigitalMode(BOOL bDigitalMode);	// 设置为数字模式, 只接受数字输入
	void	SetMaxNum(int nMaxNum);				// 设置最大输入的文字数量
	void	SetMultiMode(BOOL bMultiLine);		// 设置为多行模式	
	void	SetSel(int nStartChar, int nEndChar);


	void	SetCursorTail();					// 将光标设置到文字尾部
	char*	RefreshText();
	int		RefreshCursor();
	void	ClearText();
	char*	SetText(const char *pszText);
	void	ReplaceSel(const char *pszRplText, BOOL bCanUndo = TRUE);	//替换当前选择内容 - added by Arcol

protected:

	char	_szText[1024];	// 输入的文字
	int		_nCursorPos;	// 光标在文字串中的位置
	BOOL	_bAccountMode;	// 是否是帐号模式
	HWND	_hEdit;
};


inline void CInputBox::SetMaxNum(int nMaxNum)
{
	::SendMessage(_hEdit, EM_LIMITTEXT, nMaxNum, 0);
}

inline char* CInputBox::RefreshText()
{
	GetWindowText(_hEdit, _szText, 1000); 
	return _szText;
}

inline char* CInputBox::SetText(const char *pszText)
{
	SetWindowText(_hEdit, pszText);
	strcpy(_szText, pszText);
	return _szText;
}

inline void CInputBox::ClearText()
{
	strcpy(_szText, "");
	SetWindowText(_hEdit, "");
}

inline int CInputBox::RefreshCursor()
{
	::SendMessage(_hEdit, EM_GETSEL, (WPARAM)&_nCursorPos, 0);
	return _nCursorPos;
}

inline void CInputBox::SetCursorTail()
{
	int nLen = (int)(strlen(_szText));
	::SendMessage(_hEdit, EM_SETSEL, nLen, nLen);
}

inline void CInputBox::SetSel(int nStartChar, int nEndChar)
{
	::SendMessage(_hEdit, EM_SETSEL, nStartChar, nEndChar);
}

inline void CInputBox::HandleWindowMsg(DWORD dwMsgType, DWORD dwParam1, DWORD dwParam2)
{
	// 把游戏窗口的windows的键盘输入传递到edit控件
	// if(!(dwMsgType==WM_KEYDOWN || dwMsgType==WM_CHAR)) return;

	// if(dwMsgType==WM_IME_NOTIFY || dwMsgType==WM_IME_STARTCOMPOSITION) return;

	if(dwMsgType==WM_KEYDOWN || dwMsgType==WM_KEYUP)
	{
		if(dwParam1==VK_UP) return; // 屏蔽掉UP键, 避免触发Edit控件的对应操作
		// ::SendMessage(_hEdit, WM_KEYDOWN, dwParam1, dwParam2);
	}
	else if(dwMsgType==WM_CHAR)
	{
		if(_bAccountMode)
		{
			if(dwParam1==' ' || dwParam1==',') return;
		}
	}
	::SendMessage(_hEdit, dwMsgType, dwParam1, dwParam2);
}

inline void CInputBox::ReplaceSel(const char *pszRplText, BOOL bCanUndo)
{
	::SendMessage(_hEdit, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)(LPCTSTR)pszRplText);
}

extern CInputBox g_InputBox;
