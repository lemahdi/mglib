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


MG_NAMESPACE_BEGIN


class MG_GenericDate;
class MG_SwapLeg;


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

	MG_Digital	(	const MG_GenericDate& aMat
				,	const MG_SwapLegPtr	& aUnd
				,	const double		& aStrike);

public:
	/* Engine */
	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:

};

MG_NAMESPACE_END
