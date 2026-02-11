#ifndef MPIteratorWrappers_H
#define MPIteratorWrappers_H

template <class T>
class MPMapIterator
{
private:
	typename T::iterator mCurrent;
	typename T::iterator mEnd;
	/// 私有构造函数，因为仅必须提供参数
	MapIterator() {};
public:
	typedef typename T::mapped_type MappedType;
	typedef typename T::key_type KeyType;

	/** 构造函数.
	@remarks
		提供start和end迭代器用于初始化.
	*/
	MapIterator(typename T::iterator start, typename T::iterator end)
		: mCurrent(start), mEnd(end)
	{
	}

	bool hasMoreElements(void) const
	{
		return mCurrent != mEnd;
	}

	typename T::mapped_type getNext(void)
	{
		return (mCurrent++)->second;
	}
	typename T::mapped_type peekNextValue(void)
	{
		return mCurrent->second;
	}
	typename T::key_type peekNextKey(void)
	{
		return mCurrent->first;
	}
	MapIterator<T> & operator=( MapIterator<T> &rhs )
	{
		mCurrent = rhs.mCurrent;
		mEnd = rhs.mEnd;
		return *this;
	}
	typename T::pointer peekNextValuePtr(void)
	{
		return &(mCurrent->second);
	}
	void moveNext(void)
	{
		mCurrent++;
	}



};

#endif