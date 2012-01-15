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


MG_NAMESPACE_BEGIN

/* Closed Formula namespace */
namespace MG_CF
{
	/* Option Pricing Formulas */
	typedef double (*OptionPrice) (const double& aF, const double& aK, const double& aT, const double& aDf, const double& aVol);

	extern double CallPrice	(	const double& aF
							,	const double& aK
							,	const double& aT
							,	const double& aDf
							,	const double& aVol);

	extern double PutPrice	(	const double& aF
							,	const double& aK
							,	const double& aT
							,	const double& aDf
							,	const double& aVol);

	extern double DigitalCallPrice	(	const double& aF
									,	const double& aK
									,	const double& aT
									,	const double& aDf
									,	const double& aVol);

	extern double DigitalPutPrice	(	const double& aF
									,	const double& aK
									,	const double& aT
									,	const double& aDf
									,	const double& aVol);

	extern double StraddlePrice	(	const double& aF
								,	const double& aK
								,	const double& aT
								,	const double& aDf
								,	const double& aVol);

}


MG_NAMESPACE_END
