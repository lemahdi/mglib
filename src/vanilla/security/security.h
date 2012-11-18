/*
 * Copyright			: 2012 by MG
 * File					: security.h
 * Version				: 0.1.23
 * Date					: 01 JAN 2012
 * Purpose				: MG_Security is a base class for creating securities
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/market/termstructure.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Forward Declarations */
class MG_Model;


/* Base class for models */
class MG_Security : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	MG_Security(const MG_TermStructure& aNotional = MG_TermStructure(100.));
	// self-generated copy-constructor is enough
	virtual ~MG_Security(void);

	ASSIGN_OPERATOR(MG_Security)
	//CLONE_METHOD(MG_Security)
	SWAP_DECL(MG_Security)

public:
	/* Accessors */
	inline const std::vector<double>& Forwards(void) const { return myForwards; }
	inline const std::vector<double>& Flows(void) const { return myFlows; }
	inline const std::vector<double>& Dfs(void) const { return myDfs; }
	inline const std::vector<double>& Notionals(void) const { return myNotionals; }

	/* Engine */
	virtual void	PrePricing	(const MG_Model& aMdl)	= 0;
	virtual double	Price		(void) const			= 0;

protected:
	std::vector<double> myForwards;
	std::vector<double> myFlows;
	std::vector<double> myDfs;
	std::vector<double> myNotionals;

	MG_TermStructure myNotional;
};


MG_NAMESPACE_END
