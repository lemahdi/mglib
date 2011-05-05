#include "mgmktdata/interpolator.h"
#include "mgnova/utils/utils.h"


using namespace std;
using namespace MG;
using namespace MG_utils;


/* Interpolator Builder */
MG_InterpolatorPtr MG_Interpolator::Builder(const INTERPOL_METHOD& aInterpolMethod)
{
	switch(aInterpolMethod)
	{
	case LIN_INTERPOL:
		return MG_InterpolatorPtr(new MG_LinearInterpolator);
	default:
		return MG_InterpolatorPtr(NULL);
	}
}


/* Linear Interpolator class */
double MG_LinearInterpolator::Interpolate	(	const MG_Line	& aLine
											,	const MG_ABSC	& aAbscisses
											,	const double	& aX)
{
	if (aX <= aAbscisses.front())
		return aLine.front();
	if (aX >= aAbscisses.back())
		return aLine.back();

	size_t vX(1), vSize(aAbscisses.size());
	for(; vX<vSize; ++vX)
	{
		if (aX<aAbscisses[vX] && aX>aAbscisses[vX-1])
			break;
	}

	double vDown(aAbscisses[vX-1]), vUp(aAbscisses[vX]), vFDown(aLine[vX-1]);
	double vPente = (aLine[vX]-vFDown) / (vUp-vDown);
	return vPente*(aX-vDown) + vFDown;
}

double MG_LinearInterpolator::Interpolate	(	const MG_Curve	& aCurve
											,	const MG_ABSC	& aAbscisses
											,	const MG_ORD	& aOrdinates
											,	const double	& aX
											,	const double	& aY)
{
	if (aY <= aOrdinates.front())
		return Interpolate(DoublePtrAsVector(aCurve[0], aCurve.columns()), aAbscisses, aX);
	if (aY >= aOrdinates.back())
		return Interpolate(DoublePtrAsVector(aCurve[aCurve.rows()-1], aCurve.columns()), aAbscisses, aX);

	size_t vY(1), vSize(aOrdinates.size());
	for(; vY<vSize; ++vY)
	{
		if (aY<aOrdinates[vY] && aY>aOrdinates[vY-1])
			break;
	}

	vSize = aAbscisses.size();
	double vDown(aOrdinates[vY-1]), vUp(aOrdinates[vY]);
	double vFDownY	= Interpolate(DoublePtrAsVector(aCurve[vY-1], vSize), aAbscisses, aX);
	double vFUpY	= Interpolate(DoublePtrAsVector(aCurve[vY], vSize), aAbscisses, aX);
	double vPente = (vFUpY-vFDownY) / (vUp-vDown);
	return vPente*(aY-vDown) + vFDownY;
}