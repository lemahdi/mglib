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

extern int gExcelVersion;
extern bool gExcelVersion12plus;
extern bool gExcelVersion11minus;

#include "xlw/mgw/xloper.h"
#include "xlw/mgw/xloper12.h"
#include "xlw/mgw/xl_array.h"

// Used in cpp_xloper class by member functions that wrap calls
// to the C API, when returning before making a call to the C API
// because a calling parameter error has been picked up
#define xlretNotCalled		-1


//====================================================================
// Row and column arguments to cpp_xloper functions dealing
// with array and ranges are declared as RW and COL (INT32).
// cpp_xloper class performs version-specific check on the
// limits of the provided values and fails if limts are
// exceded.
//====================================================================

class cpp_xloper
{
public:
//--------------------------------------------------------------------
// constructors
//--------------------------------------------------------------------
	cpp_xloper(); // created as xltypeNil
	cpp_xloper(const xloper *p_oper, bool deep_copy = false);
	cpp_xloper(const xloper12 *p_oper, bool deep_copy = false);
	cpp_xloper(const char *text);	// xltypeStr: xloper ASCII byte-string
	cpp_xloper(const wchar_t *text);// xltypeStr: xloper12 unicode string
	cpp_xloper(int w);			// xltypeInt
	cpp_xloper(DWORD dw);		// xltypeNum
	cpp_xloper(int w, int min, int max); // xltypeInt (or xltypeNil)
	cpp_xloper(double d);		// xltypeNum
	cpp_xloper(bool b);			// xltypeBool
	cpp_xloper(WORD e);			// xltypeErr
	cpp_xloper(RW, RW, COL, COL);	// xltypeSRef
	cpp_xloper(const char *, RW, RW, COL, COL); // xltypeRef from sheet name
	cpp_xloper(DWORD, RW, RW, COL, COL);  // xltypeRef from sheet ID
	cpp_xloper(const VARIANT *pv);	// Takes its type from the VARTYPE
// xltypeBigData: No deep copying or memory management for this type
	cpp_xloper(const void *data, long len);

// xltypeMulti constructors
	cpp_xloper(RW rows, COL cols); // array of undetermined type
	cpp_xloper(RW rows, COL cols, const double *d_array); // array of xltypeNum
// Arrays of strings cast up or down depending on the running Excel version
	cpp_xloper(RW rows, COL cols, char **str_array); // xltypeStr (byte strings)
	cpp_xloper(RW rows, COL cols, wchar_t **str_array); // (Unicode strings)
	cpp_xloper(RW &rows, COL &cols, const xloper *input_oper); // from SRef/Ref
	cpp_xloper(RW &rows, COL &cols, const xloper12 *input_oper); // from SRef/Ref
	cpp_xloper(RW rows, COL cols, const cpp_xloper *init_array);
	cpp_xloper(const xl4_array *array);
	cpp_xloper(const xl12_array *array);

	cpp_xloper(const cpp_xloper &source); // Copy constructor

//--------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------
	~cpp_xloper();

//--------------------------------------------------------------------
// Overloaded operators
//--------------------------------------------------------------------
	cpp_xloper &operator=(const cpp_xloper &source);
	int operator=(int);		// xltypeInt
	bool operator=(bool b);		// xltypeBool
	double operator=(double);		// xltypeNum
	WORD operator=(WORD e);		// xltypeErr
	const char *operator=(const char *);	// xltypeStr
	const wchar_t *operator=(const wchar_t *); // xltypeStr
	const xloper *operator=(const xloper *);	// same type as passed-in xloper
	const xloper12 *operator=(const xloper12 *); // same type as passed-in xloper
	const VARIANT *operator=(const VARIANT *); // same type as passed-in Variant
	const xl4_array *operator=(const xl4_array *array);
	const xl12_array *operator=(const xl12_array *array);

	bool operator==(int w);
	bool operator==(bool b);
	bool operator==(double d);
	bool operator==(WORD e);
	bool operator==(const char *text);
	bool operator==(const wchar_t *text);
	bool operator==(const xloper *);
	bool operator==(const xloper12 *);
	bool operator==(const cpp_xloper &cpp_op2);

