#ifndef MPEffVector3_H
#define MPEffVector3_H

#include "MindPowerAPI.h"

#include "lwDirectX.h"
#include "MPEffectMath.h"
#include "MPEffQuaternion.h"

class MINDPOWER_API MPEffVector3
{
public:
	D3DXVECTOR3 m_vSelf;
public:
	MPEffVector3() : m_vSelf(0, 0, 0) {}
	MPEffVector3(float x, float y, float z) : m_vSelf(x, y, z) {}
	MPEffVector3(const MPEffVector3& v) : m_vSelf(v.m_vSelf) {}
	MPEffVector3(const D3DXVECTOR3& dxV) : m_vSelf(dxV) {}

	D3DXVECTOR3& GetDXValue()	{ return m_vSelf; }

	// 数学操作
	inline MPEffVector3 operator + ( const MPEffVector3& rkVector ) const
	{
		MPEffVector3 kSum;

		D3DXVec3Add(&(kSum.m_vSelf), &m_vSelf, &(rkVector.m_vSelf));

		return kSum;
	}


	inline MPEffVector3& operator *= ( float fScalar )
	{
		D3DXVec3Scale(&m_vSelf, &m_vSelf, fScalar);
		return *this;
	}

	inline MPEffVector3& operator *= ( const MPEffVector3& rkVector )
	{
		m_vSelf.x *= rkVector.m_vSelf.x;
		m_vSelf.y *= rkVector.m_vSelf.y;
		m_vSelf.z *= rkVector.m_vSelf.z;

		return *this;
	}


	inline MPEffVector3 crossProduct( const MPEffVector3& rkVector ) const
	{
		D3DXVECTOR3 kCross;
		
		D3DXVec3Cross( &kCross, &m_vSelf, &(rkVector.m_vSelf));

		return kCross;
	}

	inline float squaredLength () const
	{
		return D3DXVec3LengthSq(&m_vSelf);
	}

	/** 单位化向量.
	@note
		这个函数不会因零向量而崩溃，但如果是零向量则不会改变他的元素
	@returns 向量的先前长度.
	*/
	inline float normalise()
	{
		float fLength = D3DXVec3Length(&m_vSelf);

		// 对于零向量也可以使用，不过并不改变任何值
		if ( fLength > 1e-06f )
		{
			D3DXVec3Normalize(&m_vSelf, &m_vSelf);
		}

		return fLength;
	}


	/** 产生一个垂直的向量.
	@remarks
	This method will return a vector which is perpendicular to this
	vector. There are an infinite number of possibilities but this 
	method will guarantee to generate one of them. If you need more 
	control you should use the Quaternion class.
	*/
	inline MPEffVector3 perpendicular(void) const
	{
		static const float fSquareZero = float(1e-06 * 1e-06);

		MPEffVector3 perp = this->crossProduct( MPEffVector3::UNIT_X );

		// 检查长度 length
		if( perp.squaredLength() < fSquareZero )
		{
			/* 这个轴是Y轴，所以要换一个轴.
			*/
			perp = this->crossProduct( MPEffVector3::UNIT_Y );
		}

		return perp;
	}

	inline bool operator == ( const MPEffVector3& rkVector ) const
	{
		return ( m_vSelf.x == rkVector.m_vSelf.x 
			&& m_vSelf.y == rkVector.m_vSelf.y 
			&& m_vSelf.z == rkVector.m_vSelf.z );
	}


	/** 在给定的最大角度内产生一个随机的向量.
	@remarks
		这个方法假设随机数的种子已经产生
	@param 
	angle 给定的最大偏移角度
	@param 
	up 任何垂直这个向量的向量
	@returns 
		偏移这个向量随机角度的向量。这个随机向量没有单位化
	*/
	inline MPEffVector3 randomDeviant( const MPRadian& angle,
		const MPEffVector3& up = MPEffVector3::ZERO ) const
	{
		MPEffVector3 newUp;

		if (up == MPEffVector3::ZERO)
		{
			// 创建一个向上的向量
			newUp = this->perpendicular();
		}
		else
		{
			newUp = up;
		}

		// 将up向量绕着方向向量旋转随机角度
		MPEffQuaternion q;
		q.FromAngleAxis( MPRadian(MPEffectMath::UnitRandom() * MPEffectMath::TWO_PI), *this );
		newUp = q * newUp;

		// Finally rotate this by given angle around randomised up
		q.FromAngleAxis( angle, newUp );
		return q * (*this);
	}

	// 特殊的值
	static const MPEffVector3 ZERO;
	static const MPEffVector3 UNIT_X;
	static const MPEffVector3 UNIT_Y;
	static const MPEffVector3 UNIT_Z;
	static const MPEffVector3 NEGATIVE_UNIT_X;
	static const MPEffVector3 NEGATIVE_UNIT_Y;
	static const MPEffVector3 NEGATIVE_UNIT_Z;
	static const MPEffVector3 UNIT_SCALE;

};
#endif