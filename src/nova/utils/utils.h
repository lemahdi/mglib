/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.17
 * Date					: 26 DEC 2010
 * Purpose				: Some useful functions
 * Author				: MM Akkouh
 * Notes				: Functions
 *							- itoa: transform an integer to a string
 *							- ftoa: transform a float to a string
 */


#pragma once


#include <iostream>
#include <string>
#include <vector>

#ifndef __CYGWIN__
#include "xlw/CellMatrix.h"
#endif

#include "nova/glob/genericdate.h"
#include "nova/wrapper/vector.h"
#include "nova/wrapper/matrix.h"


namespace MG_utils
{

extern std::string itoa(const int& vInteger);

extern std::string ftoa(const double& vDouble);

extern std::string ToUpper(const std::string& aStr);

extern long FromXLDateToJulianDay(const double& aXLDate);

extern double FromJulianDayToXLDate(const long& aJulianDay);

extern std::vector<double> DoublePtrAsVector(const double* aDbl, const size_t& aSize);

#ifndef __CYGWIN__
extern std::vector<double> FromCellMatrixToVectorDouble(const xlw::CellMatrix& aCM, const size_t& aIndex);

extern xlw::MJMatrix FromCellMatrixToMJMatrix(const xlw::CellMatrix& aCM);

extern std::vector<std::string> FromCellMatrixToVectorStr(const xlw::CellMatrix& aCM, const std::vector<bool>& aIsDate = std::vector<bool>());

extern std::vector<MG::MG_Date> FromCellMatrixToVectorDate(const xlw::CellMatrix& aCM, const size_t& aIndex);

extern std::vector<int> FromCellMatrixToInterpolVector(const xlw::CellMatrix& aCM);
#endif

extern MG::MG_Vector FromCellMatrixToMGVectorDouble(const xlw::CellMatrix& aCM, const size_t& aIndex);

extern MG::MG_Matrix FromCellMatrixToMGMatrix(const xlw::CellMatrix& aCM);

extern MG::MG_Vector FromCellMatrixToMGVectorDate(const xlw::CellMatrix& aCM, const size_t& aIndex);

extern void SplitFrequency(const std::string& aTimesFreq, int& aTimes, std::string& aFreq);

extern void GetFromIndexName(const MG::INDEX_NAME& aIndexNm, int& aPeriod, unsigned int& aTimes, MG::FREQUENCY_NAME& aFreqNm);

extern MG::FREQUENCY_NAME GetFrequencyFromIndex(const MG::INDEX_NAME& aIdxNm);

extern bool BuildGenericDates(const std::string& aStStr, MG::MG_GenericDate& aSt, const std::string& aEdStr, MG::MG_GenericDate& aEd);

extern MG::MG_ObjectPtr CloneCPtr(const MG::MG_ObjectPtr& aCPtr);

namespace Vector
{

extern void VectorPlus (std::vector<double>& aDest, const std::vector<double>& aV);
extern void VectorPlus (std::vector<double>& aDest, const double& aVal);

extern void VectorMinus(std::vector<double>& aDest, const std::vector<double>& aV);
extern void VectorMinus(std::vector<double>& aDest, const double& aVal);

extern void VectorMult(std::vector<double>& aDest, const std::vector<double>& aV);
extern void VectorMult(std::vector<double>& aDest, const double& aVal);

extern void VectorDiv(std::vector<double>& aDest, const std::vector<double>& aV);
extern void VectorDiv(std::vector<double>& aDest, const double& aVal);

extern double VectorSum(const std::vector<double>& aV);
extern double VectorSumProduct(const std::vector<double>& aV1, const std::vector<double>& aV2);

}

}
