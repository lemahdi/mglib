#include "mgnova/utils/utils.h"


using namespace std;


namespace MG_utils
{
	/* Transforming an integer to a string */
	string itoa(const int& vInteger)
	{
		unsigned int vTmp = vInteger;
		vector<char> vInts;
		while(vTmp != 0)
		{
			vInts.push_back((const char)(vTmp%10) + '0');
			vTmp /= 10;
		}
		size_t vSize = vInts.size();
		string vIntStr;
		unsigned int vNeg = 0;
		if (vInteger < 0)
		{
			vNeg = 1;
			vIntStr[0] = '-';
		}
		vIntStr.resize(vSize+vNeg);
		for(size_t i=vSize-1; i>=0&&i<vSize; i--)
			vIntStr[i] = vInts[vSize-1+vNeg-i];

		return vIntStr;
	}

	/* Transforming a string to an all case upper string */
	string ToUpper(const string& aStr)
	{
		string vStr = aStr;
		char vC;
		unsigned int vWidth = (unsigned int)('A' - 'a');
		for(size_t i=0; i<vStr.size(); i++)
		{
			vC = vStr[i];
			if (vC >= 'a' && vC <= 'z')
				vStr[i] = vC + (char)vWidth;
		}
		return vStr;
	}
}
