#include "mgmktdata/marketdata.h"

#include <math.h>


using namespace std;
using namespace MG;


/* Base Market Data class */
MG_MarketData::MG_MarketData(	const MG_MarketData& aRight)
							:	MG_XLObject(aRight)
							,	myAsOf			(aRight.myAsOf)
							,	myInterpolator	(aRight.myInterpolator)
{}

void MG_MarketData::Swap(MG_MarketData &aRight)
{
	myAsOf.Swap(aRight.myAsOf);
}

MG_MarketData::MG_MarketData(	const MG_Date			& aAsOf
							,	const INTERPOL_METHOD	& aInterpolMethod)
							:	MG_XLObject()
							,	myAsOf(aAsOf)
{
	if (aInterpolMethod != NONE_INTERPOL)
		myInterpolator = MG_Interpolator::Builder(aInterpolMethod);
}


/* Zero Curve - Discount Factors class */
MG_ZeroCurve::MG_ZeroCurve	(	const MG_ZeroCurve& aRight)
							:	MG_MarketData(aRight)
							,	myMaturities(aRight.myMaturities)
							,	myCurve		(aRight.myCurve)
{}

void MG_ZeroCurve::Swap(MG_ZeroCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	swap(myMaturities, aRight.myMaturities);
	swap(myCurve, aRight.myCurve);
}

MG_ZeroCurve::MG_ZeroCurve	(	const MG_Date			& aAsOf
							,	const MG_ABSC			& aMaturities
							,	const MG_Line			& aCurve
							,	const INTERPOL_METHOD	& aInterpolMethod)
							:	MG_MarketData(aAsOf, aInterpolMethod)
							,	myMaturities(aMaturities)
							,	myCurve		(aCurve)
{
	myXLName = MG_ZERO_XL_NAME;
}

double MG_ZeroCurve::ComputeValue(const double& aMaturity, const double& , const double& )
{
	double vRate = myInterpolator->Interpolate(myCurve, 0, MG_Interpolator::ID_COL, myMaturities, aMaturity);
	return exp(-vRate*aMaturity);
}


/* Volatility class */
MG_VolatilityCurve::MG_VolatilityCurve	(	const MG_Date			& aAsOf
										,	const INTERPOL_METHOD	& aInterpolMethod)
										:	MG_MarketData(aAsOf, aInterpolMethod)
{}

/* IR Volatility class */
MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_IRVolatilityCurve& aRight)
											:	MG_VolatilityCurve(aRight)
											,	myMaturities(aRight.myMaturities)
											,	myTenors	(aRight.myTenors)
											,	myCurve		(aRight.myCurve)
{}

void MG_IRVolatilityCurve::Swap(MG_IRVolatilityCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	swap(myMaturities, aRight.myMaturities);
	swap(myTenors, aRight.myTenors);
	myCurve.Swap(aRight.myCurve);
}

MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_Date			& aAsOf
											,	const MG_ABSC			& aMaturities
											,	const MG_COOR			& aTenors
											,	const MG_Matrix			& aCurve
											,	const INTERPOL_METHOD	& aInterpolMethod)
											:	MG_VolatilityCurve(aAsOf, aInterpolMethod)
											,	myMaturities(aMaturities)
											,	myTenors	(aTenors)
											,	myCurve		(aCurve)
{
	myXLName = MG_IRVOL_XL_NAME;
}

double MG_IRVolatilityCurve::ComputeValue(const double& aTenor, const double& aMaturity, const double& )
{
	return myInterpolator->Interpolate(myCurve, myTenors, myMaturities, aTenor, aMaturity);
}


/* Dividends Table class */
MG_DividendsTable::MG_DividendsTable	(	const MG_DividendsTable& aRight)
										:	MG_MarketData(aRight)
										,	myExDivDates	(aRight.myExDivDates)
										,	myPaymentDates	(aRight.myPaymentDates)
										,	myCurve			(aRight.myCurve)
{
	myZeroCurve = MG_ZeroCurvePtr(new MG_ZeroCurve(*aRight.myZeroCurve));
}

void MG_DividendsTable::Swap(MG_DividendsTable &aRight)
{
	MG_MarketData::Swap(aRight);
	swap(myExDivDates, aRight.myExDivDates);
	swap(myPaymentDates, aRight.myPaymentDates);
	swap(myCurve, aRight.myCurve);
	myZeroCurve.Swap(aRight.myZeroCurve);
}

MG_DividendsTable::MG_DividendsTable(	const MG_Date			& aAsOf
									,	const MG_ABSC			& aExDivDates
									,	const MG_ABSC			& aPaymentDates
									,	const MG_Line			& aCurve
									,	const MG_ZeroCurvePtr	& aZeroCurve)
									:	MG_MarketData(aAsOf)
									,	myExDivDates(aExDivDates)
									,	myPaymentDates(aPaymentDates)
									,	myCurve		(aCurve)
									,	myZeroCurve	(aZeroCurve)
{
	myXLName = MG_DIVS_XL_NAME;
}

double MG_DividendsTable::ComputeValue(const double& aT1, const double& aT2, const double& )
{
	if (aT1>myExDivDates.back().GetJulianDay() || aT2<myExDivDates.front().GetJulianDay())
		return 0.;

	double vRes(0.), vDivJul(0.);
	for(size_t i=0; i<myExDivDates.size(); ++i)
	{
		vDivJul = myExDivDates[i].GetJulianDay();
		if (vDivJul>=aT1 && vDivJul<=aT2)
			vRes += myCurve[i] * myZeroCurve->ComputeValue((myPaymentDates[i].GetJulianDay()-myAsOf.GetJulianDay())/365.25);
		if (vDivJul > aT2)
			break;
	}

	return vRes;
}
