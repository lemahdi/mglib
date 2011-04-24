
/*
 Copyright (C) 1998, 1999, 2001, 2002, 2003, 2004 J�r�me Lecomte
 Copyright (C) 2007, 2008 Eric Ehlers
 Copyright (C) 2009 Narinder S Claire


 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*!
\file XlfExcel.cpp
\brief Implements the classes XlfExcel.
*/

// $Id: XlfExcel.cpp 474 2008-03-05 15:40:40Z ericehlers $

#include <xlw/XlfExcel.h>
#include <cstdio>
#include <stdexcept>
#include <xlw/XlfOper.h>
#include <xlw/XlfOperImpl4.h>
#include <xlw/XlfOperImpl12.h>
#include <xlw/macros.h>
// Stop header precompilation
#ifdef _MSC_VER
#pragma hdrstop
#endif

#ifndef NDEBUG
#include <xlw/XlfExcel.inl>
#endif

extern "C"
{
    //! Main API function to Excel.
    int (__cdecl *Excel4_)(int xlfn, LPXLOPER operRes, int count, ...);
    //! Main API function to Excel, passing the argument as an array.
    int (__stdcall *Excel4v_)(int xlfn, LPXLOPER operRes, int count, LPXLOPER far opers[]);
}

xlw::XlfExcel *xlw::XlfExcel::this_ = 0;

//! Internal implementation of XlfExcel.
struct xlw::XlfExcelImpl {
    //! Ctor.
    XlfExcelImpl(): handle_(0) {}
    //! Handle to the DLL module.
    HINSTANCE handle_;
};

/*!
You always have the choice with the singleton in returning a pointer or
a reference. By returning a reference and declaring the copy ctor and the
assignment otor private, we limit the risk of a wrong use of XlfExcel
(typically duplication).
*/
xlw::XlfExcel& xlw::XlfExcel::Instance() {
    if (!this_) {
        this_ = new XlfExcel;
        // intialize library first because log displays
        // XLL name in header of log window
        this_->InitLibrary();
    }
    return *this_;
}

/*!
If no title is specified, the message is assumed to be an error log
*/
void xlw::XlfExcel::MsgBox(const char *errmsg, const char *title) {
    LPVOID lpMsgBuf;
    // retrieve message error from system err code
    if (!title) {
        DWORD err = GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL);
        // Process any inserts in lpMsgBuf.
        char completeMessage[255];
        sprintf(completeMessage,"%s due to error %d :\n%s", errmsg, err, (LPCSTR)lpMsgBuf);
        MessageBox(NULL, completeMessage,"XLL Error", MB_OK | MB_ICONINFORMATION);
        // Free the buffer.
        LocalFree(lpMsgBuf);
    } else {
        MessageBox(NULL, errmsg, title, MB_OK | MB_ICONINFORMATION);
    }
    return;
}

/*!
If msg is 0, the status bar is cleared.
*/
void xlw::XlfExcel::SendMessage(const char *msg) {
    if (msg)
        Call(xlcMessage, 0, 2, (LPXLFOPER)XlfOper(true), (LPXLFOPER)XlfOper(msg));
    else
        Call(xlcMessage, 0, 1, (LPXLFOPER)XlfOper(false));
    return;
}

bool xlw::XlfExcel::IsEscPressed() const {
    XlfOper ret;
    Call(xlAbort, ret, 1, (LPXLFOPER)XlfOper(false));
    return ret.AsBool();
}

xlw::XlfExcel::XlfExcel(): impl_(0), offset_(0) {
    impl_ = new XlfExcelImpl();
    return;
}

xlw::XlfExcel::~XlfExcel() {
    FreeMemory(true);
    delete impl_;
    this_ = 0;
    return;
}

bool set_excel12() {
    XLOPER xRet1, xRet2, xTemp1, xTemp2;
    xTemp1.xltype = xTemp2.xltype = xltypeInt;
    xTemp1.val.w = 2;
    xTemp2.val.w = xltypeInt;
    Excel4_(xlfGetWorkspace, &xRet1, 1, &xTemp1);
    Excel4_(xlCoerce, &xRet2, 2, &xRet1, &xTemp2);
    Excel4_(xlFree, 0, 1, &xRet1);
    return (xRet2.val.w == 12);
}

