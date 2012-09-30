/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 16 JAN 2012
 * Purpose				: MG_CapFloor is Cap or Floor option
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgsecurity/option.h"


MG_NAMESPACE_BEGIN


class MG_GenericDate;
class MG_SwapLeg;


/* Cap / Floor */
class MG_CapFloor : public MG_VanillaOption
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_CapFloor)

	ASSIGN_OPERATOR(MG_CapFloor)
	CLONE_METHOD(MG_CapFloor)
	SWAP_DECL(MG_CapFloor)

	virtual ~MG_CapFloor(void);

	MG_CapFloor	(	const MG_GenericDate& aMat
				,	const MG_SwapLegPtr	& aUnd
				,	const double		& aStrike);

public:
	/* Engine */
	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:

};

MG_NAMESPACE_END
