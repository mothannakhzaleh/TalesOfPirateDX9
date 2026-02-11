#ifndef MPParticleAffectorFactory_H
#define MPParticleAffectorFactory_H

#include "MPEffPrerequisites.h"

class MINDPOWER_API MPParticleAffectorFactory
{
protected:
	std::vector<MPParticleAffector*> m_Affectors;
public:
	MPParticleAffectorFactory() {};
	virtual ~MPParticleAffectorFactory();
	/** 返回工厂的名字, 这个名字标识了这个工厂创建的粒子影响因子的类型 */
	virtual string getName() const = 0;

	/** 创建一个新的影响因子对象.*/
	virtual MPParticleAffector* createAffector(MPParticleSystem* psys) = 0;

	/** 析构影响因子对象. */
	virtual void destroyAffector(MPParticleAffector* e);
};
#endif