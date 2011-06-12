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

#include "xlw/MJmatrices.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Linear / .. */
class MG_Interpolator
{
protected:
	typedef xlw::MJMatrix		MG_Line;
	typedef xlw::MJMatrix		MG_Curve;
	typedef std::vector<double>	MG_ABSC;
	typedef std::vector<double>	MG_ORD;

	typedef double (*MG_InterpolLineFunc)	(const MG_Line&, const size_t&, const INTERPOL_DIM&, const MG_ABSC&, const double&);
	typedef double (*MG_InterpolMatrixFunc)	(const MG_Curve&, const MG_ABSC&, const MG_ORD&, const double&, const double&, const int&);

public:
	static long					CreateInterpolTypes		(const std::vector<int>& aInterpolMeths);
	static MG_InterpolLineFunc	GetInterpolatorFunction	(int& aInterpolType);

public:
	static double Interpolate_StepUpLeft(	const MG_Line	& aLine, const size_t& aLineIndex, const INTERPOL_DIM& aLineDim
										,	const MG_ABSC	& aAbscisses
										,	const double	& aX = 0);

	static double Interpolate_StepUpRight	(	const MG_Line	& aLine, const size_t& aLineIndex, const INTERPOL_DIM& aLineDim
											,	const MG_ABSC	& aAbscisses
											,	const double	& aX = 0);

	static double Interpolate_Linear(	const MG_Line	& aLine, const size_t& aLineIndex, const INTERPOL_DIM& aLineDim
									,	const MG_ABSC	& aAbscisses
									,	const double	& aX = 0);

	static double Interpolate_Dim2	(	const MG_Curve				& aCurve
									,	const MG_ABSC				& aAbscisses
									,	const MG_ORD				& aOrdinates
									,	const double				& aX = 0
									,	const double				& aY = 0
									,	const MG_InterpolLineFunc	& a1stInterpolFunc = NULL
									,	const MG_InterpolLineFunc	& a2ndInterpolFunc = NULL);

};


MG_NAMESPACE_END
