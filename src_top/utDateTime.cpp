

#include "stdafx.h"
//#include "utDataFormat.h"
//#include "utCommon.h"
//#include "utExceptions.h"
#include "utDateTime.h"

//////////////////////////////////////////////////////////////////////////////

DATE utDateTime::DT_LOWERLIMIT = COleDateTime(1753,1,1,0,0,0).m_dt;
DATE utDateTime::DT_UPPERLIMIT = COleDateTime(9999,12,31,0,0,0).m_dt;
const DATE utDateTime::NULL_DATE_VALUE = DATE(0.0); 
const DATE utDateTime::FIRST_TIME_LOGON = utDateTime(utDateTime::DT_LOWERLIMIT) + utDateTimeSpan(1, 0, 0, 0);

// FILE LITERALS
static const CString sFile(_T("utDateTime.cpp"));
static const int MAX_BUFFER_DATE = 256;
static const int MAX_BUFFER_TIME = 256;

/////////////////////////////////////////////////////////////////////////////////////
// utDateTime
/////////////////////////////////////////////////////////////////////////////////////

utDateTime::utDateTime()
{
	Reset();
}
utDateTime::utDateTime(bool bCurrentTime)
{
	if (bCurrentTime)
	{
		//Default constructor is up with current date and time
		ResetToCurrentTime();
	}
	else
	{
		Reset();
	}
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Apply IsValid over cDT.
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Use utCommon instead of literals
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Removed new utDataFormat in all the constructors
utDateTime::utDateTime(const utDateTime & cDT)
{
	if (!cDT.IsValid())
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}	
	*this = cDT;
}

