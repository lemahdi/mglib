#ifndef __JUST_A_TEST_H__
#define __JUST_A_TEST_H__


#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/DoubleOrNothing.h>
#include <xlw/ArgList.h>
#include <xlw/Wrapper.h>

#include "mgnova/glob/genericdate.h"


using namespace xlw;
using namespace MG;
using namespace std;


//<xlw:libraryname=MyTestLibrary
MG_Date // just a test
JustATest(const MG_Date& Date);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Market data robot
Robot(const MG_Date& AsOf // as of date
		, CellMatrix& MktData // market data robot
		);

//<xlw:libraryname=MyTestLibrary
double // Discount Factor pricing model
Price(MG_XLObjectPtr& Security // financial security
		, MG_XLObjectPtr& Model // financial model
		);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Discount Factor pricing model
DfModel(const MG_Date& AsOf // as of date
		, MG_XLObjectPtr& Robot // market data robot
		);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Black & Scholes pricing model
BSModel(const MG_Date& AsOf // as of date
		, MG_XLObjectPtr& Robot // market data robot
		);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating n zero curve
ZeroCurve_Create(const MG_Date& AsOf // s of date
					   , const CellMatrix& Maturities // maturities
					   , const CellMatrix& ZeroRates // zero coupons rates
					   , const string& Ccy // currency
					   , const string& UnderIndex // underlying index
					   , const string& InterpolMeth = "LINEAR" // interpolation method: LINEAR (def.), CONTINUOUS, CONSTANT, STEPUPLEFT, STEPUPRIGHT, POLYNOMIAL, CUBICSPLINE, AKIMA
					   );

