#include "mgnumerical/closedformulas.h"
#include "mgnumerical/distributions.h"

#include <math.h>


namespace MG
{
	inline int sensOpt(eOPTION opt)
	{
		switch(opt)
		{
			case _call:
			case _calldigit:
				return 1;

			case _put:
			case _putdigit:
				return -1;

			case _straddle:
				return 1;

			default:
				return 0;
		}
	}

	double VanillaPrice(double Fwd, double K, double T, double DF, double Vol, eOPTION TypeOp)
	{
		double std	= Vol * sqrt(T);

		double dd	= std < EPS || fabs(K) < EPS || Fwd / K < 0. ? Fwd < K ? -INFTY : INFTY : log(Fwd / K) / std + 0.5 * std;
		
		double sens	= sensOpt(TypeOp);

		switch(TypeOp)
		{
			case _call:
			case _put:
				return sens * DF * (Fwd * MG_NormalDist::CdfFunc(sens * dd) 
									- K * MG_NormalDist::CdfFunc(sens * (dd - std)));

			case _straddle:
				return Fwd * (2. * MG_NormalDist::CdfFunc(dd) - 1.) 
								- K * (2. * MG_NormalDist::CdfFunc(dd - std) - 1.);

			case _calldigit:
				return DF*MG_NormalDist::CdfFunc((dd - std));

			case _putdigit:
				return DF*(1. - MG_NormalDist::CdfFunc((dd - std)));

			default:
				return 0.;
		}

	}


}