#include "nova/glob/argconvdef.h"


using namespace std;


MG_NAMESPACE_BEGIN


MG_ArgConv::ArgConvPair MonthsArgConv[] =
{
	MG_ArgConv::ArgConvPair("JAN",K_JAN),
	MG_ArgConv::ArgConvPair("FEB",K_FEB),
	MG_ArgConv::ArgConvPair("MAR",K_MAR),
	MG_ArgConv::ArgConvPair("APR",K_APR),
	MG_ArgConv::ArgConvPair("MAI",K_MAY),
	MG_ArgConv::ArgConvPair("JUN",K_JUN),
	MG_ArgConv::ArgConvPair("JUL",K_JUL),
	MG_ArgConv::ArgConvPair("AUG",K_AUG),
	MG_ArgConv::ArgConvPair("SEP",K_SEP),
	MG_ArgConv::ArgConvPair("OCT",K_OCT),
	MG_ArgConv::ArgConvPair("NOV",K_NOV),
	MG_ArgConv::ArgConvPair("DEC",K_DEC),
};
const MG_ArgConv		MonthsConvertor	= MG_ArgConv		("Months Three Letters", MonthsArgConv, NB_MONTHS_3L);
const MG_ArgConvReverse	MonthsRevertor	= MG_ArgConvReverse	("Months Three Letters", MonthsArgConv, NB_MONTHS_3L);


MG_ArgConv::ArgConvPair IndexNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("FIXED",K_FIXED),
	MG_ArgConv::ArgConvPair("OIS",K_OIS),
	MG_ArgConv::ArgConvPair("EONIA",K_EONIA),
	MG_ArgConv::ArgConvPair("SONIA",K_SONIA),
	MG_ArgConv::ArgConvPair("LIBOR1M",K_LIBOR1M),
	MG_ArgConv::ArgConvPair("LIBOR3M",K_LIBOR3M),
	MG_ArgConv::ArgConvPair("LIBOR6M",K_LIBOR6M),
	MG_ArgConv::ArgConvPair("LIBOR12M",K_LIBOR12M),
	MG_ArgConv::ArgConvPair("EUBOR1M",K_EUBOR1M),
	MG_ArgConv::ArgConvPair("EUBOR3M",K_EUBOR3M),
	MG_ArgConv::ArgConvPair("EUBOR6M",K_EUBOR6M),
	MG_ArgConv::ArgConvPair("EUBOR12M",K_EUBOR12M),
	MG_ArgConv::ArgConvPair("CMS1",K_CMS1),
	MG_ArgConv::ArgConvPair("CMS2",K_CMS2),
	MG_ArgConv::ArgConvPair("CMS3",K_CMS3),
	MG_ArgConv::ArgConvPair("CMS4",K_CMS4),
	MG_ArgConv::ArgConvPair("CMS5",K_CMS5),
	MG_ArgConv::ArgConvPair("CMS6",K_CMS6),
	MG_ArgConv::ArgConvPair("CMS7",K_CMS7),
	MG_ArgConv::ArgConvPair("CMS8",K_CMS8),
	MG_ArgConv::ArgConvPair("CMS9",K_CMS9),
	MG_ArgConv::ArgConvPair("CMS10",K_CMS10),
	MG_ArgConv::ArgConvPair("CMS11",K_CMS11),
	MG_ArgConv::ArgConvPair("CMS12",K_CMS12),
	MG_ArgConv::ArgConvPair("CMS13",K_CMS13),
	MG_ArgConv::ArgConvPair("CMS14",K_CMS14),
	MG_ArgConv::ArgConvPair("CMS15",K_CMS15),
	MG_ArgConv::ArgConvPair("CMS16",K_CMS16),
	MG_ArgConv::ArgConvPair("CMS17",K_CMS17),
	MG_ArgConv::ArgConvPair("CMS18",K_CMS18),
	MG_ArgConv::ArgConvPair("CMS19",K_CMS19),
	MG_ArgConv::ArgConvPair("CMS20",K_CMS20),
	MG_ArgConv::ArgConvPair("CMS21",K_CMS21),
	MG_ArgConv::ArgConvPair("CMS22",K_CMS22),
	MG_ArgConv::ArgConvPair("CMS23",K_CMS23),
	MG_ArgConv::ArgConvPair("CMS24",K_CMS24),
	MG_ArgConv::ArgConvPair("CMS25",K_CMS25),
	MG_ArgConv::ArgConvPair("CMS26",K_CMS26),
	MG_ArgConv::ArgConvPair("CMS27",K_CMS27),
	MG_ArgConv::ArgConvPair("CMS28",K_CMS28),
	MG_ArgConv::ArgConvPair("CMS29",K_CMS29),
	MG_ArgConv::ArgConvPair("CMS30",K_CMS30),
	MG_ArgConv::ArgConvPair("CMS21",K_CMS31),
	MG_ArgConv::ArgConvPair("CMS22",K_CMS32),
	MG_ArgConv::ArgConvPair("CMS23",K_CMS33),
	MG_ArgConv::ArgConvPair("CMS24",K_CMS34),
	MG_ArgConv::ArgConvPair("CMS25",K_CMS35),
	MG_ArgConv::ArgConvPair("CMS26",K_CMS36),
	MG_ArgConv::ArgConvPair("CMS27",K_CMS37),
	MG_ArgConv::ArgConvPair("CMS28",K_CMS38),
	MG_ArgConv::ArgConvPair("CMS29",K_CMS39),
	MG_ArgConv::ArgConvPair("CMS40",K_CMS40),
	MG_ArgConv::ArgConvPair("CMS21",K_CMS41),
	MG_ArgConv::ArgConvPair("CMS22",K_CMS42),
	MG_ArgConv::ArgConvPair("CMS23",K_CMS43),
	MG_ArgConv::ArgConvPair("CMS24",K_CMS44),
	MG_ArgConv::ArgConvPair("CMS25",K_CMS45),
	MG_ArgConv::ArgConvPair("CMS26",K_CMS46),
	MG_ArgConv::ArgConvPair("CMS27",K_CMS47),
	MG_ArgConv::ArgConvPair("CMS28",K_CMS48),
	MG_ArgConv::ArgConvPair("CMS29",K_CMS49),
	MG_ArgConv::ArgConvPair("CMS50",K_CMS50),
};
const MG_ArgConv		IndexNameConvertor	= MG_ArgConv		("Index", IndexNameArgConv, NB_INDEX);
const MG_ArgConvReverse	IndexNameRevertor	= MG_ArgConvReverse	("Index", IndexNameArgConv, NB_INDEX);


