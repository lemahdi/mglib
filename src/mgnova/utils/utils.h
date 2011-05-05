/*
 * Copyright			: 2010 by MG
 * File					: utils.h
 * Version				: 0.1.17
 * Last changed			: 26 DEC 2010
 * Purpose				: Some useful functions
 * Author				: MM Akkouh
 * Notes				: Functions
 *							- itoa: transform an integer to a string
 */


#pragma once


#include <iostream>
#include <string>
#include <vector>

#include "xlw/CellMatrix.h"


namespace MG_utils
{

extern std::string itoa(const int& vInteger);

extern std::string ToUpper(const std::string& aStr);

extern long FromXLDateToJulianDay(const double& aXLDate);

extern double FromJulianDayToXLDate(const long& aJulianDay);

extern std::vector<double> DoublePtrAsVector(const double* aDbl, const size_t& aSize);

extern std::vector<double> FromCellMatrixToVectorDouble(const xlw::CellMatrix& aCM, const size_t& aIndex, const bool& aIsRow = true);

extern xlw::MJMatrix FromCellMatrixToMJMatrix(const xlw::CellMatrix& aCM);

}
