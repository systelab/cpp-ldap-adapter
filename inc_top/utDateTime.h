/*
$Workfile: /TOP/CM/SW/_Inc/utDateTime.h jpedro $

DESCRIPTION:	

NOTICE:		Copyright 2000, Instrumentation Laboratory,
			Lexington, MA.  All rights reserved

REVISION HISTORY:

$Log: utDateTime.h $
Revision NTE190_CL_Current_jpedro/1 2016/07/19 11:42:00 jpedro
IL12-4335 defect fixing

Revision Systelab_CM_WorkInProgress_xdepedro/2 2014/10/15 16:25:37 xdepedro
No Comment Entered

Revision COAG_TOP_00010_SW_CM_UnitTested_SLT2019_nsilveira/1 2013/11/21 17:10:03 nsilveira
Fix for CR13341

Revision NTE54_CM_750_xrosell/3 2011/11/28 09:23:40 xrosell
6795: "Provide Laboratory Accreditation Reports for TOP users"

Revision NTE54_CM_750_xrosell/1 2011/02/16 13:11:16 xrosell
6494: "Time to Overdue field of QC not adjusted during a DST when freq. enabled"

Revision NTE54_CM_700_xrosell/10 2010/06/28 09:42:26 xrosell
Daylight Saving Time handling

Revision NTE54_CM_700_xrosell/8 2010/04/23 10:29:07 xrosell
5000000913 #64 145-R5 : QC FREQUENCY BY ABSOLUTE TIME

Revision NTE54_CM_700_xrosell/6 2009/08/19 13:07:46 xrosell
	Remove ASSERT condition in startDate

Revision NTE54_CM_700_xrosell/4 2009/07/13 12:33:53 xrosell
	5000001307: "Instrument Status alarms graph to display frequency by time instead counter" -> add year and month unit conversions

Revision NTE54_CM_700_xrosell/2 2009/07/02 11:02:12 xrosell
	5000001307: "Instrument Status alarms graph to display frequency by time instead counter" -> Interface definition.

 * 
 * 1     28/03/07 11:41 Admin
 * 
 * 9     25/04/05 9:31 Nestor
 * .NTE Compatibility
 * 
 * 8     20/05/04 10:41 Victor
 * dtUnit: UT_NOT_DEFINED
 * 
 * 7     24/10/03 11:53 Andreas
 * 
 * 6     7/04/03 17:56 Berenguer
 * Added methods to fix defect 2200.
 * 
 * 5     7/02/03 12:18 Xtapia
 * Added const CString utDateTime::GetTimeHourMinute() const
 * 
 * 4     22/03/02 10:08 Andreas
 * 
 * 3     2/02/02 11:58 Xtapia
 * Added GetDateMonthYear method
 * 
 * 2     8/01/02 13:00 Xtapia
 * Added Format
 * 
 *    Rev 1.21   03 Aug 2001 09:13:52   JPTeruel
 * DB methods modified
 * 
 *    Rev 1.20   25 Jul 2001 16:41:20   XTapia
 * Change double constants by DATE. Added constant limits.
 * 
 *    Rev 1.19   25 Jul 2001 13:35:38   XTapia
 * Added upper limit date
 * 
 *    Rev 1.18   02 Jul 2001 12:21:16   NSilveira
 * Updated.
 * 
 *    Rev 1.17   21 Jun 2001 15:56:12   JPTeruel
 * utDateFormat pointer deleted
 * 
 *    Rev 1.16   18 Jun 2001 09:56:14   jbosch
 * added dtUnit enum
 * 
 *    Rev 1.15   13 Jun 2001 11:46:30   JPTeruel
 * Added new constructor
 * 
 *    Rev 1.14   Jun 02 2001 12:26:08   XTapia
 * Added GetTimeStamp and fixed Time format detection
 * 
 *    Rev 1.13   May 29 2001 15:09:02   XTapia
 * Fixed problem with utDataFormat member
 * 
 *    Rev 1.12   May 23 2001 16:27:26   XTapia
 * Updated docjet format
 * 
 *    Rev 1.11   May 15 2001 12:26:12   XTapia
 * Added IsEmpty method
 * 
 *    Rev 1.10   11 May 2001 11:26:46   JordiT
 * Update methods providing services to DB module.
 * 
 *    Rev 1.9   May 11 2001 09:03:24   XTapia
 * Update
 * 
 *    Rev 1.8   04 May 2001 07:17:52   jbosch
 * Added IsPrintable() function
 * 
 *    Rev 1.3   11 Apr 2001 10:19:32   XTapia
 * Fixing Docjet and compiling problems
 * 
 *    Rev 1.2   09 Apr 2001 13:53:28   JPTeruel
 * Updated for Docjet
 * 
 *    Rev 1.1   Apr 06 2001 15:15:48   XTapia
 * DocJet format
 * 
 *    Rev 1.0   06 Apr 2001 14:27:44   JordiT
 * Initial revision.
*/