MG_ArgConv::ArgConvPair CurrenciesNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("USD",K_USD),
	MG_ArgConv::ArgConvPair("EUR",K_EUR),
	MG_ArgConv::ArgConvPair("JPY",K_JPY),
	MG_ArgConv::ArgConvPair("GBP",K_GBP),
};
const MG_ArgConv		CurrenciesNameConvertor	= MG_ArgConv		("Currencies Name Three Letters", CurrenciesNameArgConv, NB_CURRENCIES);
const MG_ArgConvReverse	CurrenciesNameRevertor	= MG_ArgConvReverse	("Currencies Name Three Letters", CurrenciesNameArgConv, NB_CURRENCIES);


MG_ArgConv::ArgConvPair CalendarsNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("USD",K_USD_CAL),
	MG_ArgConv::ArgConvPair("EUR",K_EUR_CAL),
	MG_ArgConv::ArgConvPair("JPY",K_JPY_CAL),
	MG_ArgConv::ArgConvPair("GBP",K_GBP_CAL),
};
const MG_ArgConv		CalendarsNameConvertor	= MG_ArgConv		("Calendars Name", CalendarsNameArgConv, NB_CALENDARS);
const MG_ArgConvReverse	CalendarsNameRevertor	= MG_ArgConvReverse	("Calendars Name", CalendarsNameArgConv, NB_CALENDARS);


MG_ArgConv::ArgConvPair FrequencyNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("D",K_DAILY),
	MG_ArgConv::ArgConvPair("W",K_WEEKLY),
	MG_ArgConv::ArgConvPair("M",K_MONTHLY),
	MG_ArgConv::ArgConvPair("B",K_BIMONTHLY),
	MG_ArgConv::ArgConvPair("Q",K_QUARTERLY),
	MG_ArgConv::ArgConvPair("S",K_SEMESTERLY),
	MG_ArgConv::ArgConvPair("Y",K_YEARLY),
};
const MG_ArgConv		FrequencyNameConvertor	= MG_ArgConv		("Frequencies", FrequencyNameArgConv, NB_FREQUENCIES);
const MG_ArgConvReverse	FrequencyNameRevertor	= MG_ArgConvReverse	("Frequencies", FrequencyNameArgConv, NB_FREQUENCIES);


