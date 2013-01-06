#include "nova/numerical/interpolator.h"
#include "nova/utils/utils.h"
#include "nova/glob/argconvdef.h"
#include "nova/glob/exception.h"


using namespace std;
using namespace MG_utils;


MG_NAMESPACE_BEGIN


/* Step Up Left Interpolation */
double Interpolate_StepUpLeft	(	const MG_Matrix	& aLine
								,	const size_t	& aLineIndex
								,	const MG_Vector	& aAbscisses
								,	const double	& aX)
{
	if (aX < aAbscisses.Front())
		return aLine(0,0);
	if (aX > aAbscisses.Back())
		return aLine.Last();
	size_t vIdxX = gsl_interp_bsearch(aAbscisses.GetPtr()->data, aX, 0, aAbscisses.Size());
	return gsl_matrix_get(aLine.GetPtr(), aLineIndex, vIdxX);
}

/* Step Up Right Interpolation */
double Interpolate_StepUpRight	(	const MG_Matrix	& aLine
								,	const size_t	& aLineIndex
								,	const MG_Vector	& aAbscisses
								,	const double	& aX)
{
	if (aX < aAbscisses.Front())
		return aLine(0,0);
	if (aX > aAbscisses.Back())
		return aLine.Last();
	size_t vIdxX = gsl_interp_bsearch(aAbscisses.GetPtr()->data, aX, 0, aAbscisses.Size());
	return gsl_matrix_get(aLine.GetPtr(), aLineIndex, vIdxX+1<aAbscisses.Size()?vIdxX+1:vIdxX);
}

/* Constant Interpolation */
double Interpolate_Constant	(	const MG_Matrix	& aLine
							,	const size_t	& aLineIndex
							,	const MG_Vector	& //aAbscisses
							,	const double	& )//aX
{
	return aLine(aLineIndex, 0);
}

