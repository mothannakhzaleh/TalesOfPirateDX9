#ifndef MPEffectMath_H
#define MPEffectMath_H

#include "MindPowerAPI.h"

#include <string>
#include <math.h>

/// 前置声明
class MPDegree;

/** 给出的角度以弧度为单位的包装类.
@remarks
弧度值和角度值可以相互转换，而且转换会自动进行
*/
class MPRadian
{
	float m_fRad;

public:
	explicit MPRadian ( float r=0 ) : m_fRad(r) {}
	MPRadian ( const MPDegree& d );
	const MPRadian& operator = ( const float& f ) { m_fRad = f; return *this; }
	const MPRadian& operator = ( const MPRadian& r ) { m_fRad = r.m_fRad; return *this; }
	const MPRadian& operator = ( const MPDegree& d );

	float valueDegrees() const; 
	float valueRadians() const { return m_fRad; }
	float valueAngleUnits() const;

	MPRadian operator + ( const MPRadian& r ) const { return MPRadian ( m_fRad + r.m_fRad ); }
	MPRadian operator + ( const MPDegree& d ) const ;
	MPRadian& operator += ( const MPRadian& r ) { m_fRad += r.m_fRad; return *this; }
	MPRadian& operator += ( const MPDegree& d ) ;
	MPRadian operator - () { return MPRadian(-m_fRad); }
	MPRadian operator - ( const MPRadian& r ) const { return MPRadian ( m_fRad - r.m_fRad ); }
	MPRadian operator - ( const MPDegree& d ) const ;
	MPRadian& operator -= ( const MPRadian& r ) { m_fRad -= r.m_fRad; return *this; }
	MPRadian& operator -= ( const MPDegree& d ) ;
	MPRadian operator * ( float f ) const { return MPRadian ( m_fRad * f ); }
	MPRadian operator * ( const MPRadian& f ) const { return MPRadian ( m_fRad * f.m_fRad ); }
	MPRadian& operator *= ( float f ) { m_fRad *= f; return *this; }
	MPRadian operator / ( float f ) const { return MPRadian ( m_fRad / f ); }
	MPRadian& operator /= ( float f ) { m_fRad /= f; return *this; }

	bool operator <  ( const MPRadian& r ) const { return m_fRad <  r.m_fRad; }
	bool operator <= ( const MPRadian& r ) const { return m_fRad <= r.m_fRad; }
	bool operator == ( const MPRadian& r ) const { return m_fRad == r.m_fRad; }
	bool operator != ( const MPRadian& r ) const { return m_fRad != r.m_fRad; }
	bool operator >= ( const MPRadian& r ) const { return m_fRad >= r.m_fRad; }
	bool operator >  ( const MPRadian& r ) const { return m_fRad >  r.m_fRad; }
};

/** 给出的角度以角度为单位的包装类.
@remarks
弧度值和角度值可以相互转换，而且转换会自动进行
*/
class MPDegree
{
	float m_fDeg; // if you get an error here - make sure to define/typedef 'Real' first

public:
	explicit MPDegree ( float d=0 ) : m_fDeg(d) {}
	MPDegree ( const MPRadian& r ) : m_fDeg(r.valueDegrees()) {}
	const MPDegree& operator = ( const float& f ) { m_fDeg = f; return *this; }
	const MPDegree& operator = ( const MPDegree& d ) { m_fDeg = d.m_fDeg; return *this; }
	const MPDegree& operator = ( const MPRadian& r ) { m_fDeg = r.valueDegrees(); return *this; }

	float valueDegrees() const { return m_fDeg; }
	float valueRadians() const; 
	float valueAngleUnits() const;

	MPDegree operator + ( const MPDegree& d ) const { return MPDegree ( m_fDeg + d.m_fDeg ); }
	MPDegree operator + ( const MPRadian& r ) const { return MPDegree ( m_fDeg + r.valueDegrees() ); }
	MPDegree& operator += ( const MPDegree& d ) { m_fDeg += d.m_fDeg; return *this; }
	MPDegree& operator += ( const MPRadian& r ) { m_fDeg += r.valueDegrees(); return *this; }
	MPDegree operator - () { return MPDegree(-m_fDeg); }
	MPDegree operator - ( const MPDegree& d ) const { return MPDegree ( m_fDeg - d.m_fDeg ); }
	MPDegree operator - ( const MPRadian& r ) const { return MPDegree ( m_fDeg - r.valueDegrees() ); }
	MPDegree& operator -= ( const MPDegree& d ) { m_fDeg -= d.m_fDeg; return *this; }
	MPDegree& operator -= ( const MPRadian& r ) { m_fDeg -= r.valueDegrees(); return *this; }
	MPDegree operator * ( float f ) const { return MPDegree ( m_fDeg * f ); }
	MPDegree operator * ( const MPDegree& f ) const { return MPDegree ( m_fDeg * f.m_fDeg ); }
	MPDegree& operator *= ( float f ) { m_fDeg *= f; return *this; }
	MPDegree operator / ( float f ) const { return MPDegree ( m_fDeg / f ); }
	MPDegree& operator /= ( float f ) { m_fDeg /= f; return *this; }

	bool operator <  ( const MPDegree& d ) const { return m_fDeg <  d.m_fDeg; }
	bool operator <= ( const MPDegree& d ) const { return m_fDeg <= d.m_fDeg; }
	bool operator == ( const MPDegree& d ) const { return m_fDeg == d.m_fDeg; }
	bool operator != ( const MPDegree& d ) const { return m_fDeg != d.m_fDeg; }
	bool operator >= ( const MPDegree& d ) const { return m_fDeg >= d.m_fDeg; }
	bool operator >  ( const MPDegree& d ) const { return m_fDeg >  d.m_fDeg; }
};

