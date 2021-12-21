/*
$Workfile: /TOP/CM/SW/_Inc/utAssert.h xdepedro $

DESCRIPTION:

NOTICE:		Copyright 2000, Instrumentation Laboratory,
			Lexington, MA.  All rights reserved

REVISION HISTORY:

$Log: utAssert.h $
Revision Systelab_CM_WorkInProgress_xdepedro/2 2014/10/15 16:25:37 xdepedro
No Comment Entered

Revision NTE190_CM_Current_jpedro/4 2011/01/04 18:28:21 jpedro
Clean some todo tasks

Revision NTE190_CM_Current_jpedro/2 2009/07/20 19:19:48 jpedro
	New assert function

 * 
 * 1     28/03/07 11:41 Admin
 * 
 * 10    13/07/06 16:35 Nestor
 * 
 * 9     12/07/06 15:15 Nestor
 * 
 * 8     4/05/06 15:47 Nestor
 * 
 * 7     24/09/03 18:12 Jpedro
 * DocJet update
 * 
 * 6     13/02/03 11:35 Jpedro
 * Update
 * 
 * 5     28/01/03 18:05 Xtapia
 * Replaced UNICODE _THIS_FILE_ macro by  _THIS_CHAR_FILE_ in the rest of
 * assertions
 * 
 * 4     28/01/03 14:50 Jpedro
 * _assert not unicode. Found a workaround
 * 
 * 3     24/01/03 19:34 Xtapia
 * Replaced assert() macro by _assert() method. Reason: to change __FILE__
 * expansion, that causes too long strings appear in assertion dialogs, by
 * our own short filename definition: _THIS_FILE_.
 * 
 * 2     12/12/02 10:35 Xtapia
 * Added assert configuration support
 * 
*/

#ifndef UTASSERT_H
#define UTASSERT_H


// Group = CM-UT-HLD

#if !defined(D_NO_ASSERTIONS)

#ifdef NDEBUG
	#undef NDEBUG
	#include <assert.h>
	#define NDEBUG
#else
	#include <assert.h>
#endif

namespace utAsserts
{
	extern bool enable;
	extern void AssertFailed(const wchar_t * msg, const wchar_t * file, int line);
}

#if _MSC_VER >= 1400

#define _CRT_TCHAR(_String) (const _TCHAR*)(CString(_String))

#else

#define _CRT_WIDE(_String) _String
#define _CRT_TCHAR(_String) _String
#define _wassert _assert

#endif

// Assert Arbitrary Implementation Conditions
#if !defined(D_NO_ASSERT_IMP)
// Assertion macro provides the debug functionality of the assertion macros besides 
// the integration with the traces. An assertion failure will be logged as an error 
// and the comment will be the failed assertion, besides calling the assert method. 
// In fact the assertion has not failed, but done its job: it has detected an unacceptable 
// situation. 
#define ASSERT_(f) if (!(f)) utAsserts::AssertFailed(_T ( #f ),(_THIS_FILE_), (__LINE__));
#else
	#define ASSERT_(f) ((void)0)
#endif
#endif

// Description:
// Assertion macros provide the debug functionality of the assertion macros besides the integration with the TU through traces. An assertion failure will be logged as an error and the comment will be the failed assertion, besides calling the assert method. In fact the assertion has not failed, but done its job: it has detected an unacceptable situation. The multiple parameters versions expand in multiple assertions.
//
// Returns:
// true if assertion success, 
//
// Parameters:
//  c1
// Specifies a boolean expression (including pointer values) that evaluates to nonzero or 0.
//
// Remarks:
// ·	Evaluates its argument (assertion predicate) either to true or false. If the condition is true (nonzero), it does nothing and execution continues. If the result is false (0), the macro traces a diagnostic message and transfer the control to an assertion action (aborts the program.
// ·	Assertions should not have side effects.
// ·	Functions that implement assertions should be const, and only const member functions should be used in assertion expressions.
// ·	Be careful to avoid loops if your assertion expressions call member functions.
// Assert Arbitrary Implementation Conditions
// bool ASSERT_( c1);
// Assert Preconditions
// bool PRE_( c1);
// Assert Postconditions
// bool POST_( c1);
// Assert Loops
// bool LOOP_( c1);
//
// See Also:
//	PRE_, POST_, LOOP_
//
//// Assert pre and post conditions with trace
//#if !defined(D_NO_ASSERT_PRE)
////
//// See Also:
////	ASSERT_, POST_, LOOP_
////
//	#define PRE_(f) if (!(f) ){ \
//					utTrace(_THIS_SUBSYSTEM_, _T ( #f ), (T_ERROR), (_THIS_FILE_), (__LINE__),utTrace::VERBOSE); \
//					if(utAsserts::enable) {(void)( (f) || (_wassert( _CRT_WIDE(#f), _CRT_TCHAR(_THIS_CHAR_FILE_), __LINE__), 0) );} }
//#else
//	#define PRE_(f) ((void)0)
//#endif
//
//#if !defined(D_NO_ASSERT_POST)	
////
//// See Also:
////	ASSERT_, PRE_, LOOP_
////
//	#define POST_(f) if (!(f) ){ \
//					utTrace(_THIS_SUBSYSTEM_, _T ( #f ), (T_ERROR), (_THIS_FILE_), (__LINE__),utTrace::VERBOSE); \
//					if(utAsserts::enable) {(void)( (f) || (_wassert( _CRT_WIDE(#f), _CRT_TCHAR(_THIS_CHAR_FILE_), __LINE__), 0) );} }
//#else
//	#define POST_(f) ((void)0)
//#endif
//
//// Assert loops
//#if !defined(D_NO_ASSERT_LOOP)
//// See Also:
////	ASSERT_, _PRE, POST_
////
//	#define LOOP_(f) if (!(f) ){ \
//					utTrace(_THIS_SUBSYSTEM_, _T ( #f ), (T_ERROR), (_THIS_FILE_), (__LINE__),utTrace::VERBOSE); \
//					if(utAsserts::enable) {(void)( (f) || (_wassert( _CRT_WIDE(#f), _CRT_TCHAR(_THIS_CHAR_FILE_), __LINE__), 0) );} }
//#else
//	#define LOOP_(f)  ((void)0)
//#endif
//
//#else
//	#define ASSERT_(f)  ((void)0)
//	#define PRE_(f)     ((void)0)
//	#define POST_(f)    ((void)0)
//	#define LOOP_(f)    ((void)0)
//#endif

