#include "mgnova/termstructure.h"


using namespace std;
using namespace MG;


/* IR Descriptor class */
MG_TermStructure::MG_TermStructure	(	const MG_TermStructure& aRight)
									:	MG_XLObject(aRight)
									,	myPayDates		(aRight.myPayDates)
									,	myValues		(aRight.myValues)
									,	myInterpolType	(aRight.myInterpolType)
{
	vector<double> vPayDts(myPayDates.size());
	for(size_t i=0; i<vPayDts.size(); ++i)
		vPayDts[i] = myPayDates[i].GetJulianDay();
	MG_Vector vPayDtsVect(vPayDts);

	MG_Vector vValsVect(myValues);
	MG_Matrix vValsMat(vValsVect);

	int vInterpolType(myInterpolType);
	MG_Interpolator::CreateGSLSplineInterp(myInterpolator, vInterpolType, vPayDtsVect, vValsMat);
}

void MG_TermStructure::Swap(MG_TermStructure& aRight)
{
	MG_XLObject::Swap(aRight);

	myPayDates.swap(aRight.myPayDates);
	myValues.swap(aRight.myValues);
	swap(const_cast<int&>(myInterpolType), const_cast<int&>(aRight.myInterpolType));
	swap(myInterpolator, aRight.myInterpolator);
}

MG_TermStructure::MG_TermStructure	(	const vector<MG_Date>	& aPayDates
									,	const vector<double>	& aValues
									,	const int				& aInterpolType)
									:	myPayDates		(aPayDates)
									,	myValues		(aValues)
									,	myInterpolType	(aInterpolType)
{
	myXLName = MG_TERMSTRUCT_XL_NAME;

	vector<double> vPayDts(aPayDates.size());
	for(size_t i=0; i<vPayDts.size(); ++i)
		vPayDts[i] = aPayDates[i].GetJulianDay();
	MG_Vector vPayDtsVect(vPayDts);

	MG_Vector vValsVect(aValues);
	MG_Matrix vValsMat(vValsVect);

	int vInterpolType(myInterpolType);
	MG_Interpolator::CreateGSLSplineInterp(myInterpolator, vInterpolType, vPayDtsVect, vValsMat);
}

MG_TermStructure::~MG_TermStructure()
{
	gsl_spline_free(myInterpolator);
}

double MG_TermStructure::CptValue(const MG_Date& aPayDt) const
{
	double vPayDt = aPayDt.GetJulianDay();
	return gsl_spline_eval(myInterpolator, vPayDt, NULL);
}
