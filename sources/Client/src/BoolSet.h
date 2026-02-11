//--------------------------------------------------------------
// 名称:布尔集合类
// 作者:lh 2004-11-01
// 用途:可以快速判断是否所有布尔都为false
//--------------------------------------------------------------

#pragma once

class CBoolSet
{
public:
	CBoolSet() = default;

	// bit范围0~31
	bool IsTrue( unsigned int bit )		{ return (_value & 0x00000001 << bit)!=0;	}
	bool IsFalse( unsigned int bit )    { return (_value & 0x00000001 << bit)==0;	}
	void SetTrue( unsigned int bit )	{ _value |= 0x00000001 << bit;				}	// 位为true
	void SetFalse( unsigned int bit )	{ _value &= ~(0x00000001 << bit);			}	// 位为false

	void Set( unsigned int bit, bool v ){ if(v) SetTrue(bit); else SetFalse(bit);	}
	bool Get( unsigned int bit )		{ return (_value & 0x00000001 << bit)!=0;	}

	void AllFalse()						{ _value = 0;			}	// 全false
	void AllTrue()						{ _value = 0xffffffff;	}	// 全true

	bool IsNone()						{ return _value==0;		}	// 是否全为false
	bool IsAny()						{ return _value!=0;		}	// 是否有某一位为true

private:
	uint32_t _value{ 0 };
};
