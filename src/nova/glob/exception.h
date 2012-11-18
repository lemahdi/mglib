/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.21
 * Date					: 17 MAR 2011
 * Purpose				: MG_Exception is the MG library exception class
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/typedef.h"

#include <exception>
#include <string>
#include <assert.h>


MG_NAMESPACE_BEGIN


class MG_Exception : public std::exception
{
	FAKE_ASSIGN_OPERATOR(MG_Exception)

public:
	/* Constructors / Destructor */
	MG_Exception(const std::string& aFile, const unsigned int& aLine, const std::string& aMsg);
#ifdef __CYGWIN__
	virtual ~MG_Exception(void) throw();
#endif

	inline std::string GetMessage(void) const { return myMsg; }

private:
	std::string myMsg;
};


MG_NAMESPACE_END
