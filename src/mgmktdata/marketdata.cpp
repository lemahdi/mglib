#include "mgmktdata/marketdata.h"

#include <math.h>


using namespace std;
using namespace MG;


#define MG_ZERO_TYPE	"ZERO"
#define MG_IRVOL_TYPE	"IRVOL"
#define MG_EQVOL_TYPE	"EQVOL"
#define MG_DIV_TYPE		"DIV"


/* Base Market Data class */
MG_MarketData::MG_MarketData(	const MG_MarketData& aRight)
							:	MG_XLObject(aRight)
							,	myAsOf			(aRight.myAsOf)
							,	myType			(aRight.myType)
							,	myCurrency		(aRight.myCurrency)
							,	myUnderIndex	(aRight.myUnderIndex)
{}

void MG_MarketData::Swap(MG_MarketData &aRight)
{
	MG_XLObject::Swap(aRight);
	myAsOf.Swap(aRight.myAsOf);
	myType.swap(aRight.myType);
	myCurrency.swap(aRight.myCurrency);
	myUnderIndex.swap(aRight.myUnderIndex);
}

MG_MarketData::MG_MarketData(	const MG_Date	& aAsOf
							,	const string	& aType
							,	const string	& aCcy
							,	const string	& aUnderIndex)
							:	MG_XLObject()
							,	myAsOf			(aAsOf)
							,	myType			(aType)
							,	myCurrency		(aCcy)
							,	myUnderIndex	(aUnderIndex)
{}

MG_MarketData::~MG_MarketData()
{}


/* Empty Market Data */
MG_EmptyMD::MG_EmptyMD	(	const string& aType
						,	const string& aCcy
						,	const string& aUnderIndex)
						:	MG_MarketData(MG_Date(), aType, aCcy, aUnderIndex)
{}

double MG_EmptyMD::ComputeValue(const double& , const double& , const double& ) const
{
	return 0.;
}


/* Zero Curve - Discount Factors class */
MG_ZeroCurve::MG_ZeroCurve	(	const MG_ZeroCurve& aRight)
							:	MG_MarketData(aRight)
							,	myInterpolator(aRight.myInterpolator)
{}

void MG_ZeroCurve::Swap(MG_ZeroCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	myInterpolator.Swap(aRight.myInterpolator);
}

MG_ZeroCurve::MG_ZeroCurve	(	const MG_Date		& aAsOf
							,	const vector<double>& aMaturities
							,	const vector<double>& aCurve
							,	const string		& aCcy
							,	const string		& aUnderIndex
							,	const int			& aInterpolType)
							:	MG_MarketData(aAsOf, MG_ZERO_TYPE, aCcy, aUnderIndex)
							,	myInterpolator(aMaturities, aCurve, aInterpolType)
{
	myXLName = MG_ZERO_XL_NAME;
}

MG_ZeroCurve::~MG_ZeroCurve()
{}

double MG_ZeroCurve::ComputeValue(const double& aMaturity, const double& , const double& ) const
{
	return DiscountFactor(aMaturity);
}

double MG_ZeroCurve::DiscountFactor(const double& aMaturity) const
{
	double vRate = myInterpolator.Eval(aMaturity) / 100.;
	return exp(-vRate*aMaturity);
}

double MG_ZeroCurve::Libor	(	const double& aMatSt
							,	const double& aMatEd
							,	const double& aDelta)
{
	double vDfS = DiscountFactor(aMatSt);
	double vDfE = DiscountFactor(aMatEd);
	double vFwd = 1./aDelta * (vDfS/vDfE - 1.);

	return vFwd;
}


/* Flat Volatility class */
MG_FlatVolCurve::MG_FlatVolCurve(	const MG_FlatVolCurve& aRight)
								:	MG_VolatilityCurve(aRight)
								,	myValue(aRight.myValue)
{}

void MG_FlatVolCurve::Swap(MG_FlatVolCurve &aRight)
{
	MG_VolatilityCurve::Swap(aRight);
	std::swap(myValue, aRight.myValue);
}

MG_FlatVolCurve::MG_FlatVolCurve(	const MG_Date	& aAsOf
								,	const double	& aValue
								,	const string	& aType
								,	const string	& aCcy
								,	const string	& aUnderIndex)
								:	MG_VolatilityCurve(aAsOf, aType, aCcy, aUnderIndex)
								,	myValue(aValue)
{}

MG_FlatVolCurve::~MG_FlatVolCurve()
{}

double MG_FlatVolCurve::ComputeValue(const double&, const double&, const double&) const
{
	return myValue;
}


/* Volatility class */
MG_VolatilityCurve::MG_VolatilityCurve	(	const MG_VolatilityCurve& aRight)
										:	MG_MarketData(aRight)
										,	myInterpolator(aRight.myInterpolator)
{}

void MG_VolatilityCurve::Swap(MG_VolatilityCurve &aRight)
{
	MG_MarketData::Swap(aRight);
	myInterpolator.Swap(aRight.myInterpolator);
}

