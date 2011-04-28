#include "mgxll/cpp_iface/justatest.h"
#include "xlw/mgw/xlcache.h"


using namespace std;
using namespace MG;


short JustATest()
{
	Coord vTop, vBottom;
	MG_XL_Cached::GetCaller(vTop, vBottom);

	string vSheetName;
	MG_XL_Cached::GetSheetNm(vSheetName);

	return 5;
}