	bool operator!=(int w) {return !operator==(w);}
	bool operator!=(bool b) {return !operator==(b);}
	bool operator!=(double d) {return !operator==(d);}
	bool operator!=(WORD e) {return !operator==(e);}
	bool operator!=(const char *text) {return !operator==(text);}
	bool operator!=(const wchar_t *text) {return !operator==(text);}
	bool operator!=(const xloper *p_op) {return !operator==(p_op);}
	bool operator!=(const xloper12 *p_op) {return !operator==(p_op);}
	bool operator!=(const cpp_xloper &cpp_op2) {return !operator==(cpp_op2);}

	operator int(void) const;
	operator bool(void) const;
	operator double(void) const;
	operator xloper(void); // get a shallow copy
	operator xloper12(void); // get a shallow copy

	void operator+=(double); // coersion to double and addition
	void operator+=(int w) {operator+=((double)w);}
	void operator-=(double d) {operator+=(-d);}
	void operator-=(int w) {operator+=((double)-w);}
	void operator++(void) {operator+=(1.0);}
	void operator--(void) {operator+=(-1.0);}
// If this type is numeric, coerces Op to double and adds. If this
// type is a string, coerces Op to string and concatenates. Else
// does nothing.
	void operator+=(const cpp_xloper &Op);

	double operator*=(double); // Coerce to double and multiply

	xloper *OpAddr(void);		// return xloper address
	xloper12 *OpAddr12(void);	// return xloper12 address
//--------------------------------------------------------------------
// string oper functions
//--------------------------------------------------------------------
	bool Concat(const cpp_xloper &op); // coersion to strings and concatentation
	size_t Len(void) const; // returns 0 if not a string
	wchar_t First(void) const;  // get the first char or 0 if not a string
	wchar_t Mid(int posn) const;  // Nth char: 1st=1. Rtn 0 if !string
	operator char *(void) const; // deep copy as byte string, NULL if not a string
	operator wchar_t *(void) const; // deep copy Unicode string, NULL if not string
	void operator+=(const char *); // coerce to string and concatenate
	void operator+=(const wchar_t *);

//--------------------------------------------------------------------
// property get and set functions
//--------------------------------------------------------------------
	int  GetType(void) const ;
	bool GetErrVal(WORD &e) const;
	void SetType(int new_type);
	void SetToError(int err_code);
	bool SetToCallerValue(void);
	bool SetTypeMulti(RW array_rows, COL array_cols);
	bool SetCell(RW rwFirst, RW rwLast, COL colFirst, COL colLast);
	bool IsType(int) const;
	bool IsStr(void) const		{return IsType(xltypeStr);}
	bool IsNum(void) const		{return IsType(xltypeNum);}
	bool IsBool(void) const		{return IsType(xltypeBool);}
	bool IsTrue(void) const; // Explicit check for TRUE
	bool IsFalse(void) const; // Explicit check for FALSE
	bool IsInt(void) const		{return IsType(xltypeInt);}
	bool IsErr(WORD err = 0) const;
	bool IsMulti(void) const	{return IsType(xltypeMulti);}
	bool IsNil(void) const		{return IsType(xltypeNil);}
	bool IsMissing(void) const	{return IsType(xltypeMissing);}
	bool IsNilMissing(void) const	{return IsType(xltypeNil | xltypeMissing);}
	bool IsRef(void) const		{return IsType(xltypeRef | xltypeSRef);}
	bool IsBigData(void) const;

