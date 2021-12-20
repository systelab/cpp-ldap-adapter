/*
$Workfile: /TOP/CM/SW/Utility Library/utDateTimeSpan.cpp xdepedro $

DESCRIPTION:

NOTICE:		Copyright 2001, Instrumentation Laboratory,
			Lexington, MA.  All rights reserved

REVISION HISTORY:

$Log: utDateTimeSpan.cpp $
Revision Systelab_CM_WorkInProgress_xdepedro/2 2014/10/15 16:25:37 xdepedro
No Comment Entered

Revision NTE190_CM_Current_jpedro/2 2011/12/14 10:26:55 jpedro
Added Format method

Revision NTE54_CM_700_xrosell/4 2009/07/13 12:33:53 xrosell
	5000001307: "Instrument Status alarms graph to display frequency by time instead counter" -> add year and month unit conversions

Revision NTE54_CM_700_xrosell/2 2009/07/02 11:01:19 xrosell
	5000001307: "Instrument Status alarms graph to display frequency by time instead counter" -> Interface definition.

Revision NTE76_CM_700_xdepedro/2 2008/12/11 15:19:54 xdepedro
	Modified run jobs session progress bar.

 * 
 * 1     28/03/07 11:42 Admin
 * 
 * 5     25/04/05 9:31 Nestor
 * .NTE Compatibility
 * 
 * 4     18/06/02 15:45 Joanb
 * Added GetTotalMinutes as a public member.
 * 
 * 3     21/01/02 17:29 Jpedro
 * Added operator double
 * 
 * 2     18/12/01 11:19 Joanb
 * 
 *    Rev 1.3   21 Jun 2001 16:00:48   JPTeruel
 * Added new methods
 * 
 *    Rev 1.2   19 Jun 2001 08:51:32   JPTeruel
 * Added methods for the DB
 * 
 *    Rev 1.1   18 Jun 2001 13:40:28   JPTeruel
 * New constructor added
 * 
 *    Rev 1.0   13 Jun 2001 10:40:54   JPTeruel
 * Initial Revision
 
*/

#include "stdafx.h"
#include "utCommon.h"
#include "utExceptions.h"
#include "utDateTimeSpan.h"

// FILE LITERALS
static const CString sFile(_T("utDateTimeSpan.cpp"));

utDateTimeSpan::utDateTimeSpan()
:COleDateTimeSpan()
{
}


utDateTimeSpan::utDateTimeSpan( double dblSpanSrc )
:COleDateTimeSpan(dblSpanSrc)
{
}

utDateTimeSpan::utDateTimeSpan( long lDays, int nHours, int nMins, int nSecs )
:COleDateTimeSpan(lDays,nHours,nMins,nSecs)
{
}

utDateTimeSpan::utDateTimeSpan( const utDateTimeSpan& dateSpanSrc )
:COleDateTimeSpan(dateSpanSrc.m_span)
{
}

const utDateTimeSpan& utDateTimeSpan::operator=( const utDateTimeSpan& dateSpanSrc )
{
	if (this!=&dateSpanSrc)
	{
		COleDateTimeSpan::operator =(dateSpanSrc);
	}
	return *this;
}

double utDateTimeSpan::GetDateTimeSpanDB(void) const
{
	return m_span;
}
	
void utDateTimeSpan::SetDateTimeSpanDB(const double & days)
{
	*this = utDateTimeSpan(days);
}

long utDateTimeSpan::GetDays( ) const
{
	return COleDateTimeSpan::GetDays();
}

long utDateTimeSpan::GetHours( ) const
{
	return COleDateTimeSpan::GetHours();
}

long utDateTimeSpan::GetMinutes( ) const
{
	return COleDateTimeSpan::GetMinutes();
}

long utDateTimeSpan::GetSeconds( ) const
{
	return COleDateTimeSpan::GetSeconds();
}

