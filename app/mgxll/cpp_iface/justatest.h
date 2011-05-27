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
MG_XLObjectPtr // just an object test
BSModel(const MG_Date& AsOf // as of date
		, const double& Vol // bs volatility
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

#endif
