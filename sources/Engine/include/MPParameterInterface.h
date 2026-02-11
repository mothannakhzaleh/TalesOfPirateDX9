#ifndef MPParameterInterface_H
#define MPParameterInterface_H

#include "MPEffPrerequisites.h"

/// 可用的参数列表
enum MPParameterType
{
	PT_BOOL,
	PT_REAL,
	PT_INT,
	PT_UNSIGNED_INT,
	PT_SHORT,
	PT_UNSIGNED_SHORT,
	PT_LONG,
	PT_UNSIGNED_LONG,
	PT_STRING,
	PT_VECTOR3,
	PT_MATRIX3,
	PT_MATRIX4,
	PT_QUATERNION,
	PT_COLOURVALUE
};

/// 参数定义,用于支持MPParameterInteface的反射机制
class MINDPOWER_API MPParameterDef
{
public:
	String name;
	String description;
	MPParameterType paramType;
	MPParameterDef(const String& newName, const String& newDescription, MPParameterType newType)
		: name(newName), description(newDescription), paramType(newType) {}
};
typedef std::vector<MPParameterDef> MPParameterList;

/** 参数对象得到/设置参数的抽象类.*/
class MINDPOWER_API MPParamCommand
{
public:
	virtual String doGet(const void* target) const = 0;
	virtual void doSet(void* target, const String& val) = 0;

	virtual ~MPParamCommand() { }
};
typedef std::map<String, MPParamCommand* > ParamCommandMap;

class MINDPOWER_API MPParamDictionary
{
	friend class MPParameterInterface;
protected:
	/// 参数定义
	MPParameterList m_ParamDefs;

	/// 用于get/set的参数对象
	ParamCommandMap m_ParamCommands;

	/** 返回有名参数的参数对象. */
	MPParamCommand* getParamCommand(const String& name)
	{
		ParamCommandMap::iterator i = m_ParamCommands.find(name);
		if (i != m_ParamCommands.end())
		{
			return i->second;
		}
		else
		{
			return 0;
		}
	}

	const MPParamCommand* getParamCommand(const String& name) const
	{
		ParamCommandMap::const_iterator i = m_ParamCommands.find(name);
		if (i != m_ParamCommands.end())
		{
			return i->second;
		}
		else
		{
			return 0;
		}
	}
public:
	MPParamDictionary()  {}

	/** 增加这个类的参数定义. 
	@param paramDef 参数的MPParameterDef对象
	@param paramCmd 参数命令派生类对象的指针用于处理getting/setting.
	NB: 这个类不析构这个指针

	*/
	void addParameter(const MPParameterDef& paramDef, MPParamCommand* paramCmd)
	{
		m_ParamDefs.push_back(paramDef);
		m_ParamCommands[paramDef.name] = paramCmd;
	}

	const MPParameterList& getParameters(void) const
	{
		return m_ParamDefs;
	}
};

typedef std::map<String, MPParamDictionary> MPParamDictionaryMap;

/** 定义使用反射风格，设置自定义参数的公共接口.
@remarks
	这个类保存了供子类使用的类静态名字-参数map字典。更多细节请查看MPParamDictionary. 
@remarks
	要使用这个类，每个子类必须在构造函数中调用createParamDictionary，来创
	建那个类的参数字典.
*/
class MINDPOWER_API MPParameterInterface
{
public:

	virtual ~StringInterface() {}

	MPParamDictionary* getParamDictionary(void)
	{
		MPParamDictionaryMap::iterator i = m_sDictionary.find(m_ParamDictName);
		if (i != m_sDictionary.end())
		{
			return &(i->second);
		}
		else
		{
			return 0;
		}
	}

	const MPParamDictionary* getParamDictionary(void) const
	{
		MPParamDictionaryMap::const_iterator i = m_sDictionary.find(m_ParamDictName);
		if (i != m_sDictionary.end())
		{
			return &(i->second);
		}
		else
		{
			return 0;
		}
	}

	const MPParameterList& getParameters(void) const;

	virtual bool setParameter(const String& name, const String& value);
	
	virtual void setParameterList(const NameValuePairList& paramList);

	virtual String getParameter(const String& name) const
	{
		// 得到字典
		const MPParamDictionary* dict = getParamDictionary();

		if (dict)
		{
			// 查找参数对象
			const MPParamCommand* cmd = dict->getParamCommand(name);

			if (cmd)
			{
				return cmd->doGet(this);
			}
		}

		// 失败
		return "";
	}

	virtual void copyParametersTo(MPParameterInterface* dest) const
	{
		// 得到字典
		const MPParamDictionary* dict = getParamDictionary();

		if (dict)
		{
			// 遍历自己的参数
			ParameterList::const_iterator i;

			for (i = dict->mParamDefs.begin(); 
				i != dict->mParamDefs.end(); ++i)
			{
				dest->setParameter(i->name, getParameter(i->name));
			}
		}


	}

	static void cleanupDictionary () ;

protected:
	/** 内部方法用于创建类的参数字典.
	@remarks
		这个方法将会检查这个类的参数字典,如果不存在则创建一个.NB:必须提
	供类的名字(RTTI没有使用)
	@param
	className 类名(字典用)
	@returns
		true 如果字典被创建, false 如果字典已经存在
	*/
	bool createParamDictionary(const String& className)
	{
		m_ParamDictName = className;
		if (m_sDictionary.find(className) == m_sDictionary.end())
		{
			m_sDictionary[className] = ParamDictionary();
			return true;
		}
		return false;

	}

protected:

	/// 参数字典
	static MPParamDictionaryMap m_sDictionary;
	/// 用于查找的类名 (必须由子类初始化)
	String m_ParamDictName;

};

#endif