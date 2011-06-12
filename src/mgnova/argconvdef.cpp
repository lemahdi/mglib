#include "mgnova/argconvdef.h"


using namespace std;


MG_NAMESPACE_BEGIN


MG_ArgConv::ArgConvPair MonthsArgConv[] =
{
	MG_ArgConv::ArgConvPair("JAN",JAN),
	MG_ArgConv::ArgConvPair("FEB",FEB),
	MG_ArgConv::ArgConvPair("MAR",MAR),
	MG_ArgConv::ArgConvPair("APR",APR),
	MG_ArgConv::ArgConvPair("MAI",MAY),
	MG_ArgConv::ArgConvPair("JUN",JUN),
	MG_ArgConv::ArgConvPair("JUL",JUL),
	MG_ArgConv::ArgConvPair("AUG",AUG),
	MG_ArgConv::ArgConvPair("SEP",SEP),
	MG_ArgConv::ArgConvPair("OCT",OCT),
	MG_ArgConv::ArgConvPair("NOV",NOV),
	MG_ArgConv::ArgConvPair("DEC",DEC),
};
const MG_ArgConv		MonthsConvertor	= MG_ArgConv		("Months Three Letters", MonthsArgConv, NB_MONTHS_3L);
const MG_ArgConvReverse	MonthsRevertor	= MG_ArgConvReverse	("Months Three Letters", MonthsArgConv, NB_MONTHS_3L);


MG_ArgConv::ArgConvPair CurrenciesNameArgConv[] =
{
	MG_ArgConv::ArgConvPair("USD",USD),
	MG_ArgConv::ArgConvPair("EUR",EUR),
	MG_ArgConv::ArgConvPair("JPY",JPY),
	MG_ArgConv::ArgConvPair("GBP",GBP),
};
const MG_ArgConv		CurrenciesNameConvertor	= MG_ArgConv		("Currencies Name Three Letters", CurrenciesNameArgConv, NB_CURRENCIES);
const MG_ArgConvReverse	CurrenciesNameRevertor	= MG_ArgConvReverse	("Currencies Name Three Letters", CurrenciesNameArgConv, NB_CURRENCIES);


MG_ArgConv::ArgConvPair InterpolMethodArgConv[] =
{
	MG_ArgConv::ArgConvPair("NONE",NONE_INTERPOL),
	MG_ArgConv::ArgConvPair("STEPUPLEFT",STEPUPLEFT_INTERPOL),
	MG_ArgConv::ArgConvPair("STEPUPRIGHT",STEPUPRIGHT_INTERPOL),
	MG_ArgConv::ArgConvPair("LINEAR",LIN_INTERPOL),
};
const MG_ArgConv		InterpolMethodConvertor	= MG_ArgConv		("Interpolation Methods", InterpolMethodArgConv, NB_INTERPOL_METH);
const MG_ArgConvReverse	InterpolMethodRevertor	= MG_ArgConvReverse	("Interpolation Methods", InterpolMethodArgConv, NB_INTERPOL_METH);

MG_NAMESPACE_END