double utDateTimeSpan::GetTotalDays( ) const
{
	return m_span;
}
double utDateTimeSpan::GetTotalHours() const
{
	return COleDateTimeSpan::GetTotalHours();
}

double utDateTimeSpan::GetTotalMinutes() const
{
	return COleDateTimeSpan::GetTotalMinutes();
}

double utDateTimeSpan::GetTotalSeconds() const
{
	return COleDateTimeSpan::GetTotalSeconds();
}

CString utDateTimeSpan::Format(const CString &  pFormat) const
{
	return COleDateTimeSpan::Format(pFormat);
}

bool utDateTimeSpan::operator==( const utDateTimeSpan& dateSpan ) const
{
	return COleDateTimeSpan::operator==(dateSpan)==TRUE;
}

bool utDateTimeSpan::operator!=( const utDateTimeSpan& dateSpan ) const
{
	return COleDateTimeSpan::operator!=(dateSpan)==TRUE;
}
bool utDateTimeSpan::operator<( const utDateTimeSpan& dateSpan ) const
{
	return COleDateTimeSpan::operator<(dateSpan)==TRUE;
}
bool utDateTimeSpan::operator>( const utDateTimeSpan& dateSpan ) const
{
	return COleDateTimeSpan::operator>(dateSpan)==TRUE;
}
bool utDateTimeSpan::operator<=( const utDateTimeSpan& dateSpan ) const
{
	return COleDateTimeSpan::operator<=(dateSpan)==TRUE;
}
bool utDateTimeSpan::operator>=( const utDateTimeSpan& dateSpan ) const
{
	return COleDateTimeSpan::operator>=(dateSpan)==TRUE;
}

void utDateTimeSpan::SetDateTimeSpan( long lDays, int nHours, int nMins, int nSecs )
{
	COleDateTimeSpan::SetDateTimeSpan( lDays, nHours, nMins, nSecs );
}

utDateTimeSpan utDateTimeSpan::operator+(const utDateTimeSpan& dateSpan) const
{
	return static_cast<utDateTimeSpan>(COleDateTimeSpan::operator+(dateSpan));
}

utDateTimeSpan utDateTimeSpan::operator-(const utDateTimeSpan& dateSpan) const
{
	return static_cast<utDateTimeSpan>(COleDateTimeSpan::operator-(dateSpan));
}

const utDateTimeSpan& utDateTimeSpan::operator+=(const utDateTimeSpan dateSpan)
{
	COleDateTimeSpan::operator+=(dateSpan);
	return *this;
}

const utDateTimeSpan& utDateTimeSpan::operator-=(const utDateTimeSpan dateSpan)
{
	COleDateTimeSpan::operator-=(dateSpan);
	return *this;
}

utDateTimeSpan::operator double( ) const
{
	 return COleDateTimeSpan::operator double();
}

utDateTimeSpan utDateTimeSpan::Convert (int i_value, dtUnit i_Unit)
{
	utDateTimeSpan spanValue;
	switch (i_Unit)
	{
	case dtUnit::UT_DAY:
			spanValue.SetDateTimeSpan (i_value, 0, 0, 0);
		break;
		case dtUnit::UT_HOUR:
			spanValue.SetDateTimeSpan (0, i_value, 0, 0);
		break;
		case dtUnit::UT_MINUTE:
			spanValue.SetDateTimeSpan (0, 0, i_value, 0);
		break;
		case dtUnit::UT_SECOND:
			spanValue.SetDateTimeSpan (0, 0, 0, i_value);
		break;
		case dtUnit::UT_WEEK:
			spanValue.SetDateTimeSpan (7 * i_value, 0, 0, 0);
		break;
		//Year and month are not "exactly", while so they are day, hour, minute and seconds 
		case dtUnit::UT_YEAR:
		case dtUnit::UT_MONTH:
		default:
			throw utRuntimeError(MSG_::INVALID_TIMESPAN_CONVERT_UNIT, sFile, __LINE__);
		break;
	}
	return spanValue;
}
