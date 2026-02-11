#ifndef MPParticleAffector_H
#define MPParticleAffector_H

#include "MindPowerAPI.h"

class MPParticleSystem;
class MPParticle;

/** 实现粒子影响因子接口抽象类.
@remarks
粒子影响因子是在粒子系统中在粒子的生存期中修改粒子。他们打包成
类型，比如‘力（vector force）’影响因子，‘衰减器（fader）’影响因子；每
个类型以不同的方式将修改粒子，使用不同的参数。
@par
因为你可以使用很多类型的影响因子，引擎没有规定哪些可用因子。它
提供了一些内在的因子，按它也允许插件或应用程序扩展影响因子。这可以
通过创建ParticleAffector的子类用来扩展你需要的发射行为，当然同时需要
创建ParticleAffectorFactory的子类用来创建你的新影响因子的对象。你在
ParticleSystemManager调用addAffectorFactory注册这个工厂,这样就可以在代
码或脚本文件中通过名字创建这个新的影响因子
@par
粒子发射器也是用相同的方式.在创建粒子特效时,这样粒子系统有很大
的扩展性,可以随意组合粒子影响因子类型和他们的参数.
*/
class MINDPOWER_API MPParticleAffector
{
public:
	MPParticleAffector(MPParticleSystem* mParticleSystem): m_ParticleSystem(mParticleSystem) {}

	virtual ~MPParticleAffector();

	/** 让影响因子初始化所有新创建的粒子
	@remarks
	这里是影响因子有机会将他的特效初始化粒子.因子希望初始化一些
	或所以粒子,这依赖影响因子本身
	@param
	pParticle 需要初始化粒子的指针.
	*/
	virtual void _initParticle(MPParticle* pParticle) { /* by default do nothing */ }

	/** 允许影响因子的数据作用在所有活动的粒子
	@remarks
	这里使影响因子有机会将他们的数据作用到系统中的粒子.影响因子
	希望将他们的特效作用到一些或所有粒子,这些依赖粒子因子的方法
	@param
	pSystem 所作用的粒子系统的指针.
	@param
	timeElapsed 从上次调用到现在所花的时间(秒).
	*/
	virtual void _affectParticles(MPParticleSystem* pSystem, float timeElapsed) = 0;

	/** 返回影响因子的类型. 
	@remarks
	这个参数决定了影响因子的类型,这样便可以通过这个参数来创建影
	响因子
	*/
	const String &getType(void) const { return m_Type; }
protected:
	/** 内部方法用来为子类设置基本参数定义
	@remarks
	因为StringInterface包含每个类的参数字典,子类需要调用它来请求
	基类也把它的参数加入到字典中.不要在构造函数中做这件事,因为构造
	函数不是以虚函数运行,它没有虚函数的上下文
	@par
	子类必须在调用这个方法之前调用他们自己的createParamDictionary
	方法
	*/
	void addBaseParameters(void) { /* 实际上没有做任何事 - 将来可能会做一些事 */ }

protected:
	/// 粒子影响因子的类型, 必须由子类初始化
	string m_Type;

	MPParticleSystem* m_ParticleSystem;

};
#endif