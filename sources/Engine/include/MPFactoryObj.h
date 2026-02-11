#ifndef MPFactoryObj_H
#define MPFactoryObj_H

/** 抽象工厂类. 本身没有任何功能, 但派生类可以增加功能.
*/
template< typename T > class FactoryObj
{
public:
	virtual ~FactoryObj() {};

	/** 返回工厂类型.	*/
	virtual const string& getType() const = 0;

	/** 创建一个新对象.
	@param name 创建对象的名字
	@return 由这个工厂创建的对象。这个对象的类型依赖于工厂.
	*/
	virtual T* createInstance( const string& name ) = 0;    
	/** 析构一个由这个工厂创建的对象.
	@param ptr 指向需要背析构的对象指针
	*/
	virtual void destroyInstance( T* ) = 0;    
};

#endif