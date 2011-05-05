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

#include "xlw/MJmatrices.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Base class for interpolators */
class MG_Interpolator : public MG_Object
{
protected:
	typedef xlw::MJMatrix		MG_Curve;
	typedef std::vector<double>	MG_Line;
	typedef std::vector<double>	MG_ABSC;
	typedef std::vector<double>	MG_ORD;

public:
	virtual double Interpolate	(	const MG_Line		& aLine
								,	const MG_ABSC		& aAbscisses
								,	const double		& aX = 0) = 0;
	virtual double Interpolate	(	const MG_Curve		& aCurve
								,	const MG_ABSC		& aAbscisses
								,	const MG_ORD		& aOrdinates
								,	const double		& aX = 0
								,	const double		& aY = 0) = 0;

public:
	static MG_InterpolatorPtr Builder(const INTERPOL_METHOD& aInterpolMethod);

};


/* Linear interpolator */
class MG_LinearInterpolator : public MG_Interpolator
{
public:
	/* Constructors / Destructor */
	CLONE_METHOD(MG_LinearInterpolator)

public:
	virtual double Interpolate	(	const MG_Line		& aLine
								,	const MG_ABSC		& aAbscisses
								,	const double		& aX = 0);
	virtual double Interpolate	(	const MG_Curve		& aCurve
								,	const MG_ABSC		& aAbscisses
								,	const MG_ORD		& aOrdinates
								,	const double		& aX = 0
								,	const double		& aY = 0);
};
typedef	Singleton<MG_LinearInterpolator> MG_SLinearInterpolator;


MG_NAMESPACE_END
