/*
 * Copyright			: 2010 by MG
 * File					: xlcache.h
 * Version				: 0.1.22
 * Last changed			: 24 APR 2011
 * Purpose				: MG_XL_Cached is a class for managing the persistence through excel
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/glob/typedef.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_XL_Cached
{
public:
	static void GetCaller	(Coord& aTopC, Coord& aBottomC);
	static void GetSheetNm	(std::string& aSheetName);

};


MG_NAMESPACE_END
