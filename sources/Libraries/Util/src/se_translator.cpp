///*
// Copyright (c) 2001 
// Author: Konstantin Boukreev 
// E-mail: konstantin@mail.primorye.ru 
// Created: 25.12.2001 15:18:21
// Version: 1.0.0
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Konstantin Boukreev makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//
//*/
//
////#include "stdafx.h"
//#include "util.h"
//#include <iostream>
//#include <tchar.h>
//#include <math.h>
//#include "stdio.h"
//#include "stdlib.h"
//
//#include "se_translator.h"
//
//// 0x40010005 Control C
//// 0xc0000135 Dll Not Found
//// 0xc0000142 Dll Initialization failed
//// 0xc06d007e Module Not Found
//// 0xc06d007f Procedure Not Found
//// 0x40010008 Control-Break
//
//#define EXCEPTION_NO_MEMORY STATUS_NO_MEMORY
//
//// static 
//void se_translator::translator(unsigned, EXCEPTION_POINTERS * pexp)
//{
//	switch (pexp->ExceptionRecord->ExceptionCode)
//	{
//	case EXCEPTION_NO_MEMORY:					throw no_memory(pexp);				break;
//	case EXCEPTION_ACCESS_VIOLATION:			throw access_violation(pexp);		break;
//	case EXCEPTION_DATATYPE_MISALIGNMENT:		throw datatype_misalignment(pexp);	break;
//	case EXCEPTION_BREAKPOINT:					throw breakpoint(pexp);				break;
//	case EXCEPTION_SINGLE_STEP:					throw single_step(pexp);			break;
//	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		throw array_bounds_exceeded(pexp);	break;
//	case EXCEPTION_FLT_DENORMAL_OPERAND:		throw flt_denormal_operand(pexp);	break;
//	case EXCEPTION_FLT_DIVIDE_BY_ZERO:			throw flt_divide_by_zero(pexp);		break;
//	case EXCEPTION_FLT_INEXACT_RESULT:			throw flt_inexact_result(pexp);		break;
//	case EXCEPTION_FLT_INVALID_OPERATION:		throw flt_invalid_operation(pexp);	break;
//	case EXCEPTION_FLT_OVERFLOW:				throw flt_overflow(pexp);			break;
//	case EXCEPTION_FLT_STACK_CHECK:				throw flt_stack_check(pexp);		break;
//	case EXCEPTION_FLT_UNDERFLOW:				throw flt_underflow(pexp);			break;
//	case EXCEPTION_INT_DIVIDE_BY_ZERO:			throw int_divide_by_zero(pexp);		break;
//	case EXCEPTION_INT_OVERFLOW:				throw int_overflow(pexp);			break;
//	case EXCEPTION_PRIV_INSTRUCTION:			throw priv_instruction(pexp);		break;
//	case EXCEPTION_IN_PAGE_ERROR:				throw in_page_error(pexp);			break;
//	case EXCEPTION_ILLEGAL_INSTRUCTION:			throw illegal_instruction(pexp);	break;
//	case EXCEPTION_NONCONTINUABLE_EXCEPTION:	throw noncontinuable_exception(pexp); break;
//	case EXCEPTION_STACK_OVERFLOW:				throw stack_overflow(pexp);			break;
//	case EXCEPTION_INVALID_DISPOSITION:			throw invalid_disposition(pexp);	break;
//	case EXCEPTION_GUARD_PAGE:					throw guard_page(pexp);				break;
//	case EXCEPTION_INVALID_HANDLE:				throw invalid_handle(pexp);			break;
//	case 0xE06D7363 :							throw microsoft_cpp(pexp);			break;
//	default:									throw exception(pexp);				break;
//	};		
//}
//
//// static 
//const char * se_translator::name(unsigned code)
//{
//	static char buf [11] = {'0', 'x', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
//
//	switch (code)
//	{
//	case EXCEPTION_NO_MEMORY:					return "No Memory";
//	case EXCEPTION_ACCESS_VIOLATION :			return "Access Violation";
//	case EXCEPTION_DATATYPE_MISALIGNMENT :		return "Datatype Misalignment";
//	case EXCEPTION_BREAKPOINT :					return "Breakpoint";
//	case EXCEPTION_SINGLE_STEP :				return "Single Step";
//	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :		return "Array Bounds Exceeded";
//	case EXCEPTION_FLT_DENORMAL_OPERAND :		return "Float Denormal Operand";
//	case EXCEPTION_FLT_DIVIDE_BY_ZERO :			return "Float Divide by Zero";
//	case EXCEPTION_FLT_INEXACT_RESULT :			return "Float Inexact Result";
//	case EXCEPTION_FLT_INVALID_OPERATION :		return "Float Invalid Operation";
//	case EXCEPTION_FLT_OVERFLOW :				return "Float Overflow";
//	case EXCEPTION_FLT_STACK_CHECK :			return "Float Stack Check";
//	case EXCEPTION_FLT_UNDERFLOW :				return "Float Underflow";
//	case EXCEPTION_INT_DIVIDE_BY_ZERO :			return "Integer Divide by Zero";
//	case EXCEPTION_INT_OVERFLOW :				return "Integer Overflow";
//	case EXCEPTION_PRIV_INSTRUCTION :			return "Privileged Instruction";
//	case EXCEPTION_IN_PAGE_ERROR :				return "In Page Error";
//	case EXCEPTION_ILLEGAL_INSTRUCTION :		return "Illegal Instruction";
//	case EXCEPTION_NONCONTINUABLE_EXCEPTION :	return "Noncontinuable Exception";
//	case EXCEPTION_STACK_OVERFLOW :				return "Stack Overflow";
//	case EXCEPTION_INVALID_DISPOSITION :		return "Invalid Disposition";
//	case EXCEPTION_GUARD_PAGE :					return "Guard Page";
//	case EXCEPTION_INVALID_HANDLE :				return "Invalid Handle";
//	case 0xE06D7363 :							return "Microsoft C++ Exception";
//	default :									return _itoa(code, &buf[2], 16);
//	};		
//}
//
//const char * se_translator::description(unsigned code)
//{
//	switch (code)
//	{
//	case EXCEPTION_NO_MEMORY:					return "The allocation attempt failed because of a lack of available memory or heap corruption.";
//	case EXCEPTION_ACCESS_VIOLATION :			return "The thread attempted to read from or write to a virtual address for which it does not have the appropriate access";
//	case EXCEPTION_DATATYPE_MISALIGNMENT :		return "The thread attempted to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries, 32-bit values on 4-byte boundaries, and so on";
//	case EXCEPTION_BREAKPOINT :					return "A breakpoint was encountered";
//	case EXCEPTION_SINGLE_STEP :				return "A trace trap or other single-instruction mechanism signaled that one instruction has been executed";
//	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :		return "The thread attempted to access an array element that is out of bounds, and the underlying hardware supports bounds checking";
//	case EXCEPTION_FLT_DENORMAL_OPERAND :		return "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value";
//	case EXCEPTION_FLT_DIVIDE_BY_ZERO :			return "The thread attempted to divide a floating-point value by a floating-point divisor of zero";
//	case EXCEPTION_FLT_INEXACT_RESULT :			return "The result of a floating-point operation cannot be represented exactly as a decimal fraction";
//	case EXCEPTION_FLT_INVALID_OPERATION :		return "This exception represents any floating-point exception not included in this list";
//	case EXCEPTION_FLT_OVERFLOW :				return "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type";
//	case EXCEPTION_FLT_STACK_CHECK :			return "The stack overflowed or underflowed as the result of a floating-point operation";
//	case EXCEPTION_FLT_UNDERFLOW :				return "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type";
//	case EXCEPTION_INT_DIVIDE_BY_ZERO :			return "The thread attempted to divide an integer value by an integer divisor of zero";
//	case EXCEPTION_INT_OVERFLOW :				return "The result of an integer operation caused a carry out of the most significant bit of the result";
//	case EXCEPTION_PRIV_INSTRUCTION :			return "The thread attempted to execute an instruction whose operation is not allowed in the current machine mode";
//	case EXCEPTION_IN_PAGE_ERROR :				return "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network";
//	case EXCEPTION_ILLEGAL_INSTRUCTION :		return "The thread tried to execute an invalid instruction";
//	case EXCEPTION_NONCONTINUABLE_EXCEPTION :	return "The thread attempted to continue execution after a noncontinuable exception occurred";
//	case EXCEPTION_STACK_OVERFLOW :				return "The thread used up its stack";
//	case EXCEPTION_INVALID_DISPOSITION :		return "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception";
//	case EXCEPTION_GUARD_PAGE :					return "Guard Page Exception";
//	case EXCEPTION_INVALID_HANDLE :				return "Invalid Handle Exception";
//	case 0xE06D7363 :							return "Microsoft C++ Exception";
//	default :									return "Unknown Exception";
//	};		
//}