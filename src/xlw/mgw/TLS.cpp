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
// This module contains the thread-local storage (TLS) structure and
// initialisation and retrieval functions to enable the add-in to manage
// thread-local 'static' data and so export thread-safe functions that
// Excel 12+ can call multi-threadedly.
//============================================================================
//============================================================================
#include <windows.h>
#pragma warning (disable : 4311)
#include "xlw/mgw/cpp_xloper.h"

#if _MSC_VER > 1200 // Later than Visual Studio 6.0
#define EXPORT comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)
#else // else need to use DEF file or __declspec(dllexport)
#define EXPORT
#endif


// Relies on the TLS API functions 

struct TLS_data
{
	DWORD ThreadID; // Not required, but useful for debugging

// Used to return thread-local persistent xloper to worksheet function
// calls that do not require the value to persist from call to call, i.e.,
// that are reusable by other functions called by this thread. 
	xloper xloper_shared_ret_val;
	xloper12 xloper12_shared_ret_val;

// Used to return thread-local static xl4_array and xl12_array
// pointers, to which dynamic memory is assigned that persists
// from one call to the next.  This enables memory allocated in
// the previous call to be freed on entry before pointer re-use. 
	xl4_array *xl4_array_shared_ptr;
	xl12_array *xl12_array_shared_ptr;

// Add other required thread-local static data here...
};

//============================================================
// Index to address of shared memory. The one index is used for
// all threads that use this instance of the DLL.  Using this,
// threads can allocate and obtain their own thread-local 
// storage.
//============================================================
DWORD TlsIndex;

//============================================================
// Get the address of the TLS_data structure allocated for this thread.
//============================================================
TLS_data *get_TLS_data(void)
{
// Get a pointer to this thread's static memory
	void *pTLS = TlsGetValue(TlsIndex);
	if(!pTLS) // No TLS memory for this thread yet
	{
		if((pTLS = calloc(1, sizeof(TLS_data)))==NULL)
		// Display some error message (omitted)
			return NULL;
		TlsSetValue(TlsIndex, pTLS); // Associate with this thread
	}
	return (TLS_data *)pTLS;
}
//============================================================
xloper *get_thread_local_xloper(void)
{
	TLS_data *pTLS = get_TLS_data();

	if(pTLS)
		return &(pTLS->xloper_shared_ret_val);

	return NULL;
}
//============================================================
xloper12 *get_thread_local_xloper12(void)
{
	TLS_data *pTLS = get_TLS_data();

	if(pTLS)
		return &(pTLS->xloper12_shared_ret_val);

	return NULL;
}
//============================================================
xl4_array *get_thread_local_xl4_array(size_t size)
{
	if(size <= 0)
		return NULL;

	TLS_data *pTLS = get_TLS_data();
	if(!pTLS)
		return NULL;

	if(pTLS->xl4_array_shared_ptr)
		free(pTLS->xl4_array_shared_ptr);

	size_t mem_size = sizeof(xl4_array) + (size - 1) * sizeof(double);
	return pTLS->xl4_array_shared_ptr = (xl4_array *)malloc(mem_size);
}
//============================================================
xl12_array *get_thread_local_xl12_array(size_t size)
{
	if(size <= 0)
		return NULL;

	TLS_data *pTLS = get_TLS_data();
	if(!pTLS)
		return NULL;

	if(pTLS->xl12_array_shared_ptr)
		free(pTLS->xl12_array_shared_ptr);

	size_t mem_size = sizeof(xl12_array) + (size - 1) * sizeof(double);
	return pTLS->xl12_array_shared_ptr = (xl12_array *)malloc(mem_size);
}
//============================================================
// Returns 4-col array: {ThreadID, StartTime, EndTime, RtnOpAdr}
// Trigger is used to force dependency and thereby test XL12 MT
// calculation engine.  Function is registered with type '$' to
// inform Excel that it is thread-safe and eligible for MT calc
double __stdcall get_system_time_C(long trigger);

