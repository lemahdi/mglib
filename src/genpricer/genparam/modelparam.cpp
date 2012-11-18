#include "genpricer/genparam/modelparam.h"


using namespace std;
using namespace MG;


/*
 * Pricing Model Base Class
 */
MG_ModelParam::MG_ModelParam(const MG_ModelParam& aRight) : MG_XLObject(aRight)
{}

void MG_ModelParam::Swap(MG_ModelParam& aRight)
{}

MG_ModelParam::MG_ModelParam()
{}

MG_ModelParam::~MG_ModelParam()
{}
