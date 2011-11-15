#include "mgmktdata/robot.h"


using namespace std;
using namespace MG;


/*
 * Robot Base Class
 */
MG_Robot::MG_Robot(const MG_Robot& aRight) : MG_XLObject(aRight), myAsOf(aRight.myAsOf)
{}

void MG_Robot::Swap(MG_Robot& aRight)
{
	myAsOf.Swap(aRight.myAsOf);
}

MG_Robot::~MG_Robot()
{}

MG_Robot::MG_Robot(const MG_Date& aAsOf, const vector<MG_MarketDataPtr>& aMktData) : myAsOf(aAsOf)
{
	myXLName = MG_ROBOT_XL_NAME;

	for(size_t i=0; i<aMktData.size(); ++i)
		AddMktData(aMktData[i]);
}

void MG_Robot::AddMktData(const MG_MarketDataPtr& aMktData)
{
	string vKey = CreateKey(*aMktData);
	myKeyData.insert(make_pair(vKey, aMktData)); // be aware here, aMktData is not inserted if vKey already exist
}

MG_MarketDataPtr MG_Robot::GetMktData(const string& aType, const string& aCcy, const string& aUnderIndex)
{
	MG_EmptyMD vEmpty(aType, aCcy, aUnderIndex);
	string vKey = CreateKey(vEmpty);
	
	MG_MarketDataPtr vMktData(NULL);
	mapStrMkt::iterator vIt = myKeyData.find(vKey);
	if (vIt != myKeyData.end())
		vMktData = vIt->second;

	return vMktData;
}

string MG_Robot::CreateKey(const MG_MarketData& aMktData) const
{
	string vKey = aMktData.GetType() + "_" + aMktData.GetCurrency() + "_" + aMktData.GetUnderIndex();
	return vKey;
}