MG_ArgConv::ArgConvPair DayCountNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("ACTUAL",K_ACT),
	MG_ArgConv::ArgConvPair("A360",K_ACT_360),
	MG_ArgConv::ArgConvPair("A365",K_ACT_365),
	MG_ArgConv::ArgConvPair("30/360",K_U_30_360),
	MG_ArgConv::ArgConvPair("30/360E",K_E_30_360),
	MG_ArgConv::ArgConvPair("B252",K_BUS_252),
};
const MG_ArgConv		DayCountNameConvertor	= MG_ArgConv		("Day Count Basis", DayCountNameArgConv, NB_DAYCOUNT);
const MG_ArgConvReverse	DayCountNameRevertor	= MG_ArgConvReverse	("Day Count Basis", DayCountNameArgConv, NB_DAYCOUNT);


MG_ArgConv::ArgConvPair StubRuleNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("SS",K_SHORT_START),
	MG_ArgConv::ArgConvPair("SE",K_SHORT_END),
	MG_ArgConv::ArgConvPair("LS",K_LONG_START),
	MG_ArgConv::ArgConvPair("LE",K_LONG_END),
};
const MG_ArgConv		StubRuleNameConvertor	= MG_ArgConv		("Stub Rule", StubRuleNameArgConv, NB_STUBRULE);
const MG_ArgConvReverse	StubRuleNameRevertor	= MG_ArgConvReverse	("Stub Rule", StubRuleNameArgConv, NB_STUBRULE);


MG_ArgConv::ArgConvPair AdjustmentNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("ADJ",K_ADJUSTED),
	MG_ArgConv::ArgConvPair("UNADJ",K_UNADJUSTED),
};
const MG_ArgConv		AdjustmentNameConvertor	= MG_ArgConv		("Adjustment", AdjustmentNameArgConv, NB_ADJ);
const MG_ArgConvReverse	AdjustmentNameRevertor	= MG_ArgConvReverse	("Adjustment", AdjustmentNameArgConv, NB_ADJ);


MG_ArgConv::ArgConvPair AdjustmentRuleNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("FIXED",K_FIXED_RULE),
	MG_ArgConv::ArgConvPair("FP",K_FOLLOWING_PAY),
	MG_ArgConv::ArgConvPair("F",K_FOLLOWING),
	MG_ArgConv::ArgConvPair("MF",K_MODIFIED_FOLLOWING),
	MG_ArgConv::ArgConvPair("MFN",K_MODIFIED_FOLLOWING_NOREDEMP),
	MG_ArgConv::ArgConvPair("PP",K_PREVIOUS_PAY),
	MG_ArgConv::ArgConvPair("P",K_PREVIOUS),
	MG_ArgConv::ArgConvPair("MP",K_MODIFIED_PREVIOUS),
	MG_ArgConv::ArgConvPair("MPN",K_MODIFIED_PREVIOUS_NOREDEMP),
};
const MG_ArgConv		AdjustmentRuleNameConvertor	= MG_ArgConv		("Adjustment Rule", AdjustmentRuleNameArgConv, NB_ADJRULE);
const MG_ArgConvReverse	AdjustmentRuleNameRevertor	= MG_ArgConvReverse	("Adjustment Rule", AdjustmentRuleNameArgConv, NB_ADJRULE);


MG_ArgConv::ArgConvPair TimingNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("ADV",K_ADVANCE),
	MG_ArgConv::ArgConvPair("ARR",K_ARREARS),
};
const MG_ArgConv		TimingNameConvertor	= MG_ArgConv		("Timing", TimingNameArgConv, NB_TIMING);
const MG_ArgConvReverse	TimingNameRevertor	= MG_ArgConvReverse	("Timing", TimingNameArgConv, NB_TIMING);


MG_ArgConv::ArgConvPair RcvPayNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("R",K_RCV),
	MG_ArgConv::ArgConvPair("P",K_PAY),
};
const MG_ArgConv		RcvPayNameConvertor	= MG_ArgConv		("Receive / Pay", RcvPayNameArgConv, NB_RCVPAY);
const MG_ArgConvReverse	RcvPayNameRevertor	= MG_ArgConvReverse	("Receive / Pay", RcvPayNameArgConv, NB_RCVPAY);


MG_ArgConv::ArgConvPair NXNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("NONE",K_NX_NONE),
	MG_ArgConv::ArgConvPair("START",K_NX_START),
	MG_ArgConv::ArgConvPair("END",K_NX_END),
	MG_ArgConv::ArgConvPair("BOTH",K_NX_BOTH),
};
const MG_ArgConv		NXNameConvertor	= MG_ArgConv		("Notional Exchange", NXNameArgConv, NB_NX);
const MG_ArgConvReverse	NXNameRevertor	= MG_ArgConvReverse	("Notional Exchange", NXNameArgConv, NB_NX);


