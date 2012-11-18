#include "mggenpricer/infra/arg.h"


using namespace std;
using namespace MG;


MG_Arg::MG_Arg(void) : myType(ARG_ERR) {}
MG_Arg::MG_Arg(const double& aDbl) : myType(ARG_DOUBLE), myDouble(aDbl) {}
MG_Arg::MG_Arg(const std::string& aStr) : myType(ARG_STRING), myString(aStr) {}
MG_Arg::MG_Arg(const MG_Date& aDt) : myType(ARG_DATE), myDate(aDt), myDouble(aDt.GetJulianDay()) {}
MG_Arg::MG_Arg(const MG_StdVectDblPtr& aVDbl) : myType(ARG_V_DOUBLE), myVDouble(aVDbl) {}
MG_Arg::MG_Arg(const bool& aBool) : myType(ARG_BOOL), myBool(aBool) {}
MG_Arg::MG_Arg(const MG_StdVectBoolPtr& aVBool) : myType(ARG_V_BOOL), myVBool(aVBool) {}

//MG_Arg::~MG_Arg() {}


