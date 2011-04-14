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

#include "mgnova/typedef.h"
#include "mgnova/matrix.h"


MG_NAMESPACE_BEGIN


/* Base class for interpolators */
class MG_Interpolator : public MG_Object
{
protected:
	typedef MG_Matrix			MG_Curve;
	typedef std::vector<double>	MG_ABSC;
	typedef std::vector<double>	MG_COOR;

public:
	/* Constructors / Destructor */
	MG_Interpolator(void);
	MG_Interpolator(const MG_Interpolator& aRight);
	ASSIGN_OPERATOR(MG_Interpolator)
	//CLONE_METHOD(MG_Interpolator)
	SWAP_DECL(MG_Interpolator)
	virtual ~MG_Interpolator(void);

public:
	virtual double Interpolate	(	const MG_Curve		& aCurve
								,	const MG_ABSC		& aAbscisses
								,	const MG_COOR		& aCoordinates
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
	MG_LinearInterpolator(void);
	MG_LinearInterpolator(const MG_LinearInterpolator& aRight);
	ASSIGN_OPERATOR(MG_LinearInterpolator)
	CLONE_METHOD(MG_LinearInterpolator)
	SWAP_DECL(MG_LinearInterpolator)
	virtual ~MG_LinearInterpolator(void);

public:
	virtual double Interpolate	(	const MG_Curve		& aCurve
								,	const MG_ABSC		& aAbscisses
								,	const MG_COOR		& aCoordinates
								,	const double		& aX = 0
								,	const double		& aY = 0);
};


MG_NAMESPACE_END
