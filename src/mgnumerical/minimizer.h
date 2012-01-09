/*
 * Copyright			: 2011 by MG
 * File					: minimizer.h
 * Version				: 0.1.23
 * Last changed			: 21 NOV 2011
 * Purpose				: MG_Minimizer is a base class for minimizing functions
 * Author				: MM Akkouh
 * Notes				: This class does not guarantee to find an absolute minima
 */


#pragma once


#include "mgnova/typedef.h"
#include "mgnova/object.h"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/wrapper/function.h"

#include "gsl/gsl_min.h"


MG_NAMESPACE_BEGIN


/* Minimizer Class */
class MG_Minimizer : public MG_XLObject
{
public:
	enum MINIMIZER_TYPE { GOLDEN_SECTION, BRENT, QUAD_GOLDEN };
	struct MinimizerBounds
	{
		double myLower;
		double myUpper;
		double myMinimum;
		double myFLower;
		double myFUpper;
		double myFMinimum;
	};
	static const gsl_min_fminimizer_type* From_MGType_To_GSLType(const MINIMIZER_TYPE& aType);

	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Minimizer)

	ASSIGN_OPERATOR(MG_Minimizer)
	CLONE_METHOD(MG_Minimizer)
	SWAP_DECL(MG_Minimizer)

	MG_Minimizer(const MINIMIZER_TYPE& aType, const double& aEpsAbs, const double& aEpsRel, const size_t& aMaxIter);

	virtual ~MG_Minimizer(void);

	/* State */
	void Load(const MG_FunctionPtr& aFunc, const MinimizerBounds& aBds, const bool& aWithValues = false);
	inline int		GetStatus(void) const { return myStatus; }
	inline size_t	GetNbIter(void) const { return myNbIter; }

	/* Engine */
	double Minimize(MinimizerBounds& aBds);

private:
	std::string				myName;
	gsl_min_fminimizer*		myMinimizer;
	const MINIMIZER_TYPE	myType;
	MG_FFunctionPtr			myFunc;
	
	int				myStatus;
	double			myEpsAbs;
	double			myEpsRel;
	size_t			myMaxIter;
	size_t			myNbIter;

};


MG_NAMESPACE_END

