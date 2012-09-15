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
//============================================================
// THREAD_LOCAL STORAGE
//============================================================
// See TLS.cpp

// Get the address of a static xloper that has been allocated
// for this thread that can be used to return an xloper pointer
// to a worksheet function.  Caller must check that NULL is not
// returned.
xloper *get_thread_local_xloper(void);
xloper12 *get_thread_local_xloper12(void);

// Get the address of a static pointer to an xl4_array or
// xl12_array.  Caller must check that NULL is not returned.
xl4_array *get_thread_local_xl4_array(size_t size);
xl12_array *get_thread_local_xl12_array(size_t size);

