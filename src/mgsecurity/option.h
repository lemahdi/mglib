/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 15 SEP 2012
 * Purpose				: This is an abstraction for all kind of options
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/glob/genericdate.h"
#include "mgsecurity/security.h"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/glob/typedef.h"


MG_NAMESPACE_BEGIN


/* Option */
class MG_VanillaOption : public MG_Security
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_VanillaOption)

	ASSIGN_OPERATOR(MG_VanillaOption)
	//CLONE_METHOD(MG_VanillaOption)
	SWAP_DECL(MG_VanillaOption)

	virtual ~MG_VanillaOption(void);

	MG_VanillaOption(	const MG_GenericDate& aMat
					,	const MG_SecurityPtr& aUnd
					,	const double		& aStrike);

public:
	/* State */
	inline const MG_GenericDate& MaturityDate(void) const { return myMatDt; }
	inline const MG_SecurityPtr& Underlying(void) const { return myUnderlying; }
	inline double Strike(void) const { return myStrike; }
	
	/* Engine */
	void PrePricing(const MG_Model& aMdl);

protected:
	MG_GenericDate myMatDt;
	MG_SecurityPtr myUnderlying;
	double myStrike;

};


MG_NAMESPACE_END