	bool IsNullErr(void) const		{return IsErr(xlerrNull);}
	bool IsDiv0Err(void) const		{return IsErr(xlerrDiv0);}
	bool IsValueErr(void) const		{return IsErr(xlerrValue);}
	bool IsRefErr(void) const		{return IsErr(xlerrRef);}
	bool IsNameErr(void) const		{return IsErr(xlerrName);}
	bool IsNumErr(void) const		{return IsErr(xlerrNum);}
	bool IsNaErr(void) const		{return IsErr(xlerrNA);}

//--------------------------------------------------------------------
// property get and set functions for xltypeRef and xltypeSRef
//--------------------------------------------------------------------
	bool GetRangeSize(RW &rows, COL &cols) const; // For use with xltypeSRef/Ref
	bool IsActiveRef(void) const; // Is the xloper a reference on the active sheet?
	bool ConvertRefToMulti(void);
	bool ConvertRefToValues(void);
	bool ConvertRefToSingleValue(void);
	bool ConvertSRefToRef(void);
	RW GetTopRow(void) const; // counts from 1
	RW GetBottomRow(void) const; // counts from 1
	COL GetLeftColumn(void) const; // counts from 1
	COL GetRightColumn(void) const; // counts from 1
	bool SetTopRow(RW row); // counts from 1
	bool SetBottomRow(RW row); // counts from 1
	bool SetLeftColumn(COL col); // counts from 1
	bool SetRightColumn(COL col); // counts from 1
	wchar_t *GetSheetName(void) const;
	DWORD GetSheetID(void) const;
	bool SetSheetName(wchar_t *sheet_name) const;
	bool SetSheetID(DWORD id) const;

//--------------------------------------------------------------------
// property get and set functions for xltypeMulti
//--------------------------------------------------------------------
	void InitialiseArray(RW rows, COL cols, const double *init_data);
	void InitialiseArray(RW rows, COL cols, const cpp_xloper *init_array);
	int  GetArrayEltType(RW row, COL column) const;
	int  GetArrayEltType(DWORD offset) const;
	bool SetArrayEltType(RW row, COL column, int new_type);
	bool SetArrayEltType(DWORD offset, int new_type);
	bool GetArraySize(DWORD &size) const;
	bool GetArraySize(RW &rows, COL &cols) const;

	bool GetArrayElt(DWORD offset, int &w) const;
	bool GetArrayElt(DWORD offset, bool &b) const;
	bool GetArrayElt(DWORD offset, double &d) const;
	bool GetArrayElt(DWORD offset, WORD &e) const;
	bool GetArrayElt(DWORD offset, char *&text) const; // makes new string
	bool GetArrayElt(DWORD offset, wchar_t *&text) const; // makes new string
	bool GetArrayElt(DWORD offset, xloper *&p_op) const; // get pointer only
	bool GetArrayElt(DWORD offset, xloper12 *&p_op) const; // get pointer only
	bool GetArrayElt(DWORD offset, VARIANT &vt) const; // get deep copy
	bool GetArrayElt(DWORD offset, cpp_xloper &Elt) const; // get deep copy

	bool GetArrayElt(RW row, COL column, int &w) const;
	bool GetArrayElt(RW row, COL column, bool &b) const;
	bool GetArrayElt(RW row, COL column, double &d) const;
	bool GetArrayElt(RW row, COL column, WORD &e) const;
	bool GetArrayElt(RW row, COL column, char *&text) const; // makes new string
	bool GetArrayElt(RW row, COL column, wchar_t *&text) const; // makes new string
	bool GetArrayElt(RW row, COL column, xloper *&p_op) const; // get pointer only
	bool GetArrayElt(RW row, COL column, xloper12 *&p_op) const; // get pointer only
	bool GetArrayElt(RW row, COL column, VARIANT &vt) const; // get deep copy
	bool GetArrayElt(RW row, COL column, cpp_xloper &Elt) const; // get deep copy

	bool SetArrayElt(DWORD offset, int w);
	bool SetArrayElt(DWORD offset, bool b);
	bool SetArrayElt(DWORD offset, double d);
	bool SetArrayElt(DWORD offset, WORD e);
	bool SetArrayElt(DWORD offset, const char *text);
	bool SetArrayElt(DWORD offset, const wchar_t *text);
	bool SetArrayElt(DWORD offset, const xloper *p_source);
	bool SetArrayElt(DWORD offset, const xloper12 *p_source);
	bool SetArrayElt(DWORD offset, const VARIANT &vt);
	bool SetArrayElt(DWORD offset, const cpp_xloper &Source);

