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

//============================================================================
//============================================================================
// This source file contains definitions of constant xloper12s, functions that
// convert between xloper12s and other data types, compare xloper12s, clone
// xloper12s, convert to and from Variant data types and xlopers.
//============================================================================
//============================================================================

#include <windows.h>
#pragma warning (disable : 4996)
#pragma warning (disable : 4267)
#include "xlw/mgw/xloper.h"
#include "xlw/mgw/xloper12.h"

// Use this structure to initialise constant xloper12s that are not doubles or strings.
typedef struct
{
	INT32 int1;
	INT32 int2;
	INT32 int3;
	INT32 int4;
	DWORD xltype;
}
	const_xloper12;

const_xloper12 xloper12BooleanTrue = {1, 0, 0, 0, xltypeBool};
const_xloper12 xloper12BooleanFalse = {0, 0, 0, 0, xltypeBool};
const_xloper12 xloper12Missing = {0, 0, 0, 0, xltypeMissing};
const_xloper12 xloper12Nil = {0, 0, 0, 0, xltypeNil};
const_xloper12 xloper12ErrNull = {0, 0, 0, 0, xltypeErr};
const_xloper12 xloper12ErrDiv0 = {7, 0, 0, 0, xltypeErr};
const_xloper12 xloper12ErrValue = {15, 0, 0, 0, xltypeErr};
const_xloper12 xloper12ErrRef = {23, 0, 0, 0, xltypeErr};
const_xloper12 xloper12ErrName = {29, 0, 0, 0, xltypeErr};
const_xloper12 xloper12ErrNum = {36, 0, 0, 0, xltypeErr};
const_xloper12 xloper12ErrNa = {42, 0, 0, 0, xltypeErr};

xloper12 *p_xl12True = ((xloper12 *)&xloper12BooleanTrue);
xloper12 *p_xl12False = ((xloper12 *)&xloper12BooleanFalse);
xloper12 *p_xl12Missing = ((xloper12 *)&xloper12Missing);
xloper12 *p_xl12Nil = ((xloper12 *)&xloper12Nil);
xloper12 *p_xl12ErrNull = ((xloper12 *)&xloper12ErrNull);
xloper12 *p_xl12ErrDiv0 = ((xloper12 *)&xloper12ErrDiv0);
xloper12 *p_xl12ErrValue = ((xloper12 *)&xloper12ErrValue);
xloper12 *p_xl12ErrRef = ((xloper12 *)&xloper12ErrRef);
xloper12 *p_xl12ErrName = ((xloper12 *)&xloper12ErrName);
xloper12 *p_xl12ErrNum = ((xloper12 *)&xloper12ErrNum);
xloper12 *p_xl12ErrNa = ((xloper12 *)&xloper12ErrNa);

//-------------------------------------------------------------------
// Excel worksheet cell error codes are passed
// via VB OLE Variant arguments in 'ulVal'.
// These are equivalent to the offset below
// plus the value defined in "xlcall.h"
//
// This is easier than using the VT_SCODE variant
// property 'scode'.
//-------------------------------------------------------------------
#define VT_XL_ERR_OFFSET		2148141008ul

