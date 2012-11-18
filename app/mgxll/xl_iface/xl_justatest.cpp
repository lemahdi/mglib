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
{ "MktData"," market data robot ","XLF_OPER"}
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
LPXLFOPER MktDataa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper MktDatab(
	(MktDataa));
CellMatrix MktData(
	MktDatab.AsCellMatrix("MktData"));

MG_XLObjectPtr result(
	Robot(
		AsOf,
		MktData)
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
{ "Security"," financial security ","XLF_OPER"},
{ "Model"," financial model ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerPrice("xlPrice",
"MG_Price",
" Discount Factor pricing model ",
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
LPXLFOPER Securitya,
LPXLFOPER Modela)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Securityb(
	(Securitya));
MG_XLObjectPtr Security(
	Securityb.AsMGXLObject("Security"));

XlfOper Modelb(
	(Modela));
MG_XLObjectPtr Model(
	Modelb.AsMGXLObject("Model"));

double result(
	Price(
		Security,
		Model)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
DfModelArgs[]=
{
{ "AsOf"," as of date ","XLF_OPER"},
{ "Robot"," market data robot ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerDfModel("xlDfModel",
"MG_DfModel",
" Discount Factor pricing model ",
LibraryName,
DfModelArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlDfModel(
LPXLFOPER AsOfa,
LPXLFOPER Robota)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper Robotb(
	(Robota));
MG_XLObjectPtr Robot(
	Robotb.AsMGXLObject("Robot"));

MG_XLObjectPtr result(
	DfModel(
		AsOf,
		Robot)
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
{ "Robot"," market data robot ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerBSModel("xlBSModel",
"MG_BSModel",
" Black & Scholes pricing model ",
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
LPXLFOPER Robota)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper Robotb(
	(Robota));
MG_XLObjectPtr Robot(
	Robotb.AsMGXLObject("Robot"));

MG_XLObjectPtr result(
	BSModel(
		AsOf,
		Robot)
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
ZeroCurve_CreateArgs[]=
{
{ "AsOf"," s of date ","XLF_OPER"},
{ "Maturities"," maturities ","XLF_OPER"},
{ "ZeroRates"," zero coupons rates ","XLF_OPER"},
{ "Ccy"," currency ","XLF_OPER"},
{ "UnderIndex"," underlying index ","XLF_OPER"},
{ "[InterpolMeth]"," interpolation method: LINEAR (def.), CONTINUOUS, CONSTANT, STEPUPLEFT, STEPUPRIGHT, POLYNOMIAL, CUBICSPLINE, AKIMA ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerZeroCurve_Create("xlZeroCurve_Create",
"MG_ZeroCurve_Create",
" Creating n zero curve ",
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
LPXLFOPER AsOfa,
LPXLFOPER Maturitiesa,
LPXLFOPER ZeroRatesa,
LPXLFOPER Ccya,
LPXLFOPER UnderIndexa,
LPXLFOPER InterpolMetha)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper Maturitiesb(
	(Maturitiesa));
CellMatrix Maturities(
	Maturitiesb.AsCellMatrix("Maturities"));

XlfOper ZeroRatesb(
	(ZeroRatesa));
CellMatrix ZeroRates(
	ZeroRatesb.AsCellMatrix("ZeroRates"));

XlfOper Ccyb(
	(Ccya));
string Ccy(
	Ccyb.AsString("Ccy"));

XlfOper UnderIndexb(
	(UnderIndexa));
string UnderIndex(
	UnderIndexb.AsString("UnderIndex"));

XlfOper InterpolMethb(
	(InterpolMetha));
string InterpolMeth(
	InterpolMethb.AsStringWD("InterpolMeth","LINEAR"));

MG_XLObjectPtr result(
	ZeroCurve_Create(
		AsOf,
		Maturities,
		ZeroRates,
		Ccy,
		UnderIndex,
		InterpolMeth)
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
{ "ZeroCurve"," zero curve ","XLF_OPER"},
{ "Maturity"," maturity ","XLF_OPER"}
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
LPXLFOPER ZeroCurvea,
LPXLFOPER Maturitya)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper ZeroCurveb(
	(ZeroCurvea));
MG_XLObjectPtr ZeroCurve(
	ZeroCurveb.AsMGXLObject("ZeroCurve"));

XlfOper Maturityb(
	(Maturitya));
double Maturity(
	Maturityb.AsDouble("Maturity"));

double result(
	ComputeZeroRate(
		ZeroCurve,
		Maturity)
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
{ "AsOf"," s of date ","XLF_OPER"},
{ "Maturities"," maturities ","XLF_OPER"},
{ "Tenors"," tenors ","XLF_OPER"},
{ "Volatilities"," volatilities ","XLF_OPER"},
{ "Ccy"," currency ","XLF_OPER"},
{ "UnderIndex"," underlying index ","XLF_OPER"},
{ "InterpolMeths"," 2 interpolation methods: LINEAR (def.), CONTINUOUS, CONSTANT, STEPUPLEFT, STEPUPRIGHT, POLYNOMIAL, CUBICSPLINE, AKIMA ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerVolatilityCurve_Create("xlVolatilityCurve_Create",
"MG_VolatilityCurve_Create",
" Creating n IR volatility curve ",
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
LPXLFOPER AsOfa,
LPXLFOPER Maturitiesa,
LPXLFOPER Tenorsa,
LPXLFOPER Volatilitiesa,
LPXLFOPER Ccya,
LPXLFOPER UnderIndexa,
LPXLFOPER InterpolMethsa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper Maturitiesb(
	(Maturitiesa));
CellMatrix Maturities(
	Maturitiesb.AsCellMatrix("Maturities"));

XlfOper Tenorsb(
	(Tenorsa));
CellMatrix Tenors(
	Tenorsb.AsCellMatrix("Tenors"));

XlfOper Volatilitiesb(
	(Volatilitiesa));
CellMatrix Volatilities(
	Volatilitiesb.AsCellMatrix("Volatilities"));

XlfOper Ccyb(
	(Ccya));
string Ccy(
	Ccyb.AsString("Ccy"));

XlfOper UnderIndexb(
	(UnderIndexa));
string UnderIndex(
	UnderIndexb.AsString("UnderIndex"));

XlfOper InterpolMethsb(
	(InterpolMethsa));
CellMatrix InterpolMeths(
	InterpolMethsb.AsCellMatrix("InterpolMeths"));

MG_XLObjectPtr result(
	VolatilityCurve_Create(
		AsOf,
		Maturities,
		Tenors,
		Volatilities,
		Ccy,
		UnderIndex,
		InterpolMeths)
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
{ "VolCurve"," volatility curve ","XLF_OPER"},
{ "Tenor"," tenor ","XLF_OPER"},
{ "Maturity"," maturity ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerComputeVolatility("xlComputeVolatility",
"MG_ComputeVolatility",
" Interpolating n IR volatility curve ",
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
LPXLFOPER VolCurvea,
LPXLFOPER Tenora,
LPXLFOPER Maturitya)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper VolCurveb(
	(VolCurvea));
MG_XLObjectPtr VolCurve(
	VolCurveb.AsMGXLObject("VolCurve"));

XlfOper Tenorb(
	(Tenora));
double Tenor(
	Tenorb.AsDouble("Tenor"));

XlfOper Maturityb(
	(Maturitya));
double Maturity(
	Maturityb.AsDouble("Maturity"));

double result(
	ComputeVolatility(
		VolCurve,
		Tenor,
		Maturity)
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
{ "AsOf"," s of date ","XLF_OPER"},
{ "ExDivDates"," exdiv dates ","XLF_OPER"},
{ "PaymentDates"," dividends payment dates ","XLF_OPER"},
{ "Dividends"," dividends ","XLF_OPER"},
{ "Ccy"," currency ","XLF_OPER"},
{ "UnderIndex"," underlying index ","XLF_OPER"},
{ "ZC"," zero curve ","XLF_OPER"}
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
LPXLFOPER AsOfa,
LPXLFOPER ExDivDatesa,
LPXLFOPER PaymentDatesa,
LPXLFOPER Dividendsa,
LPXLFOPER Ccya,
LPXLFOPER UnderIndexa,
LPXLFOPER ZCa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper AsOfb(
	(AsOfa));
MG_Date AsOf(
	AsOfb.AsMGDate("AsOf"));

XlfOper ExDivDatesb(
	(ExDivDatesa));
CellMatrix ExDivDates(
	ExDivDatesb.AsCellMatrix("ExDivDates"));

XlfOper PaymentDatesb(
	(PaymentDatesa));
CellMatrix PaymentDates(
	PaymentDatesb.AsCellMatrix("PaymentDates"));

XlfOper Dividendsb(
	(Dividendsa));
CellMatrix Dividends(
	Dividendsb.AsCellMatrix("Dividends"));

XlfOper Ccyb(
	(Ccya));
string Ccy(
	Ccyb.AsString("Ccy"));

XlfOper UnderIndexb(
	(UnderIndexa));
string UnderIndex(
	UnderIndexb.AsString("UnderIndex"));

XlfOper ZCb(
	(ZCa));
MG_XLObjectPtr ZC(
	ZCb.AsMGXLObject("ZC"));

MG_XLObjectPtr result(
	DividendsTable_Create(
		AsOf,
		ExDivDates,
		PaymentDates,
		Dividends,
		Ccy,
		UnderIndex,
		ZC)
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
{ "Dividends"," dividends ","XLF_OPER"},
{ "FirstDate"," first date of computation ","XLF_OPER"},
{ "LastDate"," last date of computation ","XLF_OPER"}
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
LPXLFOPER Dividendsa,
LPXLFOPER FirstDatea,
LPXLFOPER LastDatea)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Dividendsb(
	(Dividendsa));
MG_XLObjectPtr Dividends(
	Dividendsb.AsMGXLObject("Dividends"));

XlfOper FirstDateb(
	(FirstDatea));
MG_Date FirstDate(
	FirstDateb.AsMGDate("FirstDate"));

XlfOper LastDateb(
	(LastDatea));
MG_Date LastDate(
	LastDateb.AsMGDate("LastDate"));

double result(
	ComputeDiscountedDivs(
		Dividends,
		FirstDate,
		LastDate)
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
{ "DealDesc"," deal description ","XLF_OPER"}
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
LPXLFOPER DealDesca)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper DealDescb(
	(DealDesca));
CellMatrix DealDesc(
	DealDescb.AsCellMatrix("DealDesc"));

MG_XLObjectPtr result(
	GenSec_Create(
		DealDesc)
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
{ "Type"," generator type: TAUS, MT19937, .. ","XLF_OPER"},
{ "Dim"," generator dimension, for quasi random ","XLF_OPER"}
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
LPXLFOPER Typea,
LPXLFOPER Dima)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Typeb(
	(Typea));
string Type(
	Typeb.AsString("Type"));

XlfOper Dimb(
	(Dima));
int Dim(
	Dimb.AsInt("Dim"));

MG_XLObjectPtr result(
	RandGen_Create(
		Type,
		Dim)
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
{ "Date"," reference date ","XLF_OPER"},
{ "Days"," days fter ","XLF_OPER"},
{ "Calendar"," calendar for business days ","XLF_OPER"}
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
LPXLFOPER Datea,
LPXLFOPER Daysa,
LPXLFOPER Calendara)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Dateb(
	(Datea));
MG_Date Date(
	Dateb.AsMGDate("Date"));

XlfOper Daysb(
	(Daysa));
int Days(
	Daysb.AsInt("Days"));

XlfOper Calendarb(
	(Calendara));
string Calendar(
	Calendarb.AsString("Calendar"));

MG_Date result(
	NextBusinessDay(
		Date,
		Days,
		Calendar)
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
{ "Date"," reference date ","XLF_OPER"},
{ "Days"," days before ","XLF_OPER"},
{ "Calendar"," calendar for business days ","XLF_OPER"}
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
LPXLFOPER Datea,
LPXLFOPER Daysa,
LPXLFOPER Calendara)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Dateb(
	(Datea));
MG_Date Date(
	Dateb.AsMGDate("Date"));

XlfOper Daysb(
	(Daysa));
int Days(
	Daysb.AsInt("Days"));

XlfOper Calendarb(
	(Calendara));
string Calendar(
	Calendarb.AsString("Calendar"));

MG_Date result(
	PreviousBusinessDay(
		Date,
		Days,
		Calendar)
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
{ "Date1"," first date ","XLF_OPER"},
{ "Date2"," second date ","XLF_OPER"},
{ "DayCount"," day count: ACT, A365, A360, 30/360, 30/360E, B252 ","XLF_OPER"},
{ "[IsFrac]"," true: fraction, false: days difference (def.) ","XLF_OPER"},
{ "[Calendar]"," calendar for business days ","XLF_OPER"}
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
LPXLFOPER Date1a,
LPXLFOPER Date2a,
LPXLFOPER DayCounta,
LPXLFOPER IsFraca,
LPXLFOPER Calendara)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Date1b(
	(Date1a));
MG_Date Date1(
	Date1b.AsMGDate("Date1"));

XlfOper Date2b(
	(Date2a));
MG_Date Date2(
	Date2b.AsMGDate("Date2"));

XlfOper DayCountb(
	(DayCounta));
string DayCount(
	DayCountb.AsString("DayCount"));

XlfOper IsFracb(
	(IsFraca));
bool IsFrac(
	IsFracb.AsBoolWD("IsFrac",true));

XlfOper Calendarb(
	(Calendara));
string Calendar(
	Calendarb.AsStringWD("Calendar",""));

double result(
	BetweenDates(
		Date1,
		Date2,
		DayCount,
		IsFrac,
		Calendar)
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
{ "Date"," reference date ","XLF_OPER"},
{ "Freq"," frequency: nY, Y, A, S, Q, B, nM, M, nW, W, nD, D ","XLF_OPER"},
{ "[Times]"," times number to dd frequency (1 by def.) ","XLF_OPER"},
{ "[Calendar]"," calendar for business days (EUR by def.) ","XLF_OPER"},
{ "[AdjRule]"," djustment rule: FIXED, FP, F, MF (def.), PP, P, MP ","XLF_OPER"},
{ "[EndOfMonth]"," true: go to the end of the month (def.) ","XLF_OPER"}
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
LPXLFOPER Datea,
LPXLFOPER Freqa,
LPXLFOPER Timesa,
LPXLFOPER Calendara,
LPXLFOPER AdjRulea,
LPXLFOPER EndOfMontha)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Dateb(
	(Datea));
MG_Date Date(
	Dateb.AsMGDate("Date"));

XlfOper Freqb(
	(Freqa));
string Freq(
	Freqb.AsString("Freq"));

XlfOper Timesb(
	(Timesa));
int Times(
	Timesb.AsIntWD("Times",1));

XlfOper Calendarb(
	(Calendara));
string Calendar(
	Calendarb.AsStringWD("Calendar",CALENDAR_NAME_DEF_STR));

XlfOper AdjRuleb(
	(AdjRulea));
string AdjRule(
	AdjRuleb.AsStringWD("AdjRule","MF"));

XlfOper EndOfMonthb(
	(EndOfMontha));
bool EndOfMonth(
	EndOfMonthb.AsBoolWD("EndOfMonth",true));

MG_Date result(
	AddPeriod(
		Date,
		Freq,
		Times,
		Calendar,
		AdjRule,
		EndOfMonth)
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
IRIndex_CreateArgs[]=
{
{ "[IndexName]"," index name: LIBOR3M, EUBOR6M (def.), CMS1, ... ","XLF_OPER"},
{ "[Currency]"," currency (EUR by def.) ","XLF_OPER"},
{ "[DayCount]"," day count: ACTUAL, A360, A365 (def.), 30/360, 30/360E, B252 ","XLF_OPER"},
{ "[AdjRule]"," adjustment rule: FIXED, FP, F, MF (def.), PP, P, MP ","XLF_OPER"},
{ "[ResetTiming]"," reset timing: ADV (def.), ARR ","XLF_OPER"},
{ "[PayTiming]"," pay timing: ADV, ARR (def.) ","XLF_OPER"},
{ "[ResetCalendar]"," reset calendar (EUR by def.) ","XLF_OPER"},
{ "[PayCalendar]"," pay calendar (EUR by def.) ","XLF_OPER"},
{ "[ResetGap]"," reset gap (-2 by def.) ","XLF_OPER"},
{ "[PayGap]"," pay gap (0 by def.) ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerIRIndex_Create("xlIRIndex_Create",
"MG_IRIndex_Create",
" create an interest rate index ",
LibraryName,
IRIndex_CreateArgs,
10
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlIRIndex_Create(
LPXLFOPER IndexNamea,
LPXLFOPER Currencya,
LPXLFOPER DayCounta,
LPXLFOPER AdjRulea,
LPXLFOPER ResetTiminga,
LPXLFOPER PayTiminga,
LPXLFOPER ResetCalendara,
LPXLFOPER PayCalendara,
LPXLFOPER ResetGapa,
LPXLFOPER PayGapa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper IndexNameb(
	(IndexNamea));
string IndexName(
	IndexNameb.AsStringWD("IndexName",INDEX_NAME_DEF_STR));

XlfOper Currencyb(
	(Currencya));
string Currency(
	Currencyb.AsStringWD("Currency",CURRENCY_NAME_DEF_STR));

XlfOper DayCountb(
	(DayCounta));
string DayCount(
	DayCountb.AsStringWD("DayCount","A365"));

XlfOper AdjRuleb(
	(AdjRulea));
string AdjRule(
	AdjRuleb.AsStringWD("AdjRule","MF"));

XlfOper ResetTimingb(
	(ResetTiminga));
string ResetTiming(
	ResetTimingb.AsStringWD("ResetTiming","ADV"));

XlfOper PayTimingb(
	(PayTiminga));
string PayTiming(
	PayTimingb.AsStringWD("PayTiming","ARR"));

XlfOper ResetCalendarb(
	(ResetCalendara));
string ResetCalendar(
	ResetCalendarb.AsStringWD("ResetCalendar",CALENDAR_NAME_DEF_STR));

XlfOper PayCalendarb(
	(PayCalendara));
string PayCalendar(
	PayCalendarb.AsStringWD("PayCalendar",CALENDAR_NAME_DEF_STR));

XlfOper ResetGapb(
	(ResetGapa));
int ResetGap(
	ResetGapb.AsIntWD("ResetGap",-2));

XlfOper PayGapb(
	(PayGapa));
int PayGap(
	PayGapb.AsIntWD("PayGap",0));

MG_XLObjectPtr result(
	IRIndex_Create(
		IndexName,
		Currency,
		DayCount,
		AdjRule,
		ResetTiming,
		PayTiming,
		ResetCalendar,
		PayCalendar,
		ResetGap,
		PayGap)
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
Schedule_CreateArgs[]=
{
{ "StartDate"," start date ","XLF_OPER"},
{ "EndDate"," end date ","XLF_OPER"},
{ "IRIndex"," interest rate index ","XLF_OPER"},
{ "Freq"," frequency: Y, A, S, Q, B, M, W, D ","XLF_OPER"},
{ "[IntRule]"," interest rule: ADJ (def.), UNADJ ","XLF_OPER"},
{ "[StubRule]"," stub rule: SS (def.), LS, SE, LE ","XLF_OPER"},
{ "[IsDecompound]"," decompounding frequency: true (def.) or false ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerSchedule_Create("xlSchedule_Create",
"MG_Schedule_Create",
" create a schedule ",
LibraryName,
Schedule_CreateArgs,
7
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlSchedule_Create(
LPXLFOPER StartDatea,
LPXLFOPER EndDatea,
LPXLFOPER IRIndexa,
LPXLFOPER Freqa,
LPXLFOPER IntRulea,
LPXLFOPER StubRulea,
LPXLFOPER IsDecompounda)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper StartDateb(
	(StartDatea));
MG_Date StartDate(
	StartDateb.AsMGDate("StartDate"));

XlfOper EndDateb(
	(EndDatea));
MG_Date EndDate(
	EndDateb.AsMGDate("EndDate"));

XlfOper IRIndexb(
	(IRIndexa));
MG_XLObjectPtr IRIndex(
	IRIndexb.AsMGXLObject("IRIndex"));

XlfOper Freqb(
	(Freqa));
string Freq(
	Freqb.AsString("Freq"));

XlfOper IntRuleb(
	(IntRulea));
string IntRule(
	IntRuleb.AsStringWD("IntRule","ADJ"));

XlfOper StubRuleb(
	(StubRulea));
string StubRule(
	StubRuleb.AsStringWD("StubRule","SS"));

XlfOper IsDecompoundb(
	(IsDecompounda));
bool IsDecompound(
	IsDecompoundb.AsBoolWD("IsDecompound",true));

MG_XLObjectPtr result(
	Schedule_Create(
		StartDate,
		EndDate,
		IRIndex,
		Freq,
		IntRule,
		StubRule,
		IsDecompound)
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
Schedule_GetDataArgs[]=
{
{ "Schedule"," schedule ","XLF_OPER"},
{ "Data"," ResetDate, StartDate, EndDate, PayDate, InterestDays, InterestTerms, FwdStartDate, FwdEndDate ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerSchedule_GetData("xlSchedule_GetData",
"MG_Schedule_GetData",
" get information from schedule ",
LibraryName,
Schedule_GetDataArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlSchedule_GetData(
LPXLFOPER Schedulea,
LPXLFOPER Dataa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Scheduleb(
	(Schedulea));
MG_XLObjectPtr Schedule(
	Scheduleb.AsMGXLObject("Schedule"));

XlfOper Datab(
	(Dataa));
string Data(
	Datab.AsString("Data"));

CellMatrix result(
	Schedule_GetData(
		Schedule,
		Data)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
TermStructure_CreateArgs[]=
{
{ "PayDates"," payment dates ","XLF_OPER"},
{ "Values"," values ","XLF_OPER"},
{ "[InterpolMeth]"," interpolation method (LINEAR by def.) ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerTermStructure_Create("xlTermStructure_Create",
"MG_TermStructure_Create",
" create a term structure ",
LibraryName,
TermStructure_CreateArgs,
3
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlTermStructure_Create(
LPXLFOPER PayDatesa,
LPXLFOPER Valuesa,
LPXLFOPER InterpolMetha)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper PayDatesb(
	(PayDatesa));
CellMatrix PayDates(
	PayDatesb.AsCellMatrix("PayDates"));

XlfOper Valuesb(
	(Valuesa));
CellMatrix Values(
	Valuesb.AsCellMatrix("Values"));

XlfOper InterpolMethb(
	(InterpolMetha));
string InterpolMeth(
	InterpolMethb.AsStringWD("InterpolMeth","LINEAR"));

MG_XLObjectPtr result(
	TermStructure_Create(
		PayDates,
		Values,
		InterpolMeth)
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
TermStructure_ComputeArgs[]=
{
{ "TermStruct"," term structure ","XLF_OPER"},
{ "PayDate"," payment date ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerTermStructure_Compute("xlTermStructure_Compute",
"MG_TermStructure_Compute",
" create a term structure ",
LibraryName,
TermStructure_ComputeArgs,
2
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlTermStructure_Compute(
LPXLFOPER TermStructa,
LPXLFOPER PayDatea)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper TermStructb(
	(TermStructa));
MG_XLObjectPtr TermStruct(
	TermStructb.AsMGXLObject("TermStruct"));

XlfOper PayDateb(
	(PayDatea));
MG_Date PayDate(
	PayDateb.AsMGDate("PayDate"));

double result(
	TermStructure_Compute(
		TermStruct,
		PayDate)
	);
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////

namespace
{
XLRegistration::Arg
SwapLeg_CreateArgs[]=
{
{ "Start"," start date or term ","XLF_OPER"},
{ "End"," end date or duration ","XLF_OPER"},
{ "RcvPay"," receive or pay ","XLF_OPER"},
{ "IRIndex"," interest rate index ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerSwapLeg_Create("xlSwapLeg_Create",
"MG_SwapLeg_Create",
" create a swap leg ",
LibraryName,
SwapLeg_CreateArgs,
4
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlSwapLeg_Create(
LPXLFOPER Starta,
LPXLFOPER Enda,
LPXLFOPER RcvPaya,
LPXLFOPER IRIndexa)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Startb(
	(Starta));
MG_GenericDate Start(
	Startb.AsMGGenDate("Start"));

XlfOper Endb(
	(Enda));
MG_GenericDate End(
	Endb.AsMGGenDate("End"));

XlfOper RcvPayb(
	(RcvPaya));
string RcvPay(
	RcvPayb.AsString("RcvPay"));

XlfOper IRIndexb(
	(IRIndexa));
MG_XLObjectPtr IRIndex(
	IRIndexb.AsMGXLObject("IRIndex"));

MG_XLObjectPtr result(
	SwapLeg_Create(
		Start,
		End,
		RcvPay,
		IRIndex)
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
CallPut_CreateArgs[]=
{
{ "Maturity","too lazy to comment this one ","XLF_OPER"},
{ "Underlying"," underlying ","XLF_OPER"},
{ "CallPut"," call (C) or put (P) ","XLF_OPER"},
{ "Strike"," strike ","XLF_OPER"}
};
  XLRegistration::XLFunctionRegistrationHelper
registerCallPut_Create("xlCallPut_Create",
"MG_CallPut_Create",
" create a call or put ",
LibraryName,
CallPut_CreateArgs,
4
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlCallPut_Create(
LPXLFOPER Maturitya,
LPXLFOPER Underlyinga,
LPXLFOPER CallPuta,
LPXLFOPER Strikea)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

XlfOper Maturityb(
	(Maturitya));
MG_GenericDate Maturity(
	Maturityb.AsMGGenDate("Maturity"));

XlfOper Underlyingb(
	(Underlyinga));
MG_XLObjectPtr Underlying(
	Underlyingb.AsMGXLObject("Underlying"));

XlfOper CallPutb(
	(CallPuta));
string CallPut(
	CallPutb.AsString("CallPut"));

XlfOper Strikeb(
	(Strikea));
double Strike(
	Strikeb.AsDouble("Strike"));

MG_XLObjectPtr result(
	CallPut_Create(
		Maturity,
		Underlying,
		CallPut,
		Strike)
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