#if !defined(__utDateTime_h)
#define __utDateTime_h

// group = CM-UT-HLD

class utDateTimeSpan;
enum class dtUnit : int;

// Description:
// Day of the week to print Lab Accreditation Automatic Report.
enum class EDayOfWeek
{
	EDOW_SUNDAY = 1,
	EDOW_MONDAY = 2,
	EDOW_TUESDAY = 3,
	EDOW_WEDNESDAY = 4,
	EDOW_THURSDAY = 5,
	EDOW_FRIDAY = 6,
	EDOW_SATURDAY = 7,
};

//////////////////////////////////////////////////////////////////////
// Description: 
//	Interface to manage date and times.
// Remarks: 
//	This class encapsulates all Date and time management
class utDateTime: protected COleDateTime
{
public:

	// Description:
	//		Constructor of utDateTime class
	// Remarks: 
	//		Default constructor which provides the availability to initilize the date and
	//		time information using the current date and time
	// Throws: 
	//		
	// Returns: 
	//		
	// Note:
	//		
	// See Also:
	//		~utDateTime
	utDateTime();
	explicit utDateTime(// false (default) constructs the object with a null value.
				// true constructs the object with current time.
				bool bCurrentTime); 	
	utDateTime(const utDateTime& obj);
		
    explicit utDateTime(DATE dtSrc);

	explicit utDateTime(const SYSTEMTIME& obj);

	explicit utDateTime(const __time32_t& obj);
	explicit utDateTime(const __time64_t& obj);
	utDateTime
	(
		int nYear,	// Number of the Year
		int nMonth,	// Number of the month
		int nDay,	// Number of the day
		int nHour,	// Number of the Hour
		int nMin,	// Number of the Minute
		int nSec	// Number of the second
	);

	operator DATE() const throw();
	
	
	// Description:
	//		Destructor of utDateTime class
	// Remarks: 
	//		It resets all internal information
	// Throws: 
	//		
	// Returns: 
	//		
	// Note:
	//		
	// See Also:
	//		utDateTime
	virtual ~utDateTime();


	__time32_t GetTimeT() const;

	__time64_t utDateTime::GetTimeT64() const;

	// Description:
	//		It gets the date and time as a string using always 4 digits for year
	// Remarks: 
	//		It gets the date and time as a string using always 4 digits for year
	// Throws: 
	//		
	// Returns: 
	//		It returns a CString reference of a internal buffer
	// Note:
	//		
	// See Also:
	//		SetDateTime4Y
	const CString GetDateTime4Y(void) const;

	// Description:
	//		It gets the date as a string
	// Remarks: 
	//		It gets the date as a string
	// Throws: 
	//		
	// Returns: 
	//		It returns a CString reference of a internal buffer
	// Note:
	//		
	// See Also:
	//		SetDate4Y
	const CString GetDate4Y(void) const;

	// Description:
	//		It returns the date including only month and year
	// Throws: 
	//		
	// See Also:
	//		GetDateTime4Y
	CString GetDateMonthYear(void) const;

	// Description:
	//		It gets the year as number
	// Remarks: 
	//		It gets the year as number
	// Throws: 
	//		
	// Returns: 
	//		It returns an integer indicating the year 
	// Note:
	//		
	// See Also:
	//		SetYear
	int GetYear(void) const;

	// Description:
	//		It gets the month as number
	// Remarks: 
	//		It gets the month as number
	// Throws: 
	//		
	// Returns: 
	//		It returns an integer indicating the month
	// Note:
	//		
	// See Also:
	//		SetMonth
	int GetMonth(void) const;

	// Description:
	//		It gets the day as number
	// Remarks: 
	//		It gets the day  as number
	// Throws: 
	//		
	// Returns: 
	//		It returns an integer indicating the day
	// Note:
	//		
	// See Also:
	//		SetDay
	int GetDay(void) const;

	int GetDayOfWeek(void) const;

