#include "mgnova/object.h"


using namespace MG;


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
