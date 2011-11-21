#include "mgnumerical/closedformulas.h"
#include "mgnumerical/distributions.h"

#include <math.h>


using namespace std;
using namespace MG;


template<>
double MG_ClosedFormulas::VanillaPrice<MG_CF::CALL>(	const double& aF
													,	const double& aK
													,	const double& aT
													,	const double& aDf
													,	const double& aVol)
{
	double vStd = aVol * sqrt(aT);
	double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

	return aDf*(aF*MG_NormalDist::CdfFunc(vDD) - aK*MG_NormalDist::CdfFunc(vDD-vStd));
}

template<>
double MG_ClosedFormulas::VanillaPrice<MG_CF::PUT>	(	const double& aF
													,	const double& aK
													,	const double& aT
													,	const double& aDf
													,	const double& aVol)
{
	double vStd = aVol * sqrt(aT);
	double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

	return aDf*(aK*MG_NormalDist::CdfFunc(vStd-vDD) - aF*MG_NormalDist::CdfFunc(-vDD));
}


template<>
double MG_ClosedFormulas::VanillaPrice<MG_CF::STRADDLE>(	const double& aF
														,	const double& aK
														,	const double& aT
														,	const double& aDf
														,	const double& aVol)
{
	double vStd = aVol * sqrt(aT);
	double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

	return aF*(2.*MG_NormalDist::CdfFunc(vDD)-1.) - aK*(2.*MG_NormalDist::CdfFunc(vDD-vStd)-1.);
}


template<>
double MG_ClosedFormulas::VanillaPrice<MG_CF::CALL_DIGIT>	(	const double& aF
															,	const double& aK
															,	const double& aT
															,	const double& aDf
															,	const double& aVol)
{
	double vStd = aVol * sqrt(aT);
	double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

	return aDf*MG_NormalDist::CdfFunc((vDD-vStd));
}

template<>
double MG_ClosedFormulas::VanillaPrice<MG_CF::PUT_DIGIT>(	const double& aF
														,	const double& aK
														,	const double& aT
														,	const double& aDf
														,	const double& aVol)
{
	double vStd = aVol * sqrt(aT);
	double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

	return aDf*(1. - MG_NormalDist::CdfFunc((vDD-vStd)));
}
