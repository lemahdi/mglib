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

#define MAX_XL4_STR_LEN		255u
#define MAX_XL11_ROWS		65536
#define MAX_XL11_COLS		256
#define MAX_XL11_UDF_ARGS	30

#define XLOPER_SIZE		(sizeof(xloper))
//===================================================================
// Frees dll-allocated xloper memory using free() and assumes that all
// types that have memory were allocated in a way that is compatible
// with freeing by a call to free(), including all strings within
// arrays.
//===================================================================
void free_xloper(xloper *p_op);

//===================================================================

char *deep_copy_xlstring(const char *xlstring);
char *deep_copy_xlstring(const wchar_t *xl12string);
wchar_t *deep_copy_xl12string(const char *xlstring);
wchar_t *deep_copy_xl12string(const wchar_t *xl12string);
char *new_xlstring(const char *text);
char *new_xlstring(const wchar_t *text);
wchar_t *new_xl12string(const char *text);
wchar_t *new_xl12string(const wchar_t *text);
char *append_xlstring(const char *xlstring, const char *new_text);
char *append_xlstring(const char *xlstring, const wchar_t *new_text);
wchar_t *append_xl12string(const wchar_t *xl12string, const char *new_text);
wchar_t *append_xl12string(const wchar_t *xl12string, const wchar_t *new_text);
wchar_t *concat_xlstrings(const wchar_t *string1, const wchar_t *string2);
char *concat_xlstrings(const char *string1, const char *string2);

bool is_input_missing(const xloper *p_op);
bool coerce_xloper(const xloper *p_op, xloper &ret_val, int target_type);
bool coerce_to_string(const xloper *p_op, char *&text); // makes new string
bool coerce_to_double(const xloper *p_op, double &d);
bool coerce_to_int(const xloper *p_op, int &w);
bool coerce_to_short(const xloper *p_op, short &s);
bool coerce_to_bool(const xloper *p_op, bool &b);
double *coerce_to_double_array(const xloper *p_op, double invalid_value, RW &rows, COL &cols);
void set_to_double(xloper *p_op, double d);
void set_to_int(xloper *p_op, int w);
void set_to_bool(xloper *p_op, bool b);
void set_to_text(xloper *p_op, const char *text);
void set_to_text(xloper *p_op, const wchar_t *text);
void set_to_err(xloper *p_op, WORD e);
bool set_to_xltypeMulti(xloper *p_op, RW rows, COL cols);
bool set_to_xltypeSRef(xloper *p_op, RW rwFirst, RW rwLast, COL colFirst, COL colLast);
bool set_to_xltypeRef(xloper *p_op, DWORD idSheet, RW rwFirst, RW rwLast, COL colFirst, COL colLast);
bool set_to_type(xloper *p_op, int new_type);
bool xloper_size(const xloper *p_op, RW &rows, COL &cols);
bool xloper_equals_text(const xloper &op, const wchar_t *p);
bool xloper_equals_text(const xloper &op, const char *p);
bool xlopers_equal(const xloper &op1, const xloper &op2);
bool clone_xloper(xloper *p_target, const xloper *p_source);

// Routines for converting Variants to/from xlopers or other types
bool xloper_to_vt(const xloper *p_op, VARIANT &var, bool convert_array);
bool vt_to_xloper(xloper &op, const VARIANT *pv, bool convert_array);
VARIANT * as_variant(const char *text);

extern xloper *p_xlTrue;
extern xloper *p_xlFalse;
extern xloper *p_xlMissing;
extern xloper *p_xlNil;
extern xloper *p_xlErrNull;
extern xloper *p_xlErrDiv0;
extern xloper *p_xlErrValue;
extern xloper *p_xlErrRef;
extern xloper *p_xlErrName;
extern xloper *p_xlErrNum;
extern xloper *p_xlErrNa;