	bool SetArrayElt(RW row, COL column, int w);
	bool SetArrayElt(RW row, COL column, bool b);
	bool SetArrayElt(RW row, COL column, double d);
	bool SetArrayElt(RW row, COL column, WORD e);
	bool SetArrayElt(RW row, COL column, const char *text);
	bool SetArrayElt(RW row, COL column, const wchar_t *text);
	bool SetArrayElt(RW row, COL column, const xloper *p_source);
	bool SetArrayElt(RW row, COL column, const xloper12 *p_source);
	bool SetArrayElt(RW row, COL column, const VARIANT &vt);
	bool SetArrayElt(RW row, COL column, const cpp_xloper &Source);

	bool Transpose(void);

	double *ConvertMultiToDouble(void);
	bool SameShapeAs(const cpp_xloper &Op) const;

	bool ArrayEltEq(RW row, COL col, const char *) const;
	bool ArrayEltEq(RW row, COL col, const wchar_t *) const;
	bool ArrayEltEq(RW row, COL col, const xloper *) const;
	bool ArrayEltEq(RW row, COL col, const xloper12 *) const;
	bool ArrayEltEq(RW row, COL col, const cpp_xloper &) const;
	bool ArrayEltEq(DWORD offset, const char *) const;
	bool ArrayEltEq(DWORD offset, const wchar_t *) const;
	bool ArrayEltEq(DWORD offset, const xloper *) const;
	bool ArrayEltEq(DWORD offset, const xloper12 *) const;
	bool ArrayEltEq(DWORD offset, const cpp_xloper &) const;

//--------------------------------------------------------------------
// other public functions
//--------------------------------------------------------------------
	void Clear(void);	// Clears the xlopers without freeing memory
	xloper *ExtractXloper(void); // extract xloper, clear cpp_xloper
	xloper12 *ExtractXloper12(void); // extract xloper12, clear cpp_xloper
	VARIANT ExtractVariant(void); // extract VARIANT, clear cpp_xloper
	void Free(void); // free memory
	bool ConvertToString(void);
	bool AsVariant(VARIANT &var) const; // Return an equivalent Variant
	xl4_array *AsDblArray(void) const; // Return an xl4_array
	bool Alert(int dialog_type = 2);  // Display as string in alert dialog
//-------------------------------------------------------------------
// Wrapper functions for Excel4() and Excel12().  Sets cpp_xloper to
// result of call and returns Excel4()/Excel12() return code.
//-------------------------------------------------------------------
	int Excel(int xlfn);
	int Excel(int xlfn, int count, xloper *p_op1, ...);
	int Excel(int xlfn, int count, xloper12 *p_op1, ...);
	int Excel(int xlfn, int count, cpp_xloper *p_op1, ...);
	int Excel(int xlfn, int count, xloper *p_array[]);
	int Excel(int xlfn, int count, xloper12 *p_array[]);
	int Excel(int xlfn, int count, cpp_xloper *p_array[]);

private:
	inline void cpp_xloper::FreeOp(void);  // free xloper and initialise
	inline void cpp_xloper::FreeOp12(void);  // free xloper12 and initialise
	inline void cpp_xloper::ClearOp(void);
	inline void cpp_xloper::ClearOp12(void);
	inline bool RowValid(RW rw) const
	{return rw >= 0 && rw < (gExcelVersion12plus ? MAX_XL12_ROWS : MAX_XL11_ROWS);}

	inline bool ColValid(COL col) const
	{return col >= 0 && col < (gExcelVersion12plus?MAX_XL12_COLS : MAX_XL11_COLS);}

	inline bool RowColValid(RW rw, COL col) const
	{return RowValid(rw) && ColValid(col);}

	inline void SetArraySize(void);

	bool MultiRCtoOffset(RW row, COL col, DWORD &offset) const;

// Either or both these can be initialised: only one will be initialised
// unless OpAddr/ExtractXloper is called in version 12+ or OpAddr12/ExtractXloper12
// is called in version 11-.  The version normally initialised is the one
// corresponding to the running version to remove unnecessary conversions.

	xloper m_Op;
	bool m_DLLtoFree;
	bool m_XLtoFree;

	xloper12 m_Op12;
	bool m_DLLtoFree12;
	bool m_XLtoFree12;

	DWORD m_ArraySize;
};
