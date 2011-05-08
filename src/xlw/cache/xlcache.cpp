#include "xlw/cache/xlcache.h"
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
	wstring vSN = wstring(vSheetName);
	string vTmp(vSN.begin(), vSN.end());

	// look for the beginning of the name
	string::iterator vIt(vTmp.begin());
	size_t vStart(0);
	while (vIt != vTmp.end())
	{
		if (*vIt == '[')
			break;
		++vStart;
		++vIt;
	}

	// looking for the ending of the name
	size_t vNot = vTmp.find_first_of("]");
	vIt = vTmp.end()-1;
	size_t vEnd(0);
	while (vIt != vTmp.begin()+vNot+1)
	{
		if (*vIt>='a'&&*vIt<='Z' || *vIt>='0'&&*vIt<='9')
			break;
		++vEnd;
		--vIt;
	}

	aSheetName.assign(vTmp.begin()+vStart, vTmp.end()-vEnd);
}

