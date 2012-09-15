#include "mgnova/market/closedformulas.h"
#include "mgnova/numerical/distributions.h"

//#include "gsl/gsl_nan.h"
//#include "gsl/gsl_sys.h"

#include <math.h>


using namespace std;


MG_NAMESPACE_BEGIN

namespace MG_CF
{
	/* Option Pricing Formulas */
	double
	CallPrice	(	const double& aF
				,	const double& aK
				,	const double& aT
				,	const double& aDf
				,	const double& aVol)
	{
		double vStd = aVol * sqrt(aT);
		double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

		return aDf*(aF*MG_NormalDist::CdfFunc(vDD) - aK*MG_NormalDist::CdfFunc(vDD-vStd));
	}

	double
	PutPrice(	const double& aF
			,	const double& aK
			,	const double& aT
			,	const double& aDf
			,	const double& aVol)
	{
		double vStd = aVol * sqrt(aT);
		double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

		return aDf*(aK*MG_NormalDist::CdfFunc(vStd-vDD) - aF*MG_NormalDist::CdfFunc(-vDD));
	}

	double
	DigitalCallPrice(	const double& aF
					,	const double& aK
					,	const double& aT
					,	const double& aDf
					,	const double& aVol)
	{
		double vStd = aVol * sqrt(aT);
		double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

		return aF*(2.*MG_NormalDist::CdfFunc(vDD)-1.) - aK*(2.*MG_NormalDist::CdfFunc(vDD-vStd)-1.);
	}

	double
	DigitalPutPrice	(	const double& aF
					,	const double& aK
					,	const double& aT
					,	const double& aDf
					,	const double& aVol)
	{
		double vStd = aVol * sqrt(aT);
		double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

		return aDf*MG_NormalDist::CdfFunc((vDD-vStd));
	}

	double
	StraddlePrice	(	const double& aF
					,	const double& aK
					,	const double& aT
					,	const double& aDf
					,	const double& aVol)
	{
		double vStd = aVol * sqrt(aT);
		double vDD = vStd<K_EPS || fabs(aK)<K_EPS || aF/aK<0. ? aF<aK ? -K_INFTY : K_INFTY : log(aF/aK)/vStd+0.5*vStd;

		return aDf*(1. - MG_NormalDist::CdfFunc((vDD-vStd)));
	}
}


MG_NAMESPACE_END

