#include "mgnova/object.h"

#include "mginfra/func.h"
#include "mgnumerical/distributions.h"
#include "mgnova/calendar.h"

using namespace MG;


bool MG_Object::Dummy()
{
	MG_SFuncBuilder::Instance()->Init();
	//MG_RCalendar::Init();
	return true;
}
bool MG_Object::ourDummy = MG_Object::Dummy();

MG_XLObject::MG_XLObject() : myXLId(NON_PERSISTENT_XL_OBJECT)
{}

MG_XLObject::MG_XLObject(	const MG_XLObject &aRight)
						:	myXLName(aRight.myXLName)
						,	myXLId	(NON_PERSISTENT_XL_OBJECT)
{}

void MG_XLObject::Swap(MG_XLObject& aRight)
{
	swap(myXLName, aRight.myXLName);
	myXLId = NON_PERSISTENT_XL_OBJECT;
}