MG_VolatilityCurve::MG_VolatilityCurve	(	const MG_Date	& aAsOf
										,	const string	& aType
										,	const string	& aCcy
										,	const string	& aUnderIndex)
										:	MG_MarketData(aAsOf, aType, aCcy, aUnderIndex)
{}

MG_VolatilityCurve::~MG_VolatilityCurve()
{}


/* IR Volatility class */
MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_IRVolatilityCurve& aRight)
											:	MG_VolatilityCurve(aRight)
											,	myTransCurve(aRight.myTransCurve)
{}

void MG_IRVolatilityCurve::Swap(MG_IRVolatilityCurve &aRight)
{
	MG_VolatilityCurve::Swap(aRight);
	myTransCurve.Swap(aRight.myTransCurve);
}

MG_IRVolatilityCurve::MG_IRVolatilityCurve	(	const MG_Date		& aAsOf
											,	const vector<double>& aMaturities
											,	const vector<double>& aTenors
											,	const MG_Matrix		& aCurve
											,	const string		& aCcy
											,	const string		& aUnderIndex
											,	const int			& aInterpolTypes)
											:	MG_VolatilityCurve	(aAsOf, MG_IRVOL_TYPE, aCcy, aUnderIndex)
											,	myTransCurve		(aCurve.Cols(), aCurve.Rows())
{
	myXLName = MG_IRVOL_XL_NAME;
	aCurve.Transpose(myTransCurve);

	myInterpolator = MG_2DInterpolator(aTenors, aMaturities, myTransCurve, aInterpolTypes);
}

double MG_IRVolatilityCurve::ComputeValue(const double& aTenor, const double& aMaturity, const double& ) const
{
	return myInterpolator.Eval(aTenor, aMaturity);
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

void MG_DividendsTable::Swap(MG_DividendsTable& aRight)
{
	MG_MarketData::Swap(aRight);
	myExDivDates.swap(aRight.myExDivDates);
	myPaymentDates.swap(aRight.myPaymentDates);
	myCurve.swap(aRight.myCurve);
	myZeroCurve.Swap(aRight.myZeroCurve);
}

MG_DividendsTable::MG_DividendsTable(	const MG_Date			& aAsOf
									,	const vector<MG_Date>	& aExDivDates
									,	const vector<MG_Date>	& aPaymentDates
									,	const vector<double>	& aCurve
									,	const string			& aCcy
									,	const string			& aUnderIndex
									,	const MG_ZeroCurvePtr	& aZeroCurve)
									:	MG_MarketData(aAsOf, MG_DIV_TYPE, aCcy, aUnderIndex)
									,	myExDivDates	(aExDivDates)
									,	myPaymentDates	(aPaymentDates)
									,	myExDivJulInterp(aExDivDates)
									,	myCurve			(aCurve)
									,	myZeroCurve		(aZeroCurve)
{
	myXLName = MG_DIVS_XL_NAME;
}

double MG_DividendsTable::ComputeValue(const double& aT1, const double& aT2, const double& ) const
{
	if (aT1>myExDivJulInterp.Back() || aT2<myExDivJulInterp.Front())
		return 0.;

	size_t vIMin = gsl_interp_bsearch(myExDivJulInterp.GetPtr()->data, aT1, 0, myExDivJulInterp.Size()-1);
	size_t vIMax = gsl_interp_bsearch(myExDivJulInterp.GetPtr()->data, aT2, 0, myExDivJulInterp.Size()-1);

	double vRes(0.);
	for(size_t i=vIMin; i<=vIMax; ++i)
		vRes += myCurve[i] * myZeroCurve->ComputeValue((myPaymentDates[i].GetJulianDay()-myAsOf.GetJulianDay())/365.25);

	return vRes;
}

/* Equity Volatility class */
MG_EQVolatilityCurve::MG_EQVolatilityCurve	(	const MG_EQVolatilityCurve& aRight)
											:	MG_VolatilityCurve(aRight)
											,	myTransCurve(aRight.myTransCurve)
{}

void MG_EQVolatilityCurve::Swap(MG_EQVolatilityCurve &aRight)
{
	MG_VolatilityCurve::Swap(aRight);
	myTransCurve.Swap(aRight.myTransCurve);
}

MG_EQVolatilityCurve::MG_EQVolatilityCurve	(	const MG_Date		& aAsOf
											,	const vector<double>& aStrikes
											,	const vector<double>& aMaturities
											,	const MG_Matrix		& aCurve
											,	const string		& aCcy
											,	const string		& aUnderIndex
											,	const int			& aInterpolTypes)
											:	MG_VolatilityCurve(aAsOf, MG_EQVOL_TYPE, aCcy, aUnderIndex)
{
	myXLName = MG_EQ_XL_NAME;
	aCurve.Transpose(myTransCurve);

	myInterpolator = MG_2DInterpolator(aStrikes, aMaturities, myTransCurve, aInterpolTypes);
}

double MG_EQVolatilityCurve::ComputeValue(const double& aStrike, const double& aMaturity, const double& ) const
{
	return myInterpolator.Eval(aStrike, aMaturity);
}
