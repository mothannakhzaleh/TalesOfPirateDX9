/**
/*   Copyright (c) 2003by  Marco Welti
/*
/*   This document is  bound by the  QT Public License
/*   (http://www.trolltech.com/licenses/qpl.html).
/*   See License.txt for more information.
/*
/*
/*
/*   ALL RIGHTS RESERVED.
/*
/*   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
/*   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/*   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/*   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/*   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/*   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
/*   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
/*   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/*   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/***********************************************************************/

#ifndef Stacktrace_H
#define Stacktrace_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <stdexcept>
#include <iostream>
#include <string>

#include <windows.h>

// Only valid in the following environment: Intel platform, MS VC++ 5/6/7
#ifndef _X86_
#error Only INTEL envirnoments are supported!
#endif

// Only MS VC++ 5 to 7
#if (_MSC_VER < 1100) || (_MSC_VER > 1300)
//#error Only MS VC++ 5/6/7 supported. Check if the '_CrtMemBlockHeader' has not changed with this compiler!
#endif

#include <iostream>

typedef enum eAllocCheckOutput
{
	ACOutput_Simple,
	ACOutput_Advanced,
};

class SEHTranslator
{
private:
	typedef void(*SExceptionHandler)(UINT, EXCEPTION_POINTERS*);
	SExceptionHandler mOldHandler;

public:
	SEHTranslator();
	~SEHTranslator();

	static std::string name(DWORD);
	static std::string description(DWORD);

private:
	static void translate(UINT id ,EXCEPTION_POINTERS *exception);
};

class RuntimeStack
{
private:
	std::string mStack;

public:
	RuntimeStack();
	RuntimeStack(EXCEPTION_POINTERS *ep);
	~RuntimeStack();

	friend std::ostream& operator<< (std::ostream&, const RuntimeStack&);
	std::ostream& print(std::ostream &os) const;

private:
	void gatherStack();
	int storeStack(LPEXCEPTION_POINTERS lpExceptPtr);
};


class TraceableException : public std::runtime_error
{
protected:
	RuntimeStack mStack;
	mutable std::string mText;

public:
	TraceableException(const std::string &what);
	~TraceableException() {}
	const char* what() const;

protected:
	TraceableException(const std::string &what, EXCEPTION_POINTERS *ep);

	const RuntimeStack& getStackTrace() const { return mStack; }
	const std::string& getText() const { return mText; }
	void setText(const std::string &text) { mText = text; }
};

class SystemException : public TraceableException
{
private:
	DWORD mId;

public:
	SystemException(DWORD id, EXCEPTION_POINTERS *ep);

	const char* what() const;

	std::string name() const { return SEHTranslator::name(mId); };
	std::string description() const { return SEHTranslator::description(mId); };

};

template<DWORD id>
class DetailedSystemException : public SystemException
{
public:
	DetailedSystemException(EXCEPTION_POINTERS *ep)
		: SystemException(id, ep) {}
};

class AccessViolationException : public SystemException
{
private:
	bool mIsReadFailure;
protected:
	AccessViolationException(bool raf, EXCEPTION_POINTERS *ep)
		: SystemException(EXCEPTION_ACCESS_VIOLATION, ep), mIsReadFailure(raf) {};

	bool isReadAccessViolation() { return mIsReadFailure; };
};

class WriteAccessViolationException : public AccessViolationException
{
public:
	WriteAccessViolationException(EXCEPTION_POINTERS *ep)
		: AccessViolationException(false, ep) {}
};

class ReadAccessViolationException : public AccessViolationException
{
public:
	ReadAccessViolationException(EXCEPTION_POINTERS *ep)
		: AccessViolationException(true, ep) {}
};


typedef DetailedSystemException<EXCEPTION_BREAKPOINT> BreakpointException;
typedef DetailedSystemException<EXCEPTION_SINGLE_STEP> SingleStepException;

namespace FloatingPointException
{

	template<DWORD id>
	class FloatingPointException : public SystemException
	{
	public:
		FloatingPointException(EXCEPTION_POINTERS *ep)
			: SystemException(id, ep){ }
	};

	typedef FloatingPointException<EXCEPTION_FLT_DENORMAL_OPERAND> DenormalOperandException;
	typedef FloatingPointException<EXCEPTION_FLT_DIVIDE_BY_ZERO> DivideByZeroException;
	typedef FloatingPointException<EXCEPTION_FLT_INEXACT_RESULT> InexactResultException;
	typedef FloatingPointException<EXCEPTION_FLT_INVALID_OPERATION> InvalidOperationException;
	typedef FloatingPointException<EXCEPTION_FLT_OVERFLOW> OverflowException;
	typedef FloatingPointException<EXCEPTION_FLT_UNDERFLOW> UnderflowException;
	typedef FloatingPointException<EXCEPTION_FLT_STACK_CHECK> StackCheckException;

}

namespace IntegerException
{

	template<DWORD id>
	class IntegerException : public SystemException
	{
	public:
		IntegerException(EXCEPTION_POINTERS *ep)
			: SystemException(id, ep){ }
	};

	typedef IntegerException<EXCEPTION_INT_DIVIDE_BY_ZERO> DivideByZeroException;
	typedef IntegerException<EXCEPTION_INT_OVERFLOW> OverflowException;

}

typedef DetailedSystemException<EXCEPTION_ARRAY_BOUNDS_EXCEEDED> ArrayBoundsException;
typedef DetailedSystemException<EXCEPTION_DATATYPE_MISALIGNMENT> DatatypeMisalignementException;
typedef DetailedSystemException<EXCEPTION_PRIV_INSTRUCTION> PrivilegedInstructionException;
typedef DetailedSystemException<EXCEPTION_IN_PAGE_ERROR> InPageException;
typedef DetailedSystemException<EXCEPTION_ILLEGAL_INSTRUCTION> IllegalInstructionException;
typedef DetailedSystemException<EXCEPTION_NONCONTINUABLE_EXCEPTION> NoncontinuableException;
typedef DetailedSystemException<EXCEPTION_STACK_OVERFLOW> StackOverflowException;
typedef DetailedSystemException<EXCEPTION_INVALID_DISPOSITION> InvalidDispositionException;
typedef DetailedSystemException<EXCEPTION_GUARD_PAGE> GuardPageException;
typedef DetailedSystemException<EXCEPTION_INVALID_HANDLE> InvalidHandleException;

#endif
