#include "mgmktdata/marketdata.h"


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
{}

double MG_ZeroCurve::ComputeValue(const double& aMaturity, const double& , const double& )
{
	return myInterpolator->Interpolate(myCurve, myMaturities, aMaturity);
}


/* Voaltility class */
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

double MG_IRVolatilityCurve::ComputeValue(const double& aMaturity, const double& aTenor, const double& )
{
	return myInterpolator->Interpolate(myCurve, myMaturities, myTenors, aMaturity, aTenor);
}

