/*
 * Copyright			: 2011 by MG
 * File					: closedformulas.h
 * Version				: 0.1.23
 * Last changed			: 22 MAY 2011
 * Purpose				: Usefull Functions
 * Author				: Ghada EL BOURY
 * Notes				: 
 */


#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <assert.h>
#ifdef WIN32
#include <io.h>
#endif
#include "mgnova/typedef.h"
#include "normal.h"


MG_NAMESPACE_BEGIN

enum eOPTION {_call,_put,_calldigit,_putdigit,_straddle};

extern double VanillaPrice(double Fwd, double K, double T, double DF, double Vol, eOPTION TypeOp);


MG_NAMESPACE_END
