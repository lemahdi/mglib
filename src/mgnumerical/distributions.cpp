#include "mgnumerical/distributions.h"


using namespace std;
using namespace MG;


/* Random Distribution Abstract Class */
MG_RandDist::MG_RandDist(const MG_RandDist& aRight) : MG_XLObject(aRight)
{
	myRandGen = aRight.myRandGen;
}

void MG_RandDist::Swap(MG_RandDist& aRight)
{
	myRandGen.Swap(aRight.myRandGen);
}

MG_RandDist::MG_RandDist(	const MG_RandomPtr& aRandGen)
						:	MG_XLObject()
						,	myRandGen(aRandGen)
{}


/* Normal Distribution Class */
MG_NormalDist::MG_NormalDist(	const MG_NormalDist& aRight)
							:	MG_RandDist(aRight)
							,	myMeth	(aRight.myMeth)
							,	mySigma	(aRight.mySigma)
							,	myFunc	(aRight.myFunc)
{}

void MG_NormalDist::Swap(MG_NormalDist& aRight)
{
	swap(myMeth, aRight.myMeth);
	swap(mySigma, aRight.mySigma);
	swap(myFunc, aRight.myFunc);
}

MG_NormalDist::MG_NormalDist(	const MG_RandomPtr	& aRandGen
							,	const NOR_METH		& aMeth
							,	const double		& aSigma)
							:	MG_RandDist(aRandGen)
							,	myMeth	(aMeth)
							,	mySigma	(aSigma)
{
	myXLName = MG_NORDIST_XL_NAME;

	switch(myMeth)
	{
	case ZIGGURAT:
		myFunc = &gsl_ran_gaussian_ziggurat; break;

	case RATIO:
		myFunc = &gsl_ran_gaussian_ratio_method; break;

	default:
		myFunc = &gsl_ran_gaussian; break;
	}
}

MG_NormalDist::~MG_NormalDist()
{
	myFunc = NULL;
}

double MG_NormalDist::Density(const double& aX)
{
	return gsl_ran_gaussian_pdf(aX, mySigma);
}

double MG_NormalDist::DensityTail(const double& aX, const double& aLimit)
{
	return gsl_ran_gaussian_tail_pdf(aX, aLimit, mySigma);
}

double MG_NormalDist::Cdf(const double& aX)
{
	return gsl_cdf_gaussian_P(aX, mySigma);
}

double MG_NormalDist::InvCdf(const double& aP)
{
	return gsl_cdf_gaussian_Pinv(aP, mySigma);
}

double MG_NormalDist::Draw()
{
	return myFunc(myRandGen->GetGenerator(), mySigma);
}

double MG_NormalDist::DrawTail(const double& aLimit)
{
	return gsl_ran_gaussian_tail(myRandGen->GetGenerator(), aLimit, mySigma);
}

double MG_NormalDist::DensityFunc(const double& aX)
{
	return gsl_ran_ugaussian_pdf(aX);
}

double MG_NormalDist::CdfFunc(const double& aX)
{
	return gsl_cdf_ugaussian_P(aX);
}

double MG_NormalDist::InvCdfFunc(const double& aP)
{
	return gsl_cdf_ugaussian_Pinv(aP);
}