MG_ArgConv::ArgConvPair InterpolMethodArgConv[] =
{
	MG_ArgConv::ArgConvPair("NONE",NONE_INTERPOL),
	MG_ArgConv::ArgConvPair("CONSTANT",CONSTANT_INTERPOL),
	MG_ArgConv::ArgConvPair("STEPUPLEFT",STEPUPLEFT_INTERPOL),
	MG_ArgConv::ArgConvPair("STEPUPRIGHT",STEPUPRIGHT_INTERPOL),
	MG_ArgConv::ArgConvPair("LINEAR",LINEAR_INTERPOL),
	MG_ArgConv::ArgConvPair("CONTINUOUS",CONTINUOUS_INTERPOL),
	MG_ArgConv::ArgConvPair("POLYNOMIAL",POLYNOM_INTERPOL),
	MG_ArgConv::ArgConvPair("CUBICSPLINE",CUBICSPLINE_INTERPOL),
	MG_ArgConv::ArgConvPair("AKIMA",AKIMA_INTERPOL),
};
const MG_ArgConv		InterpolMethodConvertor	= MG_ArgConv		("Interpolation Methods", InterpolMethodArgConv, NB_INTERPOL_METH);
const MG_ArgConvReverse	InterpolMethodRevertor	= MG_ArgConvReverse	("Interpolation Methods", InterpolMethodArgConv, NB_INTERPOL_METH);


MG_ArgConv::ArgConvPair RandGenArgConv[] =
{
	MG_ArgConv::ArgConvPair("BOROSH13",BOROSH13),
	MG_ArgConv::ArgConvPair("CMRG",CMRG),
	MG_ArgConv::ArgConvPair("COVEYOU",COVEYOU),
	MG_ArgConv::ArgConvPair("FISHMAN18",FISHMAN18),
	MG_ArgConv::ArgConvPair("FISHMAN20",FISHMAN20),
	MG_ArgConv::ArgConvPair("FISHMAN2X",FISHMAN2X),
	MG_ArgConv::ArgConvPair("GFSR4",GFSR4),
	MG_ArgConv::ArgConvPair("KNUTHRAN",KNUTHRAN),
	MG_ArgConv::ArgConvPair("KNUTHRAN2",KNUTHRAN2),
	MG_ArgConv::ArgConvPair("KNUTHRAN2002",KNUTHRAN2002),
	MG_ArgConv::ArgConvPair("LECUYER21",LECUYER21),
	MG_ArgConv::ArgConvPair("MINSTD",MINSTD),
	MG_ArgConv::ArgConvPair("MRG",MRG),
	MG_ArgConv::ArgConvPair("MT19937",MT19937),
	MG_ArgConv::ArgConvPair("MT19937_1999",MT19937_1999),
	MG_ArgConv::ArgConvPair("MT19937_1998",MT19937_1998),
	MG_ArgConv::ArgConvPair("R250",R250),
	MG_ArgConv::ArgConvPair("RAN0",RAN0),
	MG_ArgConv::ArgConvPair("RAN1",RAN1),
	MG_ArgConv::ArgConvPair("RAN2",RAN2),
	MG_ArgConv::ArgConvPair("RAN3",RAN3),
	MG_ArgConv::ArgConvPair("RAND",RAND),
	MG_ArgConv::ArgConvPair("RAND48",RAND48),
	MG_ArgConv::ArgConvPair("RANDOM128_BSD",RANDOM128_BSD),
	MG_ArgConv::ArgConvPair("RANDOM128_GLIBC2",RANDOM128_GLIBC2),
	MG_ArgConv::ArgConvPair("RANDOM128_LIBC5",RANDOM128_LIBC5),
	MG_ArgConv::ArgConvPair("RANDOM256_BSD",RANDOM256_BSD),
	MG_ArgConv::ArgConvPair("RANDOM256_GLIBC2",RANDOM256_GLIBC2),
	MG_ArgConv::ArgConvPair("RANDOM256_LIBC5",RANDOM256_LIBC5),
	MG_ArgConv::ArgConvPair("RANDOM32_BSD",RANDOM32_BSD),
	MG_ArgConv::ArgConvPair("RANDOM32_GLIBC2",RANDOM32_GLIBC2),
	MG_ArgConv::ArgConvPair("RANDOM32_LIBC5",RANDOM32_LIBC5),
	MG_ArgConv::ArgConvPair("RANDOM64_BSD",RANDOM64_BSD),
	MG_ArgConv::ArgConvPair("RANDOM64_GLIBC2",RANDOM64_GLIBC2),
	MG_ArgConv::ArgConvPair("RANDOM64_LIBC5",RANDOM64_LIBC5),
	MG_ArgConv::ArgConvPair("RANDOM8_BSD",RANDOM8_BSD),
	MG_ArgConv::ArgConvPair("RANDOM8_GLIBC2",RANDOM8_GLIBC2),
	MG_ArgConv::ArgConvPair("RANDOM8_LIBC5",RANDOM8_LIBC5),
	MG_ArgConv::ArgConvPair("RANDOM_BSD",RANDOM_BSD),
	MG_ArgConv::ArgConvPair("RANDOM_GLIBC2",RANDOM_GLIBC2),
	MG_ArgConv::ArgConvPair("RANDOM_LIBC5",RANDOM_LIBC5),
	MG_ArgConv::ArgConvPair("RANDU",RANDU),
	MG_ArgConv::ArgConvPair("RANF",RANF),
	MG_ArgConv::ArgConvPair("RANLUX",RANLUX),
	MG_ArgConv::ArgConvPair("RANLUX389",RANLUX389),
	MG_ArgConv::ArgConvPair("RANLXD1",RANLXD1),
	MG_ArgConv::ArgConvPair("RANLXD2",RANLXD2),
	MG_ArgConv::ArgConvPair("RANLXS0",RANLXS0),
	MG_ArgConv::ArgConvPair("RANLXS1",RANLXS1),
	MG_ArgConv::ArgConvPair("RANLXS2",RANLXS2),
	MG_ArgConv::ArgConvPair("RANMAR",RANMAR),
	MG_ArgConv::ArgConvPair("SLATEC",SLATEC),
	MG_ArgConv::ArgConvPair("TAUS",TAUS),
	MG_ArgConv::ArgConvPair("TAUS2",TAUS2),
	MG_ArgConv::ArgConvPair("TAUS113",TAUS113),
	MG_ArgConv::ArgConvPair("TRANSPUTER",TRANSPUTER),
	MG_ArgConv::ArgConvPair("TT800",TT800),
	MG_ArgConv::ArgConvPair("UNI",UNI),
	MG_ArgConv::ArgConvPair("UNI32",UNI32),
	MG_ArgConv::ArgConvPair("VAX",VAX),
	MG_ArgConv::ArgConvPair("WATERMAN14",WATERMAN14),
	MG_ArgConv::ArgConvPair("ZUF",ZUF),
};
const MG_ArgConv		RandGenConvertor	= MG_ArgConv		("Random Generator Types", RandGenArgConv, NB_RAND_TYPE);
const MG_ArgConvReverse	RandGenRevertor		= MG_ArgConvReverse	("Random Generator Types", RandGenArgConv, NB_RAND_TYPE);


