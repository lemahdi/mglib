/*
 * Copyright			: 2011 by MG
 * File					: marketdata.h
 * Version				: 0.1.21
 * Last changed			: 25 MAR 2011
 * Purpose				: MG_Interpolator is a base class for interpolators
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>

#include "mgnova/object.h"
#include "mgnova/patterns/singleton.hpp"
#include "mgnova/exception.h"
#include "mgnova/wrapper/vector.h"
#include "mgnova/wrapper/matrix.h"

#include "gsl/gsl_interp.h"
#include "gsl/gsl_spline.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Linear / .. */
class MG_Interpolator
{
public:

	typedef MG_Matrix MG_Line;
	typedef MG_Matrix MG_Curve;
	typedef MG_Vector MG_ABSC;
	typedef MG_Vector MG_ORD;

	typedef double (*MG_StepWiseFunc) (const MG_Line&, const size_t&, const MG_ABSC&, const double&);

public:
	static int						CreateInterpolTypes		(const std::vector<int>& aInterpolMeths);
	static const gsl_interp_type*	GetGSLInterpolType		(int& aInterpolCode);
	
	static bool CreateGSLInterp			(gsl_interp*& aInterp, int& aInterpCode);
	static bool CreateGSLSplineInterp	(gsl_spline*& aInterp, int& aInterpCode, const MG_ABSC& aAbsc, const MG_Line& aCurve);
	static bool CreateGSLSplineInterp	(	std::vector<gsl_spline*>& aInterp
										,	int						& aInterpCode
										,	const MG_ABSC			& aAbsc
										,	const MG_Curve			& aCurve);

	MG_StepWiseFunc GetStepWiseFunc	(	int			& aInterpolCode
									,	gsl_interp*	& aInterp1
									,	gsl_spline*	& aInterp2
									,	const size_t& aSize);

public:
	static double Interpolate_StepUpLeft(	const MG_Line	& aLine
										,	const size_t	& aLineIndex
										,	const MG_ABSC	& aAbscisses
										,	const double	& aX = 0);

	static double Interpolate_StepUpRight	(	const MG_Line	& aLine
											,	const size_t	& aLineIndex
											,	const MG_ABSC	& aAbscisses
											,	const double	& aX = 0);

	static double Interpolate_Constant	(	const MG_Line	& aLine
										,	const size_t	& aLineIndex
										,	const MG_ABSC	& aAbscisses
										,	const double	& aX = 0);

	static double Interpolate_Surface	(	const std::vector<gsl_spline*>	& a1stInterp
										,	gsl_interp						* a2ndInterp
										,	const MG_ABSC					& a2ndAxe
										,	const double					& aX = 0
										,	const double					& aY = 0);

};


MG_NAMESPACE_END
