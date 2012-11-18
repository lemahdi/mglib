/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 08 JAN 2012
 * Purpose				: Tiny class for generic dates
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <string>
#include <map>

#include "nova/glob/date.h"


MG_NAMESPACE_BEGIN


class MG_GenericDate : public MG_Object
{
public:
	/* Constructors / Destructor */
    COPY_CTOR_DECL(MG_GenericDate)

	ASSIGN_OPERATOR(MG_GenericDate)
	CLONE_METHOD(MG_GenericDate)
	SWAP_DECL(MG_GenericDate)
	
	~MG_GenericDate(void);
	
    //MG_GenericDate(void);
	MG_GenericDate(void);
	MG_GenericDate(const std::string& aStart);
	MG_GenericDate(const std::string& aRefStart, const std::string& aStart);
	MG_GenericDate(const MG_GenericDate& aRefStart, const std::string& aStart);
	explicit MG_GenericDate(const MG_Date& aDt);

	/* Accessors */
	inline const MG_Date& GetDate	(void) const { return myDate; }
	inline bool	IsGenDate			(void) const { return myIsGenDate; }
	inline int GetTimes				(void) const { return myTimes; }

	inline void SetRefDate(const MG_GenericDate& aRefDt) { myRefDate = new MG_GenericDate(aRefDt); }

	/* Cast */
	inline operator const MG_Date& (void) const { return myDate; }

public:
	/* State */
	void Rebuild(const MG_Date& aAsOf);

private:
	const std::string	myStart;
	const std::string	myTerm;

	int					myTimes;
	FREQUENCY_NAME		myFreq;

	MG_GenericDate*		myRefDate;
    MG_Date				myDate;

	const bool			myIsGenDate;

};


MG_NAMESPACE_END
