#include "mgnova/utils/utils.h"
#include "mgnova/date.h"
#include "mgnova/exception.h"
#include "mgnova/argconvdef.h"

#include <sstream>
#include <math.h>


using namespace std;
using namespace MG;
#ifndef __CYGWIN__
using namespace xlw;
#endif


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

	/* Transforming an integer to a string */
	string ftoa(const double& vDouble)
	{
		string vRes = "0";
		if (vDouble == 0.)
			return vRes;

		double vBaseInt;
		double vMantisse = modf(vDouble, &vBaseInt);
		vRes = itoa((int)vBaseInt);
		if (vMantisse != 0.)
		{
			vRes += ".";
			vRes += itoa((int)vMantisse);
		}
		return vRes;
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

	/* converting an excel date to a julian day */
	long FromXLDateToJulianDay(const double& aXLDate)
	{
		MG_Date vDate(1899, 12, 31);
		vDate += int(aXLDate)-1;
		return vDate.GetJulianDay();
	}

	/* converting a julian day to an excel date */
	double FromJulianDayToXLDate(const long& aJulianDay)
	{
		MG_Date vDate1(1899, 12, 30), vDate2(aJulianDay);
		return vDate2.GetJulianDay()-vDate1.GetJulianDay();
	}

	/* converting a double* vector to std::vector */
	vector<double> DoublePtrAsVector(const double* aDbl, const size_t& aSize)
	{
		vector<double> vRes(aSize);
		for(size_t i=0; i<aSize; ++i)
			vRes[i] = aDbl[i];
		return vRes;
	}

#ifndef __CYGWIN__
	/* converting a CellMatrix to a std::vector<double> */
	vector<double> FromCellMatrixToVectorDouble(const CellMatrix& aCM, const size_t& aIndex)
	{
		if (aCM.ColumnsInStructure()!=1 && aCM.RowsInStructure()!=1)
			MG_THROW("CellMatrix should be a one row or column structure");
		bool vIsRow = aCM.ColumnsInStructure() == 1;

		size_t vSize = vIsRow ? aCM.RowsInStructure() : aCM.ColumnsInStructure();
		vector<double> vRes(vSize);
		if (vIsRow)
		{
			for(size_t i=0; i<vSize; ++i)
				vRes[i] = aCM(i, aIndex).NumericValue();
			return vRes;
		}
		for(size_t i=0; i<vSize; ++i)
			vRes[i] = aCM(aIndex, i).NumericValue();
		return vRes;
	}

	/* converting a CellMatrix to a xlw::MJMatrix */
	MJMatrix FromCellMatrixToMJMatrix(const CellMatrix& aCM)
	{
		size_t vRows(aCM.RowsInStructure()), vCols(aCM.ColumnsInStructure());
		MJMatrix vMat(vRows, vCols);
		for(size_t i=0; i<vRows; ++i)
			for(size_t j=0; j<vCols; ++j)
				vMat(i, j) = aCM(i, j).NumericValue();
		return vMat;
	}

	/* converting a CellMatrix to a std::vector<string> */
	vector<string> FromCellMatrixToVectorStr(const CellMatrix& aCM, const vector<bool>& aIsDate)
	{
		size_t vRows(aCM.RowsInStructure()), vCols(aCM.ColumnsInStructure());
		vector<string> vVect(vRows*vCols);
		CellValue vTmpCell;
		double vTmpDbl;
		MG_Date vTmpDate;
		for(size_t i=0; i<vRows; ++i)
		{
			for(size_t j=0; j<vCols; ++j)
			{
				vTmpCell = aCM(i,j);
				if (vTmpCell.IsAString())
				{
					vVect[j+i*vCols] = aCM(i,j);
					continue;
				}
				if (vTmpCell.IsANumber())
				{
					vTmpDbl = aCM(i,j);
					if (aIsDate[j])
					{
						vTmpDate = MG_Date(FromXLDateToJulianDay(vTmpDbl));
						vVect[j+i*vCols] = vTmpDate.ToString('/');
						continue;
					}
					vVect[j+i*vCols] = ftoa(vTmpDbl);
					continue;
				}
			}
		}
		return vVect;
	}

	/* converting a CellMatrix to a std::vector<MG_Date> */
	vector<MG_Date> FromCellMatrixToVectorDate(const CellMatrix& aCM, const size_t& aIndex)
	{
		if (aCM.ColumnsInStructure()!=1 && aCM.RowsInStructure()!=1)
			MG_THROW("CellMatrix should be a one row or column structure");
		bool vIsRow = aCM.ColumnsInStructure() == 1;

		size_t vSize = vIsRow ? aCM.RowsInStructure() : aCM.ColumnsInStructure();
		vector<MG_Date> vRes(vSize);
		if (vIsRow)
		{
			for(size_t i=0; i<vSize; ++i)
				vRes[i] = MG_Date(FromXLDateToJulianDay(aCM(i, aIndex).NumericValue()));
			return vRes;
		}
		for(size_t i=0; i<vSize; ++i)
			vRes[i] = MG_Date(FromXLDateToJulianDay(aCM(aIndex, i).NumericValue()));
		return vRes;
	}

	/* converting a CellMatrix of interpolation types to a sid::vector<int> */
	vector<int> FromCellMatrixToInterpolVector(const CellMatrix& aCM)
	{
		if (aCM.Size() > maxInterpoltypesNb)
		{
			ostringstream vOs;
			vOs << "Maximum number of interpolations is " << maxInterpoltypesNb << ", please advise.";
			MG_THROW(vOs.str());
		}
		vector<string> vInterpolTypesStr = FromCellMatrixToVectorStr(aCM);
		size_t vSize(vInterpolTypesStr.size());
		vector<int> vInterpolTypesInt(vSize);
		for(size_t i=0; i<vSize; ++i)
			vInterpolTypesInt[i] = InterpolMethodConvertor[vInterpolTypesStr[i]];
		return vInterpolTypesInt;
	}
#endif
	
	/* converting a CellMatrix to a MG_Vector */
	MG_Vector FromCellMatrixToMGVectorDouble(const CellMatrix& aCM, const size_t& aIndex)
	{
		if (aCM.ColumnsInStructure()!=1 && aCM.RowsInStructure()!=1)
			MG_THROW("CellMatrix should be a one row or column structure");
		bool vIsRow = aCM.ColumnsInStructure() == 1;

		size_t vSize = vIsRow ? aCM.RowsInStructure() : aCM.ColumnsInStructure();
		MG_Vector vRes(vSize);
		if (vIsRow)
		{
			for(size_t i=0; i<vSize; ++i)
				vRes[i] = aCM(i, aIndex).NumericValue();
			return vRes;
		}
		for(size_t i=0; i<vSize; ++i)
			vRes[i] = aCM(aIndex, i).NumericValue();
		return vRes;
	}

	/* converting a CellMatrix to a MG_Matrix */
	MG_Matrix FromCellMatrixToMGMatrix(const CellMatrix& aCM)
	{
		size_t vRows(aCM.RowsInStructure()), vCols(aCM.ColumnsInStructure());
		MG_Matrix vMat(vRows, vCols);
		for(size_t i=0; i<vRows; ++i)
			for(size_t j=0; j<vCols; ++j)
				vMat(i, j) = aCM(i, j).NumericValue();
		return vMat;
	}

	/* converting a CellMatrix to a MG_Vector */
	MG_Vector FromCellMatrixToMGVectorDate(const CellMatrix& aCM, const size_t& aIndex)
	{
		if (aCM.ColumnsInStructure()!=1 && aCM.RowsInStructure()!=1)
			MG_THROW("CellMatrix should be a one row or column structure");
		bool vIsRow = aCM.ColumnsInStructure() == 1;

		size_t vSize = vIsRow ? aCM.RowsInStructure() : aCM.ColumnsInStructure();
		MG_Vector vRes(vSize);
		if (vIsRow)
		{
			for(size_t i=0; i<vSize; ++i)
				vRes[i] = FromXLDateToJulianDay(aCM(i, aIndex).NumericValue());
			return vRes;
		}
		for(size_t i=0; i<vSize; ++i)
			vRes[i] = FromXLDateToJulianDay(aCM(aIndex, i).NumericValue());
		return vRes;
	}

	/* spliting a xM to x and M */
	void SplitFrequency(const string& aTimesFreq, int& aTimes, string& aFreq)
	{
		size_t vLen = aTimesFreq.length();

		if (vLen == 1)
		{
			aTimes = 1;
			aFreq = aTimesFreq;
			return;
		}

		aFreq = aTimesFreq.at(vLen-1);
		string vTimes = aTimesFreq.substr(0, vLen-1);
		aTimes = atoi(vTimes.c_str());

		if (aFreq!="Y" && aFreq!="M" && aFreq!="W" && aFreq!="D")
			MG_THROW("Frequency should be Y, M, W or D");
	}

}

