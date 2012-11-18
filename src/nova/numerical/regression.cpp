#include "nova/numerical/regression.h"

#include "gsl/gsl_fit.h"


using namespace std;
using namespace MG;


/* Abstract Base Class */
MG_Regression::MG_Regression(	const MG_Regression& aRight)
							:	MG_XLObject(aRight)
							,	myY			(aRight.myY)
							,	myW			(aRight.myW)
							,	myTheta		(aRight.myTheta)
							,	myCov		(aRight.myCov)
							,	myChiSquared(aRight.myChiSquared)
{}

MG_Regression::MG_Regression(	const size_t& aSize, const MG_Vector& aY, const MG_Vector& aW)
							:	MG_XLObject()
							,	myY			(aY)
							,	myW			(aW)
							,	myTheta		(aSize)
							,	myCov		(aSize, aSize)
							,	myChiSquared(0.)
{}

void MG_Regression::Swap(MG_Regression& aRight)
{
	MG_XLObject::Swap(aRight);
	const_cast<MG_Vector&>(myY).Swap(const_cast<MG_Vector&>(aRight.myY));
	const_cast<MG_Vector&>(myW).Swap(const_cast<MG_Vector&>(aRight.myW));
	myTheta.Swap(aRight.myTheta);
	myCov.Swap(aRight.myCov);
	swap(myChiSquared, aRight.myChiSquared);
}

/* Linear Regression Class */
MG_LinearReg::MG_LinearReg	(	const MG_LinearReg& aRight)
							:	MG_Regression(aRight)
							,	mySize	(aRight.mySize)
							,	myX		(aRight.myX)
{}

void MG_LinearReg::Swap(MG_LinearReg& aRight)
{
	MG_Regression::Swap(aRight);
	swap(const_cast<size_t&>(mySize), const_cast<size_t&>(aRight.mySize));
	const_cast<MG_Vector&>(myX).Swap(const_cast<MG_Vector&>(aRight.myX));
}

MG_LinearReg::MG_LinearReg	(	const MG_Vector& aX
							,	const MG_Vector& aY
							,	const MG_Vector& aW)
							:	MG_Regression(2, aY, aW)
							,	mySize		(aX.Size())
							,	myX			(aX)
{
	myXLName = MG_LINREG_XL_NAME;
}

/*
 * Proceed to linear regression h(X)=C0+C1.X
 * Compute also the squared sum: Sum(square(h(X)-Y))
 */
void MG_LinearReg::ProcessLinearReg()
{
	double vC0, vC1, vCov00, vCov01, vCov11;
	int vRet = gsl_fit_linear	(	myX.GetPtr()->data, 1
								,	myY.GetPtr()->data, 1
								,	mySize
								,	&vC0, &vC1, &vCov00, &vCov01, &vCov11, &myChiSquared);
	
	myTheta[0] = vC0;
	myTheta[1] = vC1;
	myCov.Elt(0, 0) = vCov00;
	myCov.Elt(0, 1) = myCov.Elt(1, 0) = vCov01;
	myCov.Elt(1, 1) = vCov11;

	// the correlation coefficient of the data can be computed using gsl_stats_correlation, it does not depend on the fit
}

/*
 * Proceed to linear regression h(X)=C0+C1.X with each feature X weighted by W
 * Compute also the squared sum: Sum(square(h(X)-Y))
 */
void MG_LinearReg::ProcessWLinearReg()
{
	double vC0, vC1, vCov00, vCov01, vCov11;
	int vRet = gsl_fit_wlinear	(	myX.GetPtr()->data, 1
								,	myW.GetPtr()->data, 1
								,	myY.GetPtr()->data, 1
								,	mySize
								,	&vC0, &vC1, &vCov00, &vCov01, &vCov11, &myChiSquared);
	
	myTheta[0] = vC0;
	myTheta[1] = vC1;
	myCov.Elt(0, 0) = vCov00;
	myCov.Elt(0, 1) = myCov.Elt(1, 0) = vCov01;
	myCov.Elt(1, 1) = vCov11;
}

