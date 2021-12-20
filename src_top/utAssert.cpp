
#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
//#include "utGlobal.h"
//#include "utExceptions.h"
#include "utAssert.h"


#ifndef D_NO_ASSERTIONS

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: XT  DATE: 09-Dec-2002 REASON: Initial version
// AUTHOR: JPT  DATE: 20/07/2009 REASON: Trace the assert in release mode
// AUTHOR: JAznar DATE: 01-Oct-2012 REASON: Assert always enabled in debug mode.
namespace utAsserts
{
	bool IsAssertEnable()
	{
		static const int ENABLE_LEN = 5;

		TCHAR value[ENABLE_LEN];

		GetPrivateProfileString( 
			_T("UT"),					// section name
			_T("ASSERT_ENABLE"),			// key name
			_T("0"),					// default string
			value,						// destination buffer
			ENABLE_LEN - 1,				// size of destination buffer
			_T(".\\LDAP.ini")			// initialization file name
		);

		return _ttoi(value) ? true : false;
	}

#ifdef NDEBUG
	bool enable = IsAssertEnable();
#else
	bool enable = true;
#endif


	void AssertFailed(const wchar_t * msg, const wchar_t * file, int line)
	{
		// NOTE: Create a utLogicError to force the trace of the assertion. It could also 
		//thrown the exception if we would like to manage the assert like an exception.
		//utLogicError logicError(CString(_T("ASSERTION FAILED: ")) + msg, CString(file), line);
		if (utAsserts::enable)
		{			
			_wassert( msg, file, line);
		} 
		
	}
}

#endif


