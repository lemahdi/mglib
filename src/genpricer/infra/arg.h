/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.14
 * Date					: 04 NOV 2012
 * Purpose				: MG_Arg is a generic class for MG_Func arguments
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/date.h"

#include <vector>


MG_NAMESPACE_BEGIN


class MG_Arg
{
public:
	enum TYPE { ARG_ERR, ARG_DOUBLE, ARG_STRING, ARG_DATE, ARG_V_DOUBLE, ARG_BOOL, ARG_V_BOOL };

	//==> ctor & dtor
	MG_Arg();
	MG_Arg(const double				& aDbl);
	MG_Arg(const std::string		& aStr);
	MG_Arg(const MG_Date			& aDt);
	MG_Arg(const MG_StdVectDblPtr	& aVDbl);
	MG_Arg(const bool				& aBool);
	MG_Arg(const MG_StdVectBoolPtr	& aVBool);

	virtual ~MG_Arg() {}

	//==> accessors
	inline const TYPE&					Type	() const { return myType; }
	inline double						Double	() const { return myDouble; }
	inline const std::string&			String	() const { return myString; }
	inline const MG_Date&				Date	() const { return myDate; }
	inline const MG_StdVectDblPtr&		VDouble	() const { return myVDouble; }
	inline bool							Bool	() const { return myBool; }
	inline const MG_StdVectBoolPtr&		VBool	() const { return myVBool; }

	//==> support
	friend std::ostream& operator<< (std::ostream& aOs, const MG_Arg& aArg)
	{
		aOs << aArg.Double();
		return aOs;
	}

private:
	TYPE myType;

	double				myDouble;
	std::string			myString;
	MG_Date				myDate;
	MG_StdVectDblPtr	myVDouble;
	bool				myBool;
	MG_StdVectBoolPtr	myVBool;
};


MG_NAMESPACE_END
