#include "mgmktdata/marketdata.h"

#include <math.h>


using namespace std;
using namespace MG;


/* Base Market Data class */
MG_MarketData::MG_MarketData(	const MG_MarketData& aRight)
							:	MG_XLObject(aRight)
							,	myAsOf			(aRight.myAsOf)
							,	myInterpolTypes	(aRight.myInterpolTypes)
{}

void MG_MarketData::Swap(MG_MarketData &aRight)
{
	myAsOf.Swap(aRight.myAsOf);
	swap(const_cast<int&>(myInterpolTypes), const_cast<int&>(aRight.myInterpolTypes));
}

MG_MarketData::MG_MarketData(	const MG_Date	& aAsOf
							,	const int		& aInterpolTypes)
							:	MG_XLObject()
							,	myAsOf			(aAsOf)
							,	myInterpolTypes	(aInterpolTypes)
{}

MG_MarketData::~MG_MarketData()
{}


/* Zero Curve - Discount Factors class */
MG_ZeroCurve::MG_ZeroCurve	(	const MG_ZeroCurve& aRight)
							:	MG_MarketData(aRight)
							,	myMaturities	(aRight.myMaturities)
							,	myCurve			(aRight.myCurve)
{
	int vInterpolType(myInterpolTypes);
	MG_Interpolator::CreateGSLSplineInterp(myInterpolator, vInterpolType, myMaturities, myCurve);
}

void MG_ZeroCurve::Swap(MG_ZeroCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	myMaturities.Swap(aRight.myMaturities);
	myCurve.Swap(aRight.myCurve);
	swap(myInterpolator, aRight.myInterpolator);
}

MG_ZeroCurve::MG_ZeroCurve	(	const MG_Date	& aAsOf
							,	const MG_ABSC	& aMaturities
							,	const MG_Line	& aCurve
							,	const int		& aInterpolTypes)
							:	MG_MarketData	(aAsOf, aInterpolTypes)
							,	myMaturities	(aMaturities)
							,	myCurve			(aCurve)
{
	myXLName = MG_ZERO_XL_NAME;

	int vInterpolType(myInterpolTypes);
	MG_Interpolator::CreateGSLSplineInterp(myInterpolator, vInterpolType, aMaturities, aCurve);
}

MG_ZeroCurve::~MG_ZeroCurve()
{
	gsl_spline_free(myInterpolator);
}

double MG_ZeroCurve::ComputeValue(const double& aMaturity, const double& , const double& )
{
	double vRate = gsl_spline_eval(myInterpolator, aMaturity, NULL);
	return exp(-vRate*aMaturity);
}


/* Volatility class */
MG_VolatilityCurve::MG_VolatilityCurve	(	const MG_VolatilityCurve& aRight)
										:	MG_MarketData(aRight)
										,	my1stInterps(aRight.my1stInterps.size(), NULL)
										,	my2ndInterp	(NULL)
{}

void MG_VolatilityCurve::Swap(MG_VolatilityCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	my1stInterps.swap(aRight.my1stInterps);
	swap(my2ndInterp, aRight.my2ndInterp);
}

MG_VolatilityCurve::MG_VolatilityCurve	(	const MG_Date	& aAsOf
										,	const int		& aInterpolTypes)
										:	MG_MarketData(aAsOf, aInterpolTypes)
										,	my2ndInterp(NULL)
{}

MG_VolatilityCurve::~MG_VolatilityCurve()
{
	for(size_t i=0; i<my1stInterps.size(); ++i)
		gsl_spline_free(my1stInterps[i]);
	gsl_interp_free(my2ndInterp);
}


/* IR Volatility class */
MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_IRVolatilityCurve& aRight)
											:	MG_VolatilityCurve(aRight)
											,	myMaturities(aRight.myMaturities)
											,	myTenors	(aRight.myTenors)
											,	myCurve		(aRight.myCurve)
											,	myTransCurve(aRight.myTransCurve)
{
	int vInterpolTypes(myInterpolTypes);
	MG_Interpolator::CreateGSLSplineInterp(my1stInterps, vInterpolTypes, myMaturities, myTransCurve);
	MG_Interpolator::CreateGSLInterp(my2ndInterp, vInterpolTypes);
}

void MG_IRVolatilityCurve::Swap(MG_IRVolatilityCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	myMaturities.Swap(aRight.myMaturities);
	myTenors.Swap(aRight.myTenors);
	myCurve.Swap(aRight.myCurve);
	myTransCurve.Swap(aRight.myTransCurve);
}

MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_Date	& aAsOf
											,	const MG_ABSC	& aMaturities
											,	const MG_ORD	& aTenors
											,	const MG_Matrix	& aCurve
											,	const int		& aInterpolTypes)
											:	MG_VolatilityCurve	(aAsOf, aInterpolTypes)
											,	myMaturities		(aMaturities)
											,	myTenors			(aTenors)
											,	myCurve				(aCurve)
											,	myTransCurve		(aCurve.Cols(), aCurve.Rows())
{
	myXLName = MG_IRVOL_XL_NAME;
	myCurve.Transpose(myTransCurve);

	int vInterpolTypes(myInterpolTypes);
	MG_Interpolator::CreateGSLSplineInterp(my1stInterps, vInterpolTypes, myMaturities, myTransCurve);
	MG_Interpolator::CreateGSLInterp(my2ndInterp, vInterpolTypes);
}

double MG_IRVolatilityCurve::ComputeValue(const double& aTenor, const double& aMaturity, const double& )
{
	return MG_Interpolator::Interpolate_Surface(my1stInterps, my2ndInterp, myTenors, aTenor, aMaturity);
}


/* Dividends Table class */
MG_DividendsTable::MG_DividendsTable	(	const MG_DividendsTable& aRight)
										:	MG_MarketData(aRight)
										,	myExDivDates	(aRight.myExDivDates)
										,	myPaymentDates	(aRight.myPaymentDates)
										,	myCurve			(aRight.myCurve)
{
	myZeroCurve = MG_ZeroCurvePtr(dynamic_cast<MG_ZeroCurve*>(aRight.myZeroCurve->Clone()));
}

void MG_DividendsTable::Swap(MG_DividendsTable &aRight)
{
	MG_MarketData::Swap(aRight);
	myExDivDates.Swap(aRight.myExDivDates);
	myPaymentDates.Swap(aRight.myPaymentDates);
	myCurve.Swap(aRight.myCurve);
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
	if (aT1>myExDivDates.Back() || aT2<myExDivDates.Front())
		return 0.;

	size_t vIMin = gsl_interp_bsearch(myExDivDates.GetPtr()->data, aT1, 0, myExDivDates.Size()-1);
	size_t vIMax = gsl_interp_bsearch(myExDivDates.GetPtr()->data, aT2, 0, myExDivDates.Size()-1);

	double vRes(0.);
	for(size_t i=vIMin; i<=vIMax; ++i)
		vRes += myCurve[i] * myZeroCurve->ComputeValue((myPaymentDates[i]-myAsOf.GetJulianDay())/365.25);

	return vRes;
}

/* Equity Volatility class */
MG_EQVolatilityCurve::MG_EQVolatilityCurve	(	const MG_EQVolatilityCurve& aRight)
											:	MG_VolatilityCurve(aRight)
											,	myStrikes	(aRight.myStrikes)
											,	myMaturities(aRight.myMaturities)
											,	myCurve		(aRight.myCurve)
											,	myTransCurve(aRight.myTransCurve)
{
	int vInterpolTypes(myInterpolTypes);
	MG_Interpolator::CreateGSLSplineInterp(my1stInterps, vInterpolTypes, myStrikes, myTransCurve);
	MG_Interpolator::CreateGSLInterp(my2ndInterp, vInterpolTypes);
}

void MG_EQVolatilityCurve::Swap(MG_EQVolatilityCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	myStrikes.Swap(aRight.myStrikes);
	myMaturities.Swap(aRight.myMaturities);
	myCurve.Swap(aRight.myCurve);
	myTransCurve.Swap(aRight.myCurve);
}

MG_EQVolatilityCurve::MG_EQVolatilityCurve	(	const MG_Date	& aAsOf
											,	const MG_ABSC	& aStrikes
											,	const MG_ORD	& aMaturities
											,	const MG_Matrix	& aCurve
											,	const int		& aInterpolTypes)
											:	MG_VolatilityCurve(aAsOf, aInterpolTypes)
											,	myStrikes	(aStrikes)
											,	myMaturities(aMaturities)
											,	myCurve		(aCurve)
{
	myXLName = MG_IRVOL_XL_NAME;
	myCurve.Transpose(myTransCurve);

	int vInterpolTypes(myInterpolTypes);
	MG_Interpolator::CreateGSLSplineInterp(my1stInterps, vInterpolTypes, myStrikes, myTransCurve);
	MG_Interpolator::CreateGSLInterp(my2ndInterp, vInterpolTypes);
}

double MG_EQVolatilityCurve::ComputeValue(const double& aStrike, const double& aMaturity, const double& )
{
	return MG_Interpolator::Interpolate_Surface(my1stInterps, my2ndInterp, myMaturities, aMaturity, aStrike);
}