// Assert Support Macros
//	Range Membership
//		 IN_RANGE_(x, low, high);
//	Set Membership
//		 IN3_(x,a,b);		IN4_(x,a,b,c);	    IN5_(x,a,b,c,d);
//	Zero Checking
//	 	 NON_ZERO2_(a,b);   NON_ZERO3_(a,b,c);	NON_ZERO4_(a,b,c,d);
//	 	 IS_ZERO2_(a,b);	IS_ZERO3_(a,b c);	IS_ZERO4_(a,b,c,d);
//	 	 GT_ZERO2_(a,b);	GT_ZERO3_(a,b,c);	GT_ZERO4_(a,b,c,d);
//	Imply
//		 IMPLIES_(p,c);
// Description:
// Useful macros intended to use in precondition and postcondition expressions.
// Return:
// logical true or false
// Parameters:
// x, a, b, c, d, low, high
// Specifies values.
// p, c
// Specifies a boolean expression (including pointer values) that evaluates to nonzero or 0.
// 
// Examples:
// 	
// 	void g( int i, char str[])
// 	{
//    PRE_( str);
//    PRE_( str[0]);
//    PRE_( IN_RANGE_( i, 0, 8 ));
//    //...
//  }
// 
// void f( int count, int *items)
// {
//    PRE_( IMPLIES_( count, items ));
//    //...
// }
//

//#define IN_RANGE_(x,low,high)	((low)<=(x)&&(x)<=(high))
//
//#define IN3_(x,a,b)			((x)==(a)||(x)==(b))
//#define IN4_(x,a,b,c)		((x)==(a)||(x)==(b)||(x)==(c))
//#define IN5_(x,a,b,c,d)		((x)==(a)||(x)==(b)||(x)==(c) ||(x)==(d))
//
//#define NON_ZERO2_(a,b)		((a)&&(b))
//#define NON_ZERO3_(a,b,c)	((a)&&(b) &&(c))
//#define NON_ZERO4_(a,b,c,d)	((a)&&(b) &&(c) &&(d))
//
//#define IS_ZERO2_(a,b)		((a)==0&&(b)==0)
//#define IS_ZERO3_(a,b,c)	((a)==0&&(b)==0&&(c)==0)
//#define IS_ZERO4_(a,b,c,d)	((a)==0&&(b)==0&&(c)==0&&(d)==0)
//
//#define GT_ZERO2_(a,b)		((a)>0&&(b)>0)
//#define GT_ZERO3_(a,b,c)	((a)>0&&(b)>0&&(c)>0)
//#define GT_ZERO4_(a,b,c,d)	((a)>0&&(b)>0&&(c)>0&&(d)>0)
//
//#define IMPLIES_(p,c)		(!(p)||(c))

// Control
// The assertion can be enabled or disabled at translation time totally or partially in order to have the ability to control the BIT the system uses.

// Defines				Assertion checking
// (default)			Enables all assertion checking
// D_NO_ASSERTIONS		Disables all assertion instrumentation
// D_NO_ASSERT_IMP		Disables implementation assertion checking: ASSERT_ macro definition
// D_NO_ASSERT_PRE		Disables precondition assertion checking: PRE_ macro definition
// D_NO_ASSERT_POST		Disables postcondition assertion checking: POST_ macro definition
// D_NO_ASSERT_LOOP		Disables loop assertion checking: LOOP_ macro definition

#endif /* UTASSERT_H */
