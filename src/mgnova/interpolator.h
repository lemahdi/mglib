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

#include "mgnova/wrapper/vector.h"
#include "mgnova/wrapper/matrix.h"
#include "mgnova/date.h"

#include "gsl/gsl_interp.h"
#include "gsl/gsl_spline.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Step Up Interpolation */
extern double Interpolate_StepUpLeft(	const MG_Matrix	& aLine
									,	const size_t	& aLineIndex
									,	const MG_Vector	& aAbscisses
									,	const double	& aX = 0);

extern double Interpolate_StepUpRight	(	const MG_Matrix	& aLine
										,	const size_t	& aLineIndex
										,	const MG_Vector	& aAbscisses
										,	const double	& aX = 0);

extern double Interpolate_Constant	(	const MG_Matrix	& aLine
									,	const size_t	& aLineIndex
									,	const MG_Vector	& aAbscisses
									,	const double	& aX = 0);

extern double Interpolate_Surface	(	const std::vector<gsl_spline*>	& a1stInterp
									,	gsl_interp						* a2ndInterp
									,	const MG_Vector					& a2ndAxe
									,	const double					& aX = 0
									,	const double					& aY = 0);

/* Interpolator class */
class MG_Interpolator : public MG_Object
{
public:
	typedef double (*MG_StepWiseFunc) (const MG_Matrix&, const size_t&, const MG_Vector&, const double&);

	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Interpolator)
		
	ASSIGN_OPERATOR(MG_Interpolator)
	SWAP_DECL(MG_Interpolator)
	
	virtual ~MG_Interpolator(void);

	MG_Interpolator(void);
	MG_Interpolator	(	const MG_Matrix	& aOrd
					,	const int		& aInterpolType);
	MG_Interpolator	(	const MG_Vector	& aOrd
					,	const int		& aInterpolType);
	MG_Interpolator	(	const std::vector<double>	& aOrd
					,	const int					& aInterpolType);

protected:
	MG_Matrix		myOrd;

	const int		myInterpolTypes;
	MG_StepWiseFunc	myStepWiseFunc;

	/* Static Functions */
public:
	static int						CreateInterpolCode		(const std::vector<int>& aInterpolMeths);

protected:
	static const gsl_interp_type*	GetGSLInterpolType		(int& aInterpolCode);
	
	static bool LoadGSLInterp		(	gsl_interp*& aInterp, int& aInterpCode);
	static bool LoadGSLSplineInterp	(	gsl_spline*		& aInterp
									,	int				& aInterpCode
									,	const MG_Vector	& aAbsc
									,	const MG_Matrix	& aOrd);
	static bool LoadGSLSplineInterp	(	std::vector<gsl_spline*>& aInterp
									,	int						& aInterpCode
									,	const MG_Vector			& aAbsc
									,	const MG_Matrix			& aOrd);

	static MG_StepWiseFunc GetStepWiseFunc(int& aInterpolCode);
};

/* 1D Interpolator class */
class MG_1DInterpolator : public MG_Interpolator
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_1DInterpolator)
		
	ASSIGN_OPERATOR(MG_1DInterpolator)
	CLONE_METHOD(MG_1DInterpolator)
	SWAP_DECL(MG_1DInterpolator)
	
	virtual ~MG_1DInterpolator(void);

	MG_1DInterpolator(void);
	MG_1DInterpolator	(	const MG_Vector	& aAbsc
						,	const MG_Matrix	& aOrd
						,	const int		& aInterpolType);
	MG_1DInterpolator	(	const MG_Vector	& aAbsc
						,	const MG_Vector	& aOrd
						,	const int		& aInterpolType);
	MG_1DInterpolator	(	const std::vector<double>	& aAbsc
						,	const std::vector<double>	& aOrd
						,	const int					& aInterpolType);
	MG_1DInterpolator	(	const std::vector<MG_Date>	& aAbsc
						,	const std::vector<double>	& aOrd
						,	const int					& aInterpolType);

public:
	/* Engine */
	double Eval(const double& aX) const;
	double Extrapolate(const double& aX, const bool& aOnTheLeft) const;

private:
	MG_Vector	myAbsc;
	gsl_spline*	myInterp;

};

/* 2D Interpolator class */
class MG_2DInterpolator : public MG_Interpolator
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_2DInterpolator)
		
	ASSIGN_OPERATOR(MG_2DInterpolator)
	CLONE_METHOD(MG_2DInterpolator)
	SWAP_DECL(MG_2DInterpolator)
	
	virtual ~MG_2DInterpolator(void);

	MG_2DInterpolator(void);
	MG_2DInterpolator	(	const MG_Vector	& aAbsc1D
						,	const MG_Vector	& aAbsc2D
						,	const MG_Matrix	& aOrd
						,	const int		& aInterpolType);
	MG_2DInterpolator	(	const std::vector<double>	& aAbsc1D
						,	const std::vector<double>	& aAbsc2D
						,	const MG_Matrix				& aOrd
						,	const int					& aInterpolType);

public:
	/* Engine */
	double Eval(const double& aX, const double& aY) const;

private:
	MG_Vector					myAbsc1D;
	MG_Vector					myAbsc2D;

	gsl_interp*					my1DInterp;
	std::vector<gsl_spline*>	my2DInterps;

};


MG_NAMESPACE_END
