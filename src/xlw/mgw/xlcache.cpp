#include "xlw/mgw/xlcache.h"
#include "xlw/mgw/cpp_xloper.h"
#include "mgnova/exception.h"

#include <locale>
#include <sstream>


using namespace MG;
using namespace std;


void MG_XL_Cached::GetCaller(Coord& aTopC, Coord& aBottomC)
{
	cpp_xloper vRetCaller;
	if (vRetCaller.Excel(xlfCaller) != xlretSuccess)
		MG_THROW("Cannot call Excel function xlfCaller.");
	aTopC.first		= vRetCaller.GetTopRow();
	aTopC.second	= vRetCaller.GetLeftColumn();
	aBottomC.first	= vRetCaller.GetBottomRow();
	aBottomC.second	= vRetCaller.GetRightColumn();
}

void MG_XL_Cached::GetSheetNm(string& aSheetName)
{
	cpp_xloper vRetCaller, vRetSheetname;

	if (vRetCaller.Excel(xlfCaller) != xlretSuccess)
		MG_THROW("Cannot call Excel function xlfCaller.");

	if (vRetSheetname.Excel(xlSheetNm, 1, &vRetCaller) != xlretSuccess)
		MG_THROW("Cannot call Excel function xlSheetNm.");

	XCHAR* vSheetName = vRetSheetname.ExtractXloper12()->val.str;
	ostringstream vOSS;
	while (*vSheetName != L'\0')
		vOSS << use_facet<ctype<XCHAR> >(locale()).narrow(*vSheetName++, '?');
	aSheetName = vOSS.str();
}