xloper * __stdcall ThreadSafeFunction(xloper *pTrigger, xloper *pLoopCount)
{
#pragma EXPORT

	cpp_xloper LoopCount(pLoopCount);
	double rtn_vals[4];

	rtn_vals[0] = (double)GetCurrentThreadId();
	rtn_vals[1] = get_system_time_C(0);

	for(int i = (int)LoopCount; i--;)
	{
		// Waste some time
		for(int j = 0; j < 1000;)
			j++;
	}
	rtn_vals[2] = get_system_time_C(0);
	cpp_xloper RetVal((RW)1, (COL)4, rtn_vals);
// Get the thread-local static xloper
	xloper *p_rtn_op = RetVal.ExtractXloper();
// Set the value of the fourth element to the address of this xltypeMulti oper
	p_rtn_op->val.array.lparray[3].xltype = xltypeNum;
	p_rtn_op->val.array.lparray[3].val.num = (double)(DWORD)p_rtn_op;
	return p_rtn_op;
}

// Same as above except that this function is not registered with
// type '$' and so will only be called on the main Excel thread

xloper * __stdcall ThreadUnsafeFunction(xloper *pTrigger, xloper *pLoopCount)
{
#pragma EXPORT

	return ThreadSafeFunction(pTrigger, pLoopCount);
}
//============================================================
//============================================================
// xloper12 versions of these two functions
//============================================================
//============================================================
xloper12 * __stdcall ThreadSafeFunction12(xloper12 *pTrigger, xloper12 *pLoopCount)
{
#pragma EXPORT

	if(pLoopCount->xltype != xltypeNum)
		return p_xl12ErrNum;

	xloper12 *p_ret_oper = get_thread_local_xloper12();

	if(!p_ret_oper)
		return p_xl12ErrValue;

// return 4 args
	p_ret_oper->xltype = xltypeMulti | xlbitDLLFree;
	p_ret_oper->val.array.rows = 1;
	p_ret_oper->val.array.columns = 4;
	p_ret_oper->val.array.lparray = (xloper12 *)calloc(4, sizeof(xloper12));

	int i;
	for(i = 0; i < 4; i++)
		p_ret_oper->val.array.lparray[i].xltype = xltypeNum;

	p_ret_oper->val.array.lparray[0].val.num = (double)GetCurrentThreadId();
	p_ret_oper->val.array.lparray[1].val.num = get_system_time_C(0);

	for(i = (int)pLoopCount->val.num; i--;)
	{
		// Waste some time
		for(int j = 0; j < 1000;)
			j++;
	}
	p_ret_oper->val.array.lparray[2].val.num = get_system_time_C(0);
	p_ret_oper->val.array.lparray[3].val.num = (double)(DWORD)p_ret_oper;
	return p_ret_oper;
}
//============================================================

//============================================================
// Same as above except that this function is not registered with type '$'
// and so will only be called on the main Excel thread
//============================================================
xloper12 * __stdcall ThreadUnsafeFunction12(xloper12 *pTrigger, xloper12 *pLoopCount)
{
#pragma EXPORT

	return ThreadSafeFunction12(pTrigger, pLoopCount);
}
//============================================================
// Called from DllMain, this function intialises and releases
// resources required for TLS storage.  See also MSDN article
// "Using Thread Local Storage in a Dynamic-Link Library"
// msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/
//	using_thread_local_storage_in_a_dynamic_link_library.asp
//============================================================
// get_TLS_data() is relied on to allocate a block of memory per
// thread.  This seems to get round a problem of the fact that
// the recalculation threads used by Excel (other than Excel's
// main thread) do not appear to fire DllMain events DLL_THREAD_ATTACH
// or DLL_THREAD_DETTACH

BOOL TLS_Action(DWORD DllMainCallReason)
{
	switch (DllMainCallReason)
	{ 
	case DLL_PROCESS_ATTACH: // The DLL is being loaded
		if((TlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES) // Allocate TLS index
			return FALSE;
		break;

	case DLL_PROCESS_DETACH: // The DLL is being unloaded 
		TlsFree(TlsIndex); // Release the TLS index.
		break;
	}
	return TRUE;
}
