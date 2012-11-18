/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 30 SEP 2012
 * Purpose				: MG_Digital is a Digital option
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgsecurity/option.h"
#include "mgnova/glob/genericdate.h"


MG_NAMESPACE_BEGIN


class MG_GenericDate;


/* Cap / Floor */
class MG_Digital : public MG_VanillaOption
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Digital)

	ASSIGN_OPERATOR(MG_Digital)
	CLONE_METHOD(MG_Digital)
	SWAP_DECL(MG_Digital)

	virtual ~MG_Digital(void);

	MG_Digital	(	const MG_GenericDate& aMatDt
				,	const MG_Security	& aUnderlying
				,	const CALLPUT_NAME	& aCallPut
				,	const double		& aStrike);
	MG_Digital	(	const std::vector<MG_Date>	& aMatDts
				,	const MG_Security			& aUnderlying
				,	const CALLPUT_NAME			& aCallPut
				,	const double				& aStrike);

public:
	/* Engine */
	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:
	MG_GenericDate myMatDt;
	CALLPUT_NAME myCallPut;

};

MG_NAMESPACE_END
