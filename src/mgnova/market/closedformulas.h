/*
 * Copyright			: 2011 by MG
 * File					: closedformulas.h
 * Version				: 0.1.23
 * Last changed			: 22 MAY 2011
 * Purpose				: Usefull Functions
 * Author				: Ghada EL BOURY
 * Notes				: 
 */


#pragma once


#include <iostream>

#include "mgnova/glob/typedef.h"

#include "gsl/gsl_nan.h"
#include "gsl/gsl_sys.h"


MG_NAMESPACE_BEGIN


/* Useful class for closed formulas */
class MG_ClosedFormulas
{
public:
	enum CF_NAME
	{
		CALL,
		PUT,
		CALL_DIGIT,
		PUT_DIGIT,
		STRADDLE
	};

	template<CF_NAME>
	static double VanillaPrice(const double& aF, const double& aK, const double& aT, const double& aDf, const double& aVol);
	/*{
		return GSL_NAN;
	}*/
};
typedef MG_ClosedFormulas MG_CF;


MG_NAMESPACE_END
