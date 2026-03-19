#include "nova/numerical/distributions.h"
#include "nova/glob/typedef.h"

#include <cmath>
#include <limits>


using namespace std;
using namespace MG;


/*
 * Inverse normal CDF using Peter Acklam's rational approximation.
 * Maximum absolute error: ~1.15e-9 for p in (0, 1).
 */
static double acklam_inv_normal_cdf(double p)
{
	// Coefficients for rational approximation
	constexpr double a[] = {
		-3.969683028665376e+01,  2.209460984245205e+02,
		-2.759285104469687e+02,  1.383577518672690e+02,
		-3.066479806614716e+01,  2.506628277459239e+00
	};
	constexpr double b[] = {
		-5.447609879822406e+01,  1.615858368580409e+02,
		-1.556989798598866e+02,  6.680131188771972e+01,
		-1.328068155288572e+01
	};
	constexpr double c[] = {
		-7.784894002430293e-03, -3.223964580411365e-01,
		-2.400758277161838e+00, -2.549732539343734e+00,
		 4.374664141464968e+00,  2.938163982698783e+00
	};
	constexpr double d[] = {
		 7.784695709041462e-03,  3.224671290700398e-01,
		 2.445134137142996e+00,  3.754408661907416e+00
	};

	constexpr double p_low  = 0.02425;
	constexpr double p_high = 1.0 - p_low;

	if (p <= 0.0) return -K_INFTY;
	if (p >= 1.0) return  K_INFTY;

	double q, r, x;
	if (p < p_low) {
		q = std::sqrt(-2.0 * std::log(p));
		x = (((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
		    ((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1.0);
	} else if (p <= p_high) {
		q = p - 0.5;
		r = q * q;
		x = (((((a[0]*r+a[1])*r+a[2])*r+a[3])*r+a[4])*r+a[5])*q /
		    (((((b[0]*r+b[1])*r+b[2])*r+b[3])*r+b[4])*r+1.0);
	} else {
		q = std::sqrt(-2.0 * std::log(1.0 - p));
		x = -(((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
		     ((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1.0);
	}
	return x;
}

/*
 * Draw a standard normal variate using the Marsaglia polar method.
 * This avoids calls to gsl_randist and works with any MG_Random generator.
 */
static double polar_normal_draw(MG_Random& rng)
{
	double u, v, s;
	do {
		u = 2.0 * rng.DrawUniform() - 1.0;
		v = 2.0 * rng.DrawUniform() - 1.0;
		s = u * u + v * v;
	} while (s >= 1.0 || s == 0.0);
	return u * std::sqrt(-2.0 * std::log(s) / s);
}


/* Random Distribution Abstract Class */
MG_RandDist::MG_RandDist(const MG_RandDist& aRight) : MG_XLObject(aRight)
{
	myRandGen = aRight.myRandGen;
}

void MG_RandDist::Swap(MG_RandDist& aRight)
{
	MG_XLObject::Swap(aRight);
	myRandGen.swap(aRight.myRandGen);
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
{}

void MG_NormalDist::Swap(MG_NormalDist& aRight)
{
	MG_RandDist::Swap(aRight);
	swap(myMeth, aRight.myMeth);
	swap(mySigma, aRight.mySigma);
}

MG_NormalDist::MG_NormalDist(	const MG_RandomPtr	& aRandGen
							,	const NOR_METH		& aMeth
							,	const double		& aSigma)
							:	MG_RandDist(aRandGen)
							,	myMeth	(aMeth)
							,	mySigma	(aSigma)
{
	myXLName = MG_NORDIST_XL_NAME;
}

double MG_NormalDist::Density(const double& aX)
{
	double z = aX / mySigma;
	return K_ONEOVERROOTTWOPI * std::exp(-0.5 * z * z) / mySigma;
}

double MG_NormalDist::DensityTail(const double& aX, const double& aLimit)
{
	// Density of the upper tail N(0,sigma) conditioned on X >= aLimit
	if (aX < aLimit) return 0.0;
	double tail_prob = 0.5 * std::erfc(aLimit / (mySigma * K_ROOTTWO));
	return Density(aX) / tail_prob;
}

double MG_NormalDist::Cdf(const double& aX)
{
	return 0.5 * std::erfc(-aX / (mySigma * K_ROOTTWO));
}

double MG_NormalDist::InvCdf(const double& aP)
{
	return mySigma * acklam_inv_normal_cdf(aP);
}

double MG_NormalDist::Draw()
{
	return mySigma * polar_normal_draw(*myRandGen);
}

double MG_NormalDist::DrawTail(const double& aLimit)
{
	// Use inverse CDF method for efficiency: sample uniformly on [Cdf(aLimit), 1)
	// then apply InvCdf to get a sample from the upper tail.
	double p_min = Cdf(aLimit);
	// Fall back to rejection sampling when double precision cannot represent (1 - p_min)
	if (p_min >= 1.0) {
		double z;
		do { z = Draw(); } while (z < aLimit);
		return z;
	}
	double u = p_min + (1.0 - p_min) * myRandGen->DrawUniform();
	return InvCdf(u);
}

double MG_NormalDist::DensityFunc(const double& aX)
{
	return K_ONEOVERROOTTWOPI * std::exp(-0.5 * aX * aX);
}

double MG_NormalDist::CdfFunc(const double& aX)
{
	return 0.5 * std::erfc(-aX / K_ROOTTWO);
}

double MG_NormalDist::InvCdfFunc(const double& aP)
{
	return acklam_inv_normal_cdf(aP);
}


/* Uniform Distribution Class */
MG_UniformDist::MG_UniformDist	(	const MG_UniformDist& aRight)
								:	MG_RandDist(aRight)
								,	myA(aRight.myA)
								,	myB(aRight.myB)
{}

void MG_UniformDist::Swap(MG_UniformDist& aRight)
{
	MG_RandDist::Swap(aRight);
	swap(myA, aRight.myA);
	swap(myB, aRight.myB);
}

MG_UniformDist::MG_UniformDist(	const MG_RandomPtr	& aRandGen
							,	const double& aA
							,	const double& aB)
							:	MG_RandDist(aRandGen)
							,	myA(aA)
							,	myB(aB)
{
	myXLName = MG_UDIST_XL_NAME;
}

double MG_UniformDist::Density(const double& aX)
{
	if (aX < myA || aX > myB) return 0.0;
	return 1.0 / (myB - myA);
}

double MG_UniformDist::Cdf(const double& aX)
{
	if (aX <= myA) return 0.0;
	if (aX >= myB) return 1.0;
	return (aX - myA) / (myB - myA);
}

double MG_UniformDist::InvCdf(const double& aP)
{
	return myA + aP * (myB - myA);
}

double MG_UniformDist::Draw()
{
	return myA + (myB - myA) * myRandGen->DrawUniform();
}


/* Lognormal Distribution Class */
MG_LogNormalDist::MG_LogNormalDist	(	const MG_LogNormalDist& aRight)
								:	MG_RandDist(aRight)
								,	myZeta	(aRight.myZeta)
								,	mySigma	(aRight.mySigma)
{}

void MG_LogNormalDist::Swap(MG_LogNormalDist& aRight)
{
	MG_RandDist::Swap(aRight);
	swap(myZeta, aRight.myZeta);
	swap(mySigma, aRight.mySigma);
}

MG_LogNormalDist::MG_LogNormalDist	(	const MG_RandomPtr	& aRandGen
									,	const double		& aZeta
									,	const double		& aSigma)
									:	MG_RandDist(aRandGen)
									,	myZeta	(aZeta)
									,	mySigma	(aSigma)
{
	myXLName = MG_LOGNDIST_XL_NAME;
}

double MG_LogNormalDist::Density(const double& aX)
{
	if (aX <= 0.0) return 0.0;
	double z = (std::log(aX) - myZeta) / mySigma;
	return K_ONEOVERROOTTWOPI * std::exp(-0.5 * z * z) / (aX * mySigma);
}

double MG_LogNormalDist::Cdf(const double& aX)
{
	if (aX <= 0.0) return 0.0;
	double z = (std::log(aX) - myZeta) / mySigma;
	return 0.5 * std::erfc(-z / K_ROOTTWO);
}

double MG_LogNormalDist::InvCdf(const double& aP)
{
	return std::exp(myZeta + mySigma * acklam_inv_normal_cdf(aP));
}

double MG_LogNormalDist::Draw()
{
	// Draw a standard normal then transform to lognormal
	double z = polar_normal_draw(*myRandGen);
	return std::exp(myZeta + mySigma * z);
}