		// Description:
	//		It gets the hour as number
	// Remarks: 
	//		It gets the hour as number
	// Throws: 
	//		
	// Returns: 
	//		It returns an integer indicating the hour
	// Note:
	//		
	// See Also:
	//		SetHour
	int GetHour(void) const;

	// Description:
	//		It gets the minute as number
	// Remarks: 
	//		It gets the minute as number
	// Throws: 
	//		
	// Returns: 
	//		It returns an integer indicating the minute
	// Note:
	//		
	// See Also:
	//		SetMinute
	int GetMinute(void) const;

	// Description:
	//		It gets the second as number
	// Remarks: 
	//		It gets the second as number
	// Throws: 
	//		
	// Returns: 
	//		It returns an integer indicating the second
	// Note:
	//		
	// See Also:
	//		SetSecond
	int GetSecond(void) const;

	// Description:
	//		It gets whether the date and time is empty (not initialized) or not 
	// Remarks: 
	//		It gets whether the date and time is valid or not 
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether the date and time is empty or not 
	// Note:
	//		Note that an empty date is considered valid.
	// See Also:
	//		IsValid, IsPrintable
	bool IsEmpty(void) const;

	// Description:
	//		It gets whether the date and time is valid or not 
	// Remarks: 
	//		It gets whether the date and time is valid or not.
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether the date and time is valid or not 
	// Note:
	//		Note that an empty date is considered valid.
	// See Also:
	//		IsEmpty, IsPrintable
	bool IsValid(void) const;
	
	// Description:
	//		It gets whether the date and time is printable or not 
	// Remarks: 
	//		It gets whether the date and time is printable or not.
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether the date and time is printable or not 
	// Note:
	//		Is Printable if it is valid and it is not empty (null).
	// See Also:
	//		IsEmpty, IsValid
	bool IsPrintable(void) const;
	
	// Description:
	//		It resets the information to a not-initialized status
	// Remarks: 
	//		It resets the information to a not-initialized status
	// Throws: 
	//		
	// Returns: 
	//
	// Note:
	//		
	// See Also:
	//		resetToCurrentTime
	void Reset(void);
	
	// Description:
	//		It resets the information to the current date and time
	// Remarks: 
	//		It resets the information to the current date and time
	// Throws: 
	//		
	// Returns: 
	//
	// Note:
	//		
	// See Also:
	//		Reset
	void ResetToCurrentTime(void);

	// Description:
	//		Assign Operator to get utDateTime information
	// Remarks: 
	//		Assign Operator to get utDateTime information
	// Throws: 
	//		
	// Returns: 
	//		It returns a reference of the class itself
	// Note:
	//		
	// See Also:
	//
	const utDateTime& operator =
	(
		const utDateTime& obj // utDateTime Structure containing the date and time information
	);
	
	// Description:
	//		Assign Operator to get SYSTEMTIME information
	// Remarks: 
	//		Assign Operator to get SYSTEMTIME information
	// Throws: 
	//		
	// Returns: 
	//		It returns a reference of the class itself
	// Note:
	//		
	// See Also:
	//
	const utDateTime& operator =
	(
		const SYSTEMTIME& obj // SYSTEMTIME Structure containing the date and time information
	);

	// Description:
	//		Equal Operator to compare two date and time informations
	// Remarks: 
	//		Equal Operator to compare two date and time informations
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether both information are equal or not
	// Note:
	//		
	// See Also:
	//
	bool operator ==
	(
		const utDateTime& obj // utDateTime to be compared
	) const;

	// Description:
	//		Not-Equal Operator to compare two date and time informations
	// Remarks: 
	//		Not-Equal Operator to compare two date and time informations
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether both information are differents or not
	// Note:
	//		
	// See Also:
	//
	bool operator !=
	(
		const utDateTime& obj // utDateTime to be compared
	) const;
	
	// Description:
	//		Major Operator to compare two date and time informations
	// Remarks: 
	//		major Operator to compare two date and time informations
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether external information is major than the own one
	// Note:
	//		
	// See Also:
	//
	bool operator >
	(
		const utDateTime& obj // utDateTime to be compared
	) const;
	
	// Description:
	//		Major or Equal Operator to compare two date and time informations
	// Remarks: 
	//		major or Equal Operator to compare two date and time informations
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether external information is major or equal than the own one
	// Note:
	//		
	// See Also:
	//
	bool operator >=
	(
		const utDateTime& obj // utDateTime to be compared
	) const;

