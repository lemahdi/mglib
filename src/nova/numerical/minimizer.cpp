#include "nova/numerical/minimizer.h"

#include "gsl/gsl_errno.h"
#include "nova/glob/exception.h"


using namespace std;
using namespace MG;


/*
 * Minimizer Class
 */
const gsl_min_fminimizer_type* MG_Minimizer::From_MGType_To_GSLType(const MINIMIZER_TYPE& aType)
{
	switch(aType)
	{
	case GOLDEN_SECTION:
		return gsl_min_fminimizer_goldensection;
		
	case BRENT:
		return gsl_min_fminimizer_brent;
		
	case QUAD_GOLDEN:
		return gsl_min_fminimizer_quad_golden;

	default: return NULL;
	}
}

MG_Minimizer::MG_Minimizer	(	const MG_Minimizer& aRight)
							:	MG_XLObject(aRight)
							,	myName		(aRight.myName)
							,	myMinimizer	(NULL)
							,	myType		(aRight.myType)
							,	myStatus(aRight.myStatus)
							,	myEpsAbs(aRight.myEpsAbs), myEpsRel(aRight.myEpsRel)
							,	myMaxIter(aRight.myMaxIter), myNbIter(aRight.myNbIter)
{}

void MG_Minimizer::Swap(MG_Minimizer& aRight)
{
	MG_XLObject::Swap(aRight);
	myName.swap(aRight.myName);
	swap(myMinimizer, aRight.myMinimizer);
	myFunc->Swap(*aRight.myFunc);
	swap(const_cast<MINIMIZER_TYPE&>(myType), const_cast<MINIMIZER_TYPE&>(aRight.myType));
	swap(myStatus, aRight.myStatus);
	swap(myEpsAbs, aRight.myEpsAbs);
	swap(myEpsRel, aRight.myEpsRel);
	swap(myMaxIter, aRight.myMaxIter);
	swap(myNbIter, aRight.myNbIter);
}

MG_Minimizer::MG_Minimizer	(	const MINIMIZER_TYPE& aType
							,	const double		& aEpsAbs
							,	const double		& aEpsRel
							,	const size_t		& aMaxIter)
							:	MG_XLObject()
							,	myMinimizer	(NULL)
							,	myType		(aType)
							,	myFunc		(NULL)
							,	myEpsAbs(aEpsAbs), myEpsRel(aEpsRel)
							,	myMaxIter(aMaxIter), myNbIter(0), myStatus(GSL_EOF)
{
	myXLName = MG_MINIMIZER_XL_NAME;

	const gsl_min_fminimizer_type* vType = From_MGType_To_GSLType(myType);
	if (vType)
	{
		myMinimizer = gsl_min_fminimizer_alloc(vType);
		myName = gsl_min_fminimizer_name(myMinimizer);
	}
}

MG_Minimizer::~MG_Minimizer()
{
	gsl_min_fminimizer_free(myMinimizer);
}

void MG_Minimizer::Load(const MG_FunctionPtr& aFunc, const MinimizerBounds& aBds, const bool& aWithValues)
{
	myFunc = aFunc;
	if (aWithValues)
		gsl_min_fminimizer_set_with_values	(	myMinimizer, myFunc->GetFunc()
											,	aBds.myMinimum, aBds.myFMinimum
											,	aBds.myLower, aBds.myFLower
											,	aBds.myUpper, aBds.myFUpper);
	else
		gsl_min_fminimizer_set(myMinimizer, myFunc->GetFunc(), aBds.myMinimum, aBds.myLower, aBds.myUpper);
}

double MG_Minimizer::Minimize(MinimizerBounds& aBds)
{
	myNbIter = 0;
	double vLowX, vUpX;
	do
	{
		++myNbIter;
		myStatus = gsl_min_fminimizer_iterate(myMinimizer);
		if (myStatus==GSL_EBADFUNC || myStatus==GSL_FAILURE)
			MG_THROW("Error while iterating in minimizer.");

		vLowX		= gsl_min_fminimizer_x_lower(myMinimizer);
		vUpX		= gsl_min_fminimizer_x_upper(myMinimizer);
		myStatus	= gsl_min_test_interval(vLowX, vUpX, myEpsAbs, myEpsRel);
	} while (myStatus==GSL_CONTINUE && myNbIter<myMaxIter);

	aBds.myMinimum	= gsl_min_fminimizer_x_minimum(myMinimizer);
	aBds.myLower	= gsl_min_fminimizer_x_lower(myMinimizer);
	aBds.myUpper	= gsl_min_fminimizer_x_upper(myMinimizer);
	aBds.myFMinimum	= gsl_min_fminimizer_f_minimum(myMinimizer);
	aBds.myFLower	= gsl_min_fminimizer_f_lower(myMinimizer);
	aBds.myFUpper	= gsl_min_fminimizer_f_upper(myMinimizer);

	return aBds.myMinimum;
}
