#include "mgmktdata/interpolator.h"
#include "mgnova/utils/utils.h"
#include "mgnova/argconvdef.h"


using namespace std;
using namespace MG;
using namespace MG_utils;


/* Interpolator Function Pointer */
MG_Interpolator::MG_InterpolLineFunc
MG_Interpolator::GetInterpolatorFunction(int& aInterpolType)
{
	int vInterpolType = aInterpolType & interpoltypeMask;
	aInterpolType = aInterpolType >> maxBitInterpoltype;

	switch(vInterpolType)
	{
	case (interpoltypeStepUpLeft):
		return &MG_Interpolator::Interpolate_StepUpLeft;

	case (interpoltypeStepUpRight):
		return &MG_Interpolator::Interpolate_StepUpRight;

	case (interpoltypeLinear):
		return &MG_Interpolator::Interpolate_Linear;

	default: return NULL;
	}
}

/* Interpolator Code */
long
MG_Interpolator::CreateInterpolTypes(const std::vector<int>& aInterpolMeths)
{
	long vCode(interpoltypeNone);
	for(int i=int(aInterpolMeths.size()-1); i>=0; --i)
	{
		vCode = vCode << maxBitInterpoltype;
		switch(aInterpolMeths[i])
		{
		case STEPUPLEFT_INTERPOL:
			vCode += interpoltypeStepUpLeft;
			continue;

		case STEPUPRIGHT_INTERPOL:
			vCode += interpoltypeStepUpRight;
			continue;

		case LIN_INTERPOL:
			vCode += interpoltypeLinear;
			continue;

		default:
			if (aInterpolMeths[i] >= maxBitInterpoltype)
				MG_THROW("Maximum interpolation methods cannot exceed 16, please extend your algorithm.");
		}
	}
	return vCode;
}

/* Step Up Left Interpolation */
double
MG_Interpolator::Interpolate_StepUpLeft	(	const MG_Line		& aLine
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
	return aLine(vIsRow*aLineIndex+(1-vIsRow)*(vX), (1-vIsRow)*aLineIndex+vIsRow*(vX));
}

/* Step Up Right Interpolation */
double
MG_Interpolator::Interpolate_StepUpRight(	const MG_Line		& aLine
										,	const size_t		& aLineIndex
										,	const INTERPOL_DIM	& aLineDim
										,	const MG_ABSC		& aAbscisses
										,	const double		& aX)
{
	int vIsRow = aLineDim==ID_ROW ? 1 : 0;
	if (aX <= aAbscisses[1])
		return aLine(vIsRow*aLineIndex, (1-vIsRow)*aLineIndex);
	if (aX >= aAbscisses.back())
		return aLine(vIsRow*aLineIndex+(1-vIsRow)*aLine.columns(), vIsRow*(aLine.columns()-1)+(1-vIsRow)*aLineIndex);

	size_t vX(1), vSize(aAbscisses.size());
	for(; vX<vSize; ++vX)
	{
		if (aX<aAbscisses[vX] && aX>=aAbscisses[vX-1])
			break;
	}
	return aLine(vIsRow*aLineIndex+(1-vIsRow)*(vX-1), (1-vIsRow)*aLineIndex+vIsRow*(vX-1));
}

/* Linear Interpolation */
double
MG_Interpolator::Interpolate_Linear	(	const MG_Line		& aLine
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

/* 2-Dim Interpolator */
double
MG_Interpolator::Interpolate_Dim2	(	const MG_Curve				& aCurve
									,	const MG_ABSC				& aAbscisses
									,	const MG_ORD				& aOrdinates
									,	const double				& aX
									,	const double				& aY
									,	const MG_InterpolLineFunc	& a1stInterpolFunc
									,	const MG_InterpolLineFunc	& a2ndInterpolFunc)
{
	if (aY <= aOrdinates.front())
		return a1stInterpolFunc(aCurve, 0, ID_ROW, aAbscisses, aX);
	if (aY >= aOrdinates.back())
		return a1stInterpolFunc(aCurve, aCurve.rows()-1, ID_ROW, aAbscisses, aX);

	if (aX <= aAbscisses.front())
		return a1stInterpolFunc(aCurve, 0, ID_COL, aOrdinates, aY);
	if (aX >= aAbscisses.back())
		return a1stInterpolFunc(aCurve, aCurve.columns()-1, ID_COL, aOrdinates, aY);

	size_t vY(1), vSize(aOrdinates.size());
	for(; vY<vSize; ++vY)
	{
		if (aY<aOrdinates[vY] && aY>=aOrdinates[vY-1])
			break;
	}
	if (vY == vSize)
		--vY;

	vSize = aAbscisses.size();
	double vFDownY	= a1stInterpolFunc(aCurve, vY-1, ID_ROW, aAbscisses, aX);
	double vFUpY	= a1stInterpolFunc(aCurve, vY, ID_ROW, aAbscisses, aX);

	double vDown(aOrdinates[vY-1]), vUp(aOrdinates[vY]);
	double vPente = (vFUpY-vFDownY) / (vUp-vDown);
	return vPente*(aY-vDown) + vFDownY;
}