// 这些函数不能在MPRadian类内定义，因为这些函数需要MPDegree类的定义
inline MPRadian::MPRadian(const MPDegree& d) : m_fRad(d.valueRadians()) {
}
inline const MPRadian& MPRadian::operator =(const MPDegree& d) { 
	m_fRad = d.valueRadians(); return *this; 
}
inline MPRadian MPRadian::operator +(const MPDegree& d) const {
	return MPRadian ( m_fRad + d.valueRadians() ); 
}
inline MPRadian& MPRadian::operator +=(const MPDegree& d) {
	m_fRad += d.valueRadians(); return *this; 
}
inline MPRadian MPRadian::operator -(const MPDegree& d) const {
	return MPRadian ( m_fRad - d.valueRadians() ); 
}
inline MPRadian& MPRadian::operator -=(const MPDegree& d) { 
	m_fRad -= d.valueRadians(); return *this; 
}


/** 这个类提供公共数学函数的访问.
@remarks
大多数数学函数都是C运行时库函数的别名。这里用别名使将来会提供更
优化的函数成为可能，或根据更快的运行时库或其他自定义数学库
@note
<br>这里是根据MgcMath.h摘自
<a href="http://www.magic-software.com">Wild Magic</a>.
*/
class MINDPOWER_API MPEffectMath
{
public:
	/** 角度单位. 这个功能使角度单位能够同时使用上面的角度和弧度制.
	当解析文件时，这个功能将会自动使用。这些用法可查看Angle类
	*/
	enum AngleUnit
	{
		AU_DEGREE,
		AU_RADIAN
	};

protected:
	// API内部使用的角度单位
	static AngleUnit m_sAngleUnit;

	/// 三角函数表由构造函数决定.
	static int m_TrigTableSize;

	/// 弧度值 -> 三角函数表中最小表示弧度 ( m_TrigTableSize / 2 * PI )
	static float m_TrigTableFactor;
	static float* m_SinTable;
	static float* m_TanTable;

	/** 构建三角函数表 （私有）*/
	void buildTrigTables();

	static float SinTable (float fValue);
	static float TanTable (float fValue);

public:        
	/** 默认构造函数.
	@param
		trigTableSize 设置内部使用三角函数表的大小（可选参数）
	*/
	MPEffectMath(unsigned int trigTableSize = 4096);

	/** 默认析构函数.
	*/
	~MPEffectMath();

	static const float POS_INFINITY;
	static const float NEG_INFINITY;
	static const float PI;
	static const float TWO_PI;
	static const float HALF_PI;
	static const float fDeg2Rad;
	static const float fRad2Deg;

	static const std::string BLANK;

/// 角度弧度相关
	static inline float DegreesToRadians(float degrees) { return degrees * fDeg2Rad; }
	static inline float RadiansToDegrees(float radians) { return radians * fRad2Deg; }
	/** 这个函数用于设置整个API使用的角度单位（弧度或角度）	*/
	static void setAngleUnit(AngleUnit unit);
	/** 得到整个API正在使用的角度单位. */
	static AngleUnit getAngleUnit(void);

	/** 将当前的角度单位转换乘弧度. */
	static float AngleUnitsToRadians(float units);
	/** 将弧度转换成当前的角度单位. */
	static float RadiansToAngleUnits(float radians);
	/** 将当前的角度单位转换乘角度. */
	static float AngleUnitsToDegrees(float units);
	/** 将角度转换成当前的角度单位. */
	static float DegreesToAngleUnits(float degrees);

/// 随机数相关
	static float UnitRandom ();  // [0,1]
	static float RangeRandom (float fLow, float fHigh);  // in [fLow,fHigh]

/// 三角函数相关
	/** Sine 函数.
	@param
		fValue 角的弧度值
	@param
		useTables 如果为真, 使用查表而不是通过计算，这样会更快但精度降低.
	*/
	static inline float Sin (const MPRadian& fValue, bool useTables = false) {
		return (!useTables) ? float(sin(fValue.valueRadians())) : SinTable(fValue.valueRadians());
	}
	/** Sine 函数.
	@param
		fValue 角的弧度值
	@param
		useTables 如果为真, 使用查表而不是通过计算，这样会更快但精度降低.
	*/
	static inline float Sin (float fValue, bool useTables = false) {
		return (!useTables) ? float(sin(fValue)) : SinTable(fValue);
	}
	/** Cosine 函数.
	@param
		fValue 角的弧度值
	@param
		useTables 如果为真, 使用查表而不是通过计算，这样会更快但精度降低.
	*/
	static inline float Cos (const MPRadian& fValue, bool useTables = false) {
		return (!useTables) ? float(cos(fValue.valueRadians())) : SinTable(fValue.valueRadians() + HALF_PI);
	}
	/** Cosine function.
	@param
		fValue 角的弧度值
	@param
		useTables 如果为真, 使用查表而不是通过计算，这样会更快但精度降低.
	*/
	static inline float Cos (float fValue, bool useTables = false) {
		return (!useTables) ? float(cos(fValue)) : SinTable(fValue + HALF_PI);
	}
	/** Tangent 函数.
	@param
		fValue 角的弧度值
	@param
		useTables 如果为真, 使用查表而不是通过计算，这样会更快但精度降低.
	*/
	static inline float Tan (const MPRadian& fValue, bool useTables = false) {
		return (!useTables) ? float(tan(fValue.valueRadians())) : TanTable(fValue.valueRadians());
	}
	/** Tangent function.
	@param
		fValue 角的弧度值
	@param
		useTables 如果为真, 使用查表而不是通过计算，这样会更快但精度降低.
	*/
	static inline float Tan (float fValue, bool useTables = false) {
		return (!useTables) ? float(tan(fValue)) : TanTable(fValue);
	}



};
#endif
