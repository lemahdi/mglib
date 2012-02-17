/*
 * Copyright			: 2012 by MG
 * File					: security.h
 * Version				: 0.1.23
 * Last changed			: 01 JAN 2012
 * Purpose				: MG_Security is a base class for creating securities
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/glob/object.h"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/market/schedule.h"


MG_NAMESPACE_BEGIN


/* Forward Declarations */
class MG_Model;


/* Base class for models */
class MG_Security : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	MG_Security(void);
	// self-generated copy-constructor is enough
	virtual ~MG_Security(void);

	ASSIGN_OPERATOR(MG_Security)
	//CLONE_METHOD(MG_Security)
	SWAP_DECL(MG_Security)

public:
	/* Engine */
	virtual void	PrePricing	(const MG_Model& aMdl)	= 0;
	virtual double	Price		(void) const			= 0;

protected:
};

/* Interest Rate base class security */
class MG_IRSecurity : public MG_Security
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRSecurity)

	ASSIGN_OPERATOR(MG_IRSecurity)
	//CLONE_METHOD(MG_IRSecurity)
	SWAP_DECL(MG_IRSecurity)

	virtual ~MG_IRSecurity(void);

	MG_IRSecurity(void) {}
	MG_IRSecurity(const MG_Schedule& aSched);

public:
	/* State */
	inline const MG_Schedule& GetSchedule(void) const { return mySchedule; }
	inline const MG_Date& GetStartDate	(void) const { return mySchedule.GetStartDate(); }
	inline const MG_Date& GetEndDate	(void) const { return mySchedule.GetEndDate(); }

protected:
	/* Engine */
	void GenerateSchedule(const MG_GenericDate& aStDt, const MG_GenericDate& aEdDt, const MG_IRIndex& aIdx);

protected:
	MG_Schedule mySchedule;

};

/* Interest Rate base class option */
class MG_IROption : public MG_Security
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IROption)

	ASSIGN_OPERATOR(MG_IROption)
	//CLONE_METHOD(MG_IROption)
	SWAP_DECL(MG_IROption)

	virtual ~MG_IROption(void);

	MG_IROption(void) {}
	MG_IROption(const MG_GenericDate& aMaturity, const MG_IRSecurityPtr& aUnderlying);

public:
	/* State */
	inline const MG_GenericDate& GetMaturity(void) const { return myMaturity; }

protected:
	MG_GenericDate myMaturity;
	MG_IRSecurityPtr myUnderlying;

};

MG_NAMESPACE_END
