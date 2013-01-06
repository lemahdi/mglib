//
//
//                                    XlFunctionRegistriation.cpp
//
//
/*
 Copyright (C) 2006 Mark Joshi
 Copyright (C) 2007, 2008 Eric Ehlers

 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/


#include <xlw/XlFunctionRegistration.h>
#include <xlw/xlfFuncDesc.h>
#include <xlw/xlfArgDescList.h>

using namespace xlw;
using namespace XLRegistration;

XLFunctionRegistrationData::XLFunctionRegistrationData(const std::string& FunctionName_,
                     const std::string& ExcelFunctionName_,
                     const std::string& FunctionDescription_,
                     const std::string& Library_,
                     const Arg Arguments[],
                     int NoOfArguments_,
                     bool Volatile_,
                     bool Threadsafe_,
                     const std::string& ReturnTypeCode_)
    :                FunctionName(FunctionName_),
                     ExcelFunctionName(ExcelFunctionName_),
                     FunctionDescription(FunctionDescription_),
                     Library(Library_),
                     NoOfArguments(NoOfArguments_),
                     Volatile(Volatile_),
                     Threadsafe(Threadsafe_),
                     ReturnTypeCode(ReturnTypeCode_)
{

    ArgumentNames.reserve(NoOfArguments);
    ArgumentDescriptions.reserve(NoOfArguments);

    for (int i=0; i < NoOfArguments; i++)
    {
        ArgumentNames.push_back(Arguments[i].ArgumentName);
        ArgumentDescriptions.push_back(Arguments[i].ArgumentDescription);
        ArgumentTypes.push_back(Arguments[i].ArgumentType);
    }
}

std::string XLFunctionRegistrationData::GetFunctionName() const
{
    return FunctionName;
}
std::string XLFunctionRegistrationData::GetExcelFunctionName() const
{
    return ExcelFunctionName;
}
std::string XLFunctionRegistrationData::GetFunctionDescription() const
{
    return FunctionDescription;
}

std::string XLFunctionRegistrationData::GetReturnTypeCode() const
{
    // Would prefer to specify XlfExcel::Instance().xlfOperType() as the
    // default value of parameter ReturnTypeCode in the XLFunctionRegistrationHelper
    // constructor but that doesn't work as the object may be constructed during
    // static initialization before the XlfExcel singleton can be instantiated.
    if (ReturnTypeCode.empty())
        return XlfExcel::Instance().xlfOperType();
    else
        return ReturnTypeCode;
}

std::string XLFunctionRegistrationData::GetLibrary() const
{
    return Library;
}

int XLFunctionRegistrationData::GetNoOfArguments() const{
    return NoOfArguments;
}

std::vector<std::string> XLFunctionRegistrationData::GetArgumentNames() const
{
    return ArgumentNames;
}

std::vector<std::string> XLFunctionRegistrationData::GetArgumentDescriptions() const
{
    return ArgumentDescriptions;
}

std::vector<std::string> XLFunctionRegistrationData::GetArgumentTypes() const
{
    return ArgumentTypes;
}


XLFunctionRegistrationHelper::XLFunctionRegistrationHelper(const std::string& FunctionName,
                     const std::string& ExcelFunctionName,
                     const std::string& FunctionDescription,
                     const std::string& Library,
                     const Arg Args[],
                     int NoOfArguments,
                     bool Volatile,
                     bool Threadsafe,
                     const std::string& returnTypeCode)
{
    XLFunctionRegistrationData tmp(FunctionName,
                                                                ExcelFunctionName,
                                                                FunctionDescription,
                                                                Library,
                                                                Args,
                                                                NoOfArguments,
                                                                Volatile,
                                                                Threadsafe,
                                                                returnTypeCode);

    ExcelFunctionRegistrationRegistry::Instance().AddFunction(tmp);
}

ExcelFunctionRegistrationRegistry& ExcelFunctionRegistrationRegistry::Instance()
{
    static ExcelFunctionRegistrationRegistry SingleInstance;
    return SingleInstance;
}

void ExcelFunctionRegistrationRegistry::DoTheRegistrations() const
{

    for (std::list<XLFunctionRegistrationData>::const_iterator it = RegistrationData.begin(); it !=  RegistrationData.end(); ++it)
    {
        XlfFuncDesc::RecalcPolicy policy = it->GetVolatile() ? XlfFuncDesc::Volatile : XlfFuncDesc::NotVolatile;
         XlfFuncDesc xlFunction(it->GetFunctionName(),
                                                    it->GetExcelFunctionName(),
                                                    it->GetFunctionDescription(),
                                                    it->GetLibrary(),
                                                    policy,
                                                    it->GetThreadsafe(),
                                                    it->GetReturnTypeCode());
        XlfArgDescList xlFunctionArgs;

        for (int i=0; i < it->GetNoOfArguments(); ++i)
        {
                XlfArgDesc ThisArgumentDescription(it->GetArgumentNames()[i],
                                                                                it->GetArgumentDescriptions()[i],
                                                                                it->GetArgumentTypes()[i]);
                xlFunctionArgs + ThisArgumentDescription;
                                                                                            //+ is a push_back operation
        }

         xlFunction.SetArguments(xlFunctionArgs);
         xlFunction.Register();
    }

}
void ExcelFunctionRegistrationRegistry::AddFunction(const XLFunctionRegistrationData& data)
{
    RegistrationData.push_back(data);
}

ExcelFunctionRegistrationRegistry::ExcelFunctionRegistrationRegistry()
{
}

ExcelFunctionRegistrationRegistry::ExcelFunctionRegistrationRegistry(const ExcelFunctionRegistrationRegistry& original)
{ // declared to make private
}