//<xlw:libraryname=MyTestLibrary
double // Interpolating a zero curve
ComputeZeroRate(MG_XLObjectPtr& ZeroCurve // zero curve
				  , const double& Maturity // maturity
				  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating n IR volatility curve
VolatilityCurve_Create(const MG_Date& AsOf // s of date
					   , const CellMatrix& Maturities // maturities
					   , const CellMatrix& Tenors // tenors
					   , const CellMatrix& Volatilities // volatilities
					   , const string& Ccy // currency
					   , const string& UnderIndex // underlying index
					   , const CellMatrix& InterpolMeths // 2 interpolation methods: LINEAR (def.), CONTINUOUS, CONSTANT, STEPUPLEFT, STEPUPRIGHT, POLYNOMIAL, CUBICSPLINE, AKIMA
					   );

//<xlw:libraryname=MyTestLibrary
double // Interpolating n IR volatility curve
ComputeVolatility(MG_XLObjectPtr& VolCurve // volatility curve
				  , const double& Tenor // tenor
				  , const double& Maturity // maturity
				  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating a Dividend Table for equities
DividendsTable_Create(const MG_Date& AsOf // s of date
					 , const CellMatrix& ExDivDates // exdiv dates
					 , const CellMatrix& PaymentDates // dividends payment dates
					 , const CellMatrix& Dividends // dividends
					 , const string& Ccy // currency
					 , const string& UnderIndex // underlying index
					 , const MG_XLObjectPtr& ZC // zero curve
					 );

//<xlw:libraryname=MyTestLibrary
double // Computing the discounted dividends between 2 dates
ComputeDiscountedDivs(MG_XLObjectPtr& Dividends // dividends
					  , const MG_Date& FirstDate // first date of computation
					  , const MG_Date& LastDate  // last date of computation
					  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating a generic security
GenSec_Create(const CellMatrix& DealDesc // deal description
			  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating a random generator
RandGen_Create(const string& Type // generator type: TAUS, MT19937, ..
			   , const int& Dim // generator dimension, for quasi random
			  );

//<xlw:libraryname=MyTestLibrary
MG_Date // next business day
NextBusinessDay(MG_Date Date // reference date
			   , const int& Days // days fter
			   , const string& Calendar // calendar for business days
			  );

//<xlw:libraryname=MyTestLibrary
MG_Date // previous business day
PreviousBusinessDay(MG_Date Date // reference date
			   , const int& Days // days before
			   , const string& Calendar // calendar for business days
			  );

//<xlw:libraryname=MyTestLibrary
double // previous business day
BetweenDates(MG_Date Date1 // first date
			 , MG_Date Date2 // second date 
			 , const string& DayCount // day count: ACT, A365, A360, 30/360, 30/360E, B252
			 , const bool& IsFrac = true // true: fraction, false: days difference (def.)
			 , const string& Calendar = "" // calendar for business days
			 );

//<xlw:libraryname=MyTestLibrary
MG_Date // Computing a date
AddPeriod(MG_Date Date // reference date
			   , const string& Freq // frequency: nY, Y, A, S, Q, B, nM, M, nW, W, nD, D
			   , const int& Times = 1 // times number to dd frequency (1 by def.)
			   , const string& Calendar = CALENDAR_NAME_DEF_STR // calendar for business days (EUR by def.)
			   , const string& AdjRule = "MF" // djustment rule: FIXED, FP, F, MF (def.), PP, P, MP
			   , const bool& EndOfMonth = true // true: go to the end of the month (def.)
			  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // create an interest rate index
IRIndex_Create(const string& IndexName = INDEX_NAME_DEF_STR // index name: LIBOR3M, EUBOR6M (def.), CMS1, ...
			   , const string& Currency = CURRENCY_NAME_DEF_STR // currency (EUR by def.)
			   , const string& DayCount = "A365" // day count: ACTUAL, A360, A365 (def.), 30/360, 30/360E, B252
			   , const string& AdjRule = "MF" // adjustment rule: FIXED, FP, F, MF (def.), PP, P, MP
			   , const string& ResetTiming = "ADV" // reset timing: ADV (def.), ARR
			   , const string& PayTiming = "ARR" // pay timing: ADV, ARR (def.)
			   , const string& ResetCalendar = CALENDAR_NAME_DEF_STR // reset calendar (EUR by def.)
			   , const string& PayCalendar = CALENDAR_NAME_DEF_STR // pay calendar (EUR by def.)
			   , const int& ResetGap = -2 // reset gap (-2 by def.)
			   , const int& PayGap = 0 // pay gap (0 by def.)
			  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // create a schedule
Schedule_Create(const MG_Date& StartDate // start date
			   , const MG_Date& EndDate // end date
			   , const MG_XLObjectPtr& IRIndex // interest rate index
			   , const string& Freq // frequency: Y, A, S, Q, B, M, W, D
			   , const string& IntRule = "ADJ" // interest rule: ADJ (def.), UNADJ
			   , const string& StubRule = "SS" // stub rule: SS (def.), LS, SE, LE
			   , const bool& IsDecompound = true // decompounding frequency: true (def.) or false
			  );

//<xlw:libraryname=MyTestLibrary
CellMatrix // get information from schedule
Schedule_GetData(MG_XLObjectPtr& Schedule // schedule
				 , const string& Data // ResetDate, StartDate, EndDate, PayDate, InterestDays, InterestTerms, FwdStartDate, FwdEndDate
				 );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // create a term structure
TermStructure_Create(const CellMatrix& PayDates // payment dates
				 , const CellMatrix& Values // values
				 , const string& InterpolMeth = "LINEAR" // interpolation method (LINEAR by def.)
				 );

//<xlw:libraryname=MyTestLibrary
double // create a term structure
TermStructure_Compute(MG_XLObjectPtr& TermStruct // term structure
				 , const MG_Date& PayDate // payment date
				 );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // create a swap leg
SwapLeg_Create(const MG_GenericDate& Start // start date or term
			   , MG_GenericDate& End // end date or duration
			   , const string& RcvPay // receive or pay
			   , MG_XLObjectPtr& IRIndex // interest rate index
				 );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // create a call or put
CallPut_Create(const MG_GenericDate& Maturity
			   , MG_XLObjectPtr& Underlying // underlying
			   , const string& CallPut // call (C) or put (P)
			   , const double& Strike // strike
				 );

#endif
