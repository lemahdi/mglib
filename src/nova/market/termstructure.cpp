#include "nova/market/termstructure.h"


using namespace std;
using namespace MG;


/* Term Structure class */
MG_TermStructure::MG_TermStructure	(	const MG_TermStructure& aRight)
									:	MG_XLObject(aRight)
									,	myOriginPayDates(aRight.myOriginPayDates)
									,	myOriginValues	(aRight.myOriginValues)
									,	myInterpolator	(aRight.myInterpolator)
{}

void MG_TermStructure::Swap(MG_TermStructure& aRight)
{
	MG_XLObject::Swap(aRight);
	const_cast<vector<MG_Date>&>(myOriginPayDates).swap(const_cast<vector<MG_Date>&>(aRight.myOriginPayDates));
	swap(const_cast<vector<double>&>(myOriginValues), const_cast<vector<double>&>(aRight.myOriginValues));
	myInterpolator.Swap(aRight.myInterpolator);
}

MG_TermStructure::MG_TermStructure	(	const double& aVal)
									:	myOriginPayDates(vector<MG_Date>(1))
									,	myOriginValues	(vector<double>(1, aVal))
									,	myInterpolator	(vector<MG_Date>(1), vector<double>(1, aVal), CONSTANT_INTERPOL)
{
	myXLName = MG_TERMSTRUCT_XL_NAME;
}

MG_TermStructure::MG_TermStructure	(	const vector<MG_Date>	& aPayDates
									,	const vector<double>	& aValues
									,	const int				& aInterpolType)
									:	myOriginPayDates(aPayDates)
									,	myOriginValues	(aValues)
									,	myInterpolator	(aPayDates, aValues, aInterpolType)
{
	myXLName = MG_TERMSTRUCT_XL_NAME;
}

MG_TermStructure::~MG_TermStructure()
{}

double MG_TermStructure::CptValue(const MG_Date& aPayDt) const
{
	return myInterpolator.Eval(aPayDt.GetJulianDay());
}
