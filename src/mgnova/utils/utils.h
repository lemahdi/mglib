/*
 * Copyright			: 2010 by MG
 * File					: utils.h
 * Version				: 0.1.17
 * Last changed			: 26 DEC 2010
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

#include "mgnova/date.h"


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

}
