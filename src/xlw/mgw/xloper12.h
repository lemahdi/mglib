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

#ifndef VARIANT
#include <ole2.h>
#endif

#include "xlw/xlcall32.h"

#define MAX_XL12_STR_LEN	32767u
#define MAX_XL12_ROWS		1048576
#define MAX_XL12_COLS		16384
#define MAX_XL12_UDF_ARGS	255

#define XLOPER12_SIZE		(sizeof(xloper12))
//===================================================================
// Frees dll-allocated xloper memory using free() and assumes that all
// types that have memory were allocated in a way that is compatible
// with freeing by a call to free(), including all strings within
// arrays.
//===================================================================
void free_xloper(xloper12 *p_op);

//===================================================================

bool is_input_missing(const xloper12 *p_op);
bool coerce_xloper(const xloper12 *p_op, xloper12 &ret_val, int target_type);
bool coerce_to_string(const xloper12 *p_op, char *&text); // makes new string
bool coerce_to_string(const xloper12 *p_op, wchar_t *&text); // makes new string
bool coerce_to_string(const xloper *p_op, wchar_t *&text); // makes new string
bool coerce_to_double(const xloper12 *p_op, double &d);
bool coerce_to_int(const xloper12 *p_op, int &w);
bool coerce_to_short(const xloper12 *p_op, short &s);
bool coerce_to_bool(const xloper12 *p_op, bool &b);
double *coerce_to_double_array(const xloper12 *p_op, double invalid_value, COL &cols, RW &rows);
void set_to_double(xloper12 *p_op, double d);
void set_to_int(xloper12 *p_op, int w);
void set_to_bool(xloper12 *p_op, bool b);
void set_to_text(xloper12 *p_op, const char *text);
void set_to_text(xloper12 *p_op, const wchar_t *text);
void set_to_err(xloper12 *p_op, WORD e);
bool set_to_xltypeMulti(xloper12 *p_op, RW rows, COL cols);
bool set_to_xltypeSRef(xloper12 *p_op, RW rwFirst, RW rwLast, COL colFirst, COL colLast);
bool set_to_xltypeRef(xloper12 *p_op, DWORD idSheet, RW rwFirst, RW rwLast, COL colFirst, COL colLast);
bool set_to_type(xloper12 *p_op, int new_type);
bool xloper_size(const xloper12 *p_op, RW &rows, COL &cols);
bool xloper_equals_text(const xloper12 &op, const char *p);
bool xloper_equals_text(const xloper12 &op, const wchar_t *p);
bool xlopers_equal(const xloper12 &op1, const xloper12 &op2);
bool clone_xloper(xloper12 *p_target, const xloper12 *p_source);

// Routines for converting Variants to/from xloper12s or other types
bool xloper_to_vt(const xloper12 *p_op, VARIANT &var, bool convert_array);
bool vt_to_xloper(xloper12 &op, const VARIANT *pv, bool convert_array);

// Routines for converting xlopers to/from xloper12s
bool xloper12_to_xloper(xloper *p_target, const xloper12 *p_source);
bool xloper_to_xloper12(xloper12 *p_target, const xloper *p_source);


extern xloper12 *p_xl12True;
extern xloper12 *p_xl12False;
extern xloper12 *p_xl12Missing;
extern xloper12 *p_xl12Nil;
extern xloper12 *p_xl12ErrNull;
extern xloper12 *p_xl12ErrDiv0;
extern xloper12 *p_xl12ErrValue;
extern xloper12 *p_xl12ErrRef;
extern xloper12 *p_xl12ErrName;
extern xloper12 *p_xl12ErrNum;
extern xloper12 *p_xl12ErrNa;
