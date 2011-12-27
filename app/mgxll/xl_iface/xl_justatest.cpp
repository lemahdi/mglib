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
RobotArgs[]=
{
{ "AsOf"," as of date ","XLF_OPER"},
{ "aMktData"," market data robot ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerRobot("xlRobot",
"MG_Robot",
" Market data robot ",
LibraryName,
RobotArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlRobot(
LPXLFOPER AsOfa,
LPXLFOPER aMktDataa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper aMktDatab(
	(aMktDataa));
CellMatrix aMktData(
	aMktDatab.AsCellMatrix("aMktData"));

MG_XLObjectPtr result(
	Robot(
		AsOf,
		aMktData)
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
BSModelArgs[]=
{
{ "AsOf"," as of date ","XLF_OPER"},
{ "aRobot"," market data robot ","XLF_OPER"}
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
LPXLFOPER aRobota)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper aRobotb(
	(aRobota));
MG_XLObjectPtr aRobot(
	aRobotb.AsMGXLObject("aRobot"));

MG_XLObjectPtr result(
	BSModel(
		AsOf,
		aRobot)
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
{ "Strike"," strike ","XLF_OPER"},
{ "Maturity"," maturity ","XLF_OPER"},
{ "Forward"," forward ","XLF_OPER"}
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
LPXLFOPER Strikea,
LPXLFOPER Maturitya,
LPXLFOPER Forwarda)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Strikeb(
	(Strikea));
double Strike(
	Strikeb.AsDouble("Strike"));

XlfOper Maturityb(
	(Maturitya));
double Maturity(
	Maturityb.AsDouble("Maturity"));

XlfOper Forwardb(
	(Forwarda));
double Forward(
	Forwardb.AsDouble("Forward"));

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
{ "aZeroRates"," volatilities ","XLF_OPER"},
{ "aCcy"," currency ","XLF_OPER"},
{ "aUnderIndex"," underlying index ","XLF_OPER"},
{ "aInterpolMeth"," interpolation method (LINEAR by def.) ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerZeroCurve_Create("xlZeroCurve_Create",
"MG_ZeroCurve_Create",
" Creating an zero curve ",
LibraryName,
ZeroCurve_CreateArgs,
6
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlZeroCurve_Create(
LPXLFOPER aAsOfa,
LPXLFOPER aMaturitiesa,
LPXLFOPER aZeroRatesa,
LPXLFOPER aCcya,
LPXLFOPER aUnderIndexa,
LPXLFOPER aInterpolMetha)
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

XlfOper aCcyb(
	(aCcya));
string aCcy(
	aCcyb.AsString("aCcy"));

XlfOper aUnderIndexb(
	(aUnderIndexa));
string aUnderIndex(
	aUnderIndexb.AsString("aUnderIndex"));

XlfOper aInterpolMethb(
	(aInterpolMetha));
CellMatrix aInterpolMeth(
	aInterpolMethb.AsCellMatrix("aInterpolMeth"));

MG_XLObjectPtr result(
	ZeroCurve_Create(
		aAsOf,
		aMaturities,
		aZeroRates,
		aCcy,
		aUnderIndex,
		aInterpolMeth)
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
{ "aMaturity"," maturity ","XLF_OPER"}
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
LPXLFOPER aMaturitya)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aZeroCurveb(
	(aZeroCurvea));
MG_XLObjectPtr aZeroCurve(
	aZeroCurveb.AsMGXLObject("aZeroCurve"));

XlfOper aMaturityb(
	(aMaturitya));
double aMaturity(
	aMaturityb.AsDouble("aMaturity"));

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
{ "aVolatilities"," volatilities ","XLF_OPER"},
{ "aCcy"," currency ","XLF_OPER"},
{ "aUnderIndex"," underlying index ","XLF_OPER"},
{ "aInterpolMeths"," interpolation methods (LINEAR by def.) ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerVolatilityCurve_Create("xlVolatilityCurve_Create",
"MG_VolatilityCurve_Create",
" Creating an IR volatility curve ",
LibraryName,
VolatilityCurve_CreateArgs,
7
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
LPXLFOPER aVolatilitiesa,
LPXLFOPER aCcya,
LPXLFOPER aUnderIndexa,
LPXLFOPER aInterpolMethsa)
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

XlfOper aCcyb(
	(aCcya));
string aCcy(
	aCcyb.AsString("aCcy"));

XlfOper aUnderIndexb(
	(aUnderIndexa));
string aUnderIndex(
	aUnderIndexb.AsString("aUnderIndex"));

XlfOper aInterpolMethsb(
	(aInterpolMethsa));
CellMatrix aInterpolMeths(
	aInterpolMethsb.AsCellMatrix("aInterpolMeths"));

MG_XLObjectPtr result(
	VolatilityCurve_Create(
		aAsOf,
		aMaturities,
		aTenors,
		aVolatilities,
		aCcy,
		aUnderIndex,
		aInterpolMeths)
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
{ "aTenor"," tenor ","XLF_OPER"},
{ "aMaturity"," maturity ","XLF_OPER"}
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
LPXLFOPER aTenora,
LPXLFOPER aMaturitya)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aVolCurveb(
	(aVolCurvea));
MG_XLObjectPtr aVolCurve(
	aVolCurveb.AsMGXLObject("aVolCurve"));

XlfOper aTenorb(
	(aTenora));
double aTenor(
	aTenorb.AsDouble("aTenor"));

XlfOper aMaturityb(
	(aMaturitya));
double aMaturity(
	aMaturityb.AsDouble("aMaturity"));

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
{ "aCcy"," currency ","XLF_OPER"},
{ "aUnderIndex"," underlying index ","XLF_OPER"},
{ "aZC"," zero curve ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerDividendsTable_Create("xlDividendsTable_Create",
"MG_DividendsTable_Create",
" Creating a Dividend Table for equities ",
LibraryName,
DividendsTable_CreateArgs,
7
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
LPXLFOPER aCcya,
LPXLFOPER aUnderIndexa,
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

XlfOper aCcyb(
	(aCcya));
string aCcy(
	aCcyb.AsString("aCcy"));

XlfOper aUnderIndexb(
	(aUnderIndexa));
string aUnderIndex(
	aUnderIndexb.AsString("aUnderIndex"));

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
		aCcy,
		aUnderIndex,
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

namespace
{
XLRegistration::Arg
RandGen_CreateArgs[]=
{
{ "aType"," generator type: TAUS, MT19937, .. ","XLF_OPER"},
{ "aDim"," generator dimension, for quasi random ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerRandGen_Create("xlRandGen_Create",
"MG_RandGen_Create",
" Creating a random generator ",
LibraryName,
RandGen_CreateArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlRandGen_Create(
LPXLFOPER aTypea,
LPXLFOPER aDima)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aTypeb(
	(aTypea));
string aType(
	aTypeb.AsString("aType"));

XlfOper aDimb(
	(aDima));
int aDim(
	aDimb.AsInt("aDim"));

MG_XLObjectPtr result(
	RandGen_Create(
		aType,
		aDim)
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
NextBusinessDayArgs[]=
{
{ "aDate"," reference date ","XLF_OPER"},
{ "aDays"," days after ","XLF_OPER"},
{ "aCalendar"," calendar for business days ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerNextBusinessDay("xlNextBusinessDay",
"MG_NextBusinessDay",
" next business day ",
LibraryName,
NextBusinessDayArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlNextBusinessDay(
LPXLFOPER aDatea,
LPXLFOPER aDaysa,
LPXLFOPER aCalendara)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aDateb(
	(aDatea));
MG_Date aDate(
	aDateb.AsMGDate("aDate"));

XlfOper aDaysb(
	(aDaysa));
int aDays(
	aDaysb.AsInt("aDays"));

XlfOper aCalendarb(
	(aCalendara));
string aCalendar(
	aCalendarb.AsString("aCalendar"));

MG_Date result(
	NextBusinessDay(
		aDate,
		aDays,
		aCalendar)
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
PreviousBusinessDayArgs[]=
{
{ "aDate"," reference date ","XLF_OPER"},
{ "aDays"," days before ","XLF_OPER"},
{ "aCalendar"," calendar for business days ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerPreviousBusinessDay("xlPreviousBusinessDay",
"MG_PreviousBusinessDay",
" previous business day ",
LibraryName,
PreviousBusinessDayArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlPreviousBusinessDay(
LPXLFOPER aDatea,
LPXLFOPER aDaysa,
LPXLFOPER aCalendara)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aDateb(
	(aDatea));
MG_Date aDate(
	aDateb.AsMGDate("aDate"));

XlfOper aDaysb(
	(aDaysa));
int aDays(
	aDaysb.AsInt("aDays"));

XlfOper aCalendarb(
	(aCalendara));
string aCalendar(
	aCalendarb.AsString("aCalendar"));

MG_Date result(
	PreviousBusinessDay(
		aDate,
		aDays,
		aCalendar)
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
BetweenDatesArgs[]=
{
{ "aDate1"," first date ","XLF_OPER"},
{ "aDate2"," second date ","XLF_OPER"},
{ "aDayCount"," day count: ACT, A365, A360, 30/360, 30/360E, B252 ","XLF_OPER"},
{ "aIsFrac"," 0: fraction, 1: days difference (def.) ","XLF_OPER"},
{ "aCalendar"," calendar for business days ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerBetweenDates("xlBetweenDates",
"MG_BetweenDates",
" previous business day ",
LibraryName,
BetweenDatesArgs,
5
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlBetweenDates(
LPXLFOPER aDate1a,
LPXLFOPER aDate2a,
LPXLFOPER aDayCounta,
LPXLFOPER aIsFraca,
LPXLFOPER aCalendara)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aDate1b(
	(aDate1a));
MG_Date aDate1(
	aDate1b.AsMGDate("aDate1"));

XlfOper aDate2b(
	(aDate2a));
MG_Date aDate2(
	aDate2b.AsMGDate("aDate2"));

XlfOper aDayCountb(
	(aDayCounta));
string aDayCount(
	aDayCountb.AsString("aDayCount"));

XlfOper aIsFracb(
	(aIsFraca));
int aIsFrac(
	aIsFracb.AsIntWD("aIsFrac",1));

XlfOper aCalendarb(
	(aCalendara));
string aCalendar(
	aCalendarb.AsStringWD("aCalendar",""));

double result(
	BetweenDates(
		aDate1,
		aDate2,
		aDayCount,
		aIsFrac,
		aCalendar)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
AddPeriodArgs[]=
{
{ "aDate"," reference date ","XLF_OPER"},
{ "aFreq"," frequency: nY, Y, A, S, Q, B, nM, M, nW, W, nD, D ","XLF_OPER"},
{ "aTimes"," times number to add frequency ","XLF_OPER"},
{ "aCalendar"," calendar for business days ","XLF_OPER"},
{ "aAdjRule"," adjustment rule: FIXED, FP, F, MF (def.), PP, P, MP ","XLF_OPER"},
{ "aEndOfMonth"," 0: go to the end of the month (def.) ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerAddPeriod("xlAddPeriod",
"MG_AddPeriod",
" Computing a date ",
LibraryName,
AddPeriodArgs,
6
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlAddPeriod(
LPXLFOPER aDatea,
LPXLFOPER aFreqa,
LPXLFOPER aTimesa,
LPXLFOPER aCalendara,
LPXLFOPER aAdjRulea,
LPXLFOPER aEndOfMontha)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper aDateb(
	(aDatea));
MG_Date aDate(
	aDateb.AsMGDate("aDate"));

XlfOper aFreqb(
	(aFreqa));
string aFreq(
	aFreqb.AsString("aFreq"));

XlfOper aTimesb(
	(aTimesa));
int aTimes(
	aTimesb.AsInt("aTimes"));

XlfOper aCalendarb(
	(aCalendara));
string aCalendar(
	aCalendarb.AsString("aCalendar"));

XlfOper aAdjRuleb(
	(aAdjRulea));
string aAdjRule(
	aAdjRuleb.AsStringWD("aAdjRule","MF"));

XlfOper aEndOfMonthb(
	(aEndOfMontha));
bool aEndOfMonth(
	aEndOfMonthb.AsBoolWD("aEndOfMonth",true));

MG_Date result(
	AddPeriod(
		aDate,
		aFreq,
		aTimes,
		aCalendar,
		aAdjRule,
		aEndOfMonth)
	);
double vXLDate = MG_utils::FromJulianDayToXLDate(result.GetJulianDay());
return XlfOper(vXLDate);
EXCEL_END
}
}



//////////////////////////