/*!
Load \c XlfCALL32.DLL to interface excel (this library is shipped with Excel)
and link it to the XLL.
*/
void xlw::XlfExcel::InitLibrary() {
    HINSTANCE handle = LoadLibrary("XLCALL32.DLL");
    if (handle == 0)
        throw std::runtime_error("Could not load library XLCALL32.DLL");
    Excel4_ = (int (__cdecl *)(int, struct xloper *, int, ...))GetProcAddress(handle, "Excel4");
    if (Excel4_ == 0)
        throw std::runtime_error("Could not get address of Excel4 callback");
    Excel4v_ = (int (__stdcall *)(int, struct xloper *, int, struct xloper *[]))GetProcAddress(handle, "Excel4v");
    if (Excel4v_ == 0)
        throw std::runtime_error("Could not get address of Excel4v callback");

    excel12_ = set_excel12();
    if (excel12_) {
        static XlfOperImpl12 xlfOperImpl12;
        xlfOperType_ = "Q";
        xlfXloperType_ = "U";
        wStrType_ = "C%";
    } else {
        static XlfOperImpl4 xlfOperImpl4;
        xlfOperType_ = "P";
        xlfXloperType_ = "R";
        wStrType_ = "C";
    }

    impl_->handle_ = handle;
    return;
}

std::string xlw::XlfExcel::GetName() const {
    std::string ret;
    XlfOper xName;
    int err = Call(xlGetName, (LPXLFOPER)xName, 0);
    if (err != xlretSuccess)
        std::cerr << XLW__HERE__ << "Could not get DLL name" << std::endl;
    else
        ret=xName.AsString();
    return ret;
}

#ifdef __MINGW32__
int __cdecl xlw::XlfExcel::Call(int xlfn, LPXLFOPER pxResult, int count, ...) const
#else
int cdecl xlw::XlfExcel::Call(int xlfn, LPXLFOPER pxResult, int count, ...) const
#endif
{
    if (excel12_)
        return Call12v(xlfn, (LPXLOPER12)pxResult, count, (LPXLOPER12 *)(&count + 1));
    else
        return Call4v(xlfn, (LPXLOPER)pxResult, count, (LPXLOPER *)(&count + 1));
}

int __cdecl xlw::XlfExcel::Call4(int xlfn, LPXLOPER pxResult, int count, ...) const {
    return Call4v(xlfn, pxResult, count, (LPXLOPER *)(&count + 1));
}

int __cdecl xlw::XlfExcel::Call12(int xlfn, LPXLOPER12 pxResult, int count, ...) const {
    return Call12v(xlfn, pxResult, count, (LPXLOPER12 *)(&count + 1));
}

/*!
If one (or more) cells referred as argument is(are) uncalculated, the framework
throws an exception and returns immediately to Excel.

If \c pxResult is not 0 and has auxilliary memory, flags it for deletion
with XlfOper::xlbitCallFreeAuxMem.

\sa XlfOper::~XlfOper
*/
int xlw::XlfExcel::Callv(int xlfn, LPXLFOPER pxResult, int count, LPXLFOPER pxdata[]) const {
    if (excel12_)
        return Call12v(xlfn, (LPXLOPER12)pxResult, count, (LPXLOPER12*)pxdata);
    else
        return Call4v(xlfn, (LPXLOPER)pxResult, count, (LPXLOPER*)pxdata);
}