	// Description:
	//		Minor Operator to compare two date and time informations
	// Remarks: 
	//		Minor Operator to compare two date and time informations
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether external information is minor than the own one
	// Note:
	//		
	// See Also:
	//
	bool operator <
	(
		const utDateTime& obj	// utDateTime to be compared
	) const;
	
	// Description:
	//		Minor or Equal Operator to compare two date and time informations
	// Remarks: 
	//		Minor or Equal Operator to compare two date and time informations
	// Throws: 
	//		
	// Returns: 
	//		It returns a boolean indicating whether external information is minor or equal than the own one
	// Note:
	//		
	// See Also:
	//
	bool operator <=
	(
		const utDateTime& obj	// utDateTime to be compared
	) const;

	// Description:
	//		It sets the date information using Year, Month and Day information 
	// Remarks: 
	//		It sets the date information using Year, Month and Day information 
	// Throws: 
	//		INVALID_DATE
	// Returns: 
	//
	// Note:
	//		
	// See Also:
	//
	void SetDate
	(
		int nYear,	// The number of the year
		int nMonth,	// The number of the month
		int nDay	// The number of the day
	);
	
	// Description:
	//		It sets the date information using Hour, Minute and Second information 
	// Remarks: 
	//		It sets the date information using Hour, Minute and Second information 
	// Throws: 
	//		INVALID_TIME
	// Returns: 
	//
	// Note:
	//		
	// See Also:
	//
	void SetTime
	(
		int nHour,	// The number of the Hour
		int nMin,	// The number of the Minute
		int nSec	// The number of the second
	);
	
	// Description:
	//		It sets the date and time information using Year, Month, Day, Hour, Minute and Second information 
	// Remarks: 
	//		It sets the date and time information using Year, Month, Day, Hour, Minute and Second information 
	// Throws: 
	//		INVALID_DATETIME
	// Returns: 
	//
	// Note:
	//		
	// See Also:
	//
	void SetDateTime
	(
		int nYear,	// The number of the year
		int nMonth,	// The number of the month 
		int nDay,	// The number of the day
		int nHour,	// The number of the Hour
		int nMin,	// The number of the Minute
		int nSec	// The number of the second
	);

	// Description:
	//		It sets the date and time information using SYSTEMTIME information 
	// Remarks: 
	//		It sets the date and time information using SYSTEMTIME information 
	// Throws: 
	//		INVALID_DATETIME
	// Returns: 
	//
	// Note:
	//		
	// See Also:
	//
	void SetDateTime
	(
		const SYSTEMTIME& obj
	);

	
	// Description:
	//		It gets the date and time in a fixed format for DB purposes
	// Remarks: 
	//		It gets the date and time in a fixed format for DB purposes
	// Throws: 
	//
	// Returns: 
	//		reference to an internal double containing the date and time
	// Note:
	//		
	// See Also:
	//		SetDateTimeDB
	double GetDateTimeDB(
		// Wanted returned value for null dates
		double NullValue = 0.0) const;

	// Description:
	//		It gets the date and time using US format
	// Remarks: 
	//		It gets the date and time using US format
	// Throws: 
	//
	// Returns: 
	//		A CString reference of an internal buffer
	// Note:
	//		
	// See Also:
	//		GetDateTimeDB, SetDateTimeDB
	const CString GetUSDateTimeDB(void) const;

	// Description:
	//		It sets the date and time in a fixed format for DB purposes
	// Remarks: 
	//		It sets the date and time in a fixed format for DB purposes
	// Throws: 
	//		INVALID_DATETIME
	// Returns: 
	//		
	// Note:
	//		
	// See Also:
	//		GetDateTimeDB
	void SetDateTimeDB
	(
		double DT,				// Reference to a double provided by the client
		double NullValue = 0.0	// If DT == NullValue then reset the date.
	);

	// Description:
	//		It gets the date using a specified format
	// Remarks: 
	//		It gets the date using a specified format
	// Throws: 
	//
	// Returns: 
	//		A CString reference of an internal buffer
	// Note:
	//		
	// See Also:
	//		GetTime, GetDateTime		
	const CString GetDate(void) const;

	// Description:
	//	Returns a string representing this date time object.
	//
	// Remarks:
	//	This date time object is represented by the returned string consisting
	//  of the date and the time represented by this object. Time in the
	//  string only consists of hours and minutes.
	//
	// Returns:
	//	The string representing the date and time (with hours and minutes)
	const CString GetDateTimeHourMinute() const;
	
