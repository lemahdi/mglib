#include "mgmktdata/interpolator.h"
#include "mgnova/utils/utils.h"
#include "mgnova/argconvdef.h"


using namespace std;
using namespace MG;
using namespace MG_utils;


/* Interpolator Code */
int
MG_Interpolator::CreateInterpolTypes(const std::vector<int>& aInterpolMeths)
{
	int vCode(interpoltypeNone);
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

		case POLYNOM_INTERPOL:
			vCode += interpoltypePolynomial;
			continue;

		case CUBICSPLINE_INTERPOL:
			vCode += interpoltypeCubicSpline;
			continue;

		case CONSTANT_INTERPOL:
			vCode += interpoltypeConstant;
			continue;

		default:
			if (aInterpolMeths[i] >= maxBitInterpoltype)
				MG_THROW("Maximum interpolation methods cannot exceed 16, please extend your algorithm.");
		}
	}
	return vCode;
}

/* Get GSL Interpol Type */
const gsl_interp_type*
MG_Interpolator::GetGSLInterpolType(int& aInterpolCode)
{
	int vInterpolCode = aInterpolCode & interpoltypeMask;
	aInterpolCode = aInterpolCode >> maxBitInterpoltype;

	switch(vInterpolCode)
	{
	case (interpoltypeLinear):
		return gsl_interp_linear;
/*
	case (interpoltypePolynomial):
		return gsl_interp_polynomial;

	case (interpoltypeCubicSpline):
		return gsl_interp_cubic_spline;
*/

	default: return NULL;
	}
}

bool MG_Interpolator::CreateGSLInterp(gsl_interp*& aInterp, int& aInterpCode)
{
	const gsl_interp_type* vGSLInterpType = MG_Interpolator::GetGSLInterpolType(aInterpCode);
	if (!vGSLInterpType)
		return false;

	aInterp = gsl_interp_alloc(vGSLInterpType, 2);
	return true;
}

bool MG_Interpolator::CreateGSLSplineInterp	(	gsl_spline*		& aInterp
											,	int				& aInterpCode
											,	const MG_ABSC	& aAbsc
											,	const MG_Line	& aCurve)
{
	const gsl_interp_type* vGSLInterpType = MG_Interpolator::GetGSLInterpolType(aInterpCode);
	if (!vGSLInterpType)
		return false;

	size_t vSize(aAbsc.Size());
	aInterp = gsl_spline_alloc(vGSLInterpType, vSize);
	gsl_spline_init(aInterp, aAbsc.GetPtr()->data, aCurve.GetPtr()->data, vSize);

	return true;
}

bool MG_Interpolator::CreateGSLSplineInterp	(	vector<gsl_spline*>	& aInterps
											,	int					& aInterpCode
											,	const MG_ABSC		& aAbsc
											,	const MG_Curve		& aCurve)
{
	const gsl_interp_type* vGSLInterpType = MG_Interpolator::GetGSLInterpolType(aInterpCode);
	if (!vGSLInterpType)
		return false;

	size_t vSize1(aCurve.Rows());
	size_t vSize2(aAbsc.Size());
	aInterps.resize(vSize1);
	for(size_t i=0; i<vSize1; ++i)
	{
		aInterps[i] = gsl_spline_alloc(vGSLInterpType, vSize2);
		gsl_vector_const_view vView = gsl_matrix_const_row(aCurve.GetPtr(), i);
		gsl_spline_init(aInterps[i], aAbsc.GetPtr()->data, vView.vector.data, vSize2);
	}
	
	return true;
}

/* Interpolator Function Pointer */
MG_Interpolator::MG_StepWiseFunc
MG_Interpolator::GetStepWiseFunc(	int			& aInterpolCode
								,	gsl_interp*	& aInterp1
								,	gsl_spline*	& aInterp2
								,	const size_t& aSize)
{
	int vInterpolCode = aInterpolCode & interpoltypeMask;
	aInterpolCode = aInterpolCode >> maxBitInterpoltype;

	switch(vInterpolCode)
	{
	case (interpoltypeStepUpLeft):
		return &MG_Interpolator::Interpolate_StepUpLeft;

	case (interpoltypeStepUpRight):
		return &MG_Interpolator::Interpolate_StepUpRight;

	case (interpoltypeConstant):
		gsl_interp_free(aInterp1);
		return &MG_Interpolator::Interpolate_Constant;

	default: return NULL;
	}
}


/* Step Up Left Interpolation */
double
MG_Interpolator::Interpolate_StepUpLeft	(	const MG_Line		& aLine
										,	const size_t		& aLineIndex
										,	const MG_ABSC		& aAbscisses
										,	const double		& aX)
{
	size_t vIdxX = gsl_interp_bsearch(aAbscisses.GetPtr()->data, aX, 0, aAbscisses.Size()-1);
	return gsl_matrix_get(aLine.GetPtr(), aLineIndex, vIdxX);
}

/* Step Up Right Interpolation */
double
MG_Interpolator::Interpolate_StepUpRight(	const MG_Line		& aLine
										,	const size_t		& aLineIndex
										,	const MG_ABSC		& aAbscisses
										,	const double		& aX)
{
	size_t vIdxX = gsl_interp_bsearch(aAbscisses.GetPtr()->data, aX, 0, aAbscisses.GetPtr()->size);
	return gsl_matrix_get(aLine.GetPtr(), aLineIndex, vIdxX-1);
}

/* Constant Interpolation */
double
MG_Interpolator::Interpolate_Constant	(	const MG_Line		& aLine
										,	const size_t		& aLineIndex
										,	const MG_ABSC		& //aAbscisses
										,	const double		& )//aX
{
	return aLine(0, aLineIndex);
}

/* 2-Dim Interpolator */
double
MG_Interpolator::Interpolate_Surface(	const vector<gsl_spline*>	& a1stInterp
									,	gsl_interp					* a2ndInterp
									,	const MG_ORD				& a2ndAxe
									,	const double				& aX
									,	const double				& aY)
{
	size_t vIdxY = gsl_interp_bsearch(a2ndAxe.GetPtr()->data, aX, 0, a2ndAxe.GetPtr()->size);
	size_t vSizeY(a1stInterp[vIdxY]->size);
	double vFUpY(0.);
	if (aY <= a1stInterp[vIdxY]->x[0])
		vFUpY = a1stInterp[vIdxY]->y[0];
	else if (aY >= a1stInterp[vIdxY]->x[vSizeY-1])
		vFUpY = a1stInterp[vIdxY]->y[vSizeY-1];
	else
		vFUpY = gsl_spline_eval(a1stInterp[vIdxY], aY, NULL);
	if (vIdxY==0 || aX>=a2ndAxe.Back() || a2ndAxe[vIdxY]==aX)
		return vFUpY;

	double vFDownY	= gsl_spline_eval(a1stInterp[vIdxY-1], aY, NULL);
	double vXs[2], vYs[2];
	vXs[0] = a2ndAxe[vIdxY-1];
	vXs[1] = a2ndAxe[vIdxY];
	vYs[0] = vFDownY;
	vYs[1] = vFUpY;

	gsl_interp_init(a2ndInterp, vXs, vYs, 2);
	return gsl_interp_eval(a2ndInterp, vXs, vYs, aX, NULL);
}
