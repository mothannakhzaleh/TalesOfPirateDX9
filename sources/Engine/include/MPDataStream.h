#ifndef MPDataStream_H
#define MPDataStream_H

#include "MPEffPrerequisites.h"
#include "MPSharedPtr.h"

/** 通用类用于包装读入的数据.
@remarks
This class performs basically the same tasks as std::basic_istream, 
except that it does not have any formatting capabilities, and is
designed to be subclassed to receive data from multiple sources,
including libraries which have no compatiblity with the STL's
stream interfaces. As such, this is an abstraction of a set of 
wrapper classes which pretend to be standard stream classes but 
can actually be implemented quite differently. 
@par
	通常如果插件或应用程序提供ArchiveFactory，那么也应提供可以访问那种
Archive数据流的MPDataStream的派生类，除非默认提供的可以满足要求
@note
	由于性能的要求，这里不保证它是线程安全的。如果你希望异步访问数据流
你必须自己组织互斥量，避免竞争
*/
class MINDPOWER_API MPDataStream
{
public:
	/// 构造函数用来创建匿名的流
	MPDataStream() : m_Size(0) {}
	/// 构造函数用来创建命名的流
	MPDataStream(const String& name) : m_Name(name), m_Size(0) {}
	virtual ~MPDataStream() {}

	// 流操作
	template<typename T> MPDataStream& operator>>(T& val);
	/** 从流中读取所需字节的数据, 当遇到eof则停止.
	@param buf 缓存的指针
	@param count 需要读取的字节数
	@returns 读取的字节数
	*/
	virtual size_t read(void* buf, size_t count) = 0;
	/** 从流中得到一行数据.
	@remarks
		分隔符不包含在返回的数据中,并且会被跳过，所以下次读取的时候也不
	会出现.缓存中将包含中止符.
	@param buf 缓存的指针
	@param maxCount 读取一行数据的最大长度，不包括中止符
	@param delim 分隔符
	@returns 读取的字节数，不包括中止符
	*/
	virtual size_t readLine(char* buf, size_t maxCount, const String& delim = "\n") = 0;

	/** Returns a String containing the next line of data, optionally 
	trimmed for whitespace. 
	@remarks
	This is a convenience method for text streams only, allowing you to 
	retrieve a String object containing the next line of data. The data
	is read up to the next newline character and the result trimmed if
	required.
	@param 
	trimAfter If true, the line is trimmed for whitespace (as in 
	String.trim(true,true))
	*/
	virtual String getLine( bool trimAfter = true );

	/** 以字符串的形式返回整个流的数据. */
	virtual String getAsString(void);

	/** 从流中跳过一行.
	@param delim 行分隔符
	@returns 返回跳过的字节数
	*/
	virtual size_t skipLine(const String& delim = "\n") = 0;

	/** 跳过制定的字节数,也可以是一个负值，这时将向前跳过制定字节的数据. */
	virtual void skip(long count) = 0;

	/** 将流读取指针移到指定的字节处.
	*/
	virtual void seek( size_t pos ) = 0;

	/** 返回目前指针到头的偏移 */
	virtual size_t tell(void) const = 0;

	/** 如果到达eof，返回true.*/
	virtual bool eof(void) const = 0;

	/** 关闭流. */
	virtual void close(void) = 0;

	/** 返回流的数据的大小，如果不确定可能为0. */
	size_t size(void) const { return m_Size; }
	/// 返回流的名字，如果有的话.
	const String& getName(void) const { return m_Name; }

protected:
	/// 用于标识资源的名字（比如资源名）(可选)
	String m_Name;		
	/// 流中数据的大小(如果不能决定数据的大小可以为0)
	size_t m_Size;
#define MP_STREAM_TEMP_SIZE 128
	/// 格式读取的临时缓冲区
	char m_TmpArea[MP_STREAM_TEMP_SIZE];
};

typedef MPSharedPtr<MPDataStream> MPDataStreamPtr;
typedef std::list<MPDataStreamPtr> MPDataStreamList;
typedef MPSharedPtr<MPDataStreamList> MPDataStreamListPtr;

