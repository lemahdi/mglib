//// 
//// Autogenerated by xlw 
//// Do not edit this file, it will be overwritten 
//// by InterfaceGenerator 
////

#include <xlw/MyContainers.h>
#include <xlw/CellMatrix.h>
#include "justatest.h"
#include <xlw/xlw.h>
#include <xlw/XlFunctionRegistration.h>
#include <stdexcept>
#include <xlw/XlOpenClose.h>
#include <ctime>
namespace {
const char* LibraryName = "MyTestLibrary";
};

// dummy function to force linkage
namespace {
void DummyFunction()
{
xlAutoOpen();
xlAutoClose();
}
}

// registrations start here


namespace
{
XLRegistration::Arg
JustATestArgs[]=
{
 { "","" } 
};
  XLRegistration::XLFunctionRegistrationHelper
registerJustATest("xlJustATest",
"JustATest",
" just a test ",
LibraryName,
JustATestArgs,
0
,false
);
}



extern "C"
{
LPXLFOPER EXCEL_EXPORT
xlJustATest(
)
{
EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

short result(
	JustATest());
return XlfOper(result);
EXCEL_END
}
}



//////////////////////////
