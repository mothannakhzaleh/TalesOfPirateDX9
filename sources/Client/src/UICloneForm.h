//----------------------------------------------------------------------
// 名称:用于管理相同的表单内存
// 作者:lh 2005-03-15
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{
class CForm;

// 管理同一类克隆的表单,在删除时并不真正的删除,缓存起来,下次克隆时可直接使用 
class CCloneForm
{
public:
	CCloneForm();
	~CCloneForm();

	void	SetSample( CForm* frm )	{ _pSample = frm;	}

	CForm*	Clone();
	bool	Release( CForm* p );

private:
	CForm*	_pSample;		// 样本表单

	typedef std::vector<CForm*>	vfrm;
	vfrm	_vfrm;

    int     _nCount;		// 有效的表单个数

};

// 在同一类表单中,自动得到隐藏的表单,如果没有隐藏的表单,则自动克隆一个
class CHideForm
{
public:
	void	Init( CForm* frm )		{ _vfrm.push_back( frm );	}

	CForm*	GetHide();
	void	CloseAll();		// 关闭所有表单

	CForm*  GetForm( int n )		{ return _vfrm[n];			}
	int		GetCount()				{ return (int)_vfrm.size();	}

	CForm*  operator[]( int n )		{ return _vfrm[n];			}

private:
	typedef std::vector<CForm*>	vfrm;
	vfrm	_vfrm;

};

}

