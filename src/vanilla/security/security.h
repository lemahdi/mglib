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


#include "nova/glob/object.h"

#include <vector>


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
	/* Accessors */
	inline double GetPrice(void) const { return myPrice; }
	inline const std::vector<double>& Forwards(void) const { return myForwards; }
	inline const std::vector<double>& Flows(void) const { return myFlows; }
	inline const std::vector<double>& Dfs(void) const { return myDfs; }

	/* Engine */
	virtual void	PrePricing	(const MG_Model& aMdl)	= 0;
	virtual double	Price		(void) const			= 0;

protected:
	double myPrice;
	std::vector<double> myForwards;
	std::vector<double> myFlows;
	std::vector<double> myDfs;
};


MG_NAMESPACE_END