utDateTime::utDateTime( DATE dtSrc):
            COleDateTime(dtSrc)     
{
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

// AUTHOR: JPT  DATE:  06/21/2001 REASON: Use utCommon instead of literals
utDateTime::utDateTime(const SYSTEMTIME& systimeDT) : COleDateTime(systimeDT)
{
	if (!IsValid())
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

utDateTime::utDateTime(const __time32_t& obj) : COleDateTime(obj)
{
	if (obj==-1) 
	{	
		Reset();
	}
	if (!IsValid())
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

utDateTime::utDateTime(const __time64_t& obj) : COleDateTime(obj)
{
	if (obj == -1)
	{
		Reset();
	}
	if (!IsValid())
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JB  DATE:  2-May-2001 REASON: Remove "critical section"
utDateTime::utDateTime(int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec ) : 
	COleDateTime(iYear, iMonth, iDay, iHour, iMin, iSec )
{
	if (!IsValid())
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}
/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Removed 'delete utDataFormat'
utDateTime::~utDateTime()
{

}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: NS  DATE:  Dec 19, 2002: memset has been added
// AUTHOR: NS  DATE:  May 9, 2003: dt.tm_isdst = -1 has been added to have the C 
// run-time library code compute whether standard time or daylight savings time 
// is in effect
__time32_t utDateTime::GetTimeT() const
{
	if(!IsEmpty())
	{
		tm dt;

		memset(&dt, 0, sizeof(tm));

		dt.tm_year = GetYear() - 1900;

		dt.tm_mon = GetMonth() - 1;

		dt.tm_mday = GetDay();

		dt.tm_hour = GetHour();

		dt.tm_min = GetMinute();

		dt.tm_sec = GetSecond();

		// less than zero to have the C run-time library code 
		// compute whether standard time or daylight savings time is in effect
		dt.tm_isdst = -1; 

		return _mktime32(&dt);
	}
	else
	{
		return -1;
	}
}

__time64_t utDateTime::GetTimeT64() const
{
	if (!IsEmpty())
	{
		tm dt;

		memset(&dt, 0, sizeof(tm));

		dt.tm_year = GetYear() - 1900;

		dt.tm_mon = GetMonth() - 1;

		dt.tm_mday = GetDay();

		dt.tm_hour = GetHour();

		dt.tm_min = GetMinute();

		dt.tm_sec = GetSecond();

		// less than zero to have the C run-time library code 
		// compute whether standard time or daylight savings time is in effect
		dt.tm_isdst = -1;

		return _mktime64(&dt);
	}
	else
	{
		return -1;
	}
}

////////////////////////////////////////////////////////////////////
int utDateTime::GetYear() const
{
	return COleDateTime::GetYear();
}

int utDateTime::GetMonth() const
{
	return COleDateTime::GetMonth();
}

int utDateTime::GetDay() const
{
	return COleDateTime::GetDay();
}

int utDateTime::GetDayOfWeek() const
{
	return COleDateTime::GetDayOfWeek();
}

int utDateTime::GetHour() const
{
	return COleDateTime::GetHour();
}

int utDateTime::GetMinute() const
{
	return COleDateTime::GetMinute();
}

int utDateTime::GetSecond() const
{
	return COleDateTime::GetSecond();
}

bool utDateTime::IsEmpty() const
{
	return (COleDateTime::GetStatus() == COleDateTime::null);
}
/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JB  DATE:  4-May-2001 REASON:Changed meaning. Now, a null date time is also considered valid
bool utDateTime::IsValid() const
{
	return
		(
		 (COleDateTime::GetStatus() == COleDateTime::null) ||
		 (
		  ((COleDateTime::GetStatus() == COleDateTime::valid) && m_dt >= (DT_LOWERLIMIT - 1))
		 )
		);
}

bool utDateTime::IsPrintable() const
{
	return (IsValid() && 
		   COleDateTime::GetStatus() != COleDateTime::null);
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JB  DATE:  4-May-2001 REASON:Fixed error: resetting by m_dt = 0
void utDateTime::Reset(void)
{
	m_dt = NULL_DATE_VALUE;
	COleDateTime::SetStatus(COleDateTime::null);
}
/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Use directly the base class
void utDateTime::ResetToCurrentTime()
{
	//COleDateTime cTemp = COleDateTime::GetCurrentTime();
	//*((COleDateTime*)this) = cTemp;
	COleDateTime::operator =(COleDateTime::GetCurrentTime());
}


/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JB  DATE:  2-May-2001 REASON: Removed SetStatus statement
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Modified the copy using directly the base class
const utDateTime& utDateTime::operator =(const utDateTime& cDT)
{
	if(this != &cDT)
	{		
		COleDateTime::operator =(cDT);
		//*((COleDateTime*)this)=(COleDateTime) cDT;
		if (!IsValid())
		{
			throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
		}
	}
	return *this;
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Use directly the base class
const utDateTime& utDateTime::operator =(const SYSTEMTIME& systimeDT)
{
	//*((COleDateTime*)this) = COleDateTime(systimeDT.wYear, systimeDT.wMonth, systimeDT.wDay, systimeDT.wHour, systimeDT.wMinute, systimeDT.wSecond);
	COleDateTime::operator =(systimeDT);

	if (!IsValid()) 
	{
		COleDateTime::SetStatus(COleDateTime::invalid); // Needed for dates before LOW_LIMIT
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);	
	}
	return *this;
}

/*
utDateTime::operator const CString()
{
	return GetDateTime();
}
*/

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  06/21/2001 REASON: Use directly the base class in all the operators
bool utDateTime::operator ==(const utDateTime& cDT) const
{
	//return ((*((COleDateTime*) this) == ((COleDateTime) cDT)) == TRUE);
	return COleDateTime::operator==(cDT)==TRUE;
}

bool utDateTime::operator !=(const utDateTime& cDT) const
{
	//return ((*((COleDateTime*) this) != ((COleDateTime) cDT)) == TRUE);
	return COleDateTime::operator!=(cDT)==TRUE;
}

bool utDateTime::operator >(const utDateTime& cDT) const
{
	//return ((*((COleDateTime*) this) > ((COleDateTime) cDT)) == TRUE);
	return COleDateTime::operator>(cDT)==TRUE;
}

bool utDateTime::operator >=(const utDateTime& cDT) const
{
	//return ((*((COleDateTime*) this) >= ((COleDateTime) cDT)) == TRUE);
	return COleDateTime::operator>=(cDT)==TRUE;
}

bool utDateTime::operator <(const utDateTime& cDT) const
{
	//return ((*((COleDateTime*) this) < ((COleDateTime) cDT)) == TRUE);
	return COleDateTime::operator<(cDT)==TRUE;
}

bool utDateTime::operator <=(const utDateTime& cDT) const
{
	//return ((*((COleDateTime*) this) <= ((COleDateTime) cDT)) == TRUE);
	return COleDateTime::operator<=(cDT)==TRUE;
}

void utDateTime::SetDate(int iYear, int iMonth, int iDay)
{
	COleDateTime::SetDate(iYear, iMonth, iDay);
	if (!IsValid()) 
	{
		COleDateTime::SetStatus(COleDateTime::invalid); // Needed for dates before LOW_LIMIT
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

void utDateTime::SetTime(int iHour, int iMinute, int iSecond)
{
	COleDateTime::SetTime(iHour, iMinute, iSecond);
	if (!IsValid()) 
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

void utDateTime::SetDateTime(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond)
{
	COleDateTime::SetDateTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
	if (!IsValid()) 
	{
		COleDateTime::SetStatus(COleDateTime::invalid); // Needed for dates before LOW_LIMIT
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

void utDateTime::SetDateTime(const SYSTEMTIME& systimeDT)
{
	//(*this) = systimeDT;
	COleDateTime::operator =(systimeDT);

	if (!IsValid())
	{
		COleDateTime::SetStatus(COleDateTime::invalid); // Needed for dates before LOW_LIMIT
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	// 0 is a valid date, but we interpretate it as an empty date. 
	if (this->m_dt == 0)
	{
		Reset();
	}
}

const CString utDateTime::GetDate(void) const
{
	CString szBuffer;

	TCHAR sDate[ MAX_BUFFER_DATE + 1];

	if( IsPrintable())
	{
		SYSTEMTIME St;
		St.wYear = GetYear();
		St.wMonth = GetMonth();
		St.wDay = GetDay();
		St.wHour = 0;
		St.wMinute = 0;
		St.wSecond = 0;
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;

        if( ::GetDateFormat(LOCALE_USER_DEFAULT, 0, &St, IDataFormat.GetDateFormat(), sDate, MAX_BUFFER_DATE))
		{
			szBuffer = CString( sDate);
		}
	}

	return szBuffer;
}

const CString utDateTime::GetDate4Y() const
{
	CString szBuffer;

	TCHAR sDate[ MAX_BUFFER_DATE + 1];
	SYSTEMTIME St;

	szBuffer = _T("");
	if (IsPrintable())
	{
		St.wYear = GetYear();
		St.wMonth = GetMonth();
		St.wDay = GetDay();
		St.wHour = 0;
		St.wMinute = 0;
		St.wSecond = 0;
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
        if( ::GetDateFormat( LOCALE_USER_DEFAULT, 0, &St, IDataFormat.GetDateFormat4Y(), sDate, MAX_BUFFER_DATE))
		{
			szBuffer = sDate;
		}
	}

	return szBuffer;
}

const CString utDateTime::GetTime() const
{
	CString szBuffer;
	TCHAR sTime[ MAX_BUFFER_TIME + 1];
	SYSTEMTIME St;

	szBuffer = _T("");
	if( IsPrintable())
	{
		St.wYear = 0;
		St.wMonth = 0;
		St.wDay = 0;
		St.wHour = GetHour();
		St.wMinute = GetMinute();
		St.wSecond = GetSecond();
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
        if( ::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &St,IDataFormat.GetTimeFormat(), sTime, MAX_BUFFER_TIME))
		{
			szBuffer = CString( sTime);
		}
	}
	
	return szBuffer;
}

const CString utDateTime::GetTimeHourMinute() const
{
	CString szBuffer;
	TCHAR sTime[ MAX_BUFFER_TIME + 1];

	if( IsPrintable())
	{
		SYSTEMTIME St;
		St.wYear = 0;
		St.wMonth = 0;
		St.wDay = 0;
		St.wHour = GetHour();
		St.wMinute = GetMinute();
		St.wSecond = 0;
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
        if( ::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &St,IDataFormat.GetTimeHourMinuteFormat(), sTime, MAX_BUFFER_TIME))
		{
			szBuffer = CString( sTime);
		}
	}
	
	return szBuffer;
}

CString utDateTime::GetDateTime() const
{
	CString szBuffer;
	
	wchar_t sDate[MAX_BUFFER_DATE + 1] = {};
	wchar_t sTime[MAX_BUFFER_TIME + 1] = {};

	if (IsPrintable())
	{
		SYSTEMTIME St;
		St.wYear = GetYear();
		St.wMonth = GetMonth();
		St.wDay = GetDay();
		St.wHour = GetHour();
		St.wMinute = GetMinute();
		St.wSecond = GetSecond();
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
		/*const CString formatd = L"MM/dd/yyyy";
		const CString formatt = L"h:mm:ss tt";*/
		const CString formatd = IDataFormat.GetDateFormat();
		const CString formatt = IDataFormat.GetTimeFormat();

		if ((::GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, 0, &St, (LPCWSTR)formatd, sDate, MAX_BUFFER_DATE,nullptr)) &&
			(::GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, 0, &St, (LPCWSTR)formatt, sTime, MAX_BUFFER_TIME)))
		{ 
			szBuffer = CString( sDate) + CString(_T(" ")) + CString( sTime);
		}		
	}
	
	return std::move(szBuffer);
}

const CString utDateTime::GetDateTimeHourMinute() const
{
	return (GetDate() + CString(_T(" ")) + GetTimeHourMinute());
}

const CString utDateTime::GetDateTime4Y() const
{
	CString szBuffer;

	TCHAR sDate[ MAX_BUFFER_DATE + 1];
	TCHAR sTime[ MAX_BUFFER_TIME + 1];
	SYSTEMTIME St;

	szBuffer = _T("");
	if( IsPrintable())
	{
		St.wYear = GetYear();
		St.wMonth = GetMonth();
		St.wDay = GetDay();
		St.wHour = GetHour();
		St.wMinute = GetMinute();
		St.wSecond = GetSecond();
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
        if( (::GetDateFormat(LOCALE_USER_DEFAULT, 0, &St, IDataFormat.GetDateFormat4Y(), sDate, MAX_BUFFER_DATE)) &&
            (::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &St, IDataFormat.GetTimeFormat(), sTime, MAX_BUFFER_TIME)))
		{
			szBuffer = CString( sDate) + CString(_T(" ")) + CString( sTime);
		}
	}

	return szBuffer;
}

CString utDateTime::GetDateMonthYear() const
{
	CString szBuffer;

	TCHAR sDate[ MAX_BUFFER_DATE + 1];
	SYSTEMTIME St;

	szBuffer = _T("");
	if (IsPrintable())
	{
		St.wYear = GetYear();
		St.wMonth = GetMonth();
		St.wDay = GetDay();
		St.wHour = 0;
		St.wMinute = 0;
		St.wSecond = 0;
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
        if( ::GetDateFormat( LOCALE_USER_DEFAULT, 0, &St, IDataFormat.GetDateFormatMonthYear(), sDate, MAX_BUFFER_DATE))
		{
			szBuffer = sDate;
		}
	}

	return szBuffer;
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  08/02/2001 REASON: Added NullValue
double utDateTime::GetDateTimeDB(double NullValue ) const
{
	if (IsEmpty())
		return NullValue;
	else
		return this->m_dt;
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: BT  DATE:  7-Apr-2003 REASON: Added NullValue
const CString utDateTime::GetUSDateTimeDB(void) const
{
	CString szBuffer;

	TCHAR sDate[ MAX_BUFFER_DATE + 1];
	TCHAR sTime[ MAX_BUFFER_TIME + 1];
	SYSTEMTIME St;

	szBuffer = _T("");
	if (IsPrintable())
	{
		St.wYear = GetYear();
		St.wMonth = GetMonth();
		St.wDay = GetDay();
		St.wHour = GetHour();
		St.wMinute = GetMinute();
		St.wSecond = GetSecond();
		St.wMilliseconds = 0;
		utDataFormat IDataFormat;
        if( (::GetDateFormat(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT), 0, &St, _T("MM/dd/yyyy"), sDate, MAX_BUFFER_DATE)) &&
            (::GetTimeFormat(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT), 0, &St, _T("HH:mm:ss"), sTime, MAX_BUFFER_TIME)))
		{ 
			szBuffer = CString( sDate) + CString(_T(" ")) + CString( sTime);
		}
	}
	return szBuffer;
}

/////////////////////////////*** REVISIONS HISTORY ***///////////////////////////////
// AUTHOR: JPT  DATE:  08/02/2001 REASON: Added NullValue
void utDateTime::SetDateTimeDB(double DT,double NullValue)
{// If double == null value then reset

	if (DT == NullValue)
	{
		Reset();
		return;
	}

	this->m_dt = DT;
	COleDateTime::SetStatus(COleDateTime::valid);

	if (!IsValid())
	{
		COleDateTime::SetStatus(COleDateTime::invalid); // Needed for dates before LOW_LIMIT
		throw utRuntimeError(CString(_T("Setting invalid Time")), sFile, __LINE__);
	}
}

const CString utDateTime::GetTimeStamp(void)
{
	CString szBuffer;
	SYSTEMTIME localTime;

    ::GetLocalTime( &localTime);

	szBuffer.Format( _T("%d/%02d/%02d-%02hd:%02hd:%02hd.%03d"), localTime.wYear,
																localTime.wMonth,
																localTime.wDay,
																localTime.wHour,
																localTime.wMinute,
																localTime.wSecond,
																localTime.wMilliseconds);
	return szBuffer;
}

const CString utDateTime::GetMilitarDateTime(void)
{
	CString stBuffer;

	if (IsPrintable())
	{
		stBuffer = this->Format (_T("%Y%m%d%H%M%S"));
	}
	return stBuffer;
}

CString utDateTime::GetDBDateTime() const
{
	CString stBuffer;

	if (IsPrintable())
	{
		stBuffer = Format(_T("%Y-%m-%d %H:%M:%S"));
	}

	return stBuffer;
}


utDateTime utDateTime::operator +( const utDateTimeSpan& dateSpan ) const
{
	COleDateTime r =  COleDateTime::operator +(COleDateTimeSpan(dateSpan.operator double()));

	return utDateTime(r.operator double());/*
	SYSTEMTIME obj;
	if (r.GetAsSystemTime(obj))
	{		
		return utDateTime(obj);
	}
	else
	{
		ASSERT(false);
		return utDateTime();
	}*/
}

utDateTime utDateTime::operator -( const utDateTimeSpan& dateSpan ) const	
{
	COleDateTime r = COleDateTime::operator -(COleDateTimeSpan(dateSpan.GetTotalDays()));
	SYSTEMTIME obj;
	if (r.GetAsSystemTime(obj))
	{		
		return utDateTime(obj);
	}
	else
	{
		ASSERT(false);
		return utDateTime();
	}
}

utDateTime::operator DATE() const throw()
{
	return COleDateTime::operator DATE();
}

utDateTimeSpan utDateTime::operator -( const utDateTime& date ) const
{
	return utDateTimeSpan((COleDateTime::operator -(date)).operator DATE());
}


CString utDateTime::Format( LPCTSTR lpszFormat ) const
{
	return COleDateTime::Format( lpszFormat);
}

utDateTime utDateTime::GetAdjustedTime (const utDateTime & i_date, int intervalTime, dtUnit intervalUnit)
{
	utDateTime retDateTime = i_date;
	if (intervalUnit == dtUnit::UT_YEAR || intervalUnit == dtUnit::UT_MONTH)
	{
		long dwTimes = 0;
		int dwNumberOfMonths = 0;
		switch (intervalUnit)
		{
			case dtUnit::UT_YEAR:
				dwTimes = i_date.GetYear () / intervalTime;
				dwTimes *= intervalTime;
				retDateTime = utDateTime (dwTimes, 1, 1, 0, 0, 0);
			break;
			case dtUnit::UT_MONTH:
				dwNumberOfMonths = i_date.GetYear () * 12 + i_date.GetMonth () - 1;
				dwTimes = dwNumberOfMonths / intervalTime;
				dwTimes *= intervalTime;
				retDateTime = utDateTime (dwTimes / 12, 1 + dwTimes % 12, 1, 0, 0, 0);
			break;
		}
	}
	else
	{
		utDateTimeSpan interval = utDateTimeSpan::Convert (intervalTime, intervalUnit);

		double dTimes = i_date / interval;
		long dwTimes = (long) dTimes;
		dTimes = (double) dwTimes;

		double StartDate = dTimes * interval;
		retDateTime = utDateTime (StartDate);
	}
	return retDateTime;
}

int utDateTime::GetLastDayOfMonth(utDateTime & i_date)
{
	utDateTime firstDayNextMonth = (i_date.GetMonth() == 12) ? utDateTime(i_date.GetYear() + 1, 1, 1, 0, 0, 0) : utDateTime(i_date.GetYear(), i_date.GetMonth() + 1, 1, 0, 0, 0); // IL12-14708 
	utDateTime lastDayMonth = firstDayNextMonth - utDateTimeSpan::Convert(1, dtUnit::UT_DAY);

	return lastDayMonth.GetDay();
}

utDateTime utDateTime::AddTime (const utDateTime & i_date, int intervalTime, dtUnit intervalUnit)
{
	utDateTime retDateTime;
	if (intervalUnit == dtUnit::UT_YEAR)
	{
		// IL37-4990 Bad arithmetic with leap day
		if ((29 == i_date.GetDay()) && (2 == i_date.GetMonth()) && IsLeapYear(const_cast<utDateTime&>(i_date)) && !IsLeapYear(utDateTime(i_date.GetYear() + intervalTime, 1, 1, 0, 0, 0)))
		{
			retDateTime = utDateTime(i_date.GetYear() + intervalTime, i_date.GetMonth(), 28, i_date.GetHour(), i_date.GetMinute(), i_date.GetSecond());
		}
		else
		{
			retDateTime = utDateTime(i_date.GetYear() + intervalTime, i_date.GetMonth(), i_date.GetDay(), i_date.GetHour(), i_date.GetMinute(), i_date.GetSecond());
		}
	}
	else if (intervalUnit == dtUnit::UT_MONTH)
	{
		int dwYear = i_date.GetYear();
		int dwMonth = i_date.GetMonth() + intervalTime;

		// IL12-14708 Interval Time could be negative (Recover old check)
		while (dwMonth > 12)
		{
			dwMonth -= 12;
			dwYear++;
		}
		while (dwMonth < 1)
		{
			dwMonth += 12;
			dwYear--;
		}

		// IL37-4990 Take care with last day of the month
		int lastDayMonth = GetLastDayOfMonth(utDateTime(dwYear, dwMonth, 1, 0, 0, 0));
		if (i_date.GetDay() > lastDayMonth)
			retDateTime = utDateTime(dwYear, dwMonth, lastDayMonth, i_date.GetHour(), i_date.GetMinute(), i_date.GetSecond());
		else
			retDateTime = utDateTime(dwYear, dwMonth, i_date.GetDay(), i_date.GetHour(), i_date.GetMinute(), i_date.GetSecond());
	}
	else
	{
		retDateTime = i_date + utDateTimeSpan::Convert (intervalTime, intervalUnit);
	}
	return retDateTime;
}

bool utDateTime::IsLeapYear (utDateTime & date)
{
	return ((date.GetYear () % 4 == 0 && date.GetYear () % 100 != 0) || date.GetYear () % 400 == 0);
}

utDateTime CalculateRealDateFromZoneInfo (const SYSTEMTIME & sysTimeZoneInfo, int year)
{
	// wDay -> 1 to 5 1: first week on month, 5 last week on month
	CString stTemp;
	utDateTime dateCheck;
	utDateTime dateCheckIni;
	int daysToComplete;

	if (sysTimeZoneInfo.wDay >= 1 && sysTimeZoneInfo.wDay <= 4)
	{
		dateCheckIni = utDateTime (year, sysTimeZoneInfo.wMonth, (sysTimeZoneInfo.wDay - 1) * 7 + 1, 0, 0, 0);
		dateCheck = dateCheckIni;
		daysToComplete = (7 + sysTimeZoneInfo.wDayOfWeek - dateCheck.GetDayOfWeek () + 1) % 7;
		dateCheck = dateCheckIni + utDateTimeSpan (daysToComplete, sysTimeZoneInfo.wHour - 1, sysTimeZoneInfo.wMinute, sysTimeZoneInfo.wSecond);

		stTemp.Format(_T("%s"), dateCheck.GetDateTime ());
	}
	else if (sysTimeZoneInfo.wDay == 5)
	{
		dateCheckIni = utDateTime (year, sysTimeZoneInfo.wMonth + 1, 1, 0, 0, 0) - utDateTimeSpan (1, 0, 0, 0);
		dateCheck = dateCheckIni;
		daysToComplete = (7 + dateCheck.GetDayOfWeek () - sysTimeZoneInfo.wDayOfWeek - 1) % 7;
		dateCheck = dateCheckIni + utDateTimeSpan (-daysToComplete, sysTimeZoneInfo.wHour - 1, sysTimeZoneInfo.wMinute, sysTimeZoneInfo.wSecond);
		stTemp.Format(_T("%s"), dateCheck.GetDateTime ());
	}
	else
	{
		throw utRuntimeError(MSG_::INVALID_DATE, sFile, __LINE__);
	}
	return dateCheck;
}

bool utDateTime::GetDSTDates(utDateTime &ghostHour, utDateTime &repeatedHour, int year /* = utDateTime (true).GetYear () */)
{
	TIME_ZONE_INFORMATION timeZoneInformation;
	ZeroMemory(&timeZoneInformation, sizeof(timeZoneInformation));
	DWORD dwRet = GetTimeZoneInformation(&timeZoneInformation);
	if (dwRet == TIME_ZONE_ID_INVALID)
	{
		throw utRuntimeError(MSG_::INVALID_DST_ZONE, sFile, __LINE__);
		return false;
	}
	else if (dwRet == TIME_ZONE_ID_UNKNOWN)
	{
		return false;
	}
	else
	{
		ghostHour = CalculateRealDateFromZoneInfo(timeZoneInformation.DaylightDate, year);
		repeatedHour = CalculateRealDateFromZoneInfo(timeZoneInformation.StandardDate, year);
		return true;
	}
}

EDayOfWeek utDateTime::GetFirstDayOfWeek ()
{
	int dwFirstDayOfWeek = 1; // By default, if everything goes wrong return Sunday
	int dwRet = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK | LOCALE_RETURN_NUMBER, (LPWSTR)&dwFirstDayOfWeek,  sizeof (dwFirstDayOfWeek) / sizeof (TCHAR)); // 0: Monday, ..., 6 Sunday
	if (dwRet)
	{
		if (dwFirstDayOfWeek == 6) // -> Sunday: 1, Monday: 2, 3, 4, 5, 6, Saturday: 7
		{
			return EDayOfWeek::EDOW_SUNDAY;
		}
		else
		{
			dwFirstDayOfWeek +=2;
		}
	}
	return (EDayOfWeek)dwFirstDayOfWeek;
}

int utDateTime::GetFirstDayOfYear ()
{
	int dwFirstDayOfYear = 0; // By default, if everything goes wrong return First week is the one that contains 1/1
	int dwRet = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTWEEKOFYEAR | LOCALE_RETURN_NUMBER, (LPWSTR)&dwFirstDayOfYear,  sizeof (dwFirstDayOfYear) / sizeof (TCHAR)); // 0: week containing 1/1, 1: first full week after 1/1, 2: first week with 4 days of that year
	return dwFirstDayOfYear;
}
