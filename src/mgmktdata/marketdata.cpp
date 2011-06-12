#include "mgmktdata/marketdata.h"

#include <math.h>


using namespace std;
using namespace MG;


/* Base Market Data class */
MG_MarketData::MG_MarketData(	const MG_MarketData& aRight)
							:	MG_XLObject(aRight)
							,	myAsOf			(aRight.myAsOf)
{}

void MG_MarketData::Swap(MG_MarketData &aRight)
{
	myAsOf.Swap(aRight.myAsOf);
}

MG_MarketData::MG_MarketData(	const MG_Date& aAsOf)
							:	MG_XLObject()
							,	myAsOf			(aAsOf)
{}


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
	swap(myInterpolFunc, aRight.myInterpolFunc);
}

MG_ZeroCurve::MG_ZeroCurve	(	const MG_Date	& aAsOf
							,	const MG_ABSC	& aMaturities
							,	const MG_Line	& aCurve
							,	const int		& aInterpolTypes)
							:	MG_MarketData(aAsOf)
							,	myMaturities(aMaturities)
							,	myCurve		(aCurve)
{
	myXLName		= MG_ZERO_XL_NAME;

	int vInterpolTypes(aInterpolTypes);
	myInterpolFunc	= MG_Interpolator::GetInterpolatorFunction(vInterpolTypes);
}

double MG_ZeroCurve::ComputeValue(const double& aMaturity, const double& , const double& )
{
	double vRate = (*myInterpolFunc)(myCurve, 0, ID_COL, myMaturities, aMaturity);
	return exp(-vRate*aMaturity);
}


/* Volatility class */
MG_VolatilityCurve::MG_VolatilityCurve	(	const MG_Date	& aAsOf)
										:	MG_MarketData(aAsOf)
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
	swap(myMaturityInterpolFunc, aRight.myMaturityInterpolFunc);
	swap(myTenorInterpolFunc, aRight.myTenorInterpolFunc);
}

MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_Date	& aAsOf
											,	const MG_ABSC	& aMaturities
											,	const MG_ORD	& aTenors
											,	const MG_Matrix	& aCurve
											,	const int		& aInterpolTypes)
											:	MG_VolatilityCurve(aAsOf)
											,	myMaturities(aMaturities)
											,	myTenors	(aTenors)
											,	myCurve		(aCurve)
{
	myXLName = MG_IRVOL_XL_NAME;

	int vInterpolTypes(aInterpolTypes);
	myMaturityInterpolFunc	= MG_Interpolator::GetInterpolatorFunction(vInterpolTypes);
	myTenorInterpolFunc		= MG_Interpolator::GetInterpolatorFunction(vInterpolTypes);
}

double MG_IRVolatilityCurve::ComputeValue(const double& aTenor, const double& aMaturity, const double& )
{
	return MG_Interpolator::Interpolate_Dim2(myCurve, myTenors, myMaturities, aTenor, aMaturity, myMaturityInterpolFunc, myTenorInterpolFunc);
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
									,	myExDivDates	(aExDivDates)
									,	myPaymentDates	(aPaymentDates)
									,	myCurve			(aCurve)
									,	myZeroCurve		(aZeroCurve)
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

/* Equity Volatility class */
MG_EQVolatilityCurve::MG_EQVolatilityCurve	(	const MG_EQVolatilityCurve& aRight)
											:	MG_VolatilityCurve(aRight)
											,	myStrikes	(aRight.myStrikes)
											,	myMaturities(aRight.myMaturities)
											,	myCurve		(aRight.myCurve)
{}

void MG_EQVolatilityCurve::Swap(MG_EQVolatilityCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	swap(myStrikes, aRight.myStrikes);
	swap(myMaturities, aRight.myMaturities);
	myCurve.Swap(aRight.myCurve);
}

MG_EQVolatilityCurve::MG_EQVolatilityCurve	(	const MG_Date	& aAsOf
											,	const MG_ABSC	& aStrikes
											,	const MG_ORD	& aMaturities
											,	const MG_Matrix	& aCurve
											,	const int		& aInterpolTypes)
											:	MG_VolatilityCurve(aAsOf)
											,	myStrikes	(aStrikes)
											,	myMaturities(aMaturities)
											,	myCurve		(aCurve)
{
	myXLName = MG_IRVOL_XL_NAME;

	int vInterpolTypes(aInterpolTypes);
	myStrikeInterpolFunc	= MG_Interpolator::GetInterpolatorFunction(vInterpolTypes);
	myMaturityInterpolFunc	= MG_Interpolator::GetInterpolatorFunction(vInterpolTypes);
}

double MG_EQVolatilityCurve::ComputeValue(const double& aStrike, const double& aMaturity, const double& )
{
	return MG_Interpolator::Interpolate_Dim2(myCurve, myStrikes, myMaturities, aStrike, aMaturity, myStrikeInterpolFunc, myMaturityInterpolFunc);
}
