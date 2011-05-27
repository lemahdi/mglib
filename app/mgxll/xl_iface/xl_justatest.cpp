//// 
//// Autogenerated by xlw 
//// Do not edit this file, it will be overwritten 
//// by InterfaceGenerator 
////

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include "justatest.h"
#include <xlw/xlw.h>
#include <xlw/XlFunctionRegistration.h>
#include <stdexcept>
#include <xlw/XlOpenClose.h>
#include <ctime>
#include <mgnova/utils/utils.h>

#include <xlw/cache/cached.h>

namespace {
const char* LibraryName = "MyTestLibrary";
};

// dummy function to force linkage
namespace {
void DummyFunction()
{
xlAutoOpen();
xlAutoClose();
}
}

// registrations start here


namespace
{
XLRegistration::Arg
JustATestArgs[]=
{
{ "Date","too lazy to comment this one ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerJustATest("xlJustATest",
"MG_JustATest",
" just a test ",
LibraryName,
JustATestArgs,
1
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlJustATest(
LPXLFOPER Datea)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Dateb(
	(Datea));
MG_Date Date(
	Dateb.AsMGDate("Date"));

MG_Date result(
	JustATest(
		Date)
	);
double vXLDate = MG_utils::FromJulianDayToXLDate(result.GetJulianDay());
return XlfOper(vXLDate);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
BSModelArgs[]=
{
{ "AsOf"," as of date ","XLF_OPER"},
{ "Vol"," bs volatility ","B"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerBSModel("xlBSModel",
"MG_BSModel",
" just an object test ",
LibraryName,
BSModelArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlBSModel(
LPXLFOPER AsOfa,
double Vol)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));


MG_XLObjectPtr result(
	BSModel(
		AsOf,
		Vol)
	);
string vRefObj, vError;
if (MG_SCache::Instance()->PersistentInsert(result, vRefObj, vError))
  return XlfOper(vRefObj);
else
  return XlfOper(vError);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
CallArgs[]=
{
{ "Strike"," strike ","B"},
{ "Maturity"," maturity ","B"},
{ "Forward"," forward ","B"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerCall("xlCall",
"MG_Call",
" just an object test ",
LibraryName,
CallArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlCall(
double Strike,
double Maturity,
double Forward)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);




MG_XLObjectPtr result(
	Call(
		Strike,
		Maturity,
		Forward)
	);
string vRefObj, vError;
if (MG_SCache::Instance()->PersistentInsert(result, vRefObj, vError))
  return XlfOper(vRefObj);
else
  return XlfOper(vError);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
PriceArgs[]=
{
{ "aSec"," security ","XLF_OPER"},
{ "aMod"," model ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerPrice("xlPrice",
"MG_Price",
" pricing ",
LibraryName,
PriceArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlPrice(
LPXLFOPER aSeca,
LPXLFOPER aModa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aSecb(
	(aSeca));
MG_XLObjectPtr aSec(
	aSecb.AsMGXLObject("aSec"));

XlfOper aModb(
	(aModa));
MG_XLObjectPtr aMod(
	aModb.AsMGXLObject("aMod"));

double result(
	Price(
		aSec,
		aMod)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
ZeroCurve_CreateArgs[]=
{
{ "aAsOf"," as of date ","XLF_OPER"},
{ "aMaturities"," maturities ","XLF_OPER"},
{ "aZeroRates"," volatilities ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerZeroCurve_Create("xlZeroCurve_Create",
"MG_ZeroCurve_Create",
" Creating an zero curve ",
LibraryName,
ZeroCurve_CreateArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlZeroCurve_Create(
LPXLFOPER aAsOfa,
LPXLFOPER aMaturitiesa,
LPXLFOPER aZeroRatesa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aAsOfb(
	(aAsOfa));
MG_Date aAsOf(
	aAsOfb.AsMGDate("aAsOf"));

XlfOper aMaturitiesb(
	(aMaturitiesa));
CellMatrix aMaturities(
	aMaturitiesb.AsCellMatrix("aMaturities"));

XlfOper aZeroRatesb(
	(aZeroRatesa));
CellMatrix aZeroRates(
	aZeroRatesb.AsCellMatrix("aZeroRates"));

MG_XLObjectPtr result(
	ZeroCurve_Create(
		aAsOf,
		aMaturities,
		aZeroRates)
	);
string vRefObj, vError;
if (MG_SCache::Instance()->PersistentInsert(result, vRefObj, vError))
  return XlfOper(vRefObj);
else
  return XlfOper(vError);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
ComputeZeroRateArgs[]=
{
{ "aZeroCurve"," zero curve ","XLF_OPER"},
{ "aMaturity"," maturity ","B"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerComputeZeroRate("xlComputeZeroRate",
"MG_ComputeZeroRate",
" Interpolating a zero curve ",
LibraryName,
ComputeZeroRateArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlComputeZeroRate(
LPXLFOPER aZeroCurvea,
double aMaturity)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aZeroCurveb(
	(aZeroCurvea));
MG_XLObjectPtr aZeroCurve(
	aZeroCurveb.AsMGXLObject("aZeroCurve"));


double result(
	ComputeZeroRate(
		aZeroCurve,
		aMaturity)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
VolatilityCurve_CreateArgs[]=
{
{ "aAsOf"," as of date ","XLF_OPER"},
{ "aMaturities"," maturities ","XLF_OPER"},
{ "aTenors"," tenors ","XLF_OPER"},
{ "aVolatilities"," volatilities ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerVolatilityCurve_Create("xlVolatilityCurve_Create",
"MG_VolatilityCurve_Create",
" Creating an IR volatility curve ",
LibraryName,
VolatilityCurve_CreateArgs,
4
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlVolatilityCurve_Create(
LPXLFOPER aAsOfa,
LPXLFOPER aMaturitiesa,
LPXLFOPER aTenorsa,
LPXLFOPER aVolatilitiesa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aAsOfb(
	(aAsOfa));
MG_Date aAsOf(
	aAsOfb.AsMGDate("aAsOf"));

XlfOper aMaturitiesb(
	(aMaturitiesa));
CellMatrix aMaturities(
	aMaturitiesb.AsCellMatrix("aMaturities"));

XlfOper aTenorsb(
	(aTenorsa));
CellMatrix aTenors(
	aTenorsb.AsCellMatrix("aTenors"));

XlfOper aVolatilitiesb(
	(aVolatilitiesa));
CellMatrix aVolatilities(
	aVolatilitiesb.AsCellMatrix("aVolatilities"));

MG_XLObjectPtr result(
	VolatilityCurve_Create(
		aAsOf,
		aMaturities,
		aTenors,
		aVolatilities)
	);
string vRefObj, vError;
if (MG_SCache::Instance()->PersistentInsert(result, vRefObj, vError))
  return XlfOper(vRefObj);
else
  return XlfOper(vError);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
ComputeVolatilityArgs[]=
{
{ "aVolCurve"," volatility curve ","XLF_OPER"},
{ "aTenor"," tenor ","B"},
{ "aMaturity"," maturity ","B"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerComputeVolatility("xlComputeVolatility",
"MG_ComputeVolatility",
" Interpolating an IR volatility curve ",
LibraryName,
ComputeVolatilityArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlComputeVolatility(
LPXLFOPER aVolCurvea,
double aTenor,
double aMaturity)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aVolCurveb(
	(aVolCurvea));
MG_XLObjectPtr aVolCurve(
	aVolCurveb.AsMGXLObject("aVolCurve"));



double result(
	ComputeVolatility(
		aVolCurve,
		aTenor,
		aMaturity)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
DividendsTable_CreateArgs[]=
{
{ "aAsOf"," as of date ","XLF_OPER"},
{ "aExDivDates"," exdiv dates ","XLF_OPER"},
{ "aPaymentDates"," dividends payment dates ","XLF_OPER"},
{ "aDividends"," dividends ","XLF_OPER"},
{ "aZC"," zero curve ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerDividendsTable_Create("xlDividendsTable_Create",
"MG_DividendsTable_Create",
" Creating a Dividend Table for equities ",
LibraryName,
DividendsTable_CreateArgs,
5
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlDividendsTable_Create(
LPXLFOPER aAsOfa,
LPXLFOPER aExDivDatesa,
LPXLFOPER aPaymentDatesa,
LPXLFOPER aDividendsa,
LPXLFOPER aZCa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aAsOfb(
	(aAsOfa));
MG_Date aAsOf(
	aAsOfb.AsMGDate("aAsOf"));

XlfOper aExDivDatesb(
	(aExDivDatesa));
CellMatrix aExDivDates(
	aExDivDatesb.AsCellMatrix("aExDivDates"));

XlfOper aPaymentDatesb(
	(aPaymentDatesa));
CellMatrix aPaymentDates(
	aPaymentDatesb.AsCellMatrix("aPaymentDates"));

XlfOper aDividendsb(
	(aDividendsa));
CellMatrix aDividends(
	aDividendsb.AsCellMatrix("aDividends"));

XlfOper aZCb(
	(aZCa));
MG_XLObjectPtr aZC(
	aZCb.AsMGXLObject("aZC"));

MG_XLObjectPtr result(
	DividendsTable_Create(
		aAsOf,
		aExDivDates,
		aPaymentDates,
		aDividends,
		aZC)
	);
string vRefObj, vError;
if (MG_SCache::Instance()->PersistentInsert(result, vRefObj, vError))
  return XlfOper(vRefObj);
else
  return XlfOper(vError);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
ComputeDiscountedDivsArgs[]=
{
{ "aDividends"," dividends ","XLF_OPER"},
{ "aFirstDate"," first date of computation ","XLF_OPER"},
{ "aLastDate"," last date of computation ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerComputeDiscountedDivs("xlComputeDiscountedDivs",
"MG_ComputeDiscountedDivs",
" Computing the discounted dividends between 2 dates ",
LibraryName,
ComputeDiscountedDivsArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlComputeDiscountedDivs(
LPXLFOPER aDividendsa,
LPXLFOPER aFirstDatea,
LPXLFOPER aLastDatea)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aDividendsb(
	(aDividendsa));
MG_XLObjectPtr aDividends(
	aDividendsb.AsMGXLObject("aDividends"));

XlfOper aFirstDateb(
	(aFirstDatea));
MG_Date aFirstDate(
	aFirstDateb.AsMGDate("aFirstDate"));

XlfOper aLastDateb(
	(aLastDatea));
MG_Date aLastDate(
	aLastDateb.AsMGDate("aLastDate"));

double result(
	ComputeDiscountedDivs(
		aDividends,
		aFirstDate,
		aLastDate)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
GenSec_CreateArgs[]=
{
{ "aDealDesc"," deal description ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerGenSec_Create("xlGenSec_Create",
"MG_GenSec_Create",
" Creating a generic security ",
LibraryName,
GenSec_CreateArgs,
1
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlGenSec_Create(
LPXLFOPER aDealDesca)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aDealDescb(
	(aDealDesca));
CellMatrix aDealDesc(
	aDealDescb.AsCellMatrix("aDealDesc"));

MG_XLObjectPtr result(
	GenSec_Create(
		aDealDesc)
	);
string vRefObj, vError;
if (MG_SCache::Instance()->PersistentInsert(result, vRefObj, vError))
  return XlfOper(vRefObj);
else
  return XlfOper(vError);
EXCEL_END
}
}



//////////////////////////

