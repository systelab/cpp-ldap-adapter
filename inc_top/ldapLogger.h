/*

$Workfile: /TOP/CM/SW/_Inc/ldapLogger.h jmruiz $

DESCRIPTION: Logging by injected references for LDAPProxy

NOTICE:		Copyright 2020, Instrumentation Laboratory,
All rights reserved

REVISION HISTORY:

$Log: ldapLogger.h $

*/

#pragma once

#ifdef TRACE_BY_INJECT
#define UT_SUBSYSTEM( subsys, file) \
		static const CString _THIS_FILE_ = _T(file);\
		static char* _THIS_CHAR_FILE_ =  file;\
		static const SUBSYSTEM_T _THIS_SUBSYSTEM_ = subsys

typedef TCHAR* SUBSYSTEM_T;

static const SUBSYSTEM_T CM_LDAP = L"LDAPRW";

enum PRIORITY_T
{
	T_ERROR = 1,
	T_WARNING = 2,
	T_INTERFACE = 3,
	T_TRACE = 4,
	T_ALL
};
#endif


class ILog
{
public:
	virtual void Log(SUBSYSTEM_T stSUBSYSTEM, wstring stMsg, wstring stFile, PRIORITY_T priority, int line) = 0;
};

#ifndef TRACE_BY_INJECT
class IndirectLog : public ILog
{
public:
	virtual void Log(SUBSYSTEM_T stSUBSYSTEM, wstring stMsg, wstring stFile, PRIORITY_T priority, int line)
	{
		_LOG_<true> Log(stSUBSYSTEM, stMsg.c_str(), stFile, priority, line);
		/*_LOG_<true>(CString (stMsg.c_str()), (T_ERROR), (stFile.c_str()), (line));*/
	}

};

#else

#include "LDAPProxy.h"

using namespace System::Runtime::InteropServices;

//static ILog *pProxyLog = nullptr;

//#include "utTrace.h"
#define TRACE_INT2(msg) 
//#define TRACE_INT2(msg) utTraceInterface __traceaux__; if(utTraces::enable_debug_traces) __traceaux__.Set( (_THIS_SUBSYSTEM_), (msg), (T_INTERFACE), (_THIS_FILE_), (__LINE__), utTraceInterface::TRACE_INTERFACE )
//#define TRACE_TC(msg) utTraceInterface __tracetc__; if(utTraces::enable_debug_traces) __tracetc__.Set( (_THIS_SUBSYSTEM_), (msg), (T_INTERFACE), (_THIS_FILE_), (__LINE__), utTraceInterface::TESTCASE )
//#define TRACE_ERR(msg)
#define TRACE_ERR(msg) if (LDAPProxy::GetLog()) \
{ \
	LDAPProxy::GetLog()->Log ((_THIS_SUBSYSTEM_), (msg), (wstring(_THIS_FILE_)), (T_ERROR), (__LINE__)); \
}

#define TRACE_EX(msgManaged) if (LDAPProxy::GetLog()) \
{ \
	wstring st2; \
	wchar_t *stringPointer = (wchar_t *)Marshal::StringToHGlobalAuto(msgManaged).ToPointer(); \
	st2 = stringPointer; \
	Marshal::FreeHGlobal(IntPtr(stringPointer)); \
	LDAPProxy::GetLog()->Log ((_THIS_SUBSYSTEM_), (st2), wstring(_THIS_FILE_),  (T_ERROR), (__LINE__)); \
}

#endif