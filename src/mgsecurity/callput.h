/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 17 NOV 2012
 * Purpose				: MG_CallPut is Call or Put option
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgsecurity/option.h"
#include "mgnova/glob/genericdate.h"


MG_NAMESPACE_BEGIN


/* Cap / Floor */
class MG_CallPut : public MG_VanillaOption
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_CallPut)

	ASSIGN_OPERATOR(MG_CallPut)
	CLONE_METHOD(MG_CallPut)
	SWAP_DECL(MG_CallPut)

	virtual ~MG_CallPut(void);

	MG_CallPut	(	const MG_GenericDate& aMatDt
				,	const MG_Security	& aUnderlying
				,	const CALLPUT_NAME	& aCallPut
				,	const double		& aStrike);
	MG_CallPut	(	const std::vector<MG_Date>	& aMatDts
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