//===================================================================
//
// Functions to manage xlopers
//
//===================================================================
bool is_input_missing(const xloper12 *p_op)
{
	return !p_op || (p_op->xltype & (xltypeMissing | xltypeNil));
}
//===================================================================
// Frees dll-allocated xloper12 memory using free() and assumes that all
// types that have memory were allocated in a way that is compatible
// with freeing by a call to free(), including all strings within
// arrays.
//===================================================================
void free_xloper(xloper12 *p_op)
{
	if(p_op->xltype & xltypeMulti)
	{
// First check if string elements need to be freed then free the array.
// WARNING: Assumes all strings are allocated with calls to malloc().
		DWORD limit = p_op->val.array.rows * p_op->val.array.columns;
		xloper12 *p = p_op->val.array.lparray;

		for(;limit--; p++)
			if(p->xltype & xltypeStr)
				free(p->val.str);

		free(p_op->val.array.lparray);
	}
	else if(p_op->xltype & xltypeStr)
	{
		free(p_op->val.str);
	}
	else if(p_op->xltype & xltypeRef)
	{
		free(p_op->val.mref.lpmref);
	}
}
//-------------------------------------------------------------------
bool coerce_xloper(const xloper12 *p_op, xloper12 &ret_val, int target_type)
{
// Target will contain the information that tells Excel what type to
// convert to.
	xloper12 target;

	target.xltype = xltypeInt;
	target.val.w = target_type; // can be more than one type

	if(Excel12(xlCoerce, &ret_val, 2, p_op, &target) != xlretSuccess
	|| (ret_val.xltype & target_type) == 0)
		return false;

	return true;
}
//-------------------------------------------------------------------
bool coerce_to_string(const xloper12 *p_op, char *&text)
{
	wchar_t *str;
	xloper12 ret_val;

	text = NULL; // can test this or the return value for failure

	if(!p_op || (p_op->xltype & (xltypeMissing | xltypeNil)) != 0)
		return false;

	if(p_op->xltype != xltypeStr)
	{
// xloper is not a string type, so try to convert it.
		if(!coerce_xloper(p_op, ret_val, xltypeStr))
			return false;

		str = ret_val.val.str;
	}
	else if(!(str = p_op->val.str)) // make a working copy of the ptr
		return false;

	size_t len = str[0];

	if((text = (char *)malloc(len + 1)) == NULL) // caller must free this
	{
		if(p_op->xltype != xltypeStr)
			Excel12(xlFree, 0, 1, &ret_val);

		return false;
	}
	wcstombs(text, str + 1, len);
	text[len] = 0;

// If the string from which the copy was made was created in a call
// to coerce_xloper above, then need to free it with a call to xlFree
	if(p_op->xltype != xltypeStr)
		Excel12(xlFree, 0, 1, &ret_val);
	
	return true;
}
//-------------------------------------------------------------------
bool coerce_to_string(const xloper12 *p_op, wchar_t *&text)
{
	wchar_t *str;
	xloper12 ret_val;

	text = NULL; // can test this or the return value for failure

	if(!p_op || (p_op->xltype & (xltypeMissing | xltypeNil)) != 0)
		return false;

	if(p_op->xltype != xltypeStr)
	{
// xloper12 is not a string type, so try to convert it.
		if(!coerce_xloper(p_op, ret_val, xltypeStr))
			return false;

		str = ret_val.val.str;
	}
	else if(!(str = p_op->val.str)) // make a working copy of the ptr
		return false;

	size_t len = str[0];

	if((text = (wchar_t *)malloc((len + 1) * sizeof(wchar_t))) == NULL) // caller must free this
	{
		if(p_op->xltype != xltypeStr)
			Excel12(xlFree, 0, 1, &ret_val);

		return false;
	}

	if(len)
		memcpy(text, (char *)(str + 1), len * sizeof(wchar_t));
	text[len] = 0; // xloper12 string may not me null terminated

// If the string from which the copy was made was created in a call
// to coerce_xloper above, then need to free it with a call to xlFree
	if(p_op->xltype != xltypeStr)
		Excel12(xlFree, 0, 1, &ret_val);
	
	return true;
}
//-------------------------------------------------------------------
// Convert xloper to an xloper12 and then coerce to a string
// This ensures that, for example, reference xlopers are coerced
// to their values in wchar_t form
//-------------------------------------------------------------------
bool coerce_to_string(const xloper *p_op, wchar_t *&text)
{
	xloper12 op12;

	if(!xloper_to_xloper12(&op12, p_op))
		return false;

	bool ret_val = coerce_to_string(&op12, text);
	free_xloper(&op12);
	return ret_val;
}
//-------------------------------------------------------------------
bool coerce_to_double(const xloper12 *p_op, double &d)
{
	if(!p_op || (p_op->xltype & (xltypeMissing | xltypeNil)) != 0)
		return false;

	if(p_op->xltype == xltypeNum)
	{
		d = p_op->val.num;
		return true;
	}

// xloper is not a floating point number type, so try to convert it.
	xloper12 ret_val;

	if(!coerce_xloper(p_op, ret_val, xltypeNum))
		return false;

	d = ret_val.val.num;
	return true;
}
//-------------------------------------------------------------------
// Allocate and populate an array of doubles based on input xloper
//-------------------------------------------------------------------
double *coerce_to_double_array(const xloper12 *p_op, double invalid_value, RW &rows, COL &cols)
{
	if(!p_op || (p_op->xltype & (xltypeMissing | xltypeNil)))
		return NULL;

// xloper is not an xloper array type, so try to convert it.
	xloper12 ret_val;

	if(!coerce_xloper(p_op, ret_val, xltypeMulti))
		return NULL;

// Allocate the space for the array of doubles
	cols = ret_val.val.array.columns;
	rows = ret_val.val.array.rows;
	int size = rows * cols;
	double *d_array = (double *)malloc(size * sizeof(double));

	if(!d_array)
	{
	// Must free array memory allocated by xlCoerce
		Excel12(xlFree, 0, 1, &ret_val);
		return NULL;
	}

// Get the cell values one-by-one as doubles and place in the array.
// Store the array row-by-row in memory.
	xloper12 *p_elt = ret_val.val.array.lparray;

	if(!p_elt) // array could not be created
	{
	// Must free array memory allocated by xlCoerce
		Excel12(xlFree, 0, 1, &ret_val);
		free(d_array);
		return NULL;
	}

	double *p = d_array;

	for(; size--; p++)
		if(!coerce_to_double(p_elt++, *p))
			*p = invalid_value;

	Excel12(xlFree, 0, 1, &ret_val);
	return d_array; // caller must free this
}
//-------------------------------------------------------------------
bool coerce_to_int(const xloper12 *p_op, int &w)
{
	if(!p_op || (p_op->xltype & (xltypeMissing | xltypeNil)) != 0)
		return false;

	if(p_op->xltype == xltypeInt)
	{
		w = p_op->val.w;
		return true;
	}

	if(p_op->xltype == xltypeErr)
	{
		w = p_op->val.err;
		return true;
	}

// xloper is not an integer type, so try to convert it.
	xloper12 ret_val;

	if(!coerce_xloper(p_op, ret_val, xltypeInt))
		return false;

	w = ret_val.val.w;
	return true;
}
//-------------------------------------------------------------------
bool coerce_to_short(const xloper12 *p_op, short &s)
{
	int i;
	if(!coerce_to_int(p_op, i))
		return false;

	s = (short)i;
	return true;
}
//-------------------------------------------------------------------
bool coerce_to_bool(const xloper12 *p_op, bool &b)
{
	if(!p_op || (p_op->xltype & (xltypeMissing | xltypeNil)) != 0)
		return false;

	if(p_op->xltype == xltypeBool)
	{
		b = (p_op->val.xbool != 0);
		return true;
	}

//	if(p_op->xltype == xltypeErr)
//		return (p_op->val.err != 0);

// xloper is not a Boolean number type, so try to convert it.
	xloper12 ret_val;

	if(!coerce_xloper(p_op, ret_val, xltypeBool))
		return false;

	b = (ret_val.val.xbool != 0);
	return true;
}
//-------------------------------------------------------------------
void set_to_double(xloper12 *p_op, double d)
{
	if(!p_op) return;
	p_op->xltype = xltypeNum;
	p_op->val.num = d;
}
//-------------------------------------------------------------------
void set_to_int(xloper12 *p_op, int w)
{
	if(!p_op) return;
	p_op->xltype = xltypeInt;
	p_op->val.w = w;
}
//-------------------------------------------------------------------
void set_to_bool(xloper12 *p_op, bool b)
{
	if(!p_op) return;
	p_op->xltype = xltypeBool;
	p_op->val.xbool = (b ? 1 : 0);
}
//-------------------------------------------------------------------
void set_to_text(xloper12 *p_op, const char *text)
{
	if(!p_op) return;

	if(!(p_op->val.str = new_xl12string(text)))
		p_op->xltype = xltypeNil;
	else
		p_op->xltype = xltypeStr;
}
//-------------------------------------------------------------------
void set_to_text(xloper12 *p_op, const wchar_t *text)
{
	if(!p_op) return;

	if(!(p_op->val.str = new_xl12string(text)))
		p_op->xltype = xltypeNil;
	else
		p_op->xltype = xltypeStr;
}
//-------------------------------------------------------------------
void set_to_err(xloper12 *p_op, WORD e)
{
	if(!p_op) return;

	switch(e)
	{
	case xlerrNull:
	case xlerrDiv0:
	case xlerrValue:
	case xlerrRef:
	case xlerrName:
	case xlerrNum:
	case xlerrNA:
		p_op->xltype = xltypeErr;
		p_op->val.err = e;
		break;

	default:
		p_op->xltype = xltypeNil; // not a valid error code
	}
}
//-------------------------------------------------------------------
bool set_to_xltypeMulti(xloper12 *p_op, RW rows, COL cols)
{
	DWORD size = rows * cols;

	if(!p_op || !size || rows > MAX_XL12_ROWS-1 || cols > MAX_XL12_COLS-1)
		return false;

	p_op->xltype = xltypeMulti;
	p_op->val.array.lparray = (xloper12 *)malloc(XLOPER12_SIZE * size);
	p_op->val.array.rows = rows; // counts from 1
	p_op->val.array.columns = cols; // counts from 1
	return true;
}
//-------------------------------------------------------------------
bool set_to_xltypeSRef(xloper12 *p_op, RW rwFirst, RW rwLast, COL colFirst, COL colLast)
{
	if(!p_op || rwFirst > rwLast || colFirst > colLast)
		return false;

// Create a simple single-cell reference to a cell on the current sheet
	p_op->xltype = xltypeSRef;
	p_op->val.sref.count = 1;

	xlref12 &ref = p_op->val.sref.ref; // to simplify code
	ref.rwFirst = rwFirst;
	ref.rwLast = rwLast;
	ref.colFirst = colFirst;
	ref.colLast = colLast;
	return true;
}
//-------------------------------------------------------------------
bool set_to_xltypeRef(xloper12 *p_op, DWORD idSheet, RW rwFirst, RW rwLast, COL colFirst, COL colLast)
{
	if(!p_op || rwFirst > rwLast || colFirst > colLast)
		return false;

// Allocate memory for the xlmref and set pointer within the xloper
	xlmref12 *p = (xlmref12 *)malloc(sizeof(xlmref12));

	if(!p)
	{
		p_op->xltype = xltypeNil;
		return false;
	}

	p_op->xltype = xltypeRef;
	p_op->val.mref.lpmref = p;
	p_op->val.mref.idSheet = idSheet;
	p_op->val.mref.lpmref->count = 1;

	xlref12 &ref = p->reftbl[0]; // to simplify code
	ref.rwFirst = rwFirst;
	ref.rwLast = rwLast;
	ref.colFirst = colFirst;
	ref.colLast = colLast;
	return true;
}
//-------------------------------------------------------------------
bool set_to_type(xloper12 *p_op, int new_type)
{
	if(!p_op)
		return false;

	switch(p_op->xltype = new_type)
	{
	case xltypeStr:
		p_op->val.str = NULL;
		break;

	case xltypeRef:
		p_op->val.mref.lpmref = NULL;
		p_op->val.mref.idSheet = 0;
		break;

	case xltypeMulti:
		p_op->val.array.lparray = NULL;
		p_op->val.array.rows = p_op->val.array.columns = 0;
		break;

	case xltypeInt:
	case xltypeNum:
	case xltypeBool:
	case xltypeErr:
	case xltypeSRef:
	case xltypeMissing:
	case xltypeNil:
	case xltypeBigData:
		break;

	default: // not valid or not supported
		p_op->xltype = 0;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------
bool xloper_equals_text(const xloper12 &op, const wchar_t *p)
{
	size_t len;

	if(op.xltype != xltypeStr
	|| (len = wcslen(p)) != op.val.str[0]
	|| wcsncmp(p, op.val.str + 1, len) != 0)
		return false;

	return true;
}
//-------------------------------------------------------------------
bool xloper_equals_text(const xloper12 &op, const char *p)
{
	size_t len;

	if(op.xltype != xltypeStr
	|| (len = strlen(p)) != op.val.str[0])
		return false;

// Convert the byte string to a Unicode string and compare
	wchar_t *pwc = new wchar_t[len + 1];
	mbstowcs(pwc, p, len);
	bool ret_val = wcsncmp(pwc, op.val.str + 1, len) == 0;
	delete[] pwc;
	return ret_val;
}
//-------------------------------------------------------------------
// Get the size of a range or Multi xloper
bool xloper_size(const xloper12 *p_op, RW &rows, COL &cols)
{
	rows = cols = 0;
	const xlref12 *pref;

	switch(p_op->xltype)
	{
	case xltypeMulti:
		rows = p_op->val.array.rows;
		cols = p_op->val.array.columns;
		break;

	case xltypeSRef:
	case xltypeRef:
		if(p_op->xltype == xltypeSRef)
			pref = &(p_op->val.sref.ref);
		else
			pref = p_op->val.mref.lpmref->reftbl;

		rows = pref->rwLast - pref->rwFirst + 1;
		cols =  pref->colLast - pref->colFirst + 1;
		break;

	default:
		return false;
	}
	return true;
}
//-------------------------------------------------------------------
bool xlopers_equal(const xloper12 &op1, const xloper12 &op2)
{
	if(op1.xltype != op2.xltype)
		return false;

	int i;

	switch(op1.xltype)
	{
	case xltypeNum:
		return op1.val.num == op2.val.num;

	case xltypeStr:
		return *op1.val.str == *op2.val.str && memcmp(op1.val.str + 1, op2.val.str + 1, *op1.val.str) == 0;

	case xltypeBool:
		return op1.val.xbool == op2.val.xbool;

	case xltypeRef:
		if(op1.val.mref.idSheet != op2.val.mref.idSheet
		|| op1.val.mref.lpmref->count != op2.val.mref.lpmref->count)
			return false;

		return memcmp(op1.val.mref.lpmref->reftbl, op2.val.mref.lpmref->reftbl, sizeof(xlmref) * op1.val.mref.lpmref->count) == 0;

	case xltypeSRef:
		return memcmp(&(op1.val.sref.ref), &(op2.val.sref.ref), sizeof(xlref)) == 0;

	case xltypeMulti:
		if(op1.val.array.columns != op2.val.array.columns
		|| op1.val.array.rows != op2.val.array.rows)
			return false;

		for(i = op1.val.array.columns * op1.val.array.rows; i--; )
			if(!xlopers_equal(op1.val.array.lparray[i], op2.val.array.lparray[i]))
				return false;
		break;

	case xltypeMissing:
	case xltypeNil:
		return true;

	case xltypeInt:
		return op1.val.w == op2.val.w;

	case xltypeErr:
		return op1.val.err == op2.val.err;

	default:
		return false; // not suported by this function
	}
	return true;
}
//-------------------------------------------------------------------
// For those types that have memory associated, allocate new memory
// and copy contents.
//-------------------------------------------------------------------
bool clone_xloper(xloper12 *p_target, const xloper12 *p_source)
{
// If pointers equal take no action: memory is not duplicated.
// This should be detected by the caller prior to calling this function
	if(!p_target || !p_source || p_target == p_source)
		return false;

	*p_target = *p_source; // make a copy of the xloper contents.

	p_target->xltype &= ~(xlbitXLFree | xlbitDLLFree);

// These have no memory associated with them, so just return.
// The test for the xltypeInt intentionally catches the xltypeBigOper type.
	if(p_target->xltype & (xltypeInt | xltypeNum | xltypeBool | xltypeErr | xltypeMissing | xltypeNil | xltypeSRef))
		return true;

	switch(p_target->xltype)
	{
// These types have memory associated with them, so need to
// allocate new memory and then copy the contents from source
// memory pointers.
	case xltypeStr:
		p_target->val.str = deep_copy_xl12string(p_target->val.str);
		return true;

	case xltypeRef: // assume lpmref->count == 1
		{
			xlmref12 *p = (xlmref12 *)malloc(sizeof(xlmref12));
			memcpy(p, p_target->val.mref.lpmref, sizeof(xlmref12));
			p_target->val.mref.lpmref = p;
			return true;
		}
		break;

	case xltypeMulti:
		{
			int limit = p_target->val.array.rows * p_target->val.array.columns;
			xloper12 *p = (xloper12 *)malloc(limit * XLOPER12_SIZE);
			memcpy(p, p_target->val.array.lparray, limit * XLOPER12_SIZE);
			p_target->val.array.lparray = p;

			for(int i = limit; i--; p++)
			{
				p->xltype &= ~(xlbitXLFree | xlbitDLLFree);

				switch(p->xltype)
				{
				case xltypeStr:
					p->val.str = deep_copy_xl12string(p->val.str);
					break;

				case xltypeRef:
				case xltypeMulti:
					p->xltype = xltypeNil; // not copied

				default: // other types all fine
					break;
				}
			}
			return true;
		}
		break;

	default: // other types not handled
		p_target->xltype = xltypeNil;
		return false;
	}
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// xloper / xloper12 conversion routines
//
// Routines ignore the source memory bits and DO NOT set these
// on the converted xloper/xloper12.  The caller must set these bits
// or other flags depending on the type of the returned xloper
//-------------------------------------------------------------------
//-------------------------------------------------------------------
bool xloper_to_xloper12(xloper12 *p_target, const xloper *p_source)
{
	p_target->xltype = p_source->xltype & ~(xlbitXLFree | xlbitDLLFree);

	switch(p_target->xltype)
	{
	case xltypeNum:		p_target->val.num = p_source->val.num;		break;
	case xltypeBool:	p_target->val.xbool = p_source->val.xbool;	break;
	case xltypeInt:		p_target->val.w = p_source->val.w;			break;
	case xltypeErr:		p_target->val.err = p_source->val.err;		break;
	case xltypeSRef:
		{
			p_target->val.sref.count = 1;
			const xlref *p_ref = &(p_source->val.sref.ref);
			xlref12 *p_ref12 = &(p_target->val.sref.ref);

			p_ref12->rwFirst = p_ref->rwFirst;
			p_ref12->rwLast = p_ref->rwLast;
			p_ref12->colFirst = p_ref->colFirst;
			p_ref12->colLast = p_ref->colLast;
		}
		break;

// These types have memory associated with them, so need to allocate
// new memory and then copy the contents from source.
	case xltypeStr:
		p_target->val.str = deep_copy_xl12string(p_source->val.str);
		break;

	case xltypeRef:
		{
			xlmref *p_s_mref = p_source->val.mref.lpmref;
			int count = p_s_mref->count;
			xlmref12 *p_t_mref = (xlmref12 *)malloc(sizeof(xlmref12) + (count - 1) * sizeof(xlref12));
			if(!p_t_mref)
				return false;
			p_target->val.mref.lpmref = p_t_mref;
			p_t_mref->count = count;
			xlref12 *p_ref12 = p_t_mref->reftbl;
			xlref *p_ref = p_s_mref->reftbl;

			for(;count--; p_ref12++, p_ref++)
			{
				p_ref12->colFirst = p_ref->colFirst;
				p_ref12->colLast = p_ref->colLast;
				p_ref12->rwFirst = p_ref->rwFirst;
				p_ref12->rwLast = p_ref->rwLast;
			}
			p_target->val.mref.idSheet = p_source->val.mref.idSheet;
		}
		break;

	case xltypeMulti:
		{
			p_target->val.array.columns = p_source->val.array.columns;
			p_target->val.array.rows = p_source->val.array.rows;
			int limit = p_source->val.array.rows * p_source->val.array.columns;
			xloper12 *p_t = (xloper12 *)malloc(limit * XLOPER12_SIZE);
			if(!p_t)
				return false;
			p_target->val.array.lparray = p_t;
			xloper *p_s = p_source->val.array.lparray;

			for(;limit--;)
				xloper_to_xloper12(p_t++, p_s++);
		}
		break;
	}
	return true;
}
//-------------------------------------------------------------------
// Conversion can fail in which case returns bool false
//-------------------------------------------------------------------
bool xloper12_to_xloper(xloper *p_target, const xloper12 *p_source)
{
	p_target->xltype = (WORD)(p_source->xltype & ~(xlbitXLFree | xlbitDLLFree));

	switch(p_target->xltype)
	{
	case xltypeNum:		p_target->val.num = p_source->val.num;		break;
	case xltypeBool:	p_target->val.xbool = p_source->val.xbool;	break;
	case xltypeInt:		p_target->val.w = p_source->val.w;			break;
	case xltypeErr:		p_target->val.err = p_source->val.err;		break;
// This type can reference larger ranges or arrays than xloper
// so need to check that the xloper limits are not exceded
	case xltypeSRef:
		{
			p_target->val.sref.count = 1;
			const xlref12 *p_ref12 = &(p_source->val.sref.ref);
			xlref *p_ref = &(p_target->val.sref.ref);

#if 0 // Very safe: fail if ranges start or end outside xloper scope
			if(p_ref12->colLast >= MAX_XL11_COLS // count from 0
			|| p_ref12->rwLast >= MAX_XL11_ROWS
			|| p_ref12->colFirst >= MAX_XL11_COLS
			|| p_ref12->rwFirst >= MAX_XL11_ROWS)
			{
				return false;
			}
			p_ref->colFirst = p_ref12->colFirst;
			p_ref->rwFirst = p_ref12->rwFirst;
			p_ref->colLast = p_ref12->colLast;
			p_ref->rwLast = p_ref12->rwLast;

#else // Truncate ranges that extend beyond xloper scope
			if(p_ref12->colFirst >= MAX_XL11_COLS // count from 0
			|| p_ref12->rwFirst >= MAX_XL11_ROWS)
			{
// Range is completely outside xloper's scope so fail
				return false;
			}
			p_ref->colFirst = p_ref12->colFirst;
			p_ref->rwFirst = p_ref12->rwFirst;
			p_ref->colLast = p_ref12->colLast >= MAX_XL11_COLS ? MAX_XL11_COLS - 1 : p_ref12->colLast;
			p_ref->rwLast = p_ref12->rwLast >= MAX_XL11_ROWS ? MAX_XL11_ROWS - 1 : p_ref12->rwLast;
#endif
		}
		break;

// These types have memory associated with them, so need to allocate
// new memory and then copy the contents from source.
	case xltypeStr:
// String truncated if longer than 255 bytes and cast down to bytes
		p_target->val.str = deep_copy_xlstring(p_source->val.str);
		break;

// These last 2 types can reference larger ranges or arrays than
// xloper types so need to check that the xloper limits are not exceded
	case xltypeRef:
		{
			p_target->val.mref.idSheet = p_source->val.mref.idSheet;
			int count = p_source->val.mref.lpmref->count;
			xlmref *p_t_mref = (xlmref *)malloc(sizeof(xlmref) + (count - 1) * sizeof(xlref));
			p_target->val.mref.lpmref->count = count;
			xlref *p_ref = p_target->val.mref.lpmref->reftbl;
			xlref12 *p_ref12 = p_source->val.mref.lpmref->reftbl;

			for(;count--; p_ref12++, p_ref++)
			{
#if 0 // Very safe: fail if ranges start or end outside xloper scope
				if(p_ref12->colLast >= MAX_XL11_COLS // count from 0
				|| p_ref12->rwLast >= MAX_XL11_ROWS
				|| p_ref12->colFirst >= MAX_XL11_COLS
				|| p_ref12->rwFirst >= MAX_XL11_ROWS)
				{
					free(p_t_mref);
					p_target->val.mref.lpmref = NULL;
					return false;
				}
				p_ref->colFirst = p_ref12->colFirst;
				p_ref->rwFirst = p_ref12->rwFirst;
				p_ref->colLast = p_ref12->colLast;
				p_ref->rwLast = p_ref12->rwLast;

#else // Truncate ranges that extend beyond xloper scope
				if(p_ref12->colFirst >= MAX_XL11_COLS // count from 0
				|| p_ref12->rwFirst >= MAX_XL11_ROWS)
				{
// Range is completely outside xloper's scope so fail
					free(p_t_mref);
					p_target->val.mref.lpmref = NULL;
					return false;
				}
				p_ref->colFirst = p_ref12->colFirst;
				p_ref->rwFirst = p_ref12->rwFirst;
				p_ref->colLast = p_ref12->colLast >= MAX_XL11_COLS ? MAX_XL11_COLS - 1 : p_ref12->colLast;
				p_ref->rwLast = p_ref12->rwLast >= MAX_XL11_ROWS ? MAX_XL11_ROWS - 1 : p_ref12->rwLast;
#endif
			}
		}
		break;

	case xltypeMulti:
		{
			RW r, rows, sr = p_source->val.array.rows; // counts from 1
			COL c, cols, sc = p_source->val.array.columns; // counts from 1

// xloper xltypeMulti cannot access whole columns so max r value is MAX_XL11_ROWS - 1
			rows = sr > MAX_XL11_ROWS - 1 ? MAX_XL11_ROWS - 1 : sr;
			cols = sc > MAX_XL11_COLS ? MAX_XL11_COLS : sc;

			p_target->val.array.rows = rows;
			p_target->val.array.columns = cols;
			xloper *p_t = (xloper *)malloc(rows * cols * XLOPER_SIZE);
			p_target->val.array.lparray = p_t;
			xloper12 *p_s = p_source->val.array.lparray;

// Might need to truncate columns and rows, so need to work row-by-row
// xlytpeMulti types are row-major
			for(r = 0; r < rows; r++)
			{
				p_s = p_source->val.array.lparray + r * sc;
				for(c = 0; c < cols; c++)
					xloper12_to_xloper(p_t++, p_s++);
			}
		}
		break;
	}
	return true;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Variant and Bstr conversion routines
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// Converts a VT_BSTR wide-char string to a newly allocated
// length-counted Unicode string.  Memory returned must be freed by caller.
//-------------------------------------------------------------------
wchar_t *vt_bstr_to_xl12string(const BSTR bstr)
{
	if(!bstr)
		return NULL;

	size_t len = SysStringLen(bstr);

	if(len > MAX_XL12_STR_LEN)
		len = MAX_XL12_STR_LEN; // truncate

	wchar_t *p = (wchar_t *)malloc((len + 2)* sizeof(wchar_t));
	memcpy(p, bstr, (len + 2) * sizeof(wchar_t));
	p[0] = (wchar_t)len;
	return p;
}
//-------------------------------------------------------------------
// Converts a byte-counted string to a VT_BSTR wide-char string
// Does not rely on (or assume) that input string is null-terminated.
//-------------------------------------------------------------------
BSTR xlstring_to_vt_bstr(wchar_t *str)
{
	if(!str)
		return NULL;

	BSTR bstr = SysAllocStringLen(str + 1, str[0]);
	return bstr;
}
//-------------------------------------------------------------------
// Converts a null terminated string to a VT_BSTR wide-char string
// Assumes that input string is null-terminated.
//-------------------------------------------------------------------
BSTR zstring_to_vt_bstr(wchar_t *str)
{
	if(!str)
		return NULL;

	int len = wcslen(str);
	BSTR bstr = SysAllocStringLen(str, len);
	return bstr;
}
//-------------------------------------------------------------------
// Returns true and a Variant equivalent to the passed-in xloper, or
// false if xloper12's type could not be converted.
//-------------------------------------------------------------------
bool xloper_to_vt(const xloper12 *p_op, VARIANT &var, bool convert_array)
{
	VariantInit(&var); // type is set to VT_EMPTY

	switch(p_op->xltype)
	{
	case xltypeNum:
		var.vt = VT_R8;
		var.dblVal = p_op->val.num;
		break;

	case xltypeInt:
		var.vt = VT_I2;
		var.iVal = p_op->val.w;
		break;

	case xltypeBool:
		var.vt = VT_BOOL;
		var.boolVal = p_op->val.xbool;
		break;

	case xltypeStr:
		var.vt = VT_BSTR;
		var.bstrVal = xlstring_to_vt_bstr(p_op->val.str);
		break;

	case xltypeErr:
		var.vt = VT_ERROR;
		var.ulVal = VT_XL_ERR_OFFSET + p_op->val.err;
		break;

	case xltypeMulti:
		if(convert_array)
		{
			VARIANT temp_vt;
			SAFEARRAYBOUND bound[2];
			long elt_index[2];

			bound[0].lLbound = bound[1].lLbound = 0;
			bound[0].cElements = p_op->val.array.rows;
			bound[1].cElements = p_op->val.array.columns;

			var.vt = VT_ARRAY | VT_VARIANT; // array of Variants
			var.parray = SafeArrayCreate(VT_VARIANT, 2, bound);

			if(!var.parray)
				return false;

			xloper12 *p_op_temp = p_op->val.array.lparray;

			for(RW r = 0; r < p_op->val.array.rows; r++)
			{
				for(COL c = 0; c < p_op->val.array.columns;)
				{
// Call with last arg false, so not to convert array within array
					xloper_to_vt(p_op_temp++, temp_vt, false);
					elt_index[0] = r;
					elt_index[1] = c++;
					SafeArrayPutElement(var.parray, elt_index, &temp_vt);
				}
			}
			break;
		}
		// else, fall through to default option

	default: // type not converted
		return false;
	}
	return true;
}
//-------------------------------------------------------------------
// Converts the passed-in null-terminated byte-string to a wide-char Variant.
//-------------------------------------------------------------------
VARIANT * as_variant(wchar_t *text)
{
// VBA is not multi-threaded (Excel 12 and earlier) so if called from
// VBA this function will not be called concurrently.  Therefore it is
// safe to return the address of a single static VARIANT.  Otherwise
// a thread-local copy would be required for it to be thread-safe.
	static VARIANT RetVal;
	VariantInit(&RetVal);
	RetVal.vt = VT_BSTR;
	RetVal.bstrVal = zstring_to_vt_bstr(text);
	return &RetVal;
}

bool vt_to_xloper(xloper12 &op, const VARIANT *pv, bool convert_array);
//-------------------------------------------------------------------
// Converts the passed-in array Variant to an xltypeMulti xloper.
//-------------------------------------------------------------------
bool array_vt_to_xloper(xloper12 &op, const VARIANT *pv)
{
	if(!pv || !pv->parray)
		return false;

	int ndims = pv->parray->cDims;

	if(ndims == 0 || ndims > 2)
		return false;

	int dims[2] = {1,1};
	
	long ubound[2], lbound[2];

	for(int d = 0; d < ndims; d++)
	{
// Dimension argument counts from 1, so need to pass d + 1
		if(FAILED(SafeArrayGetUBound(pv->parray, d + 1, ubound + d))
		|| FAILED(SafeArrayGetLBound(pv->parray, d + 1, lbound + d)))
			return false;

		dims[d] = (short)(ubound[d] - lbound[d] + 1);
	}

	op.val.array.lparray = (xloper12 *)malloc(dims[0] * dims[1] * XLOPER12_SIZE);

	if(!op.val.array.lparray)
		return false;

	op.val.array.rows = dims[0];
	op.val.array.columns = dims[1];
	op.xltype = xltypeMulti;

	xloper12 *p_op = op.val.array.lparray;

// Use this union structure to retrieve elements of the SafeArray
	union
	{
		VARIANT var;
		short I2;
		double R8;
		ULONG ulVal;
		CY cyVal;
		bool boolVal;
		BSTR bstrVal;
	}
		temp_union;

	xloper12 temp_op;
	VARTYPE vt_type = pv->vt & ~VT_ARRAY;
	long element_index[2];

	void *vp;
	SafeArrayAccessData(pv->parray, &vp);
	SafeArrayUnaccessData(pv->parray);

	for(RW r = 0; r < dims[0]; r++)
	{
		for(COL c = 0; c < dims[1];)
		{
			element_index[0] = r + lbound[0];
			element_index[1] = c++ + lbound[1];

			if(FAILED(SafeArrayGetElement(pv->parray, element_index, &temp_union)))
			{
				temp_op.xltype = xltypeNil;
			}
			else
			{
				switch(vt_type)
				{
				case VT_I2:
					temp_op.xltype = xltypeInt;
					temp_op.val.w = temp_union.I2;
					break;

				case VT_R8:
					temp_op.xltype = xltypeNum;
					temp_op.val.num = temp_union.R8;
					break;

				case VT_BOOL:
					temp_op.xltype = xltypeBool;
					temp_op.val.xbool = temp_union.boolVal;
					break;

				case VT_VARIANT:
// Don't allow Variant to contain an array Variant to prevent recursion
					if(vt_to_xloper(temp_op, &temp_union.var, false))
						break;

				case VT_ERROR:
					temp_op.xltype = xltypeErr;
					temp_op.val.err = (unsigned short)(temp_union.ulVal - VT_XL_ERR_OFFSET);
					break;

				case VT_CY:
					temp_op.xltype = xltypeNum;
					temp_op.val.num = (double)(temp_union.cyVal.int64 / 1e4);
					break;

				case VT_BSTR:
					op.xltype = xltypeStr;
					op.val.str = vt_bstr_to_xl12string(temp_union.bstrVal);
					break;

				default:
					temp_op.xltype = xltypeNil;
				}
			}
			*p_op++ = temp_op;
		}
	}
	return true;
}
//-------------------------------------------------------------------
// Converts the passed-in Variant to an xloper if one of the supported types.
//-------------------------------------------------------------------
bool vt_to_xloper(xloper12 &op, const VARIANT *pv, bool convert_array)
{
	if(pv->vt & (VT_VECTOR | VT_BYREF))
		return false;

	if(pv->vt & VT_ARRAY)
	{
		if(!convert_array)
			return false;

		return array_vt_to_xloper(op, pv);
	}

	switch(pv->vt)
	{
	case VT_R8:
		op.xltype = xltypeNum;
		op.val.num = pv->dblVal;
		break;

	case VT_I2:
		op.xltype = xltypeInt;
		op.val.w = pv->iVal;
		break;

	case VT_BOOL:
		op.xltype = xltypeBool;
		op.val.xbool = pv->boolVal;
		break;

	case VT_ERROR:
		op.xltype = xltypeErr;
		op.val.err = (unsigned short)(pv->ulVal - VT_XL_ERR_OFFSET);
		break;

	case VT_BSTR:
		op.xltype = xltypeStr;
		op.val.str = vt_bstr_to_xl12string(pv->bstrVal);
		break;

	case VT_CY:
		op.xltype = xltypeNum;
		op.val.num = (double)(pv->cyVal.int64 / 1e4);
		break;

	default: // type not converted
		return false;
	}
	return true;
}
