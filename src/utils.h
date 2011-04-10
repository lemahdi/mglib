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


namespace MG_utils
{

extern std::string itoa(const int& vInteger);

extern std::string ToUpper(const std::string& aStr);

}
