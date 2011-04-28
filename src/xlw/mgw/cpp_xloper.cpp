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
// This source file (and header file) contain an example class that wraps the
// xloper to simplify the tasks of creating, reading, modifying and managing
// the memory associated with them.  The class code relies on a number of
// routines in the xloper.cpp source file.
//============================================================================
//============================================================================
#include <windows.h>
#include <stdio.h>

#include "xlw/mgw/cpp_xloper.h"
#include "xlw/mgw/TLS.h"


bool gExcelVersion12plus = true;
bool gExcelVersion11minus = false;

//===================================================================
//===================================================================
//
// Public functions for class cpp_xloper
//
//===================================================================
//===================================================================

//-------------------------------------------------------------------
// The default constructor.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper()
{
	Clear();
}
//-------------------------------------------------------------------
// The general xloper and xloper12 type constructors.
// deep_copy defaults to false, i.e. makes shallow copies by default.
// This optimises the handling of xlopers and xloper12s, supplied
// by Excel in worksheet function calls, using this class.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const xloper *p_op, bool deep_copy)
{
	Clear();

	if(!p_op)	return;

	if(deep_copy || gExcelVersion12plus)
	{
// Copy the xloper and, if it is one of the types that has memory allocated,
// make deep copy and set m_DLLtoFree to true.  clone_xloper() returns true
// is this is the case otherwise false.
		if(gExcelVersion12plus)
			m_DLLtoFree12 = xloper_to_xloper12(&m_Op12, p_op);
		else
			m_DLLtoFree = clone_xloper(&m_Op, p_op);
	}
	else // Make a shallow copy
	{
// This constructor is designed to enable arguments passed in from the
// worksheet to be read easily without the overhead of deep copying.

// Need to ensure that the memory does not get freed twice so do not set
// flags.  Leaves responsibility with the calling code to clear up memory.
		m_Op = *p_op;
		m_Op.xltype &= ~(xlbitXLFree | xlbitDLLFree);
	}
	SetArraySize();
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const xloper12 *p_op, bool deep_copy)
{
	Clear();

	if(!p_op)	return;

	if(deep_copy || gExcelVersion11minus)
	{
// Copy the xloper and, if it is one of the types that has memory allocated,
// make deep copy and set m_DLLtoFree to true.  clone_xloper() returns true
// is this is the case otherwise false.
		if(gExcelVersion11minus) // Unlikely that we will be using xloper12s, but...
			m_DLLtoFree = xloper12_to_xloper(&m_Op, p_op);
		else
			m_DLLtoFree12 = clone_xloper(&m_Op12, p_op);
	}
	else // Make a shallow copy
	{
// This constructor is designed to enable arguments passed in from the
// worksheet to be read easily without the overhead of deep copying.

// Need to ensure that the memory does not get freed twice so do not set
// flags.  Leaves responsibility with the calling code to clear up memory.
		m_Op12 = *p_op;
		m_Op12.xltype &= ~(xlbitXLFree | xlbitDLLFree);
	}
	SetArraySize();
}
//-------------------------------------------------------------------
// The Variant constructor.  Takes type from the VARTYPE.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const VARIANT *pv)
{
	Clear();

	if(!pv)	return;

	if(gExcelVersion12plus)
	{
		if(vt_to_xloper(m_Op12, pv, true) && (m_Op12.xltype == xltypeStr || m_Op12.xltype == xltypeMulti))
			m_DLLtoFree12 = true;
	}
	else
	{
		if(vt_to_xloper(m_Op, pv, true) && (m_Op.xltype == xltypeStr || m_Op.xltype == xltypeMulti))
			m_DLLtoFree = true;
	}
	SetArraySize();
}
//-------------------------------------------------------------------
// The string constructors.  Assume null-terminated input.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const char *text)
{
	Clear();

	if(!text)	return;

	if(gExcelVersion12plus)
	{
		set_to_text(&m_Op12, text);
		m_DLLtoFree12 = true;
	}
	else
	{
		set_to_text(&m_Op, text);
		m_DLLtoFree = true;
	}
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const wchar_t *text)
{
	Clear();

	if(!text)	return;

	if(gExcelVersion12plus)
	{
		set_to_text(&m_Op12, text);
		m_DLLtoFree12 = true;
	}
	else
	{
		set_to_text(&m_Op, text);
		m_DLLtoFree = true;
	}
}
//-------------------------------------------------------------------
// xltypeInt constructors.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(int w)
{
	Clear();
	if(gExcelVersion12plus)
		set_to_int(&m_Op12, w);
	else
		set_to_int(&m_Op, w);
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(int w, int min, int max)
{
	Clear();
	if(w >= min && w <= max)
	{
		if(gExcelVersion12plus)
			set_to_int(&m_Op12, w);
		else
			set_to_int(&m_Op, w);
	}
}
//-------------------------------------------------------------------
// xltypeNum (double) constructor.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(double d)
{
	Clear();
	if(gExcelVersion12plus)
		set_to_double(&m_Op12, d);
	else
		set_to_double(&m_Op, d);
}
//-------------------------------------------------------------------
// xltypeBool constructor.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(bool b)
{
	Clear();
	if(gExcelVersion12plus)
		set_to_bool(&m_Op12, b);
	else
		set_to_bool(&m_Op, b);
}
//-------------------------------------------------------------------
// xltypeErr constructor.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(WORD e)
{
	Clear();
	if(gExcelVersion12plus)
		set_to_err(&m_Op12, e);
	else
		set_to_err(&m_Op, e);
}
//-------------------------------------------------------------------
// The xltypeSRef constructor.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW rwFirst, RW rwLast, COL colFirst, COL colLast)
{
	Clear();
	if(!RowColValid(rwFirst, colFirst) || !RowColValid(rwLast, colLast))
		return;

	if(gExcelVersion12plus)
		set_to_xltypeSRef(&m_Op12, rwFirst, rwLast, colFirst, colLast);
	else
		set_to_xltypeSRef(&m_Op, (WORD)rwFirst, (WORD)rwLast, (BYTE)colFirst, (BYTE)colLast);
}
//-------------------------------------------------------------------
// The xltypeRef constructors.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const char *sheet_name, RW rwFirst, RW rwLast, COL colFirst, COL colLast)
{
	Clear();

// Check the inputs.  No need to check sheet_name, as creation of
// cpp_xloper Name will set to xltypeNil if sheet_name not valid.
	if(rwFirst > rwLast || colFirst > colLast
	|| !RowColValid(rwFirst, colFirst) || !RowColValid(rwLast, colLast))
		return;

// Get the sheetID corresponding to the sheet name provided. If
// sheet_name was NULL, a reference on the active sheet is created.
	cpp_xloper Name(sheet_name);
	cpp_xloper RetOper;

	if(gExcelVersion12plus)
	{
		if(RetOper.Excel(xlSheetId, 1, &Name) == xlretSuccess)
		{
			if(set_to_xltypeRef(&m_Op12, RetOper.m_Op12.val.mref.idSheet, rwFirst, rwLast, colFirst, colLast))
				m_DLLtoFree12 = true; // created successfully
		}
	}
	else
	{
		if(RetOper.Excel(xlSheetId, 1, &Name) == xlretSuccess)
		{
			if(set_to_xltypeRef(&m_Op, RetOper.m_Op.val.mref.idSheet, rwFirst, rwLast, colFirst, colLast))
				m_DLLtoFree = true; // created successfully
		}
	}
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(DWORD SheetID, RW rwFirst, RW rwLast, COL colFirst, COL colLast)
{
	Clear();
	if(!RowColValid(rwFirst, colFirst) || RowColValid(rwLast, colLast))
		return;

	if(gExcelVersion12plus)
	{
		if(set_to_xltypeRef(&m_Op12, SheetID, rwFirst, rwLast, colFirst, colLast))
			m_DLLtoFree12 = true;
	}
	else
	{
		if(set_to_xltypeRef(&m_Op, SheetID, (WORD)rwFirst, (WORD)rwLast, (BYTE)colFirst, (BYTE)colLast))
			m_DLLtoFree = true;
	}
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const void *data, long len) // xltypeBigData
{
	Clear();

	if(!data)	return;

// No deep copying or memory management for this type
	if(data && len > 0)
	{
		if(gExcelVersion12plus)
		{
			m_Op12.xltype = xltypeBigData;
			m_Op12.val.bigdata.h.lpbData = (unsigned char *)data;
			m_Op12.val.bigdata.cbData = len;
		}
		else
		{
			m_Op.xltype = xltypeBigData;
			m_Op.val.bigdata.h.lpbData = (unsigned char *)data;
			m_Op.val.bigdata.cbData = len;
		}
	}
}
//-------------------------------------------------------------------
// The xltypeMulti constructors.
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW rows, COL cols)
{
	Clear();
	if(!RowColValid(rows, cols))
		return;

	DWORD i = rows * cols;

	if(gExcelVersion12plus)
	{
		xloper12 *p_oper;

		if(!set_to_xltypeMulti(&m_Op12, rows, cols)
		|| !(p_oper = m_Op12.val.array.lparray))
			return;

		while(i--)
			(p_oper++)->xltype = xltypeNil; // a safe default
		m_DLLtoFree12 = true;
	}
	else
	{
		xloper *p_oper;

		if(!set_to_xltypeMulti(&m_Op, rows, cols)
		|| !(p_oper = m_Op.val.array.lparray))
			return;

		while(i--)
			(p_oper++)->xltype = xltypeNil; // a safe default
		m_DLLtoFree = true;
	}
	SetArraySize();
}
//-------------------------------------------------------------------
void cpp_xloper::InitialiseArray(RW rows, COL cols, const double *init_data)
{
	Free();
	if(!RowColValid(rows, cols) || !init_data)
		return;

	DWORD i = rows * cols;

	if(gExcelVersion12plus)
	{
		xloper12 *p_oper;

		if(!init_data || !set_to_xltypeMulti(&m_Op12, rows, cols)
		|| !(p_oper = m_Op12.val.array.lparray))
			return;

		while(i--)
		{
			p_oper->xltype = xltypeNum;
			(p_oper++)->val.num = *init_data++;
		}
		m_DLLtoFree12 = true;
	}
	else
	{
		xloper *p_oper;

		if(!init_data || !set_to_xltypeMulti(&m_Op, rows, cols)
		|| !(p_oper = m_Op.val.array.lparray))
			return;

		while(i--)
		{
			p_oper->xltype = xltypeNum;
			(p_oper++)->val.num = *init_data++;
		}
		m_DLLtoFree = true;
	}
	SetArraySize();
}
//-------------------------------------------------------------------
void cpp_xloper::InitialiseArray(RW rows, COL cols, const cpp_xloper *init_array)
{
	Free();
	if(!RowColValid(rows, cols) || !init_array)
		return;
	
	const cpp_xloper *p_cpp_oper = init_array;
	DWORD type, i = rows * cols;

	if(gExcelVersion12plus)
	{
		xloper12 *p_oper;

		if(!set_to_xltypeMulti(&m_Op12, rows, cols)
		|| !(p_oper = m_Op12.val.array.lparray))
			return;

		while(i--)
		{
			type = p_cpp_oper->m_Op12.xltype;

// Check to see if the first xloper12 has been initialised.  If not,
// check the first xloper has been initialised, (not to an array).
			if(type == xltypeNil
			&& (p_cpp_oper->m_Op.xltype & (xltypeNil | xltypeMulti)) == 0)
			{
// Special case: Converts arrays of cpp_xlopers instantiated outside
// function code up to xloper12s. This is necessary when running under
// v12+ where instantiation could happen before a global version variable
// can be set, resulting in the xloper(s) being initialised instead of
// the xloper12s.
				xloper_to_xloper12(p_oper, &(p_cpp_oper->m_Op));
			}
			else if(type != xltypeMulti) // don't permit arrays of arrays
			{
// Make deep copies of strings and xltypeRef memory
				clone_xloper(p_oper, &(p_cpp_oper->m_Op12));
			}
			p_oper++;
			p_cpp_oper++;
		}
		m_DLLtoFree12 = true;
	}
	else // gExcelVersion < 12
	{
		xloper *p_oper;

		if(!set_to_xltypeMulti(&m_Op, rows, cols)
		|| !(p_oper = m_Op.val.array.lparray))
			return;

		while(i--)
		{
			type = p_cpp_oper->m_Op.xltype;

			if(type != xltypeMulti) // don't permit arrays of arrays
			{
// Make deep copies of strings and xltypeRef memory
				clone_xloper(p_oper, &(p_cpp_oper->m_Op));
			}
			p_oper++;
			p_cpp_oper++;
		}
		m_DLLtoFree = true;
	}
	SetArraySize();
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW rows, COL cols, const double *d_array)
{
	Clear();
	InitialiseArray(rows, cols, d_array);
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW rows, COL cols, const cpp_xloper *init_array)
{
	Clear();
	InitialiseArray(rows, cols, init_array);
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW rows, COL cols, char **str_array)
{
	Clear();
	if(!RowColValid(rows, cols) || !str_array)
		return;

	DWORD i = rows * cols;

	if(gExcelVersion12plus) // cast strings up to Unicode wide strings
	{
		xloper12 *p_oper;
		wchar_t *p;

		if(!set_to_xltypeMulti(&m_Op12, rows, cols)
		|| !(p_oper = m_Op12.val.array.lparray))
			return;

		while(i--)
		{
			p = new_xl12string(*str_array++); // make deep copies
			if(p)
			{
				p_oper->xltype = xltypeStr;
				p_oper->val.str = p;
			}
			else
			{
				p_oper->xltype = xltypeNil;
			}
			p_oper++;
		}
		m_DLLtoFree12 = true; // Class will free the strings too
	}
	else // strings are always ASCII byte strings like the inputs
	{
		xloper *p_oper;
		char *p;

		if(!set_to_xltypeMulti(&m_Op, rows, cols)
		|| !(p_oper = m_Op.val.array.lparray))
			return;

		while(i--)
		{
			p = new_xlstring(*str_array++); // make deep copies
			if(p)
			{
				p_oper->xltype = xltypeStr;
				p_oper->val.str = p;
			}
			else
			{
				p_oper->xltype = xltypeNil;
			}
			p_oper++;
		}
		m_DLLtoFree = true; // Class will free the strings too
	}
	SetArraySize();
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW rows, COL cols, wchar_t **str_array)
{
	Clear();
	if(!RowColValid(rows, cols) || !str_array)
		return;

	DWORD i = rows * cols;

	if(gExcelVersion12plus) // cast strings up to Unicode wide strings
	{
		xloper12 *p_oper;
		wchar_t *p;

		if(!set_to_xltypeMulti(&m_Op12, rows, cols)
		|| !(p_oper = m_Op12.val.array.lparray))
			return;

		while(i--)
		{
			p = new_xl12string(*str_array++); // make deep copies
			if(p)
			{
				p_oper->xltype = xltypeStr;
				p_oper->val.str = p;
			}
			else
			{
				p_oper->xltype = xltypeNil;
			}
			p_oper++;
		}
		m_DLLtoFree12 = true; // Class will free the strings too
	}
	else // strings are always ASCII byte strings like the inputs
	{
		xloper *p_oper;
		char *p;

		if(!set_to_xltypeMulti(&m_Op, rows, cols)
		|| !(p_oper = m_Op.val.array.lparray))
			return;

		while(i--)
		{
			p = new_xlstring(*str_array++); // make deep copies
			if(p)
			{
				p_oper->xltype = xltypeStr;
				p_oper->val.str = p;
			}
			else
			{
				p_oper->xltype = xltypeNil;
			}
			p_oper++;
		}
		m_DLLtoFree = true; // Class will free the strings too
	}
	SetArraySize();
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW &rows, COL &cols, const xloper12 *p_input_oper)
{
	Clear();

	if(gExcelVersion11minus)
	{
		// Cast down to an xloper
		xloper temp;
		xloper12_to_xloper(&temp, p_input_oper);

		if(!coerce_xloper(&temp, m_Op, xltypeMulti))
		{
			rows = cols = 0;
		}
		else
		{
			rows = m_Op.val.array.rows;
			cols = m_Op.val.array.columns;
// Ensure destructor will tell Excel to free memory
			m_XLtoFree = true;
		}
		free_xloper(&temp);
	}
	else
	{
// Ask Excel to convert the reference to an array (xltypeMulti)
		if(!coerce_xloper(p_input_oper, m_Op12, xltypeMulti))
		{
			rows = cols = 0;
		}
		else
		{
			rows = m_Op12.val.array.rows;
			cols = m_Op12.val.array.columns;
// Ensure destructor will tell Excel to free memory
			m_XLtoFree12 = true;
		}
	}
	SetArraySize();
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(RW &rows, COL &cols, const xloper *p_input_oper)
{
	Clear();

	if(gExcelVersion12plus)
	{
		// Cast up to an xloper12
		xloper12 temp;
		xloper_to_xloper12(&temp, p_input_oper);

		if(!coerce_xloper(&temp, m_Op12, xltypeMulti))
		{
			rows = cols = 0;
		}
		else
		{
			rows = m_Op12.val.array.rows;
			cols = m_Op12.val.array.columns;
// Ensure destructor will tell Excel to free memory
			m_XLtoFree12 = true;
		}
		free_xloper(&temp);
	}
	else
	{
// Ask Excel to convert the reference to an array (xltypeMulti)
		if(!coerce_xloper(p_input_oper, m_Op, xltypeMulti))
		{
			rows = cols = 0;
		}
		else
		{
			rows = m_Op.val.array.rows;
			cols = m_Op.val.array.columns;
// Ensure destructor will tell Excel to free memory
			m_XLtoFree = true;
		}
	}
	SetArraySize();
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const xl4_array *array)
{
	Clear();
	InitialiseArray(array->rows, array->columns, array->array);
}
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const xl12_array *array)
{
	Clear();
	InitialiseArray(array->rows, array->columns, array->array);
}
//-------------------------------------------------------------------
// the copy constructor
//-------------------------------------------------------------------
cpp_xloper::cpp_xloper(const cpp_xloper &source)
{
	if(&source == this)
		return;

	Clear();
	if(gExcelVersion12plus)
		m_DLLtoFree12 = clone_xloper(&m_Op12, &(source.m_Op12)); // make a deep copy
	else
		m_DLLtoFree = clone_xloper(&m_Op, &(source.m_Op)); // make a deep copy
	SetArraySize();
}
//-------------------------------------------------------------------
// The destructor.
//-------------------------------------------------------------------
cpp_xloper::~cpp_xloper()
{
	Free();
}
//-------------------------------------------------------------------
cpp_xloper & cpp_xloper::operator=(const cpp_xloper &source)
{
	if(&source == this)
		return *this;

	Clear();
	if(gExcelVersion12plus)
		m_DLLtoFree12 = clone_xloper(&m_Op12, &(source.m_Op12)); // make a deep copy
	else
		m_DLLtoFree = clone_xloper(&m_Op, &(source.m_Op)); // make a deep copy

	SetArraySize();
	return *this;
}
//-------------------------------------------------------------------
int cpp_xloper::operator=(int w)
{
	Free();
	if(gExcelVersion12plus)
		set_to_int(&m_Op12, w);
	else
		set_to_int(&m_Op, w);

	return w;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator=(bool b)
{
	Free();
	if(gExcelVersion12plus)
		set_to_bool(&m_Op12, b);
	else
		set_to_bool(&m_Op, b);

	return b;
}
//-------------------------------------------------------------------
double cpp_xloper::operator=(double d)
{
	Free();
	if(gExcelVersion12plus)
		set_to_double(&m_Op12, d);
	else
		set_to_double(&m_Op, d);

	return d;
}
//-------------------------------------------------------------------
WORD cpp_xloper::operator=(WORD err_code)
{
	Free();
	if(gExcelVersion12plus)
		set_to_err(&m_Op12, err_code);
	else
		set_to_err(&m_Op, err_code);

	return err_code;
}
//-------------------------------------------------------------------
void cpp_xloper::SetToError(int err_code)
{
	Free();
	if(gExcelVersion12plus)
		set_to_err(&m_Op12, err_code);
	else
		set_to_err(&m_Op, err_code);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetToCallerValue(void)
{
	Free();

	if(gExcelVersion11minus)
	{
// Get a reference to the calling cell(s)
		xloper caller;
		if(Excel4(xlfCaller, &caller, 0) != xlretSuccess)
			return false;

		if(!(caller.xltype & (xltypeRef | xltypeSRef)))
		{
			Excel4(xlFree, 0, 1, &caller);
			return false;
		}

// Get the calling cell's value
		if(Excel4(xlCoerce, &m_Op, 1, &caller) != xlretSuccess)
		{
			Excel4(xlFree, 0, 1, &caller);
			return false;
		}
		SetArraySize();
		return m_XLtoFree = true;
	}
	else
	{
// Get a reference to the calling cell(s)
		xloper12 caller;
		if(Excel12(xlfCaller, &caller, 0) != xlretSuccess)
			return false;

		if(!(caller.xltype & (xltypeRef | xltypeSRef)))
		{
			Excel12(xlFree, 0, 1, &caller);
			return false;
		}

// Get the calling cell's value
		if(Excel12(xlCoerce, &m_Op12, 1, &caller) != xlretSuccess)
		{
			Excel12(xlFree, 0, 1, &caller);
			return false;
		}
		SetArraySize();
		return m_XLtoFree12 = true;
	}
}
//-------------------------------------------------------------------
const char *cpp_xloper::operator=(const char *text)
{
	Free();
	if(gExcelVersion12plus)
	{
		set_to_text(&m_Op12, text);
		m_DLLtoFree12 = true;
	}
	else
	{
		set_to_text(&m_Op, text);
		m_DLLtoFree = true;
	}
	return text;
}
//-------------------------------------------------------------------
const xloper *cpp_xloper::operator=(const xloper *p_op)
{
	Free();

// Copy the xloper and, if it is one of the types that has memory allocated,
// make deep copy and set m_DLLtoFree to true.  clone_xloper() returns true
// if this is the case, otherwise false.
	if(gExcelVersion12plus)
	{
		// cast up to xloper12
		xloper12 temp;
		xloper_to_xloper12(&temp, p_op);
		m_DLLtoFree12 = clone_xloper(&m_Op12, &temp);
		free_xloper(&temp);
	}
	else
		m_DLLtoFree = clone_xloper(&m_Op, p_op);

	SetArraySize();
	return p_op;
}
//-------------------------------------------------------------------
const xloper12 *cpp_xloper::operator=(const xloper12 *p_op)
{
	Free();

// Copy the xloper and, if it is one of the types that has memory allocated,
// make deep copy and set m_DLLtoFree to true.  clone_xloper() returns true
// if this is the case, otherwise false.
	if(gExcelVersion12plus)
		m_DLLtoFree12 = clone_xloper(&m_Op12, p_op);
	else
	{
		// cast down to xloper
		xloper temp;
		xloper12_to_xloper(&temp, p_op);
		m_DLLtoFree = clone_xloper(&m_Op, &temp);
		free_xloper(&temp);
	}
	SetArraySize();
	return p_op;
}
//-------------------------------------------------------------------
const VARIANT *cpp_xloper::operator=(const VARIANT *pv)
{
	Free();

	if(gExcelVersion12plus)
	{
		if(vt_to_xloper(m_Op12, pv, false) && m_Op12.xltype == xltypeStr)
			m_DLLtoFree12 = true;
	}
	else
	{
		if(vt_to_xloper(m_Op, pv, false) && m_Op.xltype == xltypeStr)
			m_DLLtoFree = true;
	}
	SetArraySize();
	return pv;
}
//-------------------------------------------------------------------
const xl4_array *cpp_xloper::operator=(const xl4_array *array)
{
	InitialiseArray(array->rows, array->columns, array->array);
	return array;
}
//-------------------------------------------------------------------
const xl12_array *cpp_xloper::operator=(const xl12_array *array)
{
	InitialiseArray(array->rows, array->columns, array->array);
	return array;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(const cpp_xloper &cpp_op2)
{
	if(gExcelVersion12plus)
		return xlopers_equal(m_Op12, cpp_op2.m_Op12);
	else
		return xlopers_equal(m_Op, cpp_op2.m_Op);
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(const xloper *p_op)
{
	if(gExcelVersion12plus)
	{
		// cast up to xloper12
		xloper12 temp;
		xloper_to_xloper12(&temp, p_op);
		bool ret_val = xlopers_equal(m_Op12, temp);
		free_xloper(&temp);
		return ret_val;
	}
	else
		return xlopers_equal(m_Op, *p_op);
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(const xloper12 *p_op)
{
	if(gExcelVersion12plus)
		return xlopers_equal(m_Op12, *p_op);
	else
	{
		// cast down to xloper
		xloper temp;
		xloper12_to_xloper(&temp, p_op);
		bool ret_val = xlopers_equal(m_Op, temp);
		free_xloper(&temp);
		return ret_val;
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(int w)
{
	int i;
	if(gExcelVersion12plus)
		return coerce_to_int(&m_Op12, i) && i == w;
	else
		return coerce_to_int(&m_Op, i) && i == w;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(bool b)
{
	bool a;
	if(gExcelVersion12plus)
		return coerce_to_bool(&m_Op12, a) && a == b;
	else
		return coerce_to_bool(&m_Op, a) && a == b;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(double d)
{
	double dd;
	if(gExcelVersion12plus)
		return coerce_to_double(&m_Op12, dd) && dd == d;
	else
		return coerce_to_double(&m_Op, dd) && dd == d;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(WORD e)
{
	int i;
	if(gExcelVersion12plus)
		return coerce_to_int(&m_Op12, i) && (WORD)i != e;
	else
		return coerce_to_int(&m_Op, i) && (WORD)i != e;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(const char *text)
{
	if(gExcelVersion12plus) // need to convert byte string before making comparison
	{
		if(m_Op12.xltype != xltypeStr)
			return false;

		wchar_t *t = new_xl12string(text);

		size_t i;
		wchar_t *p = m_Op12.val.str + 1;

		for(i = m_Op12.val.str[0]; i--;)
			if(*p++ != *t++)
				return false;

		if(*text)
			return false; // text is longer than m_Op.val.str
	}
	else
	{
		if(m_Op.xltype != xltypeStr)
			return false;

		BYTE i;
		char *p = m_Op.val.str + 1;

		for(i = (BYTE)m_Op.val.str[0]; i--;)
			if(*p++ != *text++)
				return false;

		if(*text)
			return false; // text is longer than m_Op.val.str
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::operator==(const wchar_t *text)
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype != xltypeStr)
			return false;

		size_t i;
		wchar_t *p = m_Op12.val.str + 1;

		for(i = m_Op12.val.str[0]; i--;)
			if(*p++ != *text++)
				return false;

		if(*text)
			return false; // text is longer than m_Op.val.str
	}
	else // need to convert to byte string before making comparison
	{
		if(m_Op.xltype != xltypeStr)
			return false;

		char *t = new_xlstring(text);
		
		BYTE i;
		char *p = m_Op.val.str + 1;

		for(i = (BYTE)m_Op.val.str[0]; i--;)
			if(*p++ != *t++)
				return false;

		if(*text)
			return false; // text is longer than m_Op.val.str
	}
	return true;
}
//-------------------------------------------------------------------
void cpp_xloper::operator+=(double d)
{
	if(gExcelVersion12plus)
	{
		switch(m_Op12.xltype)
		{
		case xltypeNum:		m_Op12.val.num += d;		break;
		case xltypeInt:		m_Op12.val.w += (int)d;	break;

		case xltypeSRef: // Only during commands
		case xltypeRef: // Only during commands
			xloper12 value, value_type;
			value_type.xltype = xltypeInt;
			value_type.val.w = xltypeNum;

			if(Excel12(xlCoerce, &value, 2, &m_Op12, &value_type) || value.xltype != xltypeNum)
				break;

			value.val.num += d;
			Excel12(xlSet, 0, 2, &m_Op12, &value);
			break;
		}
	}
	else
	{
		switch(m_Op.xltype)
		{
		case xltypeNum:		m_Op.val.num += d;		break;
		case xltypeInt:		m_Op.val.w += (int)d;	break;

		case xltypeSRef: // Only during commands
		case xltypeRef: // Only during commands
			xloper value, value_type;
			value_type.xltype = xltypeInt;
			value_type.val.w = xltypeNum;

			if(Excel4(xlCoerce, &value, 2, &m_Op, &value_type) || value.xltype != xltypeNum)
				break;

			value.val.num += d;
			Excel4(xlSet, 0, 2, &m_Op, &value);
			break;
		}
	}
}
//-------------------------------------------------------------------
double cpp_xloper::operator*=(double factor) // Coerce to double and multiply
{
	if(gExcelVersion12plus)
	{
		double num = 0.0;
		if(m_Op12.xltype != xltypeNum)
		{
			coerce_to_double(&m_Op12, num);
			Free();
			m_Op12.xltype = xltypeNum;
		}
		else
		{
			num = m_Op12.val.num;
		}
		return m_Op12.val.num = num * factor;
	}
	else
	{
		double num = 0.0;
		if(m_Op.xltype != xltypeNum)
		{
			coerce_to_double(&m_Op, num);
			Free();
			m_Op.xltype = xltypeNum;
		}
		else
		{
			num = m_Op.val.num;
		}
		return m_Op.val.num = num * factor;
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::Concat(const cpp_xloper &op) // coersion to strings and concatentation
{
	if(!ConvertToString())
		return false;

	operator+=(op);
	return true;
}
//-------------------------------------------------------------------
size_t cpp_xloper::Len(void) const // returns 0 if not a string
{
	if(gExcelVersion12plus)
	{
		return (size_t)(m_Op12.xltype == xltypeStr ? m_Op12.val.str[0] : 0);
	}
	else
	{
		return (size_t)(m_Op.xltype == xltypeStr ? (BYTE)m_Op.val.str[0] : 0);
	}
}
//-------------------------------------------------------------------
wchar_t cpp_xloper::First(void) const // get the first char or 0 if not a string
{
	if(gExcelVersion12plus)
	{
		return (wchar_t)(m_Op12.xltype == xltypeStr && m_Op12.val.str[0] ? m_Op12.val.str[1] : 0);
	}
	else
	{
		return (wchar_t)(m_Op.xltype == xltypeStr && m_Op.val.str[0] ? m_Op.val.str[1] : 0);
	}
}
//-------------------------------------------------------------------
wchar_t cpp_xloper::Mid(int posn) const // get nth char, counting from 1, or 0 if not a string
{
	if(gExcelVersion12plus)
	{
		return (wchar_t)(m_Op12.xltype == xltypeStr && posn > 0 && posn < m_Op12.val.str[0] ?
			m_Op12.val.str[posn] : 0);
	}
	else
	{
		return (wchar_t)(m_Op.xltype == xltypeStr && posn > 0 && posn < (BYTE)m_Op.val.str[0] ?
			m_Op.val.str[posn] : 0);
	}
}
//-------------------------------------------------------------------
// xloper string concatenation: if running in Excel v12+ casts the
// byte-string up to a Unicode wchar string and appends, otherwise
// simply adds to byte-string (length limits permitting)
void cpp_xloper::operator+=(const char *p)
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype != xltypeStr)
		{
			xloper12 temp;
			if(!coerce_xloper(&m_Op12, temp, xltypeStr))
				return; // No effect

			Free(); // release the old resources
			m_Op12 = temp;
			m_XLtoFree12 = true;
		}

		wchar_t *p_new = append_xl12string(m_Op12.val.str, p);

		if(!p_new)
			return;

		Free(); // release the old resources
		m_Op12.xltype = xltypeStr;
		m_Op12.val.str = p_new;
		m_DLLtoFree12 = true;
	}
	else
	{
		if(m_Op.xltype != xltypeStr)
		{
			xloper temp;
			if(!coerce_xloper(&m_Op, temp, xltypeStr))
				return; // No effect

			Free(); // release the old resources
			m_Op = temp;
			m_XLtoFree = true;
		}

		char *p_new = append_xlstring(m_Op.val.str, p);

		if(!p_new)
			return;

		Free(); // release the old resources
		m_Op.xltype = xltypeStr;
		m_Op.val.str = p_new;
		m_DLLtoFree = true;
	}
}
//-------------------------------------------------------------------
void cpp_xloper::operator+=(const wchar_t *p)
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype != xltypeStr)
		{
			xloper12 temp;
			if(!coerce_xloper(&m_Op12, temp, xltypeStr))
				return; // No effect

			Free(); // release the old resources
			m_Op12 = temp;
			m_XLtoFree12 = true;
		}

		wchar_t *p_new = append_xl12string(m_Op12.val.str, p);

		if(!p_new)
			return;

		Free(); // release the old resources
		m_Op12.xltype = xltypeStr;
		m_Op12.val.str = p_new;
		m_DLLtoFree12 = true;
	}
	else
	{
		if(m_Op.xltype != xltypeStr)
		{
			xloper temp;
			if(!coerce_xloper(&m_Op, temp, xltypeStr))
				return; // No effect

			Free(); // release the old resources
			m_Op = temp;
			m_XLtoFree = true;
		}

		char *p_new = append_xlstring(m_Op.val.str, p);

		if(!p_new)
			return;

		Free(); // release the old resources
		m_Op.xltype = xltypeStr;
		m_Op.val.str = p_new;
		m_DLLtoFree = true;
	}
}
//-------------------------------------------------------------------
// If this type is numeric, coerces Op to double and adds. If this
// type is a string, coerces Op to string and concatenates. Else
// does nothing.
void cpp_xloper::operator+=(const cpp_xloper &Op)
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeStr)
		{
			wchar_t *p_new = NULL;

			if(Op.IsStr())
			{
				p_new = concat_xlstrings(m_Op12.val.str, Op.m_Op12.val.str);
			}
			else
			{
				xloper12 temp;
				if(!coerce_xloper(&Op.m_Op12, temp, xltypeStr))
					return; // No effect
				p_new = concat_xlstrings(m_Op12.val.str, temp.val.str);
				Excel12(xlFree, 0, 1, &temp);
			}

			if(p_new)
			{
				Free(); // release the old resources
				m_Op12.xltype = xltypeStr;
				m_Op12.val.str = p_new;
				m_DLLtoFree12 = true;
			}
			return;
		}

		if(m_Op12.xltype == xltypeInt)
		{
			m_Op12.val.num = (double)m_Op12.val.w;
			m_Op12.xltype = xltypeNum;
		}

		if(m_Op12.xltype == xltypeNum)
		{
			if(Op.IsNum())
			{
				m_Op12.val.num += Op.m_Op12.val.num;
			}
			else if(Op.IsType(xltypeInt))
			{
				m_Op12.val.num += Op.m_Op12.val.w;
			}
			else
			{
				xloper12 temp;
				if(!coerce_xloper(&Op.m_Op12, temp, xltypeNum))
					return; // No effect
				m_Op12.val.num += temp.val.num;
			}
		}
	}
	else
	{
		if(m_Op.xltype == xltypeStr)
		{
			char *p_new = NULL;

			if(Op.IsStr())
			{
				p_new = concat_xlstrings(m_Op.val.str, Op.m_Op.val.str);
			}
			else
			{
				xloper temp;
				if(!coerce_xloper(&Op.m_Op, temp, xltypeStr))
					return; // No effect
				p_new = concat_xlstrings(m_Op.val.str, temp.val.str);
				Excel4(xlFree, 0, 1, &temp);
			}

			if(p_new)
			{
				Free(); // release the old resources
				m_Op.xltype = xltypeStr;
				m_Op.val.str = p_new;
				m_DLLtoFree = true;
			}
			return;
		}

		if(m_Op.xltype == xltypeInt)
		{
			m_Op.val.num = (double)m_Op.val.w;
			m_Op.xltype = xltypeNum;
		}

		if(m_Op.xltype == xltypeNum)
		{
			if(Op.IsNum())
			{
				m_Op.val.num += Op.m_Op.val.num;
			}
			else if(Op.IsType(xltypeInt))
			{
				m_Op.val.num += Op.m_Op.val.w;
			}
			else
			{
				xloper temp;
				if(!coerce_xloper(&Op.m_Op, temp, xltypeNum))
					return; // No effect
				m_Op.val.num += temp.val.num;
			}
		}
	}
}
//-------------------------------------------------------------------
// Returns the type of the xloper
//-------------------------------------------------------------------
int cpp_xloper::GetType(void) const
{
	return (gExcelVersion12plus ? m_Op12.xltype : m_Op.xltype);
}
//-------------------------------------------------------------------
void cpp_xloper::SetType(int new_type)
{
	Free();
	if(gExcelVersion12plus)
		set_to_type(&m_Op12, new_type);
	else
		set_to_type(&m_Op, new_type);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetTypeMulti(RW array_rows, COL array_cols)
{
	int array_size = array_rows * array_cols;

	if(array_size <= 0)
		return false;

	if(gExcelVersion12plus)
	{
		if(array_cols > MAX_XL12_COLS || array_rows > MAX_XL12_ROWS)
			return false;

		Free();
		set_to_type(&m_Op12, xltypeMulti);
		m_Op12.val.array.lparray = (xloper12 *)calloc(XLOPER12_SIZE, array_size);
		m_Op12.val.array.columns = array_cols;
		m_Op12.val.array.rows = array_rows;

		xloper12 *p = m_Op12.val.array.lparray;

		for(;array_size--; p++)
			p->xltype = xltypeNil; // equivalent to an empty cell

		m_DLLtoFree12 = true;
	}
	else
	{
		if(array_cols > MAX_XL11_COLS || array_rows > MAX_XL11_ROWS)
			return false;

		Free();
		set_to_type(&m_Op, xltypeMulti);
		m_Op.val.array.lparray = (xloper *)calloc(XLOPER_SIZE, array_size);
		m_Op.val.array.columns = array_cols;
		m_Op.val.array.rows = array_rows;

		xloper *p = m_Op.val.array.lparray;

		for(;array_size--; p++)
			p->xltype = xltypeNil; // equivalent to an empty cell

		m_DLLtoFree = true;
	}
	SetArraySize();
	return true;
}
//-------------------------------------------------------------------
// Change the type to an xltypeSref referencing the given cells
//-------------------------------------------------------------------
bool cpp_xloper::SetCell(RW rwFirst, RW rwLast, COL colFirst, COL colLast)
{
	Free();
	if(!RowColValid(rwFirst, colFirst) || !RowColValid(rwLast, colLast))
		return false;

	if(gExcelVersion12plus)
		return set_to_xltypeSRef(&m_Op12, rwFirst, rwLast, colFirst, colLast);
	else
		return set_to_xltypeSRef(&m_Op, (WORD)rwFirst, (WORD)rwLast, (BYTE)colFirst, (BYTE)colLast);
}
//-------------------------------------------------------------------
// Change the type and assign the given value
//-------------------------------------------------------------------
cpp_xloper::operator int(void) const
{
	int i;

	if(gExcelVersion12plus)
		return coerce_to_int(&m_Op12, i) ? i : 0;
	else
		return coerce_to_int(&m_Op, i) ? i : 0;
}
//-------------------------------------------------------------------
cpp_xloper::operator bool(void) const
{
	bool b;

	if(gExcelVersion12plus)
		return coerce_to_bool(&m_Op12, b) ? b : false;
	else
		return coerce_to_bool(&m_Op, b) ? b : false;
}
//-------------------------------------------------------------------
cpp_xloper::operator double(void) const
{
	double d;

	if(gExcelVersion12plus)
		return coerce_to_double(&m_Op12, d) ? d : 0.0;
	else
		return coerce_to_double(&m_Op, d) ? d : 0.0;
}
//-------------------------------------------------------------------
cpp_xloper::operator char *(void) const
{
	char *p;

	if(gExcelVersion12plus)
		return coerce_to_string(&m_Op12, p) ? p : NULL;
	else
		return coerce_to_string(&m_Op, p) ? p : NULL;
}
//-------------------------------------------------------------------
cpp_xloper::operator wchar_t *(void) const
{
	wchar_t *p;

	if(gExcelVersion12plus)
		return coerce_to_string(&m_Op12, p) ? p : NULL;
	else
		return coerce_to_string(&m_Op, p) ? p : NULL;
}
//-------------------------------------------------------------------
cpp_xloper::operator xloper(void) // get a shallow copy
{
	if(gExcelVersion12plus)
	{
		FreeOp();
		xloper12_to_xloper(&m_Op, &m_Op12);
		m_DLLtoFree = true;
	}
	return m_Op; // return a shallow copy on the stack
}
//-------------------------------------------------------------------
cpp_xloper::operator xloper12(void) // get a shallow copy
{
	if(gExcelVersion11minus)
	{
		FreeOp12();
		xloper_to_xloper12(&m_Op12, &m_Op);
		m_DLLtoFree12 = true;
	}
	return m_Op12; // return a shallow copy on the stack
}
//-------------------------------------------------------------------
bool cpp_xloper::AsVariant(VARIANT &var) const
{
	if(gExcelVersion12plus)
		return xloper_to_vt(&m_Op12, var, true);
	else
		return xloper_to_vt(&m_Op, var, true);
}
//-------------------------------------------------------------------
xl4_array *cpp_xloper::AsDblArray(void) const
{
	xl4_array *p_ret_array;
	double *p;
	int size;

	if(gExcelVersion12plus)
	{
		xloper12 *p_op;

		switch(m_Op12.xltype)
		{
		case xltypeMulti:
// Need to check that the xltypeMulti does not bust xl4_array limits:
// Assumption is that xl4_array supports all Excel 12 columns.
// Rows and columns count from 1 and so an xl4_array cannot represent
// a whole column so the test is >= MAX_XL11_ROWS.
// For the columns, MAX_XL12_COLS is less that then DWORD limit.
			if(m_Op12.val.array.rows >= MAX_XL11_ROWS
			|| m_Op12.val.array.columns > MAX_XL12_COLS)
				return NULL; // fail rather than truncate

			size = m_Op12.val.array.rows * m_Op12.val.array.columns;
			p_ret_array = (xl4_array *)malloc(sizeof(xl4_array) + sizeof(double) * (size - 1));
			p_ret_array->columns = m_Op12.val.array.columns;
			p_ret_array->rows = m_Op12.val.array.rows;

			if(!p_ret_array)
				return NULL;

// Get the cell values one-by-one as doubles and place in the array.
// Store the array row-by-row in memory.
			if(!(p_op = m_Op12.val.array.lparray))
			{
				free(p_ret_array);
				return NULL;
			}

			for(p = p_ret_array->array; size--; p++)
				if(!coerce_to_double(p_op++, *p))
					*p = 0.0;
			return p_ret_array;

		case xltypeNum:
		case xltypeStr:
		case xltypeBool:
			p_ret_array = (xl4_array *)malloc(sizeof(xl4_array));
			p_ret_array->columns = p_ret_array->rows = 1;
			if(!coerce_to_double(&m_Op12, p_ret_array->array[0]))
				p_ret_array->array[0] = 0.0;
			return p_ret_array;
		}
	}
	else
	{
		xloper *p_op;

		switch(m_Op.xltype)
		{
		case xltypeMulti:
			size = m_Op.val.array.rows * m_Op.val.array.columns;
			p_ret_array = (xl4_array *)malloc(sizeof(xl4_array) + sizeof(double) * (size - 1));
			p_ret_array->columns = m_Op.val.array.columns;
			p_ret_array->rows = m_Op.val.array.rows;

			if(!p_ret_array)
				return NULL;

// Get the cell values one-by-one as doubles and place in the array.
// Store the array row-by-row in memory.
			if(!(p_op = m_Op.val.array.lparray))
			{
				free(p_ret_array);
				return NULL;
			}

			for(p = p_ret_array->array; size--; p++)
				if(!coerce_to_double(p_op++, *p))
					*p = 0.0;
			return p_ret_array;

		case xltypeNum:
		case xltypeStr:
		case xltypeBool:
			p_ret_array = (xl4_array *)malloc(sizeof(xl4_array));
			p_ret_array->columns = p_ret_array->rows = 1;
			if(!coerce_to_double(&m_Op, p_ret_array->array[0]))
				p_ret_array->array[0] = 0.0;
			return p_ret_array;
		}
	}

	return NULL;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetErrVal(WORD &e) const
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeErr)
		{
			e = m_Op12.val.err;
			return true;
		}
	}
	else
	{
		if(m_Op.xltype == xltypeErr)
		{
			e = m_Op.val.err;
			return true;
		}
	}
	e = 0;
	return false;
}
//-------------------------------------------------------------------
int cpp_xloper::GetArrayEltType(RW row, COL col) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, col, offset))
		return 0;

	return GetArrayEltType(offset);
}
//-------------------------------------------------------------------
inline void cpp_xloper::SetArraySize(void)
{
	if(gExcelVersion12plus)
	{
		m_ArraySize = m_Op12.xltype == xltypeMulti ? m_Op12.val.array.columns * m_Op12.val.array.rows : 0;
	}
	else
	{
		m_ArraySize = m_Op.xltype == xltypeMulti ? m_Op.val.array.columns * m_Op.val.array.rows : 0;
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArraySize(RW &rows, COL &cols) const
{
	if(!m_ArraySize)
		return false;

	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype != xltypeMulti)
		{
			rows = cols = 0;
			return false;
		}
		rows = m_Op12.val.array.rows;
		cols = m_Op12.val.array.columns;
	}
	else
	{
		if(m_Op.xltype != xltypeMulti)
		{
			rows = cols = 0;
			return false;
		}
		rows = m_Op.val.array.rows;
		cols = m_Op.val.array.columns;
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(DWORD offset, const char *p) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op_this;
		if(GetArrayElt(offset, p_op_this))
			return xloper_equals_text(*p_op_this, p);
	}
	else
	{
		xloper *p_op_this;
		if(GetArrayElt(offset, p_op_this))
			return xloper_equals_text(*p_op_this, p);
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(DWORD offset, const xloper *p_op) const
{
	if(gExcelVersion12plus)
	{
// Cast the xloper up to an xloper 12. Need to free memory
// once the temp xloper12 is no longer needed
		xloper12 *p_op_this;
		xloper12 temp;
		xloper_to_xloper12(&temp, p_op);
		bool ret_val = false;

		if(GetArrayElt(offset, p_op_this))
			ret_val = xlopers_equal(*p_op_this, temp);

		free_xloper(&temp);
		return ret_val;
	}
	else
	{
		xloper *p_op_this;
		if(GetArrayElt(offset, p_op_this))
			return xlopers_equal(*p_op_this, *p_op);
		return false;
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(DWORD offset, const xloper12 *p_op) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op_this;
		if(GetArrayElt(offset, p_op_this))
			return xlopers_equal(*p_op_this, *p_op);
		return false;
	}
	else
	{
// Cast the xloper12 down to an xloper. Need to free memory
// once the temp xloper is no longer needed
		xloper *p_op_this;
		xloper temp;
		xloper12_to_xloper(&temp, p_op);
		bool ret_val = false;

		if(GetArrayElt(offset, p_op_this))
			ret_val = xlopers_equal(*p_op_this, temp);

		free_xloper(&temp);
		return ret_val;
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(DWORD offset, const cpp_xloper &Op) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op_this;
		if(GetArrayElt(offset, p_op_this))
			return xlopers_equal(*p_op_this, Op.m_Op12);
	}
	else
	{
		xloper *p_op_this;
		if(GetArrayElt(offset, p_op_this))
			return xlopers_equal(*p_op_this, Op.m_Op);
	}
	return false;
}
//-------------------------------------------------------------------
// Array access functions with offset parameter
//-------------------------------------------------------------------
int cpp_xloper::GetArrayEltType(DWORD offset) const
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeMulti && offset < m_ArraySize)
			return m_Op12.val.array.lparray[offset].xltype;
	}
	else
	{
		if(m_Op.xltype == xltypeMulti && offset < m_ArraySize)
			return m_Op.val.array.lparray[offset].xltype;
	}
	return 0;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArraySize(DWORD &offset) const
{
	offset = m_ArraySize;
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeMulti)
			return true;
	}
	else
	{
		if(m_Op.xltype == xltypeMulti)
			return true;
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, xloper *&p_op) const // get pointer only
{
	if(gExcelVersion12plus // Don't allow mixing of xlopers and xloper12s in this case
	||  offset >= m_ArraySize)
		return false;

	p_op = m_Op.val.array.lparray + offset;
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, xloper12 *&p_op) const // get pointer only
{
	if(gExcelVersion11minus // Don't allow mixing of xlopers and xloper12s in this case
	||  offset >= m_ArraySize)
		return false;

	p_op = m_Op12.val.array.lparray + offset;
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, VARIANT &vt) const // get deep copy
{
	if(offset >= m_ArraySize)
		return false;

	VariantClear(&vt);
	if(gExcelVersion12plus)
	{
		return xloper_to_vt(m_Op12.val.array.lparray + offset, vt, false);
	}
	else
	{
		return xloper_to_vt(m_Op.val.array.lparray + offset, vt, false);
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, cpp_xloper &op) const // makes deep copy
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			op = p_op; // overloaded operator makes a deep copy
			return true;
		}
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			op = p_op; // overloaded operator makes a deep copy
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, int &w) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_int(p_op, w);
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_int(p_op, w);
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, bool &b) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_bool(p_op, b);
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_bool(p_op, b);
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, double &d) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_double(p_op, d);
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_double(p_op, d);
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, WORD &e) const
{
	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op) && p_op->xltype == xltypeErr)
		{
			e = p_op->val.err;
			return true;
		}
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op) && p_op->xltype == xltypeErr)
		{
			e = p_op->val.err;
			return true;
		}
	}
	e = 0;
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, char *&text) const
{
	text = 0;

	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_string(p_op, text);
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_string(p_op, text);
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(DWORD offset, wchar_t *&text) const
{
	text = 0;

	if(gExcelVersion12plus)
	{
		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_string(p_op, text);
	}
	else
	{
		xloper *p_op;
		if(GetArrayElt(offset, p_op))
			return coerce_to_string(p_op, text);
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, int w)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_int(p_op, w);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_int(p_op, w);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, bool b)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_bool(p_op, b);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_bool(p_op, b);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, double d)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_double(p_op, d);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_double(p_op, d);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, WORD e)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_err(p_op, e);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_err(p_op, e);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, const char *text)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_text(p_op, text);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_text(p_op, text);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, const wchar_t *text)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_text(p_op, text);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_text(p_op, text);
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayEltType(DWORD offset, int new_type)
{
// Should not assign to an Excel-allocated array
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;
	}
	else
	{
		if(!m_DLLtoFree)
			return false;
	}

	switch(new_type)
	{
	case xltypeNum:
	case xltypeStr:
	case xltypeBool:
	case xltypeErr:
	case xltypeMissing:
	case xltypeNil:
	case xltypeInt:
		break;

	default:
		return false; // nothing else permitted for arrays
	}

	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;

		xloper12 *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_type(p_op, new_type);
			return true;
		}
	}
	else
	{
		if(!m_DLLtoFree)
			return false;

		xloper *p_op;
		if(GetArrayElt(offset, p_op))
		{
			free_xloper(p_op);
			set_to_type(p_op, new_type);
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, const xloper *p_source)
{
	if(gExcelVersion12plus)
	{
		xloper12 source12;

		if(!xloper_to_xloper12(&source12, p_source))
			return false;

		bool ret_val = SetArrayElt(offset, &source12);
		free_xloper(&source12);
		return ret_val;
	}

	if(!m_DLLtoFree || !p_source)
		return false;  // Should not assign to an Excel-allocated array

	xloper *p_op;
	if(!GetArrayElt(offset, p_op))
		return false;

	free_xloper(p_op);

	if(p_source->xltype == xltypeMulti)
	{
// can't assign array to an array element so just assign the
// top left value if array has memory assigned
		if(p_source->val.array.lparray)
			clone_xloper(p_op, p_source->val.array.lparray);
		else
			p_op->xltype = xltypeNil;
	}
	else
	{
// Don't set or clear m_DLLtoFree as this is just one element in
// an array which will already have m_DLLtoFree == true
		clone_xloper(p_op, p_source); // make a deep copy
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, const xloper12 *p_source)
{
	if(gExcelVersion11minus)
	{
		xloper source;

		if(!xloper12_to_xloper(&source, p_source))
			return false;

		bool ret_val = SetArrayElt(offset, &source);
		free_xloper(&source);
		return ret_val;
	}

	if(!m_DLLtoFree12 || !p_source)
		return false;  // Should not assign to an Excel-allocated array

	xloper12 *p_op;
	if(!GetArrayElt(offset, p_op))
		return false;

	free_xloper(p_op);

	if(p_source->xltype == xltypeMulti)
	{
// can't assign array to an array element so just assign the
// top left value if array has memory assigned
		if(p_source->val.array.lparray)
			clone_xloper(p_op, p_source->val.array.lparray);
		else
			p_op->xltype = xltypeNil;
	}
	else
	{
// Don't set or clear m_DLLtoFree as this is just one element in
// an array which will already have m_DLLtoFree == true
		clone_xloper(p_op, p_source); // make a deep copy
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, const VARIANT &vt)
{
	if(offset >= m_ArraySize)
		return false;

	bool ret_val = false;

	if(gExcelVersion12plus)
	{
		xloper12 op;
		if(vt_to_xloper(op, &vt, false))
			ret_val = SetArrayElt(offset, &op);
		free_xloper(&op);
	}
	else
	{
		xloper op;
		if(vt_to_xloper(op, &vt, false))
			ret_val = SetArrayElt(offset, &op);
		free_xloper(&op);
	}
	return ret_val;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(DWORD offset, const cpp_xloper &Source)
{
	if(gExcelVersion12plus)
	{
		if(!m_DLLtoFree12)
			return false;  // Should not assign to an Excel-allocated array

		xloper12 *p_op;
		if(!GetArrayElt(offset, p_op))
			return false;

		if(Source.m_Op12.xltype == xltypeMulti)
		{
// can't assign array to an array element so just assign the
// top left value if array has memory assigned
			if(Source.m_Op12.val.array.lparray)
				clone_xloper(p_op, Source.m_Op12.val.array.lparray);
			else
				p_op->xltype = xltypeNil;
		}
		else
		{
// Don't set or clear m_DLLtoFree as this is just one element in
// an array which will already have m_DLLtoFree == true
			clone_xloper(p_op, &(Source.m_Op12)); // make a deep copy
		}
		free_xloper(p_op);
	}
	else
	{
		if(!m_DLLtoFree)
			return false;  // Should not assign to an Excel-allocated array

		xloper *p_op;
		if(!GetArrayElt(offset, p_op))
			return false;

		if(Source.m_Op.xltype == xltypeMulti)
		{
// can't assign array to an array element so just assign the
// top left value if array has memory assigned
			if(Source.m_Op.val.array.lparray)
				clone_xloper(p_op, Source.m_Op.val.array.lparray);
			else
				p_op->xltype = xltypeNil;
		}
		else
		{
// Don't set or clear m_DLLtoFree as this is just one element in
// an array which will already have m_DLLtoFree == true
			clone_xloper(p_op, &(Source.m_Op)); // make a deep copy
		}
		free_xloper(p_op);
	}
	return true;
}
//-------------------------------------------------------------------
// Array access functions with row and column parameters
//-------------------------------------------------------------------
// private function that converts (row,col) to the offset for
// the xltypMulti contained in the m_Op or m_Op12.  Returns false
// if either the (row,col) out-of-range or is not xltypMulti.
//-------------------------------------------------------------------
bool cpp_xloper::MultiRCtoOffset(RW row, COL col, DWORD &offset) const
{
	RW rows;
	COL cols;

	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype != xltypeMulti || m_ArraySize == 0)
			return false;

		rows = m_Op12.val.array.rows;
		cols = m_Op12.val.array.columns;
	}
	else
	{
		if(m_Op.xltype != xltypeMulti || m_ArraySize == 0)
			return false;

		rows = m_Op.val.array.rows;
		cols = m_Op.val.array.columns;
	}

	if(row >= rows || col >= cols)
		return false;

	offset = cols * row + col;
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(RW row, COL column, const char *p) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return ArrayEltEq(offset, p);
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(RW row, COL column, const xloper *p_op) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return ArrayEltEq(offset, p_op);
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(RW row, COL column, const xloper12 *p_op) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return ArrayEltEq(offset, p_op);
}
//-------------------------------------------------------------------
bool cpp_xloper::ArrayEltEq(RW row, COL column, const cpp_xloper &Op) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return ArrayEltEq(offset, Op);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, cpp_xloper &op) const // get deep copy
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, op);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, xloper *&p_op) const // get pointer only
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, p_op);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, VARIANT &vt) const // get deep copy
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, vt);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, xloper12 *&p_op) const // get pointer only
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, p_op);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, int &w) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, w);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, bool &b) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, b);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, double &d) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, d);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, WORD &e) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, e);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, char *&text) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, text);
}
//-------------------------------------------------------------------
bool cpp_xloper::GetArrayElt(RW row, COL column, wchar_t *&text) const
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return GetArrayElt(offset, text);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, int w)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, w);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, bool b)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, b);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, double d)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, d);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, WORD e)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, e);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, const char *text)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, text);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, const wchar_t *text)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, text);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayEltType(RW row, COL column, int new_type)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayEltType(offset, new_type);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, const xloper *p_source)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, p_source);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, const xloper12 *p_source)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, p_source);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, const VARIANT &vt)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, vt);
}
//-------------------------------------------------------------------
bool cpp_xloper::SetArrayElt(RW row, COL column, const cpp_xloper &Source)
{
	DWORD offset;
	if(!MultiRCtoOffset(row, column, offset))
		return false;

	return SetArrayElt(offset, Source);
}
//-------------------------------------------------------------------
// End of array element accessor functions
//-------------------------------------------------------------------
bool cpp_xloper::Transpose(void)
{
	if(!IsType(xltypeMulti) || m_ArraySize == 0)
		return false;

	if(gExcelVersion12plus)
	{
		RW r, rows;
		COL c, columns;
		GetArraySize(rows, columns);

		if(rows > MAX_XL12_COLS)
			return false;

		xloper12 *new_array = (xloper12 *)malloc(XLOPER12_SIZE * rows * columns);
		xloper12 *p_source = m_Op12.val.array.lparray;
		int new_index;

		for(r = 0; r < rows; r++)
		{
			new_index = r;
			for(c = 0; c < columns; c++, new_index += rows)
				new_array[new_index] = *p_source++;
		}
		r = m_Op12.val.array.columns;
		m_Op12.val.array.columns = m_Op.val.array.rows;
		m_Op12.val.array.rows = r;

		memcpy(m_Op12.val.array.lparray, new_array, XLOPER12_SIZE * rows * columns);
		free(new_array);
	}
	else
	{
		RW r, rows;
		COL c, columns;
		GetArraySize(rows, columns);

		if(rows > MAX_XL11_COLS)
			return false;

		xloper *new_array = (xloper *)malloc(XLOPER_SIZE * rows * columns);
		xloper *p_source = m_Op.val.array.lparray;
		int new_index;

		for(r = 0; r < rows; r++)
		{
			new_index = r;
			for(c = 0; c < columns; c++, new_index += rows)
				new_array[new_index] = *p_source++;
		}
		r = m_Op.val.array.columns;
		m_Op.val.array.columns = m_Op.val.array.rows;
		m_Op.val.array.rows = (WORD)r;

		memcpy(m_Op.val.array.lparray, new_array, XLOPER_SIZE * rows * columns);
		free(new_array);
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::SameShapeAs(const cpp_xloper &Op) const
{
	if(&Op == this)
		return true;

	RW this_rows, op_rows;
	COL this_cols, op_cols;

	if(gExcelVersion12plus)
	{
		return xloper_size(&m_Op12, this_rows, this_cols)
		&& xloper_size(&Op.m_Op12, op_rows, op_cols)
		&& this_rows == op_rows && this_cols == op_cols;
	}
	else
	{
		return xloper_size(&m_Op, this_rows, this_cols)
		&& xloper_size(&Op.m_Op, op_rows, op_cols)
		&& this_rows == op_rows && this_cols == op_cols;
	}
}
//-------------------------------------------------------------------
RW cpp_xloper::GetTopRow(void) const
{
	if(gExcelVersion12plus)
	{
		if((m_Op12.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op12.xltype & xltypeRef) && m_Op12.val.mref.lpmref)
			return m_Op12.val.mref.lpmref->reftbl[0].rwFirst + 1;
		else
			return m_Op12.val.sref.ref.rwFirst + 1;
	}
	else
	{
		if((m_Op.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op.xltype & xltypeRef) && m_Op.val.mref.lpmref)
			return m_Op.val.mref.lpmref->reftbl[0].rwFirst + 1;
		else
			return m_Op.val.sref.ref.rwFirst + 1;
	}
}
//-------------------------------------------------------------------
RW cpp_xloper::GetBottomRow(void) const
{
	if(gExcelVersion12plus)
	{
		if((m_Op12.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op12.xltype & xltypeRef) && m_Op12.val.mref.lpmref)
			return m_Op12.val.mref.lpmref->reftbl[0].rwLast + 1;
		else
			return m_Op12.val.sref.ref.rwLast + 1;
	}
	else
	{
		if((m_Op.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op.xltype & xltypeRef) && m_Op.val.mref.lpmref)
			return m_Op.val.mref.lpmref->reftbl[0].rwLast + 1;
		else
			return m_Op.val.sref.ref.rwLast + 1;
	}
}
//-------------------------------------------------------------------
COL cpp_xloper::GetLeftColumn(void) const
{
	if(gExcelVersion12plus)
	{
		if((m_Op12.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op12.xltype & xltypeRef) && m_Op12.val.mref.lpmref)
			return m_Op12.val.mref.lpmref->reftbl[0].colFirst + 1;
		else
			return m_Op12.val.sref.ref.colFirst + 1;
	}
	else
	{
		if((m_Op.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op.xltype & xltypeRef) && m_Op.val.mref.lpmref)
			return m_Op.val.mref.lpmref->reftbl[0].colFirst + 1;
		else
			return m_Op.val.sref.ref.colFirst + 1;
	}
}
//-------------------------------------------------------------------
COL cpp_xloper::GetRightColumn(void) const
{
	if(gExcelVersion12plus)
	{
		if((m_Op12.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op12.xltype & xltypeRef) && m_Op12.val.mref.lpmref)
			return m_Op12.val.mref.lpmref->reftbl[0].colLast + 1;
		else
			return m_Op12.val.sref.ref.colLast + 1;
	}
	else
	{
		if((m_Op.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;

		if((m_Op.xltype & xltypeRef) && m_Op.val.mref.lpmref)
			return m_Op.val.mref.lpmref->reftbl[0].colLast + 1;
		else
			return m_Op.val.sref.ref.colLast + 1;
	}
}
//-------------------------------------------------------------------
wchar_t *cpp_xloper::GetSheetName(void) const
{
	if(gExcelVersion12plus)
	{
		if((m_Op12.xltype & (xltypeRef | xltypeSRef)) == 0)
			return NULL;
		return NULL;
	}
	else
	{
		if((m_Op.xltype & (xltypeRef | xltypeSRef)) == 0)
			return NULL;
		return NULL;
	}
}
//-------------------------------------------------------------------
DWORD cpp_xloper::GetSheetID(void) const
{
	if(gExcelVersion12plus)
	{
		if((m_Op12.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;
		return 0;
	}
	else
	{
		if((m_Op.xltype & (xltypeRef | xltypeSRef)) == 0)
			return 0;
		return 0;
	}
}
//-------------------------------------------------------------------
bool cpp_xloper::SetTopRow(RW row)
{
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetBottomRow(RW row)
{
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetLeftColumn(COL col)
{
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetRightColumn(COL col)
{
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetSheetName(wchar_t *sheet_name) const
{
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::SetSheetID(DWORD id) const
{
	return false;
}
//-------------------------------------------------------------------
bool cpp_xloper::GetRangeSize(RW &rows, COL &cols) const
{
// For use with xltypeSRef and xltypeRef
	if(!IsType(xltypeSRef | xltypeRef))
		return false;

	if(gExcelVersion12plus)
	{
		const xlref12 *pref;

		if(IsType(xltypeSRef))
		{
			pref = &m_Op12.val.sref.ref;
		}
		else
		{
			if(m_Op12.val.mref.lpmref == NULL)
				return false;

			pref = m_Op12.val.mref.lpmref->reftbl;
		}
		rows = pref->rwLast - pref->rwFirst + 1;
		cols = pref->colLast - pref->colFirst + 1;
	}
	else
	{
		const xlref *pref;

		if(IsType(xltypeSRef))
		{
			pref = &m_Op.val.sref.ref;
		}
		else
		{
			if(m_Op.val.mref.lpmref == NULL)
				return false;

			pref = m_Op.val.mref.lpmref->reftbl;
		}
		rows = pref->rwLast - pref->rwFirst + 1;
		cols = pref->colLast - pref->colFirst + 1;
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::ConvertRefToValues(void)
{
	if(gExcelVersion12plus)
	{
		xloper12 temp_op;

		if(!IsType(xltypeSRef | xltypeRef) || Excel12(xlCoerce, &temp_op, 1, &m_Op12) != xlretSuccess)
			return false;

		Free(); // free old memory
		m_Op12 = temp_op;
		m_XLtoFree12 = true;
	}
	else
	{
		xloper temp_op;

		if(!IsType(xltypeSRef | xltypeRef) || Excel4(xlCoerce, &temp_op, 1, &m_Op) != xlretSuccess)
			return false;

		Free(); // free old memory
		m_Op = temp_op;
		m_XLtoFree = true;
	}
	SetArraySize();
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::ConvertRefToSingleValue(void)
{
	if(gExcelVersion12plus)
	{
		xloper12 target_type, temp_op;
		target_type.xltype = xltypeInt;
		target_type.val.w = xltypeErr | xltypeNum | xltypeStr | xltypeBool;

		if(!IsType(xltypeSRef | xltypeRef) || Excel12(xlCoerce, &temp_op, 2, &m_Op12, &target_type) != xlretSuccess)
			return false;

		Free(); // free old memory
		m_Op12 = temp_op;
		m_XLtoFree12 = true;
	}
	else
	{
		xloper target_type, temp_op;
		target_type.xltype = xltypeInt;
		target_type.val.w = xltypeErr | xltypeNum | xltypeStr | xltypeBool;

		if(!IsType(xltypeSRef | xltypeRef) || Excel4(xlCoerce, &temp_op, 2, &m_Op, &target_type) != xlretSuccess)
			return false;

		Free(); // free old memory
		m_Op = temp_op;
		m_XLtoFree = true;
	}
	return true;
}
//-------------------------------------------------------------------
bool cpp_xloper::ConvertRefToMulti(void)
{
	if(gExcelVersion12plus)
	{
		xloper12 temp_op;

		if(!IsType(xltypeSRef | xltypeRef) || !coerce_xloper(&m_Op12, temp_op, xltypeMulti))
			return false;

		Free(); // free old memory
		m_Op12 = temp_op;
		m_XLtoFree12 = true;
	}
	else
	{
		xloper temp_op;

		if(!IsType(xltypeSRef | xltypeRef) || !coerce_xloper(&m_Op, temp_op, xltypeMulti))
			return false;

		Free(); // free old memory
		m_Op = temp_op;
		m_XLtoFree = true;
	}
	SetArraySize();
	return true;
}
//-------------------------------------------------------------------
// Preserves old type and value if unsuccessful
bool cpp_xloper::ConvertSRefToRef(void)
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeRef)
			return true;

		xloper12 temp_op = m_Op12;
		if(m_Op12.xltype != xltypeSRef)
			return false;

// No memory associated with xltypeSRef, so...
		xloper12 type;
		type.val.w = xltypeRef;
		type.xltype = xltypeInt;

		Excel12(xlCoerce, &m_Op12, 2, &m_Op12, &type);
		m_XLtoFree12 = true;
		m_DLLtoFree12 = false; // should already have this value

		if(m_Op12.xltype == xltypeRef)
			return true;

		m_Op12 = temp_op; // restore the original value
	}
	else
	{
		if(m_Op.xltype == xltypeRef)
			return true;

		xloper temp_op = m_Op;
		if(m_Op.xltype != xltypeSRef)
			return false;

// No memory associated with xltypeSRef, so...
		xloper type;
		type.val.w = xltypeRef;
		type.xltype = xltypeInt;

		Excel4(xlCoerce, &m_Op, 2, &m_Op, &type);
		m_XLtoFree = true;
		m_DLLtoFree = false; // should already have this value

		if(m_Op.xltype == xltypeRef)
			return true;

		m_Op = temp_op; // restore the original value
	}
	return false;
}
//-------------------------------------------------------------------
double *cpp_xloper::ConvertMultiToDouble(void)
{
	double *ret_array;
	DWORD size;

	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype != xltypeMulti)
			return NULL;

// Allocate the space for the array of doubles
		size = m_Op12.val.array.rows * m_Op12.val.array.columns;
		ret_array = (double *)malloc(size * sizeof(double));
		if(!ret_array)
			return NULL;

// Get the cell values one-by-one as doubles and place in the array.
// Store the array row-by-row in memory.
		xloper12 *p_op = m_Op12.val.array.lparray;

		if(!p_op)
		{
			free(ret_array);
			return NULL;
		}

		for(double *p = ret_array; size--; p++)
			if(!coerce_to_double(p_op++, *p))
				*p = 0.0;
	}
	else
	{
		if(m_Op.xltype != xltypeMulti)
			return NULL;

// Allocate the space for the array of doubles
		size = m_Op.val.array.rows * m_Op.val.array.columns;
		ret_array = (double *)malloc(size * sizeof(double));
		if(!ret_array)
			return NULL;

// Get the cell values one-by-one as doubles and place in the array.
// Store the array row-by-row in memory.
		xloper *p_op = m_Op.val.array.lparray;

		if(!p_op)
		{
			free(ret_array);
			return NULL;
		}

		for(double *p = ret_array; size--; p++)
			if(!coerce_to_double(p_op++, *p))
				*p = 0.0;
	}
	return ret_array; // caller must free the memory!
}
//-------------------------------------------------------------------
// Return the xloper as a pointer to a thread-local static xloper.
// This method should be called when returning an xloper * to
// an Excel worksheet function, and is thread-safe.
//-------------------------------------------------------------------
xloper *cpp_xloper::ExtractXloper(void)
{
// Get a thread-local static xloper
	xloper *p_ret_val = get_thread_local_xloper();
	if(!p_ret_val) // Could not get a thread-local copy
		return NULL;

	if(gExcelVersion12plus) // cast down to an xloper
	{
		FreeOp();
		xloper12_to_xloper(&m_Op, &m_Op12);
		m_DLLtoFree = true; // ensure bits get set later in this fn
		FreeOp12();
	}

	*p_ret_val = m_Op; // Make a shallow copy of data and pointers

	if((m_Op.xltype & (xltypeRef | xltypeMulti | xltypeStr)) == 0)
	{
// No need to set a flag to tell Excel to call back to free memory
		Clear();
		return p_ret_val;
	}

	if(m_XLtoFree)
	{
		p_ret_val->xltype |= xlbitXLFree;
	}
	else
	{
		if(!m_DLLtoFree) // was a read-only passed-in argument
		{
// Make a deep copy since we don't know where or how this was created
			if(!clone_xloper(p_ret_val, &m_Op))
			{
				Clear();
				return NULL;
			}
		}
		p_ret_val->xltype |= xlbitDLLFree;
		if(m_Op.xltype & xltypeMulti)
		{
			DWORD limit = m_Op.val.array.rows * m_Op.val.array.columns;
			xloper *p = m_Op.val.array.lparray;

			for(;limit--; p++)
				if(p->xltype & xltypeStr)
					p->xltype |= xlbitDLLFree;
		}
	}
// Prevent the destructor from freeing memory by resetting properties
	Clear();
	return p_ret_val;
}
//-------------------------------------------------------------------
xloper12 *cpp_xloper::ExtractXloper12(void)
{	
// Get a thread-local static xloper
	xloper12 *p_ret_val = get_thread_local_xloper12();
	if(!p_ret_val) // Could not get a thread-local copy
		return NULL;

	if(gExcelVersion11minus) // cast up to an xloper12
	{
		FreeOp12();
		xloper_to_xloper12(&m_Op12, &m_Op);
		m_DLLtoFree12 = true; // ensure bits get set later in this fn
		FreeOp();
	}

	*p_ret_val = m_Op12; // Make a shallow copy of data and pointers

	if((m_Op12.xltype & (xltypeRef | xltypeMulti | xltypeStr)) == 0)
	{
// No need to set a flag to tell Excel to call back to free memory
		Clear();
		return p_ret_val;
	}

	if(m_XLtoFree12)
	{
		p_ret_val->xltype |= xlbitXLFree;
	}
	else
	{
		if(!m_DLLtoFree12) // was a read-only passed-in argument
		{
// Make a deep copy since we don't know where or how this was created
			if(!clone_xloper(p_ret_val, &m_Op12))
			{
				Clear();
				return NULL;
			}
		}
		p_ret_val->xltype |= xlbitDLLFree;
		if(m_Op12.xltype & xltypeMulti)
		{
			DWORD limit = m_Op12.val.array.rows * m_Op12.val.array.columns;
			xloper12 *p = m_Op12.val.array.lparray;

			for(;limit--; p++)
				if(p->xltype & xltypeStr)
					p->xltype |= xlbitDLLFree;
		}
	}
// Prevent the destructor from freeing memory by resetting properties
	Clear();
	return p_ret_val;
}
//-------------------------------------------------------------------
VARIANT cpp_xloper::ExtractVariant(void) // extract VARIANT, clear cpp_xloper
{	
	VARIANT return_vt;
	VariantInit(&return_vt); // type is set to VT_EMPTY

	if(gExcelVersion12plus)
		xloper_to_vt(&m_Op12, return_vt, true);
	else
		xloper_to_vt(&m_Op, return_vt, true);

	Free();
	return return_vt;
}
//-------------------------------------------------------------------
bool cpp_xloper::ConvertToString(void)
{
	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeStr) // Already is a string
			return true;

// Create a temporary string xloper
		xloper12 temp;
		if(!coerce_xloper(&m_Op12, temp, xltypeStr))
			return false;

		Free(); // free old resources
		m_Op12.xltype = xltypeStr;
		size_t mem_size = (temp.val.str[0] + 2) * sizeof(wchar_t);
		m_Op12.val.str = (wchar_t *)malloc(mem_size);
		memcpy(m_Op12.val.str, temp.val.str, mem_size);
		m_DLLtoFree12 = true; // DLL must free this memory
		Excel12(xlFree, 0, 1, &temp);  // release the temporary xloper memory
	}
	else
	{
		if(m_Op.xltype == xltypeStr) // Already is a string
			return true;

// Create a temporary string xloper
		xloper temp;
		if(!coerce_xloper(&m_Op, temp, xltypeStr))
			return false;

		Free(); // free old resources
		m_Op.xltype = xltypeStr;
		size_t mem_size = (BYTE)temp.val.str[0] + 2;
		m_Op.val.str = (char *)malloc(mem_size);
		memcpy(m_Op.val.str, temp.val.str, mem_size);
		m_DLLtoFree = true; // DLL must free this memory
		Excel4(xlFree, 0, 1, &temp);  // release the temporary xloper memory
	}
	return true; // All done
}
//-------------------------------------------------------------------
// Permits checking of or'd types, so cannot check for xltypeBigData
bool cpp_xloper::IsType(int type) const
{
	int op_type;

	if(gExcelVersion12plus)
		op_type = m_Op12.xltype;
	else
		op_type = m_Op.xltype;

	if((op_type & xltypeBigData) == xltypeBigData)
		return false;

	return (op_type & type) != 0;
}
//-------------------------------------------------------------------
bool cpp_xloper::IsBigData(void) const
{
	int op_type;

	if(gExcelVersion12plus)
		op_type = m_Op12.xltype;
	else
		op_type = m_Op.xltype;

	return (op_type & xltypeBigData) == xltypeBigData;
}
//-------------------------------------------------------------------
bool cpp_xloper::IsTrue(void) const
{
	if(gExcelVersion12plus)
		return m_Op12.xltype == xltypeBool && m_Op12.val.xbool;

	return m_Op.xltype == xltypeBool && m_Op.val.xbool;
}
//-------------------------------------------------------------------
bool cpp_xloper::IsFalse(void) const
{
	if(gExcelVersion12plus)
		return m_Op12.xltype == xltypeBool && m_Op12.val.xbool == 0;

	return m_Op.xltype == xltypeBool && m_Op.val.xbool == 0;
}
//-------------------------------------------------------------------
bool cpp_xloper::IsErr(WORD err) const
{
	if(gExcelVersion12plus)
		return m_Op12.xltype == xltypeErr && (!err || m_Op12.val.err == err);

	return m_Op.xltype == xltypeErr && (!err || m_Op.val.err == err);
}
//-------------------------------------------------------------------
// Resets cpp_oper properties without freeing up memory 
//-------------------------------------------------------------------
inline void cpp_xloper::ClearOp(void)
{
	m_XLtoFree = m_DLLtoFree = false;
	m_Op.xltype = xltypeNil;
	m_ArraySize = 0;
}
//-------------------------------------------------------------------
inline void cpp_xloper::ClearOp12(void)
{
	m_XLtoFree12 = m_DLLtoFree12 = false;
	m_Op12.xltype = xltypeNil;
	m_ArraySize = 0;
}
//-------------------------------------------------------------------
void cpp_xloper::Clear(void)
{
	ClearOp();
	ClearOp12();
}
//-------------------------------------------------------------------
// return xloper address
//-------------------------------------------------------------------
xloper *cpp_xloper::OpAddr(void)
{
	if(gExcelVersion12plus) // need to populate the xloper
	{
		if(m_Op.xltype & (xltypeStr | xltypeMulti | xltypeRef))
		{
			if(m_DLLtoFree)
				free_xloper(&m_Op);
			else if(m_XLtoFree)
			{
				Excel4(xlFree, 0, 1, &m_Op);
			}
		}
// Destructor will free the memory for both
		xloper12_to_xloper(&m_Op, &m_Op12);
		m_XLtoFree = false;
		m_DLLtoFree = true;
	}
	return &m_Op;
}
//-------------------------------------------------------------------
// return xloper12 address
//-------------------------------------------------------------------
xloper12 *cpp_xloper::OpAddr12(void)
{
	if(gExcelVersion11minus) // need to populate the xloper12
	{
		if(m_Op12.xltype & (xltypeStr | xltypeMulti | xltypeRef))
		{
			if(m_DLLtoFree12)
				free_xloper(&m_Op12);
			else if(m_XLtoFree12)
			{
				Excel12(xlFree, 0, 1, &m_Op12);
			}
		}
// Destructor will free the memory for both
		xloper_to_xloper12(&m_Op12, &m_Op);
		m_XLtoFree12 = false;
		m_DLLtoFree12 = true;
	}
	return &m_Op12;
}
//-------------------------------------------------------------------
// Wrapper functions for Excel4() and Excel12(). Sets cpp_xloper to
// result of call and returns Excel4()/Excel12() return code.
//
// Caller must not mix xloper, xloper12, cpp_xloper arguments in the
// same call to cpp_xloper::Excel() as the compiler chooses which to
// call based on the type of the 3rd argument and expects all
// subsequent arguments to be the same type.
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn)
{
	Free();
	if(gExcelVersion12plus)
	{
		m_XLtoFree12 = true;
		int retval = Excel12(xlfn, &m_Op12, 0);
		SetArraySize();
		return retval;
	}
	else
	{
		m_XLtoFree = true;
		int retval = Excel4(xlfn, &m_Op, 0);
		SetArraySize();
		return retval;
	}
}
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn, int count, xloper *p_op1, ...)
{
	if(xlfn < 0 || count < 0 || count > (gExcelVersion12plus ? MAX_XL12_UDF_ARGS : MAX_XL11_UDF_ARGS))
		return xlretNotCalled;

	if(count == 0 || !p_op1)
		return Excel(xlfn);

	int ret_val;
	if(gExcelVersion12plus)
	{
		va_list arg_ptr;
		va_start(arg_ptr, p_op1); // Initialize

// Use Excel12v to make the call. No need to use a temporary xloper12 for
// the return value as m_Op12 cannot be passed as an argument.
// Retrieve the args as pointers to xlopers and convert to xloper12s. This
// introduces an overhead of conversion but permits more arguments to be
// passed and for the return value to be, say, a Unicode string.
		xloper *p_op;
		xloper12 *xloper12_ptr_array[MAX_XL12_UDF_ARGS];
		xloper12 xloper12_array[MAX_XL12_UDF_ARGS];

		if(!xloper_to_xloper12(xloper12_array, p_op1))
			return xlretNotCalled;

		xloper12_ptr_array[0] = xloper12_array;

		int i;
		for(i = 1; i < count; i++)
		{
			p_op = va_arg(arg_ptr, xloper *);
			if(!xloper_to_xloper12(xloper12_array + i, p_op))
				return xlretNotCalled;

			xloper12_ptr_array[i] = xloper12_array + i;
		}
		va_end(arg_ptr); // Reset

		Free();
		ret_val = Excel12v(xlfn, &m_Op12, count, xloper12_ptr_array);

		for(i = 0; i < count; i++)
			free_xloper(xloper12_array + i);

		if(ret_val == xlretSuccess)
			m_XLtoFree12 = true;
	}
	/*else // gExcelVersion < 12
	{
// Use Excel4v to make the call.  Use a temporary xloper for the return
// value in case m_Op is also being passed as an argument, in which case
// can't overwrite before freeing old resources.
		xloper temp;
		ret_val = Excel4v(xlfn, &temp, count, &p_op1);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op = temp; // shallow copy
			m_XLtoFree = true;
		}
	}*/
	SetArraySize();
	return ret_val;
}
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn, int count, xloper12 *p_op1, ...)
{
	if(xlfn < 0 || count < 0 || count > (gExcelVersion12plus ? MAX_XL12_UDF_ARGS : MAX_XL11_UDF_ARGS))
		return xlretNotCalled;

	if(count == 0 || !p_op1)
		return Excel(xlfn);

	int ret_val;

	if(gExcelVersion12plus)
	{
// Use Excel12v to make the call.  Use a temporary xloper12 for the return
// value in case m_Op12 is also being passed as an argument, in which case
// can't overwrite before freeing old resources.
		xloper12 temp;
		ret_val = Excel12v(xlfn, &temp, count, &p_op1);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op12 = temp; // shallow copy
			m_XLtoFree12 = true;
		}
	}
	/*else // gExcelVersion < 12
	{
		va_list arg_ptr;
		va_start(arg_ptr, p_op1); // Initialize
// Use Excel4v to make the call. No need to use a temporary xloper for
// the return value as m_Op cannot be passed as an argument.
// Retrieve the args as pointers to xloper12s and convert to xlopers. This
// introduces an overhead of conversion but can't call Exce12 in this
// version so can't pass xloper12s.
		xloper12 *p_op;
		xloper *xloper_ptr_array[MAX_XL11_UDF_ARGS];
		xloper xloper_array[MAX_XL11_UDF_ARGS];

		if(!xloper12_to_xloper(xloper_array, p_op1))
			return xlretNotCalled;

		xloper_ptr_array[0] = xloper_array;

		int i;
		for(i = 1; i < count; i++)
		{
			p_op = va_arg(arg_ptr, xloper12 *);
			if(!xloper12_to_xloper(xloper_array + i, p_op))
				return xlretNotCalled;

			xloper_ptr_array[i] = xloper_array + i;
		}
		va_end(arg_ptr); // Reset

		Free();
		ret_val = Excel4v(xlfn, &m_Op, count, xloper_ptr_array);

		for(i = 0; i < count; i++)
			free_xloper(xloper_array + i);

		if(ret_val == xlretSuccess)
			m_XLtoFree = true;
	}*/
	SetArraySize();
	return ret_val;
}
//-------------------------------------------------------------------
// Wrapper functions for Excel4v()/Excel12v().
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn, int count, xloper *p_array[])
{
	if(xlfn < 0 || count < 0 || count > (gExcelVersion12plus ? MAX_XL12_UDF_ARGS : MAX_XL11_UDF_ARGS))
		return xlretNotCalled;

	if(count == 0)
		return Excel(xlfn);

	int ret_val;

	if(gExcelVersion12plus)
	{
// Use Excel12v to make the call. No need to use a temporary xloper12 for
// the return value as m_Op12 cannot be passed as an argument.
// Convert the passed in array to xloper12s. This introduces an overhead
// of conversion but permits more arguments to be passed and for the
// return value to be, say, a Unicode string.
		xloper12 *xloper12_ptr_array[MAX_XL12_UDF_ARGS];
		xloper12 xloper12_array[MAX_XL12_UDF_ARGS];

		int i;
		for(i = 0; i < count; i++)
		{
			if(!xloper_to_xloper12(xloper12_array + i, p_array[i]))
				return xlretNotCalled;

			xloper12_ptr_array[i] = xloper12_array + i;
		}

		Free();
		ret_val = Excel12v(xlfn, &m_Op12, count, xloper12_ptr_array);

		for(i = 0; i < count; i++)
			free_xloper(xloper12_array + i);

		if(ret_val == xlretSuccess)
			m_XLtoFree12 = true;
	}
	/*else // gExcelVersion < 12
	{
// Use Excel4v to make the call.  Use a temporary xloper for the return
// value in case m_Op is also being passed as an argument, in which case
// can't overwrite before freeing old resources.
		xloper temp;
		ret_val = Excel4v(xlfn, &temp, count, p_array);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op = temp; // shallow copy
			m_XLtoFree = true;
		}
	}*/
	SetArraySize();
	return ret_val;
}
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn, int count, xloper12 *p_array[])
{
	if(xlfn < 0 || count < 0 || count > (gExcelVersion12plus ? MAX_XL12_UDF_ARGS : MAX_XL11_UDF_ARGS))
		return xlretNotCalled;

	if(count == 0)
		return Excel(xlfn);

	int ret_val;

	if(gExcelVersion12plus)
	{
// Use Excel12v to make the call.  Use a temporary xloper12 for the return
// value in case m_Op12 is also being passed as an argument, in which case
// can't overwrite before freeing old resources.
		xloper12 temp;
		ret_val = Excel12v(xlfn, &temp, count, p_array);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op12 = temp; // shallow copy
			m_XLtoFree12 = true;
		}
	}
	/*else // gExcelVersion < 12
	{
// Use Excel4v to make the call. No need to use a temporary xloper for
// the return value as m_Op cannot be passed as an argument.
// Convert the passed in array to xlopers. This introduces an overhead
// of conversion  but can't call Exce12 in this version so can't pass
// xloper12s.
		xloper *xloper_ptr_array[MAX_XL11_UDF_ARGS];
		xloper xloper_array[MAX_XL11_UDF_ARGS];

		int i;
		for(i = 0; i < count; i++)
		{
			if(!xloper12_to_xloper(xloper_array + i, p_array[i]))
				return xlretNotCalled;

			xloper_ptr_array[i] = xloper_array + i;
		}

		Free();
		ret_val = Excel4v(xlfn, &m_Op, count, xloper_ptr_array);

		for(i = 0; i < count; i++)
			free_xloper(xloper_array + i);

		if(ret_val == xlretSuccess)
			m_XLtoFree = true;
	}*/
	SetArraySize();
	return ret_val;
}
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn, int count, cpp_xloper *p_array[])
{
	if(xlfn < 0 || count < 0 || count > (gExcelVersion12plus ? MAX_XL12_UDF_ARGS : MAX_XL11_UDF_ARGS))
		return xlretNotCalled;

	if(count == 0)
		return Excel(xlfn);

	int ret_val;

	if(gExcelVersion12plus)
	{
		xloper12 *xloper12_ptr_array[MAX_XL12_UDF_ARGS];

		for(int i = 0; i < count; i++)
			xloper12_ptr_array[i] = &(p_array[i]->m_Op12);

		xloper12 temp;
		ret_val = Excel12v(xlfn, &temp, count, xloper12_ptr_array);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op12 = temp; // shallow copy
			m_XLtoFree12 = true;
		}
	}
	/*else // gExcelVersion < 12
	{
		xloper *xloper_ptr_array[MAX_XL11_UDF_ARGS];

		for(int i = 0; i < count; i++)
			xloper_ptr_array[i] = &(p_array[i]->m_Op);

		xloper temp;
		ret_val = Excel4v(xlfn, &temp, count, xloper_ptr_array);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op = temp; // shallow copy
			m_XLtoFree = true;
		}
	}*/
	SetArraySize();
	return ret_val;
}
//-------------------------------------------------------------------
int cpp_xloper::Excel(int xlfn, int count, cpp_xloper *p_op1, ...)
{
	if(xlfn < 0 || count < 0 || count > (gExcelVersion12plus ? MAX_XL12_UDF_ARGS : MAX_XL11_UDF_ARGS))
		return xlretNotCalled;

	if(count == 0 || !p_op1)
		return Excel(xlfn);

	int ret_val;
	va_list arg_ptr;
	va_start(arg_ptr, p_op1); // Initialize

	if(gExcelVersion12plus)
	{
		xloper12 *xloper12_ptr_array[MAX_XL12_UDF_ARGS];
		xloper12_ptr_array[0] = &(p_op1->m_Op12);
		cpp_xloper *p_cpp_op;

		for(int i = 1; i < count; i++) // retrieve the args as pointers to cpp_xlopers
		{
			p_cpp_op = va_arg(arg_ptr, cpp_xloper *);
			xloper12_ptr_array[i] = &(p_cpp_op->m_Op12);
		}
		va_end(arg_ptr); // Reset

		xloper12 temp;
		ret_val = Excel12v(xlfn, &temp, count, xloper12_ptr_array);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op12 = temp; // shallow copy
			m_XLtoFree12 = true;
		}
	}
	/*else // gExcelVersion < 12
	{
		xloper *xloper_ptr_array[MAX_XL11_UDF_ARGS];
		xloper_ptr_array[0] = &(p_op1->m_Op);
		cpp_xloper *p_cpp_op;

		for(int i = 1; i < count; i++) // retrieve the args as pointers to cpp_xlopers
		{
			p_cpp_op = va_arg(arg_ptr, cpp_xloper *);
			xloper_ptr_array[i] = &(p_cpp_op->m_Op);
		}
		va_end(arg_ptr); // Reset

		xloper temp;
		ret_val = Excel4v(xlfn, &temp, count, xloper_ptr_array);
		Free();

		if(ret_val == xlretSuccess)
		{
			m_Op = temp; // shallow copy
			m_XLtoFree = true;
		}
	}*/
	SetArraySize();
	return ret_val;
}
//-------------------------------------------------------------------
// Is the xloper a reference on the active sheet?
bool cpp_xloper::IsActiveRef(void) const
{
	DWORD id;

	if(gExcelVersion12plus)
	{
		if(m_Op12.xltype == xltypeSRef) // then convert to xltypeRef
		{
			xloper12 as_ref = {0, xltypeNil};
			xloper12 type = {0, xltypeInt};
			type.val.w = xltypeRef;
			Excel12(xlCoerce, &as_ref, 2, &m_Op12, &type);

			if(as_ref.xltype != xltypeRef)
				return false;

			id = as_ref.val.mref.idSheet;
			Excel12(xlFree, 0, 1, &as_ref);
		}
		else if(m_Op12.xltype == xltypeRef)
			id = m_Op12.val.mref.idSheet;
		else
			return false;

		xloper12 active_sheet_id;

		if(Excel12(xlSheetId, &active_sheet_id, 0)
		|| active_sheet_id.xltype != xltypeRef
		|| id != active_sheet_id.val.mref.idSheet)
		{
// No need to call xlFree: active_sheet_id's xlmref pointer is NULL
			return false;
		}
	}
	else
	{
		if(m_Op.xltype == xltypeSRef) // then convert to xltypeRef
		{
			xloper as_ref = {0, xltypeNil};
			xloper type = {0, xltypeInt};
			type.val.w = xltypeRef;
			Excel4(xlCoerce, &as_ref, 2, &m_Op, &type);

			if(as_ref.xltype != xltypeRef)
				return false;

			id = as_ref.val.mref.idSheet;
			Excel4(xlFree, 0, 1, &as_ref);
		}
		else if(m_Op.xltype == xltypeRef)
			id = m_Op.val.mref.idSheet;
		else
			return false;

		xloper active_sheet_id;

		if(Excel4(xlSheetId, &active_sheet_id, 0)
		|| active_sheet_id.xltype != xltypeRef
		|| id != active_sheet_id.val.mref.idSheet)
		{
// No need to call xlFree: active_sheet_id's xlmref pointer is NULL
			return false;
		}
	}
	return true;
}
//===================================================================
// Display xloper as string in specified type alert
bool cpp_xloper::Alert(int dialog_type)
{
	if(dialog_type < 1 || dialog_type > 3)
		dialog_type = 2; // Excel and this function's default

	if(gExcelVersion12plus)
	{
		xloper12 alert_type, ret_val;
		alert_type.val.w = dialog_type;
		alert_type.xltype = xltypeInt;

		if(m_Op12.xltype != xltypeStr)
		{
			xloper12 temp;

			if(!coerce_xloper(&m_Op12, temp, xltypeStr))
				return false;

			Excel12(xlcAlert, &ret_val, 2, &temp, &alert_type);
			Excel12(xlFree, 0, 1, &temp);
			return (ret_val.xltype == xltypeBool && ret_val.val.xbool == 1);
		}
		Excel12(xlcAlert, &ret_val, 2, &m_Op12, &alert_type);
		return (ret_val.xltype == xltypeBool && ret_val.val.xbool == 1);
	}
	else
	{
		xloper alert_type, ret_val;
		alert_type.val.w = dialog_type;
		alert_type.xltype = xltypeInt;

		if(m_Op.xltype != xltypeStr)
		{
			xloper temp;

			if(!coerce_xloper(&m_Op, temp, xltypeStr))
				return false;

			Excel4(xlcAlert, &ret_val, 2, &temp, &alert_type);
			Excel4(xlFree, 0, 1, &temp);
			return (ret_val.xltype == xltypeBool && ret_val.val.xbool == 1);
		}
		Excel4(xlcAlert, &ret_val, 2, &m_Op, &alert_type);
		return (ret_val.xltype == xltypeBool && ret_val.val.xbool == 1);
	}
}
//===================================================================
//===================================================================
// End of public functions
//===================================================================
//===================================================================
// Private functions 
//===================================================================
//===================================================================
// Free up memory and reset cpp_oper properties
// Only need to free if one of the types that has memory allocated.
//===================================================================
inline void cpp_xloper::FreeOp(void)  // free memory and initialise
{
	if((m_Op.xltype & (xltypeRef | xltypeMulti | xltypeStr)) != 0)
	{
		if(m_XLtoFree)
			Excel4(xlFree, 0, 1, &m_Op);
		else if(m_DLLtoFree)
			free_xloper(&m_Op);
	}
	ClearOp();
}
//===================================================================
inline void cpp_xloper::FreeOp12(void)  // free memory and initialise
{
	if((m_Op12.xltype & (xltypeRef | xltypeMulti | xltypeStr)) != 0)
	{
		if(m_XLtoFree12)
			Excel12(xlFree, 0, 1, &m_Op12);
		else if(m_DLLtoFree12)
			free_xloper(&m_Op12);
	}
	ClearOp12();
}
//===================================================================
void cpp_xloper::Free(void)  // free all memory and initialise
{
// Class can have both the xloper and the xloper12 defined, so need to
// check and free both.  (This can happen where the class has been
// asked to return an xloper of the other type than the running version
// default).
	FreeOp();
	FreeOp12();
}
//===================================================================
//===================================================================
//===================================================================
//===================================================================
//===================================================================
//===================================================================
//===================================================================
//===================================================================
