#include "argconvdef.h"


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


MG_NAMESPACE_END
