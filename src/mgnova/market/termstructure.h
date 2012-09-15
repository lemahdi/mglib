/*
 * Copyright			: 2012 by MG
 * File					: termstructure.h
 * Version				: 0.1.23
 * Last changed			: 02 JAN 2012
 * Purpose				: MG_TermStructure is a term structure container
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/numerical/interpolator.h"
#include "mgnova/glob/date.h"


MG_NAMESPACE_BEGIN


/* Interest Leg Coupon Leg */
class MG_TermStructure : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_TermStructure)

	ASSIGN_OPERATOR(MG_TermStructure)
	CLONE_METHOD(MG_TermStructure)
	SWAP_DECL(MG_TermStructure)

	virtual ~MG_TermStructure(void);

	MG_TermStructure	(	const std::vector<MG_Date>	& aPayDates
						,	const std::vector<double>	& aValues
						,	const int					& aInterpolType);

public:
	/* State */
	inline const std::vector<MG_Date>	& GetPayDates	(void) const { return myOriginPayDates; }
	inline const std::vector<double>	& GetValues		(void) const { return myOriginValues; }

	/* Engine */
	double CptValue(const MG_Date& aPayDt) const;

private:
	const std::vector<MG_Date>	myOriginPayDates;
	const std::vector<double>	myOriginValues;

	MG_1DInterpolator myInterpolator;

};

MG_NAMESPACE_END
