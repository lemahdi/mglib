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

#endif