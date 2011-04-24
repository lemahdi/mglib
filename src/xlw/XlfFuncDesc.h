
/*
 Copyright (C) 1998, 1999, 2001, 2002, 2003, 2004 J�r�me Lecomte
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

#ifndef INC_XlfFuncDesc_H
#define INC_XlfFuncDesc_H

/*!
\file XlfFuncDesc.h
\brief Class XlfFuncDesc - Encapsulate a function to be registered to Excel
*/

// $Id: XlfFuncDesc.h 523 2008-04-10 16:10:03Z ericehlers $

#include <xlw/XlfAbstractCmdDesc.h>
#include <xlw/XlfExcel.h>

#if defined(_MSC_VER)
#pragma once
#endif

namespace xlw {

    class XlfArgDescList;

    //! Encapsulate a function to be registered to Excel.
    /*!
    By registering your function, you make it available under Excel in
    any spreadsheet as well as in the function wizard.

    \deprecated Consider using XLRegistration::XLFunctionRegistrationHelper
    instead.
    */
    class EXCEL32_API XlfFuncDesc: public XlfAbstractCmdDesc
    {
    public:
        //! Policy to reclalculate cells.
        enum RecalcPolicy {
            //! Function gets recalculated if one of the argument has changed.
            NotVolatile,
            //! Function gets recalculated unconditionally each time the sheet is calculated.
            Volatile
        } ;

        //! \name Structors
        //@{
        //! Ctor
        XlfFuncDesc(const std::string& name, const std::string& alias,
                    const std::string& comment, const std::string& category,
                    RecalcPolicy recalcPolicy = NotVolatile, bool Threadsafe = false,
                    const std::string &returnTypeCode = XlfExcel::Instance().xlfOperType());
        //! Dtor.
        ~XlfFuncDesc();
        //@}

        //! \name Get and set properties
        //@{
        //! Sets the category where your function will appear in the function wizard.
        void SetCategory(const std::string& category);
        //! Gets the category where your function will appear in the function wizard.
        const std::string& GetCategory() const;
        //! Sets the arguments definition.
        void SetArguments(const XlfArgDescList& arguments);
        // Gets the index into our list of UDFs (not used?).
        //int GetIndex() const;
        // Sets in the index into our list of UDFs (not used?).
        //void SetIndex(int i_);
        //@}
    protected:
        //! \name Concrete implementation of function registration
        //@{
        //! Registers the function.
        int DoRegister(const std::string& dllName) const;
        //! Unregisters the function.
        int DoUnregister(const std::string& dllName) const;
        //@}

    private:
        //! Copy ctor is declared private but not defined.
        XlfFuncDesc(const XlfFuncDesc&);
        //! Assignment operator is declared private but not defined.
        XlfFuncDesc& operator=(const XlfFuncDesc&);
        //! Pointer to implementation (pimpl idiom, see \ref HS).
        struct XlfFuncDescImpl * impl_;
        //! Shared registration code
        int RegisterAs(const std::string& dllName, double mode_, double* funcId = NULL) const;
        // Is this function currently live, or has it been faux-unregistered?
        // commented out - seems unused, forces DoRegister/DoUnregister to be non-const.
        //bool live_;
        // Index into our list of UDFs (not used?).
        //int index_;
        //! Excel code for the datatype of this function's return value.
        std::string returnTypeCode_;
    };

}

#endif

