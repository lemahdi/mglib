#ifndef __JUST_A_TEST_H__
#define __JUST_A_TEST_H__


#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include <xlw/DoubleOrNothing.h>
#include <xlw/ArgList.h>
#include <xlw/Wrapper.h>

#include "mgnova/date.h"


using namespace xlw;
using namespace MG;
using namespace std;


//<xlw:libraryname=MyTestLibrary
MG_Date // just a test
JustATest(const MG_Date& Date);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Market data robot
Robot(const MG_Date& AsOf // as of date
		, CellMatrix& aMktData // market data robot
		);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // just an object test
BSModel(const MG_Date& AsOf // as of date
		, MG_XLObjectPtr& aRobot // market data robot
		);

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // just an object test
Call(const double& Strike // strike
		, const double& Maturity // maturity
		, const double& Forward // forward
		);

//<xlw:libraryname=MyTestLibrary
double // pricing
Price(MG_XLObjectPtr& aSec // security
	  , MG_XLObjectPtr& aMod // model
	  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating an zero curve
ZeroCurve_Create(const MG_Date& aAsOf // as of date
					   , const CellMatrix& aMaturities // maturities
					   , const CellMatrix& aZeroRates // volatilities
					   , const string& aCcy // currency
					   , const string& aUnderIndex // underlying index
					   , const CellMatrix& aInterpolMeth // interpolation method (LINEAR by def.)
					   );

//<xlw:libraryname=MyTestLibrary
double // Interpolating a zero curve
ComputeZeroRate(MG_XLObjectPtr& aZeroCurve // zero curve
				  , const double& aMaturity // maturity
				  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating an IR volatility curve
VolatilityCurve_Create(const MG_Date& aAsOf // as of date
					   , const CellMatrix& aMaturities // maturities
					   , const CellMatrix& aTenors // tenors
					   , const CellMatrix& aVolatilities // volatilities
					   , const string& aCcy // currency
					   , const string& aUnderIndex // underlying index
					   , const CellMatrix& aInterpolMeths // interpolation methods (LINEAR by def.)
					   );

//<xlw:libraryname=MyTestLibrary
double // Interpolating an IR volatility curve
ComputeVolatility(MG_XLObjectPtr& aVolCurve // volatility curve
				  , const double& aTenor // tenor
				  , const double& aMaturity // maturity
				  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating a Dividend Table for equities
DividendsTable_Create(const MG_Date& aAsOf // as of date
					 , const CellMatrix& aExDivDates // exdiv dates
					 , const CellMatrix& aPaymentDates // dividends payment dates
					 , const CellMatrix& aDividends // dividends
					 , const string& aCcy // currency
					 , const string& aUnderIndex // underlying index
					 , const MG_XLObjectPtr& aZC // zero curve
					 );

//<xlw:libraryname=MyTestLibrary
double // Computing the discounted dividends between 2 dates
ComputeDiscountedDivs(MG_XLObjectPtr& aDividends // dividends
					  , const MG_Date& aFirstDate // first date of computation
					  , const MG_Date& aLastDate  // last date of computation
					  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating a generic security
GenSec_Create(const CellMatrix& aDealDesc // deal description
			  );

//<xlw:libraryname=MyTestLibrary
MG_XLObjectPtr // Creating a random generator
RandGen_Create(const string& aType // generator type: TAUS, MT19937, ..
			   , const int& aDim // generator dimension, for quasi random
			  );

//<xlw:libraryname=MyTestLibrary
MG_Date // next business day
NextBusinessDay(MG_Date aDate // reference date
			   , const int& aDays // days after
			   , const string& aCalendar // calendar for business days
			  );

//<xlw:libraryname=MyTestLibrary
MG_Date // previous business day
PreviousBusinessDay(MG_Date aDate // reference date
			   , const int& aDays // days before
			   , const string& aCalendar // calendar for business days
			  );

//<xlw:libraryname=MyTestLibrary
double // previous business day
BetweenDates(MG_Date aDate1 // first date
			 , MG_Date aDate2 // second date 
			 , const string& aDayCount // day count: ACT, A365, A360, 30/360, 30/360E, B252
			 , const bool& aIsFrac = true // true: fraction, false: days difference (def.)
			 , const string& aCalendar = "" // calendar for business days
			 );

//<xlw:libraryname=MyTestLibrary
MG_Date // Computing a date
AddPeriod(MG_Date aDate // reference date
			   , const string& aFreq // frequency: nY, Y, A, S, Q, B, nM, M, nW, W, nD, D
			   , const int& aTimes // times number to add frequency
			   , const string& aCalendar // calendar for business days
			   , const string& aAdjRule = "MF" // adjustment rule: FIXED, FP, F, MF (def.), PP, P, MP
			   , const bool& aEndOfMonth = true // true: go to the end of the month (def.)
			  );

#endif
