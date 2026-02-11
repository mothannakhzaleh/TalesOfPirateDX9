#ifndef MPColorValue_H
#define MPColorValue_H

#include "MindPowerAPI.h"
#include <assert.h>

typedef unsigned int RGBA;
typedef unsigned int ARGB;
typedef unsigned int ABGR;

/** 表示颜色的类.
@remarks
	颜色由4个成员组成，每一个成员都是由0.0到1.0的浮点数组成
*/
class MINDPOWER_API MPColorValue
{
public:
	static MPColorValue Black;
	static MPColorValue White;
	static MPColorValue Red;
	static MPColorValue Green;
	static MPColorValue Blue;

	explicit MPColorValue(float red = 1.0f, float green = 1.0f,
						  float blue = 1.0f,float alpha = 1.0f ) 
		: r(red), g(green), b(blue), a(alpha)
	{ }

	bool operator==(const MPColorValue& rhs) const;
	bool operator!=(const MPColorValue& rhs) const;

	union {
		struct {
			float r,g,b,a;
		};
		float val[4];
	};

	/** 以RGBA形式返回颜色.
	*/
	RGBA getAsRGBA(void) const;

	/** 以ARGB形式返回颜色.
	*/
	ARGB getAsARGB(void) const;

	/** 以ABGR形式返回颜色 */
	ABGR getAsABGR(void) const;

	/** 以RGBA形式设置颜色.
	*/
	void setAsRGBA(const RGBA val);

	/** 以ARGB形式设置颜色.
	*/
	void setAsARGB(const ARGB val);

	/** 以ABGR形式设置颜色.
	*/
	void setAsABGR(const ABGR val);

	// 一元数学操作
	inline MPColorValue& operator += ( const MPColorValue& rkVector )
	{
		r += rkVector.r;
		g += rkVector.g;
		b += rkVector.b;
		a += rkVector.a;

		return *this;
	}

	inline MPColorValue& operator -= ( const MPColorValue& rkVector )
	{
		r -= rkVector.r;
		g -= rkVector.g;
		b -= rkVector.b;
		a -= rkVector.a;

		return *this;
	}

	inline MPColorValue& operator *= (const float fScalar )
	{
		r *= fScalar;
		g *= fScalar;
		b *= fScalar;
		a *= fScalar;
		return *this;
	}

	inline MPColorValue& operator /= (const float fScalar )
	{
		assert( fScalar != 0.0 );

		float fInv = 1.0 / fScalar;

		r *= fInv;
		g *= fInv;
		b *= fInv;
		a *= fInv;

		return *this;
	}

	inline MPColorValue& operator /= (const MPColorValue& rkVector )
	{
		r /= rkVector.r;
		g /= rkVector.g;
		b /= rkVector.b;
		a /= rkVector.a;

		return *this;
	}
};

//=============================================================================
// 二元数学操作
//=============================================================================
inline const MPColorValue operator +(const MPColorValue& lhs, const MPColorValue& rhs)
{
	MPColorValue kSum;

	kSum.r = lhs.r + rhs.r;
	kSum.g = lhs.g + rhs.g;
	kSum.b = lhs.b + rhs.b;
	kSum.a = lhs.a + rhs.a;

	return kSum;
}
//-----------------------------------------------------------------------------
inline const MPColorValue operator -(const MPColorValue& lhs, const MPColorValue& rhs)
{
	MPColorValue kDiff;

	kDiff.r = lhs.r - rhs.r;
	kDiff.g = lhs.g - rhs.g;
	kDiff.b = lhs.b - rhs.b;
	kDiff.a = lhs.a - rhs.a;

	return kDiff;
}
//-----------------------------------------------------------------------------
inline const MPColorValue operator *(const float fScalar, const MPColorValue& rhs)
{
	MPColorValue kProd;

	kProd.r = fScalar*rhs.r;
	kProd.g = fScalar*rhs.g;
	kProd.b = fScalar*rhs.b;
	kProd.a = fScalar*rhs.a;

	return kProd;
}
//-----------------------------------------------------------------------------
inline const MPColorValue operator *(const MPColorValue& lhs, const float fScalar)
{
	return (fScalar * lhs);
}
//-----------------------------------------------------------------------------
inline const MPColorValue operator *(const MPColorValue& lhs, const MPColorValue& rhs)
{
	MPColorValue kProd;

	kProd.r = lhs.r * rhs.r;
	kProd.g = lhs.g * rhs.g;
	kProd.b = lhs.b * rhs.b;
	kProd.a = lhs.a * rhs.a;

	return kProd;
}
//-----------------------------------------------------------------------------
inline const MPColorValue operator /(const MPColorValue& lhs, const MPColorValue& rhs)
{
	MPColorValue kProd;

	kProd.r = lhs.r / rhs.r;
	kProd.g = lhs.g / rhs.g;
	kProd.b = lhs.b / rhs.b;
	kProd.a = lhs.a / rhs.a;

	return kProd;
}
//-----------------------------------------------------------------------------
inline MPColorValue operator /(const MPColorValue& lhs, const float fScalar )
{
	assert( fScalar != 0.0 );

	MPColorValue kDiv;

	float fInv = 1.0 / fScalar;
	kDiv.r = lhs.r * fInv;
	kDiv.g = lhs.g * fInv;
	kDiv.b = lhs.b * fInv;
	kDiv.a = lhs.a * fInv;

	return kDiv;
}

#endif