int xlw::XlfExcel::Call4v(int xlfn, LPXLOPER pxResult, int count, LPXLOPER pxdata[]) const {
#ifndef NDEBUG
    for (int i = 0; i<count;++i)
    if (!pxdata[i]) {
        if (xlfn & xlCommand)
            std::cerr << XLW__HERE__ << "xlCommand | " << (xlfn & 0x0FFF) << std::endl;
        if (xlfn & xlSpecial)
            std::cerr << "xlSpecial | " << (xlfn & 0x0FFF) << std::endl;
        if (xlfn & xlIntl)
            std::cerr << "xlIntl | " << (xlfn & 0x0FFF) << std::endl;
        if (xlfn & xlPrompt)
            std::cerr << "xlPrompt | " << (xlfn & 0x0FFF) << std::endl;
        std::cerr << "0 pointer passed as argument #" << i << std::endl;
    }
#endif
    int xlret = Excel4v_(xlfn, pxResult, count, pxdata);
    if (pxResult) {
        int type = pxResult->xltype;

        bool hasAuxMem = (type & xltypeStr ||
                        type & xltypeRef ||
                        type & xltypeMulti ||
                        type & xltypeBigData);
        if (hasAuxMem)
            pxResult->xltype |= XlfOper::xlbitFreeAuxMem;
    }
    return xlret;
}

int xlw::XlfExcel::Call12v(int xlfn, LPXLOPER12 pxResult, int count, LPXLOPER12 pxdata[]) const {
#ifndef NDEBUG
    for (int i = 0; i<count; ++i)
        if (!pxdata[i]) {
            if (xlfn & xlCommand)
                std::cerr << XLW__HERE__ << "xlCommand | " << (xlfn & 0x0FFF) << std::endl;
            if (xlfn & xlSpecial)
                std::cerr << "xlSpecial | " << (xlfn & 0x0FFF) << std::endl;
            if (xlfn & xlIntl)
                std::cerr << "xlIntl | " << (xlfn & 0x0FFF) << std::endl;
            if (xlfn & xlPrompt)
                std::cerr << "xlPrompt | " << (xlfn & 0x0FFF) << std::endl;
            std::cerr << "0 pointer passed as argument #" << i << std::endl;
        }
#endif
    int xlret = Excel12v(xlfn, pxResult, count, pxdata);
    if (pxResult) {
        int type = pxResult->xltype;

        bool hasAuxMem = (type & xltypeStr ||
                          type & xltypeRef ||
                          type & xltypeMulti ||
                          type & xltypeBigData);
        if (hasAuxMem)
            pxResult->xltype |= XlfOper::xlbitFreeAuxMem;
    }
    return xlret;
}

namespace {

//! Needed by IsCalledByFuncWiz.
typedef struct _EnumStruct {
    bool bFuncWiz;
    short hwndXLMain;
}
EnumStruct, FAR * LPEnumStruct;

//! Needed by IsCalledByFuncWiz.
bool CALLBACK EnumProc(HWND hwnd, LPEnumStruct pEnum) {
    const size_t CLASS_NAME_BUFFER = 50;

    // first check the class of the window.  Will be szXLDialogClass
    // if function wizard dialog is up in Excel
    char rgsz[CLASS_NAME_BUFFER];
    GetClassName(hwnd, (LPSTR)rgsz, CLASS_NAME_BUFFER);
    if (2 == CompareString(MAKELCID(MAKELANGID(LANG_ENGLISH,
        SUBLANG_ENGLISH_US),SORT_DEFAULT), NORM_IGNORECASE,
        (LPSTR)rgsz,  (lstrlen((LPSTR)rgsz)>lstrlen("bosa_sdm_XL"))
        ? lstrlen("bosa_sdm_XL"):-1, "bosa_sdm_XL", -1)) {

        if(LOWORD( GetParent(hwnd)) == pEnum->hwndXLMain) {
            pEnum->bFuncWiz = TRUE;
            return false;
        }
    }
    // no luck - continue the enumeration
    return true;
}

} // empty namespace

bool xlw::XlfExcel::IsCalledByFuncWiz() const {
    XLOPER xHwndMain;
    EnumStruct enm;

    if (Excel4_(xlGetHwnd, &xHwndMain, 0) == xlretSuccess) {
        enm.bFuncWiz = false;
        enm.hwndXLMain = xHwndMain.val.w;
        EnumWindows((WNDENUMPROC) EnumProc,
            (LPARAM) ((LPEnumStruct)  &enm));
        return enm.bFuncWiz;
    }
    return false;    //safe case: Return false if not sure
}