/* 2-Dim Interpolator */
double Interpolate_Surface	(	const vector<gsl_spline*>	& a1stInterp
							,	gsl_interp					* a2ndInterp
							,	const MG_Vector				& a2ndAxe
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


MG_NAMESPACE_END


using namespace MG;


/* Interpolator Code */
int
MG_Interpolator::CreateInterpolCode(const std::vector<int>& aInterpolMeths)
{
	int vCode(interpoltypeNone);
	for(int i=int(aInterpolMeths.size()-1); i>=0; --i)
	{
		vCode = vCode << maxBitInterpoltype;
		switch(aInterpolMeths[i])
		{
		case CONSTANT_INTERPOL:
			vCode += interpoltypeConstant;
			continue;

		case STEPUPLEFT_INTERPOL:
			vCode += interpoltypeStepUpLeft;
			continue;

		case STEPUPRIGHT_INTERPOL:
			vCode += interpoltypeStepUpRight;
			continue;

		case LINEAR_INTERPOL:
			vCode += interpoltypeLinear;
			continue;

		case CONTINUOUS_INTERPOL:
			vCode += interpoltypeContinuous;
			continue;

		case POLYNOM_INTERPOL:
			vCode += interpoltypePolynomial;
			continue;

		case CUBICSPLINE_INTERPOL:
			vCode += interpoltypeCubicSpline;
			continue;

		case AKIMA_INTERPOL:
			vCode += interpoltypeAkima;
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

	case (interpoltypePolynomial):
		return gsl_interp_polynomial;

	case (interpoltypeCubicSpline):
		return gsl_interp_cspline;

	case (interpoltypeAkima):
		return gsl_interp_akima;


	default: return NULL;
	}
}

bool
MG_Interpolator::LoadGSLInterp(gsl_interp*& aInterp, int& aInterpCode)
{
	const gsl_interp_type* vGSLInterpType = MG_Interpolator::GetGSLInterpolType(aInterpCode);
	if (!vGSLInterpType)
		return false;

	aInterp = gsl_interp_alloc(vGSLInterpType, 2);
	return true;
}

bool
MG_Interpolator::LoadGSLSplineInterp(	gsl_spline*		& aInterp
									,	int				& aInterpCode
									,	const MG_Vector	& aAbsc
									,	const MG_Matrix	& aOrd)
{
	const gsl_interp_type* vGSLInterpType = MG_Interpolator::GetGSLInterpolType(aInterpCode);
	if (!vGSLInterpType)
		return false;

	size_t vSize(aAbsc.Size());
	aInterp = gsl_spline_alloc(vGSLInterpType, vSize);
	gsl_spline_init(aInterp, aAbsc.GetPtr()->data, aOrd.GetPtr()->data, vSize);

	return true;
}

bool
MG_Interpolator::LoadGSLSplineInterp(	vector<gsl_spline*>	& aInterps
									,	int					& aInterpCode
									,	const MG_Vector		& aAbsc
									,	const MG_Matrix		& aOrd)
{
	const gsl_interp_type* vGSLInterpType = MG_Interpolator::GetGSLInterpolType(aInterpCode);
	if (!vGSLInterpType)
		return false;

	size_t vSize1(aOrd.Rows());
	size_t vSize2(aAbsc.Size());
	aInterps.resize(vSize1);
	for(size_t i=0; i<vSize1; ++i)
	{
		aInterps[i] = gsl_spline_alloc(vGSLInterpType, vSize2);
		gsl_vector_const_view vView = gsl_matrix_const_row(aOrd.GetPtr(), i);
		gsl_spline_init(aInterps[i], aAbsc.GetPtr()->data, vView.vector.data, vSize2);
	}
	
	return true;
}

/* Stepwise Function Pointer */
MG_Interpolator::MG_StepWiseFunc
MG_Interpolator::GetStepWiseFunc(int& aInterpolCode)
{
	int vInterpolCode = aInterpolCode & interpoltypeMask;
	aInterpolCode = aInterpolCode >> maxBitInterpoltype;

	switch(vInterpolCode)
	{
	case (interpoltypeStepUpLeft):
		return &Interpolate_StepUpLeft;

	case (interpoltypeStepUpRight):
		return &Interpolate_StepUpRight;

	case (interpoltypeConstant):
		return &Interpolate_Constant;

	default: return NULL;
	}
}

/* Interpolator Class */
MG_Interpolator::MG_Interpolator(	const MG_Interpolator& aRight)
								:	myOrd			(aRight.myOrd)
								,	myInterpolTypes	(aRight.myInterpolTypes)
								,	myStepWiseFunc	(aRight.myStepWiseFunc)
{}

void MG_Interpolator::Swap(MG_Interpolator& aRight)
{
	myOrd.Swap(aRight.myOrd);
	swap(const_cast<int&>(myInterpolTypes), const_cast<int&>(aRight.myInterpolTypes));
	swap(myStepWiseFunc, aRight.myStepWiseFunc);
}

MG_Interpolator::~MG_Interpolator()
{}

MG_Interpolator::MG_Interpolator(void)	:	myInterpolTypes(interpoltypeNone)
										,	myStepWiseFunc	(NULL)
{}

MG_Interpolator::MG_Interpolator(	const MG_Matrix	& aOrd
								,	const int		& aInterpolType)
								:	myOrd			(aOrd)
								,	myStepWiseFunc	(NULL)
								,	myInterpolTypes	(aInterpolType)
{}

MG_Interpolator::MG_Interpolator(	const MG_Vector	& aOrd
								,	const int		& aInterpolType)
								:	myOrd			(aOrd)
								,	myStepWiseFunc	(NULL)
								,	myInterpolTypes	(aInterpolType)
{}

MG_Interpolator::MG_Interpolator(	const vector<double>& aOrd
								,	const int			& aInterpolType)
								:	myOrd			(aOrd)
								,	myStepWiseFunc	(NULL)
								,	myInterpolTypes	(aInterpolType)
{}

/* 1D Interpolator Class */
MG_1DInterpolator::MG_1DInterpolator(	const MG_1DInterpolator& aRight)
									:	MG_Interpolator(aRight)
									,	myAbsc	(aRight.myAbsc)
									,	myInterp(NULL)
{
	if (myStepWiseFunc)
		return;

	int vInterpolTypes(myInterpolTypes);
	MG_Interpolator::LoadGSLSplineInterp(myInterp, vInterpolTypes, myAbsc, myOrd);
}

void MG_1DInterpolator::Swap(MG_1DInterpolator& aRight)
{
	MG_Interpolator::Swap(aRight);
	myAbsc.Swap(aRight.myAbsc);
	swap(myInterp, aRight.myInterp);
}

MG_1DInterpolator::~MG_1DInterpolator()
{
	gsl_spline_free(myInterp);
}

MG_1DInterpolator::MG_1DInterpolator(void) : MG_Interpolator(), myInterp(NULL)
{}

MG_1DInterpolator::MG_1DInterpolator(	const MG_Vector	& aAbsc
									,	const MG_Matrix	& aOrd
									,	const int		& aInterpolType)
									:	MG_Interpolator(aOrd, aInterpolType)
									,	myAbsc	(aAbsc)
									,	myInterp(NULL)
{
	int vInterpolType(myInterpolTypes);
	bool vExist = MG_Interpolator::LoadGSLSplineInterp(myInterp, vInterpolType, myAbsc, myOrd);

	if (!vExist)
	{
		vInterpolType = myInterpolTypes;
		myStepWiseFunc = MG_Interpolator::GetStepWiseFunc(vInterpolType);
	}
}

MG_1DInterpolator::MG_1DInterpolator(	const MG_Vector	& aAbsc
									,	const MG_Vector	& aOrd
									,	const int		& aInterpolType)
									:	MG_Interpolator(aOrd, aInterpolType)
									,	myAbsc	(aAbsc)
									,	myInterp(NULL)
{
	int vInterpolType(myInterpolTypes);
	bool vExist = MG_Interpolator::LoadGSLSplineInterp(myInterp, vInterpolType, myAbsc, myOrd);

	if (!vExist)
	{
		vInterpolType = myInterpolTypes;
		myStepWiseFunc = MG_Interpolator::GetStepWiseFunc(vInterpolType);
	}
}

MG_1DInterpolator::MG_1DInterpolator(	const vector<double>& aAbsc
									,	const vector<double>& aOrd
									,	const int			& aInterpolType)
									:	MG_Interpolator(aOrd, aInterpolType)
									,	myAbsc	(aAbsc)
									,	myInterp(NULL)
{
	int vInterpolType(myInterpolTypes);
	bool vExist = MG_Interpolator::LoadGSLSplineInterp(myInterp, vInterpolType, myAbsc, myOrd);

	if (!vExist)
	{
		vInterpolType = myInterpolTypes;
		myStepWiseFunc = MG_Interpolator::GetStepWiseFunc(vInterpolType);
	}
}

MG_1DInterpolator::MG_1DInterpolator(	const vector<MG_Date>	& aAbsc
									,	const vector<double>	& aOrd
									,	const int				& aInterpolType)
									:	MG_Interpolator(aOrd, aInterpolType)
									,	myAbsc	(aAbsc)
									,	myInterp(NULL)
{
	vector<double> vAbsc(aAbsc.size());
	for(size_t i=0; i<aAbsc.size(); ++i)
		vAbsc[i] = aAbsc[i].GetJulianDay();
	myAbsc = MG_Vector(vAbsc);

	int vInterpolType(myInterpolTypes);
	bool vExist = MG_Interpolator::LoadGSLSplineInterp(myInterp, vInterpolType, myAbsc, myOrd);

	if (!vExist)
	{
		vInterpolType = myInterpolTypes;
		myStepWiseFunc = MG_Interpolator::GetStepWiseFunc(vInterpolType);
	}
}

double MG_1DInterpolator::Eval(const double& aX) const
{
	if (myStepWiseFunc)
		return myStepWiseFunc(myOrd, 0, myAbsc, aX);

	bool vOntheLeft = aX < myAbsc.Front();
	if (vOntheLeft || aX>myAbsc.Back())
		return Extrapolate(aX, vOntheLeft);

	return gsl_spline_eval(myInterp, aX, NULL);
}

double MG_1DInterpolator::Extrapolate(const double& aX, const bool& aOnTheLeft) const
{
	if (aOnTheLeft)
		return myOrd(0,0);
	return myOrd.Last();
}

/* 2D Interpolator */
MG_2DInterpolator::MG_2DInterpolator(	const MG_2DInterpolator& aRight)
									:	MG_Interpolator(aRight)
									,	myAbsc1D		(aRight.myAbsc1D)
									,	myAbsc2D		(aRight.myAbsc2D)
									,	my1DInterp		(NULL)
{
	if (myStepWiseFunc)
		return;

	int vInterpolTypes(myInterpolTypes);

	LoadGSLInterp(my1DInterp, vInterpolTypes);

	if (aRight.my2DInterps.size() > 0)
		MG_Interpolator::LoadGSLSplineInterp(my2DInterps, vInterpolTypes, myAbsc2D, myOrd);
}

void MG_2DInterpolator::Swap(MG_2DInterpolator& aRight)
{
	MG_Interpolator::Swap(aRight);
	myAbsc1D.Swap(aRight.myAbsc1D);
	myAbsc2D.Swap(aRight.myAbsc2D);
	swap(my1DInterp, aRight.my1DInterp);
	my2DInterps.swap(aRight.my2DInterps);
}

MG_2DInterpolator::~MG_2DInterpolator()
{
	gsl_interp_free(my1DInterp);
	for(size_t i=0; i<my2DInterps.size(); ++i)
		gsl_spline_free(my2DInterps[i]);
}

MG_2DInterpolator::MG_2DInterpolator(void)	:	MG_Interpolator()
											,	my1DInterp(NULL)
{}

MG_2DInterpolator::MG_2DInterpolator(	const MG_Vector	& aAbsc1D
									,	const MG_Vector	& aAbsc2D
									,	const MG_Matrix	& aOrd
									,	const int		& aInterpolType)
									:	MG_Interpolator(aOrd, aInterpolType)
									,	myAbsc1D		(aAbsc1D)
									,	myAbsc2D		(aAbsc2D)
									,	my1DInterp		(NULL)
{
	int vInterpolType(myInterpolTypes);
	bool vExist = LoadGSLSplineInterp(my2DInterps, vInterpolType, myAbsc2D,  myOrd);
	vExist = MG_Interpolator::LoadGSLInterp(my1DInterp, vInterpolType);

	if (!vExist)
	{
		vInterpolType = myInterpolTypes;
		myStepWiseFunc = MG_Interpolator::GetStepWiseFunc(vInterpolType);
	}
}

MG_2DInterpolator::MG_2DInterpolator(	const vector<double>& aAbsc1D
									,	const vector<double>& aAbsc2D
									,	const MG_Matrix		& aOrd
									,	const int			& aInterpolType)
									:	MG_Interpolator(aOrd, aInterpolType)
									,	myAbsc1D		(aAbsc1D)
									,	myAbsc2D		(aAbsc2D)
									,	my1DInterp		(NULL)
{
	int vInterpolType(myInterpolTypes);
	bool vExist = LoadGSLSplineInterp(my2DInterps, vInterpolType, myAbsc2D,  myOrd);
	vExist = vExist && MG_Interpolator::LoadGSLInterp(my1DInterp, vInterpolType);

	/*if (!vExist)
	{
		vInterpolType = myInterpolTypes;
		myStepWiseFunc = MG_Interpolator::GetStepWiseFunc(vInterpolType);
	}*/
}

double MG_2DInterpolator::Eval(const double& aX, const double& aY) const
{
	return Interpolate_Surface(my2DInterps, my1DInterp, myAbsc1D, aX, aY);
}

