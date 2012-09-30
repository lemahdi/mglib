#include "mgsecurity/security.h"
#include "mgnova/utils/utils.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Base Security class */
MG_Security::MG_Security() : MG_XLObject()
{}

void MG_Security::Swap(MG_Security& aRight)
{
	MG_XLObject::Swap(aRight);
}

MG_Security::~MG_Security()
{}