/*
 * Returns H(X)
 */
double MG_LinearReg::Estimate(const double& aX)
{
	double vY, vStdDev;
	int vRet = gsl_fit_linear_est(aX, myTheta[0], myTheta[1], myCov.Elt(0, 0), myCov.Elt(0, 1), myCov.Elt(1, 1), &vY, &vStdDev);
	return vY;
}


/* Multi Linear Regression Class */
MG_MultiReg::MG_MultiReg(	const MG_MultiReg& aRight)
						:	MG_Regression(aRight)
						,	myFeaturesNb(aRight.myFeaturesNb)
						,	myExplesNb	(aRight.myExplesNb)
						,	myX			(aRight.myX)
{
	myWorkSpace = gsl_multifit_linear_alloc(myExplesNb, myFeaturesNb);
}

MG_MultiReg::~MG_MultiReg()
{
	gsl_multifit_linear_free(myWorkSpace);
	myWorkSpace = NULL;
}

void MG_MultiReg::Swap(MG_MultiReg& aRight)
{
	MG_Regression::Swap(aRight);
	swap(const_cast<size_t&>(myFeaturesNb), const_cast<size_t&>(aRight.myFeaturesNb));
	swap(const_cast<size_t&>(myExplesNb), const_cast<size_t&>(aRight.myExplesNb));
	const_cast<MG_Matrix&>(myX).Swap(const_cast<MG_Matrix&>(aRight.myX));
	swap(myWorkSpace, aRight.myWorkSpace);
}

MG_MultiReg::MG_MultiReg(	const MG_Matrix& aX
						,	const MG_Vector& aY
						,	const MG_Vector& aW)
						:	MG_Regression(aX.Cols(), aY, aW)
						,	myFeaturesNb(aX.Cols())
						,	myExplesNb	(aX.Rows())
						,	myX			(aX)
{
	myXLName = MG_MULREG_XL_NAME;
	myWorkSpace = gsl_multifit_linear_alloc(myExplesNb, myFeaturesNb);
}

/*
 * Proceed to linear regression h(X)=C0+C1.X1+C2.X2+..
 * Compute also the squared sum: Sum(square(h(X)-Y))
 */
void MG_MultiReg::ProcessLinearReg()
{
	int vRet = gsl_multifit_linear	(	myX.GetPtr()
									,	myY.GetPtr()
									,	myTheta.GetPtr()
									,	myCov.GetPtr()
									,	&myChiSquared, myWorkSpace);

	// the coefficient of determination can be computed from the expression R²=1-ChiSqr/TSS,
	// where TSS is total sum of squares of the observations Y, see gsl_stats_tss
}

/*
 * Proceed to linear regression h(X)=C0+C1.X1+C2.X2+.. with each feature X weighted by W
 * Compute also the squared sum: Sum(square(h(X)-Y))
 */
void MG_MultiReg::ProcessWLinearReg()
{
	int vRet = gsl_multifit_wlinear	(	myX.GetPtr()
									,	myW.GetPtr()
									,	myY.GetPtr()
									,	myTheta.GetPtr()
									,	myCov.GetPtr()
									,	&myChiSquared, myWorkSpace);

	// the coefficient of determination can be computed from the expression R²=1-ChiSqr/WTSS,
	// where WTSS is weighted total sum of squares of the observations Y, see gsl_stats_wtss
}

/*
 * Returns H(X)
 */
double MG_MultiReg::Estimate(const MG_Vector& aX)
{
	double vY, vStdDev;
	int vRet = gsl_multifit_linear_est(aX.GetPtr(), myTheta.GetPtr(), myCov.GetPtr(), &vY, &vStdDev);
	return vY;
}

/*
 * Returns residuals
 */
MG_Vector MG_MultiReg::Residuals()
{
	MG_Vector vResiduals;
	gsl_multifit_linear_residuals(myX.GetPtr(), myY.GetPtr(), myTheta.GetPtr(), vResiduals.GetPtr());
	return vResiduals;
}
