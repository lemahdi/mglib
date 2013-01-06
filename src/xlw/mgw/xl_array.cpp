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
// This source file (and header file) contain examples relating to the use of
// the xl4_array (FP) and xl12_array (FP12) data types.
//============================================================================
//============================================================================
#include <windows.h>
#include "xlw/mgw/xl_array.h"
#include "xlw/mgw/TLS.h"
#include "xlw/mgw/xloper.h"
#pragma warning (disable : 4068)

//====================================================================
// Maximum number of array elements supportable by the size_t-based
// memory allocation functions = INT((2^32 - 1)/8) - 1.
// In Excel 12 this means (all rows) x (up to 511 columns)
// or (up to 32,767 rows) x (all columns)

#define MAX_SIZE_T_ARRAY_ELTS	536870910

//==================================================================
// Calculate the total size of the array (number of elements) and
// check that this does not exceed the maximum block size supported
// by size_t-based memory allocation functions.  If it does, return
// false
bool get_xl_array_size(RW rows, COL columns, size_t &size)
{
// In Excel 12, can be 2^14 columns x 2^20 rows, so INT32 is too small
	INT64 size64 = rows * columns; // RW and COL are INT32

// size_t based memory management functions cannot provide 
// memory blocks big enough for > MAX_SIZE_T_ARRAY_ELTS
	if(size64 <= 0 || size64 > MAX_SIZE_T_ARRAY_ELTS)
		return false;

// Can safely cast size64 down to a size_t now
	size = (size_t)size64;
	return true;
}
//====================================================================
//====================================================================
//====================================================================
//
//	Array functions for Excel 4 array
//
//====================================================================
//====================================================================
//==================================================================
xl4_array *new_xl4_array(RW rows, COL columns, double *array)
{
	size_t size;
	if(rows <= 0 || columns <= 0 || !get_xl_array_size(rows, columns, size))
		return NULL;

	size_t mem_size = sizeof(xl4_array) + sizeof(double) * (size - 1);

	xl4_array *ret_array = (xl4_array *)malloc(mem_size);
	ret_array->columns = columns;
	ret_array->rows = rows;

	if(array)
		memcpy(ret_array->array, array, size * sizeof(double));
	else
		memset(ret_array->array, 0, size * sizeof(double));

	return ret_array;
}
//====================================================================
bool transpose_xl4_array(xl4_array *p)
{
	size_t size;
	if(!p || p->columns < 1 || p->rows < 1
	|| !get_xl_array_size(p->rows, p->columns, size))
		return false;

	double *new_array = (double *)malloc(sizeof(double) * size);

	RW r; COL c;
	double *p_source = p->array;
	double *p_target;

	for(r = 0; r < p->rows; r++)
	{
		p_target = new_array + r;

		for(c = 0; c < p->columns; c++, p_target += p->rows)
		{
			*p_target = *p_source++;
		}
	}

	r = p->columns;
	p->columns = p->rows;
	p->rows = r;

	memcpy(p->array, new_array, sizeof(double) * size);
	free(new_array);
	return true;
}
//====================================================================
//====================================================================
//====================================================================
//
//	Array functions for Excel 12 array
//
//====================================================================
//====================================================================
//==================================================================
xl12_array *new_xl12_array(RW rows, COL columns, double *array)
{
	size_t size;
	if(rows <= 0 || columns <= 0 || !get_xl_array_size(rows, columns, size))
		return NULL;

	size_t mem_size = sizeof(xl12_array) + sizeof(double) * (size - 1);

	xl12_array *ret_array = (xl12_array *)malloc(mem_size);
	ret_array->columns = columns;
	ret_array->rows = rows;

	if(array)
		memcpy(ret_array->array, array, size * sizeof(double));
	else
		memset(ret_array, 0, size * sizeof(double));

	return ret_array;
}
//====================================================================
bool transpose_xl12_array(xl12_array *p)
{
	size_t size;
	if(!p || p->columns < 1 || p->rows < 1
	|| !get_xl_array_size(p->rows, p->columns, size))
		return false;

	double *new_array = (double *)malloc(sizeof(double) * size);

	RW r; COL c;
	double *p_source = p->array;
	double *p_target;

	for(r = 0; r < p->rows; r++)
	{
		p_target = new_array + r;

		for(c = 0; c < p->columns; c++, p_target += p->rows)
		{
			*p_target = *p_source++;
		}
	}

	r = p->columns;
	p->columns = p->rows;
	p->rows = r;

	memcpy(p->array, new_array, sizeof(double) * size);
	free(new_array);
	return true;
}
//=====================================================================
//=====================================================================
// Example worksheet functions
//=====================================================================
//=====================================================================
// This worksheet function returns the transposed array via the
// first argument modified in place.  It needs to be declared as
// returning void, so a wrapper to the function transpose_xl4_array()
// is required.
//=====================================================================
void __stdcall xl_array_transpose(xl4_array *input)
{
#pragma EXPORT

	transpose_xl4_array(input); // ignore the return value
}
//====================================================================
xl12_array * __stdcall xl12_array_example1(INT32 rows, INT32 columns)
{
#pragma EXPORT

	size_t size;
	if(!get_xl_array_size(rows, columns, size))
		return NULL;

// Get a pointer to thread-local static storage
	xl12_array *p_array = get_thread_local_xl12_array(size);

	if(!p_array) // Could not get a thread-local copy
		return NULL;

	p_array->rows = rows;
	p_array->columns = columns;

	for(size_t i = 0; i < size; i++)
		p_array->array[i] = i / 10.0;

	return p_array;
}
//=====================================================================
// This worksheet function returns the transposed array via the
// first argument modified in place.  It needs to be declared as
// returning void, so a wrapper to the function transpose_xl12_array()
// is required.
//=====================================================================
void __stdcall xl12_array_transpose(xl12_array *input)
{
#pragma EXPORT

	transpose_xl12_array(input); // ignore the return value
}
//=====================================================================
xl4_array * __stdcall xl_array_example1(int rows, int columns)
{
#pragma EXPORT
// Get a pointer to thread-local static storage. Memory allocation is
// taken care of within get_thread_local_xl4_array()
	size_t size = rows * columns;
	xl4_array *p_array = get_thread_local_xl4_array(size);

	if(!p_array) // Could not get a thread-local copy
		return NULL;

	p_array->rows = rows;
	p_array->columns = columns;

	for(size_t i = 0; i < size; i++)
		p_array->array[i] = i / 10.0;
	return p_array;
}
//=====================================================================
void __stdcall xl_array_example2(xl4_array *p_array)
{
#pragma EXPORT
	if(!p_array || !p_array->rows
	|| !p_array->columns || p_array->columns > MAX_XL11_COLS)
		return;

	int size = p_array->rows * p_array->columns;

// Change the values in the array
	for(int i = 0; i < size; i++)
		p_array->array[i] /= 10.0;
}
//=====================================================================
void __stdcall xl_array_example3(xl4_array *p_array)
{
#pragma EXPORT
	if(!p_array || !p_array->rows
	|| !p_array->columns || p_array->columns > MAX_XL11_COLS)
		return;

	int size = p_array->rows * p_array->columns;

// Change the shape of the array but not the size
	int temp = p_array->rows;
	p_array->rows = p_array->columns;
	p_array->columns = temp;

// Change the values in the array
	for(int i = 0; i < size; i++)
		p_array->array[i] /= 10.0;
}
//=====================================================================
void __stdcall xl_array_example4(xl4_array *p_array)
{
#pragma EXPORT
	if(!p_array || !p_array->rows
	|| !p_array->columns || p_array->columns > MAX_XL11_COLS)
		return;

// Reduce the size of the array
	if(p_array->rows > 1)
		p_array->rows--;

	if(p_array->columns > 1)
		p_array->columns--;

	int size = p_array->rows * p_array->columns;

// Change the values in the array
	for(int i = 0; i < size; i++)
		p_array->array[i] /= 10.0;
}
//=====================================================================
