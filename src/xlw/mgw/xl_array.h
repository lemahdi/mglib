//============================================================================
//============================================================================
//
// Financial Applications of Excel Add-in Development in C/C++ (2nd edition)
// 
// Author: Steve Dalton
// 
// Published by John Wiley & Sons Ltd, The Atrium, Southern Gate, Chichester,
// West Sussex, PO19 8SQ, UK.
// 
// All intellectual property rights and copyright in the code listed in this
// module are reserved.  You may reproduce and use this code only as permitted
// in the Rules of Use that you agreed to on installation.  To use the material
// in this source module, or material derived from it, in any other way you
// must first obtain written permission.  Requests for permission can be sent
// by email to permissions@eigensys.com.
// 
// No warranty, explicit or implied, is made by either the author or publisher
// as to the quality, fitness for a particular purpose, accuracy or
// appropriateness of material in this module.  The code and methods contained
// are intended soley for example and clarification.  You should not rely on
// any part of this code without having completely satisfied yourself that it
// is correct and appropriate for your needs.
//
//============================================================================
//============================================================================
#pragma once

#ifndef WORD
#include <windows.h>
#endif

#ifndef RW
#include "xlw/xlcall32.h"
#endif

typedef FP		xl4_array;
typedef FP12	xl12_array;

xl4_array *new_xl4_array(RW rows, COL columns, double *array = NULL);
bool transpose_xl4_array(xl4_array *p);

xl12_array *new_xl12_array(RW rows, COL columns, double *array = NULL);
bool transpose_xl12_array(xl12_array *p);