/** DataStream的公用派生类，用于处理内存中的流数据.*/
class MINDPOWER_API MPMemoryDataStream : public MPDataStream
{
public:

	/** 将已有的内存块包装成流.
	@param pMem 现存内存的指针
	@param size 内存块的大小（字节）
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(void* pMem, size_t size, bool freeOnClose = false);
	/** 将已有的内存块包装成命名的流.
	@param name 流的名字
	@param pMem 现存内存的指针
	@param size 内存块的大小（字节）
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(const String& name, void* pMem, size_t size, 
		bool freeOnClose = false);
	/** 根据另一个流的创建一个流.
	@param sourceStream 另一个数据流
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(MPDataStream& sourceStream, bool freeOnClose = true);
	/** 根据另一个流的创建一个流.
	@param sourceStream 另一个数据流(弱引用)
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(MPDataStreamPtr& sourceStream, bool freeOnClose = true);
	/** 根据另一个流的创建一个命名的流.
	@param name 流的名字
	@param sourceStream 另一个数据流
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(const String& name, MPDataStream& sourceStream, 
		bool freeOnClose = true);
	/** 根据另一个流的创建一个命名的流.
	@param name 流的名字
	@param sourceStream 另一个数据流(弱引用)
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(const String& name, const MPDataStreamPtr& sourceStream, 
		bool freeOnClose = true);
	/** 创建一个空的流.
	@param size 流的大小
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(size_t size, bool freeOnClose = true);
	/** 创建一个空的命名的流.
	@param name 流的名字
	@param size 流的大小
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPMemoryDataStream(const String& name, size_t size, bool freeOnClose = true);

	~MPMemoryDataStream();

	/** 得到这个流内存块的起始指针. */
	uchar* getPtr(void) { return m_Data; }

	/** 得到这个流目前指向的指针. */
	uchar* getCurrentPtr(void) { return m_Pos; }

	/** 设置是否在流关闭时释放内存. */
	void setFreeOnClose(bool free) { m_FreeOnClose = free; }

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek( size_t pos );
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	/// 指向数据域开头的指针
	uchar* m_Data;
	/// 指向目前所在位置的指针
	uchar* m_Pos;
	/// 指向数据域结尾的指针
	uchar* m_End;
	/// 当关闭时是否删词数据
	bool m_FreeOnClose;			
};

typedef MPSharedPtr<MPMemoryDataStream> MPMemoryDataStreamPtr;

/** DataStream的公用派生类,用于处理std::basic_istream的数据.*/
class MINDPOWER_API MPFileStreamDataStream : public MPDataStream
{
public:
	/** 从标准的stl流构建流
	@param s stl流的指针
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPFileStreamDataStream(std::ifstream* s, bool freeOnClose = true);
	/** 从标准的stl流构建命名流
	@param name 流的名字
	@param s stl流的指针
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPFileStreamDataStream(const String& name, std::ifstream* s, bool freeOnClose = true);
	/** 从标准的stl流构建命名流, 并告知其大小
	@remarks
	This variant tells the class the size of the stream too, which 
	means this class does not need to seek to the end of the stream 
	to determine the size up-front. This can be beneficial if you have
	metadata about the contents of the stream already.
	@param name 流的名字
	@param s stl流的指针
	@param size 流的大小（字节）
	@param freeOnClose 如果为true，当流释放时，内存将会被释放.
	*/
	MPFileStreamDataStream(const String& name, std::ifstream* s, size_t size, bool freeOnClose = true);

	~MPFileStreamDataStream();

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek( size_t pos );
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	std::ifstream* m_pStream;
	bool m_FreeOnClose;			
};

/** DataStream的公用派生类,用于处理C风格的文件处理的数据.*/
class MINDPOWER_API MPFileHandleDataStream : public MPDataStream
{
public:
	/// 从c风格文件方式构建流
	MPFileHandleDataStream(FILE* handle);
	/// 从c风格文件方式构建命名流
	MPFileHandleDataStream(const String& name, FILE* handle);
	~MPFileHandleDataStream();

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek( size_t pos );
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	FILE* m_FileHandle;
};


#endif