	// Description:
	//		It gets the time using a specified format
	// Remarks: 
	//		It gets the time using a specified format
	// Throws: 
	//
	// Returns: 
	//		A CString reference of an internal buffer
	// Note:
	//		
	// See Also:
	//		GetDate, GetDateTime
	const CString GetTime(void) const;

	// Description:
	//		It gets the time using a specified format. It does not include seconds.
	// Remarks: 
	//		It gets the time using a specified format. It does not include seconds.
	// Throws: 
	//
	// Returns: 
	//		A CString reference of an internal buffer
	// Note:
	//		
	// See Also:
	//		GetDate, GetDateTime		
	const CString GetTimeHourMinute(void) const;
	
	// Description:
	//		It gets the date and time using a specified format
	// Remarks: 
	//		It gets the date and time using a specified format
	// Throws: 
	//
	// Returns: 
	//		A CString reference of an internal buffer
	// Note:
	//		
	// See Also:
	//		Get Date, GetTime
	CString GetDateTime(void) const;

	// Description:
	//		Returns the local date time including milliseconds, in a fixed format
	//		for tracing purposes.
	// Remarks: 
	//		This is a static method. Example of use:
	//		utDateTime::GetTimeStamp();
	static const CString GetTimeStamp(void);

	// Description:
	//		Returns the date and time in militar format
	//		that is YYYYMMDDHHMM.
	//		It gets the date and time using a specified format
	// Throws: 
	//
	// Returns: 
	//		A CString reference of an internal buffer
	// Note:
	//		
	// See Also:
	//		Get DateTime and GetTimeStamp
	const CString GetMilitarDateTime (void);

	// Returns the date time in this format: %Y-%m-%d %H:%M:%S
	CString GetDBDateTime() const;

	// Remarks:
	//	utDateTime objects represent absolute times. utDateTimeSpan 
	// objects represent relative times. The first two operators 
	// allow you to add and subtract a utDateTimeSpan value from a 
	// utDateTime value. The third operator allows you to subtract 
	// one utDateTime value from another to yield a utDateTimeSpan 
	// value.
	utDateTime operator +( const utDateTimeSpan& dateSpan ) const;
	utDateTime operator -( const utDateTimeSpan& dateSpan ) const;	
	utDateTimeSpan operator -( const utDateTime& date ) const;

	// Description:
	//	Returns the formatted date/time value.
	// Remarks:
	// see COleDateTime
	CString Format( LPCTSTR lpszFormat ) const;


	// Description:
	//	Returns the date/time value adjusted to interval value is passed
	// Remarks:
	static utDateTime GetAdjustedTime (
		// Date to be adjusted
		const utDateTime & date, 
		// Interval Value to adjust
		int intervalTime, 
		// Interval Unit to adjust -> UT_YEAR, UT_MONTH, UT_DAY, ... se dtUnit definition
		dtUnit intervalUnit);

	// Description:
	//	Returns a new date/time value with date and interval value summed
	// Remarks:
	static utDateTime AddTime (
		// Date to be adjusted
		const utDateTime & date, 
		// Interval Value to adjust
		int intervalTime, 
		// Interval Unit to adjust -> UT_YEAR, UT_MONTH, UT_DAY, ... se dtUnit definition
		dtUnit intervalUnit);

	// Description:
	//	Returns true if year's date is a leap year 
	// Remarks:
	static bool IsLeapYear (utDateTime & date);

	static bool GetDSTDates (utDateTime &ghostHour, utDateTime &repeatedHour, int year = utDateTime (true).GetYear ());

	static EDayOfWeek GetFirstDayOfWeek ();

	static int GetFirstDayOfYear ();

	static int GetLastDayOfMonth(utDateTime & i_date);


public:

	// Lower acceptable Date Time
	static DATE DT_LOWERLIMIT;
	// Upper acceptable Date Time
	static DATE DT_UPPERLIMIT;
	// Value for a null date
	static const DATE NULL_DATE_VALUE; 
	static const DATE FIRST_TIME_LOGON;
};

// Description:
//  Minimum date
const utDateTime UT_MINIMUM_DATE( utDateTime::DT_LOWERLIMIT);

// Description:
//	Maximum date
const utDateTime UT_MAXIMUM_DATE( utDateTime::DT_UPPERLIMIT);

// Description:
//	Maximum date
const utDateTime UT_1970_01_01_DATE (1970, 1, 1, 0, 0, 0);

#endif // __utDateTime_h
