/*
$Workfile: /TOP/CM/SW/_Inc/utDateTimeSpan.h xdepedro $

DESCRIPTION:	

NOTICE:		Copyright 2001, Instrumentation Laboratory,
			Lexington, MA.  All rights reserved

REVISION HISTORY:

$Log: utDateTimeSpan.h $
Revision Systelab_CM_WorkInProgress_xdepedro/2 2014/10/15 16:25:37 xdepedro
No Comment Entered

Revision NTE190_CM_Current_jpedro/2 2011/12/14 10:26:55 jpedro
Added Format method

Revision NTE54_CM_700_xrosell/4 2009/07/13 12:33:53 xrosell
	5000001307: "Instrument Status alarms graph to display frequency by time instead counter" -> add year and month unit conversions

Revision NTE54_CM_700_xrosell/2 2009/07/02 11:02:12 xrosell
	5000001307: "Instrument Status alarms graph to display frequency by time instead counter" -> Interface definition.

Revision NTE76_CM_700_xdepedro/2 2008/12/11 15:19:54 xdepedro
	Modified run jobs session progress bar.

 * 
 * 1     28/03/07 11:41 Admin
 * 
 * 6     25/04/05 9:31 Nestor
 * .NTE Compatibility
 * 
 * 5     9/26/03 1:03p Build
 * Docjet fixing
 * 
 * 4     18/06/02 15:45 Joanb
 * Added GetTotalMinutes as a public member.
 * 
 * 3     21/01/02 17:29 Jpedro
 * Added operator double
 * 
 * 2     18/12/01 11:20 Joanb
 * 
 *    Rev 1.4   26 Jul 2001 09:39:50   XTapia
 * Added  constant definition: maximum and minimum time span
 * 
 *    Rev 1.3   21 Jun 2001 15:55:50   JPTeruel
 * Added new methods
 * 
 *    Rev 1.2   19 Jun 2001 08:51:22   JPTeruel
 * Added methods for the DB
 * 
 *    Rev 1.1   18 Jun 2001 13:40:36   JPTeruel
 * New constructor added
 * 
 *    Rev 1.0   13 Jun 2001 11:52:04   JPTeruel
 * Initial revision.
 
*/

#if !defined(UTDATETIMESPAN_H)
#define UTDATETIMESPAN_H

// group = CM-UT-HLD

// Description:
//		It represents the different type of units that can be used as date time span.
enum class dtUnit
{
	UT_YEAR,
	UT_MONTH,
	UT_DAY,
	UT_HOUR,
	UT_MINUTE,
	UT_SECOND,
	UT_WEEK,

	UT_NOT_DEFINED = -1,
};


//////////////////////////////////////////////////////////////////////
// Description: 
//	Interface to manage time spans.
class utDateTimeSpan: protected COleDateTimeSpan
{
public:
	
	// Description:
	//	Constructors
	// See Also:
	//	COleDateTimeSpan
	utDateTimeSpan();
	explicit utDateTimeSpan( double dblSpanSrc );
	utDateTimeSpan( const utDateTimeSpan& dateSpanSrc );
	utDateTimeSpan( long lDays, int nHours, int nMins, int nSecs );

	
	const utDateTimeSpan& operator=( const utDateTimeSpan& dateSpanSrc );

	// Description:
	//		It gets the time span in a fixed format for DB purposes	
	//
	// Returns: 
	//		The number of days.
	// Note:
	//		
	// See Also:
	//		SetDateTimeSpanDB
	double GetDateTimeSpanDB(void) const;

	
	// Description:
	//		It sets the time span in a fixed format for DB purposes	
	//
	// See Also:
	//		GetDateTimeSpanDB
	void SetDateTimeSpanDB(const double & days// Reference to a double provided by the client
							);

	// See Also:
	//	COleDateTimeSpan
	long GetDays( ) const;

	// See Also:
	//	COleDateTimeSpan
	long GetHours( ) const;

	// See Also:
	//	COleDateTimeSpan
	long GetMinutes( ) const;

	// See Also:
	//	COleDateTimeSpan
	long GetSeconds( ) const;

	// See Also:
	//	COleDateTimeSpan
	double GetTotalDays( ) const;
	double GetTotalHours() const;
	double GetTotalMinutes() const;
	double GetTotalSeconds() const;
	CString Format(const CString &  pFormat) const;
	// See Also:
	//	COleDateTimeSpan
	bool operator==( const utDateTimeSpan& dateSpan ) const;
	bool operator!=( const utDateTimeSpan& dateSpan ) const;
	bool operator<( const utDateTimeSpan& dateSpan ) const;
	bool operator>( const utDateTimeSpan& dateSpan ) const;
	bool operator<=( const utDateTimeSpan& dateSpan ) const;
	bool operator>=( const utDateTimeSpan& dateSpan ) const;
	operator double( ) const;

	// plus and minus operations
	utDateTimeSpan operator+(const utDateTimeSpan& dateSpan) const;
	utDateTimeSpan operator-(const utDateTimeSpan& dateSpan) const;
	const utDateTimeSpan& operator+=(const utDateTimeSpan dateSpan);
	const utDateTimeSpan& operator-=(const utDateTimeSpan dateSpan);

	// See Also:
	//	COleDateTimeSpan
	void SetDateTimeSpan( long lDays, int nHours, int nMins, int nSecs );

	// Description:
	//		it Converts from a given unit (dtUnit) to seconds in Time Span format
	//
	// Returns: 
	//		a utDAteTimeSpan
	// Note:
	//		UT_YEAR and UT_MONTH are not taken into account, because are not exact units
	//		trying to use it an ASSERT will be thrown
	//		
	static utDateTimeSpan Convert (int i_value, dtUnit i_Unit);

};
// Description:
//  Minimum span
const utDateTimeSpan UT_MINIMUM_SPAN( 0.0);

// Description:
//	Maximum span
const utDateTimeSpan UT_MAXIMUM_SPAN( 3000000.0);

// Description:
//	Maximum span
const utDateTimeSpan UT_ONE_SECOND = utDateTimeSpan (0, 0, 0, 1);


#endif // UTDATETIMESPAN_H
