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
		return MG_SLinearInterpolator::Instance();
	default:
		return MG_InterpolatorPtr(NULL);
	}
}


/* Linear Interpolator class */
template <>
MG_LinearInterpolatorPtr MG_SLinearInterpolator::myInstance = MG_LinearInterpolatorPtr(NULL);

double MG_LinearInterpolator::Interpolate	(	const MG_Line		& aLine
											,	const size_t		& aLineIndex
											,	const INTERPOL_DIM	& aLineDim
											,	const MG_ABSC		& aAbscisses
											,	const double		& aX)
{
	int vIsRow = aLineDim==ID_ROW ? 1 : 0;
	if (aX <= aAbscisses.front())
		return aLine(vIsRow*aLineIndex, (1-vIsRow)*aLineIndex);
	if (aX >= aAbscisses.back())
		return aLine(vIsRow*aLineIndex+(1-vIsRow)*aLine.columns(), vIsRow*(aLine.columns()-1)+(1-vIsRow)*aLineIndex);

	size_t vX(1), vSize(aAbscisses.size());
	for(; vX<vSize; ++vX)
	{
		if (aX<aAbscisses[vX] && aX>=aAbscisses[vX-1])
			break;
	}
	if (vX == vSize)
		--vX;

	double vDown(aAbscisses[vX-1]), vUp(aAbscisses[vX]),
		   vFDown(aLine(vIsRow*aLineIndex+(1-vIsRow)*(vX-1), (1-vIsRow)*aLineIndex+vIsRow*(vX-1)));
	double vPente = (aLine(vIsRow*aLineIndex+(1-vIsRow)*vX, vIsRow*vX+(1-vIsRow)*aLineIndex)-vFDown) / (vUp-vDown);
	return vPente*(aX-vDown) + vFDown;
}

double MG_LinearInterpolator::Interpolate	(	const MG_Curve	& aCurve
											,	const MG_ABSC	& aAbscisses
											,	const MG_ORD	& aOrdinates
											,	const double	& aX
											,	const double	& aY)
{
	if (aY <= aOrdinates.front())
		return Interpolate(aCurve, 0, ID_ROW, aAbscisses, aX);
	if (aY >= aOrdinates.back())
		return Interpolate(aCurve, aCurve.rows()-1, ID_ROW, aAbscisses, aX);

	if (aX <= aAbscisses.front())
		return Interpolate(aCurve, 0, ID_COL, aOrdinates, aY);
	if (aX >= aAbscisses.back())
		return Interpolate(aCurve, aCurve.columns()-1, ID_COL, aOrdinates, aY);

	size_t vY(1), vSize(aOrdinates.size());
	for(; vY<vSize; ++vY)
	{
		if (aY<aOrdinates[vY] && aY>=aOrdinates[vY-1])
			break;
	}
	if (vY == vSize)
		--vY;

	vSize = aAbscisses.size();
	double vFDownY	= Interpolate(aCurve, vY-1, ID_ROW, aAbscisses, aX);
	double vFUpY	= Interpolate(aCurve, vY, ID_ROW, aAbscisses, aX);

	double vDown(aOrdinates[vY-1]), vUp(aOrdinates[vY]);
	double vPente = (vFUpY-vFDownY) / (vUp-vDown);
	return vPente*(aY-vDown) + vFDownY;
}