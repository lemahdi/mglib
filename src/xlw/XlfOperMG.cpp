/*
 * Copyright : 2010-2024 by MG
 * Purpose   : MGlib-specific XlfOper extensions –
 *             AsMGDate / AsMGGenDate / AsMGXLObject
 *
 * These methods are compiled only on Windows (XLL builds).
 * They depend on MGlib types defined in nova/glob/.
 */

#if defined(_WIN32) || defined(_WIN64)

#include <xlw/XlfOper.h>
#include "xlw/cache/cached.h"
#include "nova/utils/utils.h"
#include "nova/glob/genericdate.h"
#include "nova/glob/exception.h"
#include "nova/glob/date.h"
#include "nova/glob/typedef.h"

#include <stdexcept>

using namespace xlw;
using namespace MG;

// ---------------------------------------------------------------------------
// AsMGDate
// ---------------------------------------------------------------------------

MG::MG_Date xlw::XlfOper::AsMGDate(int *pxlret) const
{
    double vXLDate = AsDouble();
    if (pxlret) *pxlret = xlretSuccess;
    return MG_utils::FromXLDateToJulianDay(vXLDate);
}

MG::MG_Date xlw::XlfOper::AsMGDate(const std::string& ErrorId, int *pxlret) const
{
    double vXLDate = AsDouble(ErrorId.c_str());
    if (pxlret) *pxlret = xlretSuccess;
    return MG_utils::FromXLDateToJulianDay(vXLDate);
}

MG::MG_Date xlw::XlfOper::AsMGDateWD(const std::string& ErrorId, const std::string& /*DefaultValue*/, int *pxlret) const
{
    // DefaultValue is not used for MG_Date: we return MG_Date() (epoch) when missing.
    // The parameter is kept for API consistency with other WD methods.
    try
    {
        double vXLDate = AsDouble(ErrorId.c_str());
        if (pxlret) *pxlret = xlretSuccess;
        return MG_utils::FromXLDateToJulianDay(vXLDate);
    }
    catch (...)
    {
        if (pxlret) *pxlret = xlretInvXloper;
        return MG_Date();
    }
}

// ---------------------------------------------------------------------------
// AsMGGenDate
// ---------------------------------------------------------------------------

MG::MG_GenericDate xlw::XlfOper::AsMGGenDate(int *pxlret) const
{
    // If cell contains a double (date serial) convert as date; else as period string
    try
    {
        double vXLDate = AsDouble();
        if (pxlret) *pxlret = xlretSuccess;
        MG_Date vDt = MG_utils::FromXLDateToJulianDay(vXLDate);
        return MG_GenericDate(vDt);
    }
    catch (...)
    {
        char* s = AsString();
        if (pxlret) *pxlret = xlretSuccess;
        return MG_GenericDate(std::string(s));
    }
}

MG::MG_GenericDate xlw::XlfOper::AsMGGenDate(const std::string& ErrorId, int *pxlret) const
{
    try
    {
        double vXLDate = AsDouble(ErrorId.c_str());
        if (pxlret) *pxlret = xlretSuccess;
        MG_Date vDt = MG_utils::FromXLDateToJulianDay(vXLDate);
        return MG_GenericDate(vDt);
    }
    catch (...)
    {
        char* s = AsString(ErrorId.c_str());
        if (pxlret) *pxlret = xlretSuccess;
        return MG_GenericDate(std::string(s));
    }
}

MG::MG_GenericDate xlw::XlfOper::AsMGGenDateWD(const std::string& ErrorId, const std::string& DefaultValue, int *pxlret) const
{
    try
    {
        return AsMGGenDate(ErrorId, pxlret);
    }
    catch (...)
    {
        if (pxlret) *pxlret = xlretSuccess;
        return MG_GenericDate(DefaultValue);
    }
}

// ---------------------------------------------------------------------------
// AsMGXLObject
// ---------------------------------------------------------------------------

std::shared_ptr<MG::MG_XLObject> xlw::XlfOper::AsMGXLObject(int *pxlret) const
{
    char* vXLNameId = AsString();
    std::string vRef(vXLNameId), vError;
    MG_XLObjectPtr output;
    if (MG_SCache::Instance()->PersistentGet(vRef, output, vError))
    {
        if (pxlret) *pxlret = xlretSuccess;
        return output;
    }
    throw std::runtime_error("conversion to MG_XLObject failed: " + vError);
}

std::shared_ptr<MG::MG_XLObject> xlw::XlfOper::AsMGXLObject(const std::string& ErrorId, int *pxlret) const
{
    char* vXLNameId = AsString(ErrorId.c_str());
    std::string vRef(vXLNameId), vError;
    MG_XLObjectPtr output;
    if (MG_SCache::Instance()->PersistentGet(vRef, output, vError))
    {
        if (pxlret) *pxlret = xlretSuccess;
        return output;
    }
    throw std::runtime_error(ErrorId + " conversion to MG_XLObject failed: " + vError);
}

std::shared_ptr<MG::MG_XLObject> xlw::XlfOper::AsMGXLObjectWD(const std::string& ErrorId, const std::string& /*DefaultValue*/, int *pxlret) const
{
    // DefaultValue is not used for MG_XLObject: we return nullptr when missing.
    // The parameter is kept for API consistency with other WD methods.
    try
    {
        return AsMGXLObject(ErrorId, pxlret);
    }
    catch (...)
    {
        if (pxlret) *pxlret = xlretInvXloper;
        return MG_XLObjectPtr(nullptr);
    }
}

#endif // _WIN32 || _WIN64
