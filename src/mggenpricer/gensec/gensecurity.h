/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 07 MAY 2011
 * Purpose				: MG_GenSecurity is a class for parsing payoffs
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mginfra/nodes.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_GenSecurity : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	// self-generated copy-constructor is enough
	virtual ~MG_GenSecurity(void);

	ASSIGN_OPERATOR(MG_GenSecurity)
	CLONE_METHOD(MG_GenSecurity)
	SWAP_DECL(MG_GenSecurity)

	MG_GenSecurity(const std::vector<std::string>& aDealDesc, const size_t& aCols);

private:
	std::vector<std::string>	myHeader;
	std::vector<std::string>	myDealDesc;
	size_t						myRows;
	size_t						myCols;

	MG_TableWalker				myWalker;
};


MG_NAMESPACE_END
