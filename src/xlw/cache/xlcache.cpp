/*
 * Copyright : 2010-2024 by MG
 * Version   : 0.1.22 (ported to XLW 6.x – uses XlfServices instead of cpp_xloper)
 * Purpose   : MG_XL_Cached – Excel caller-info helpers
 */

#include "xlw/cache/xlcache.h"
#include <xlw/XlfOper.h>
#include <xlw/XlfRef.h>
#include <xlw/XlfServices.h>
#include "nova/glob/exception.h"

using namespace MG;
using namespace xlw;

void MG_XL_Cached::GetCaller(Coord& aTopC, Coord& aBottomC)
{
    // GetCallingCell() returns an XlfOper wrapping an xltypeRef/xltypeSRef
    XlfOper caller = XlfServices.Information.GetCallingCell();
    XlfRef ref = caller.AsRef("GetCaller");

    aTopC.first = ref.GetRowBegin();
    aTopC.second = ref.GetColBegin();
    aBottomC.first = ref.GetRowEnd() - 1;   // GetRowEnd() is one-past-end
    aBottomC.second = ref.GetColEnd() - 1;
}

void MG_XL_Cached::GetSheetNm(std::string& aSheetName)
{
    XlfOper caller = XlfServices.Information.GetCallingCell();
    aSheetName = XlfServices.Information.GetSheetName(caller);
}