MG_ArgConv::ArgConvPair QuasiRandGenArgConv[] =
{
	MG_ArgConv::ArgConvPair("NIEDERREITER2",NIEDERREITER2),
	MG_ArgConv::ArgConvPair("SOBOL",SOBOL),
	MG_ArgConv::ArgConvPair("HALTON",HALTON),
	MG_ArgConv::ArgConvPair("REVERSEHALTON",REVERSEHALTON),
};
const MG_ArgConv		QuasiRandGenConvertor	= MG_ArgConv		("Quasi Random Generator Types", QuasiRandGenArgConv, NB_QUASIRAND_TYPE);
const MG_ArgConvReverse	QuasiRandGenRevertor	= MG_ArgConvReverse	("Quasi Random Generator Types", QuasiRandGenArgConv, NB_QUASIRAND_TYPE);


MG_ArgConv::ArgConvPair ModelParamGenArgConv[] =
{
	MG_ArgConv::ArgConvPair("VOLATILITY",VOLATILITY),
	MG_ArgConv::ArgConvPair("MEANREVERSION",MEANREVERSION),
	MG_ArgConv::ArgConvPair("CORRELATION",CORRELATION),
	MG_ArgConv::ArgConvPair("VOLVOL",VOLVOL),
};
const MG_ArgConv		ModelParamGenConvertor	= MG_ArgConv		("Pricing Model Parameter Types", ModelParamGenArgConv, NB_MODELPARAM_TYPE);
const MG_ArgConvReverse	ModelParamGenRevertor	= MG_ArgConvReverse	("Pricing Model Parameter Types", ModelParamGenArgConv, NB_MODELPARAM_TYPE);


MG_ArgConv::ArgConvPair CallPutNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("C",K_CALL),
	MG_ArgConv::ArgConvPair("P",K_PUT),
};
const MG_ArgConv		CallPutNameConvertor	= MG_ArgConv		("Call / Put", CallPutNameArgConv, NB_CALLPUT);
const MG_ArgConvReverse	CallPutNameRevertor		= MG_ArgConvReverse	("Call / Put", CallPutNameArgConv, NB_CALLPUT);


MG_NAMESPACE_END
