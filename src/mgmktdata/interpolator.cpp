#include "interpolator.h"


using namespace std;
using namespace MG;


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

/* Base Interpolator class */
MG_Interpolator::MG_Interpolator() : MG_Object()
{}

MG_Interpolator::MG_Interpolator(const MG_Interpolator& aRight) : MG_Object(aRight)
{}

void MG_Interpolator::Swap(MG_Interpolator& )
{}

MG_Interpolator::~MG_Interpolator()
{}


/* Linear Interpolator class */
MG_LinearInterpolator::MG_LinearInterpolator() : MG_Interpolator()
{}

MG_LinearInterpolator::MG_LinearInterpolator(const MG_LinearInterpolator& aRight) : MG_Interpolator(aRight)
{}

void MG_LinearInterpolator::Swap(MG_LinearInterpolator& )
{}

MG_LinearInterpolator::~MG_LinearInterpolator()
{}

double MG_LinearInterpolator::Interpolate	(	const MG_Curve		& //aCurve
											,	const MG_ABSC		& //aAbscisses
											,	const MG_COOR		& //aCoordinates
											,	const double		& //aX
											,	const double		& )//aY)
{
	return 0.;
}