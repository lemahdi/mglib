/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 15 SEP 2012
 * Purpose				: This is an abstraction for all kind of options
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/date.h"
#include "vanilla/security/security.h"


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

	MG_VanillaOption(	const std::vector<MG_Date>	& aMatDts
					,	const MG_Security			& aUnd
					,	const double				& aStrike);

protected:
	std::vector<MG_Date> myMatDts;
	std::auto_ptr<MG_Security> myUnderlying;
	double myStrike;

};


MG_NAMESPACE_END
