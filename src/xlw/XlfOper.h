
/*
 Copyright (C) 1998, 1999, 2001, 2002 J�r�me Lecomte
 Copyright (C) 2007, 2008 Eric Ehlers
 Copyright (C) 2009  Narinder S Claire

 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef INC_XlfOper_H
#define INC_XlfOper_H

/*!
\file XlfOper.h
\brief Class XlfOper - Wrapper for XLOPER/XLOPER12 Excel data structure
*/

// $Id: XlfOper.h 538 2008-05-19 09:34:25Z ericehlers $

#include <xlw/XlfOperImpl.h>
#include "nova/glob/genericdate.h"
#include "nova/patterns/countedptr.hpp"

#if defined(_MSC_VER)
#pragma once
#endif

#if defined(DEBUG_HEADERS)
#pragma DEBUG_HEADERS
#endif

//! Namespace for the classes in the xlw (Excel Wrapper) library
/*!
xlw is a wrapper for the Excel C API.  xlw implements an object oriented interface
in support of Excel Addins implementing worksheet functions and menu commands.
*/
namespace xlw {

    //! Wrapper for XLOPER/XLOPER12 Excel data structure
    /*!
    Class XlfOper implements a sole data member, a union of LPXLOPER and
    LPXLOPER12.  XlfOper may be initialized with a value of type LPXLFOPER
    (void*) received from Excel.  XlfOper dereferences this pointer via the
    union, depending on which version of Excel is running: The value is
    interpreted as type LPXLOPER under Excel 4, and LPXLOPER12 under Excel 12.

    The logic to dereference LPXLOPER/LPXLOPER12 is implemented in class
    XlfOperImpl and XlfOper forwards all of its calls to XlfOperImpl.

    \warning It is important \e not to add any data members or virtual
    functions to this class.  The design of xlw relies on the fact that class
    XlfOper has the same size as LPXLOPER/LPXLOPER12, this assumption allows
    values of those types received from Excel to be interpreted by the library
    as instances of XlfOper.
    */
    class EXCEL32_API XlfOper
    {
        friend class XlfOperImpl;
        friend class XlfOperImpl4;
        friend class XlfOperImpl12;
        friend class XlfExcel;

    public:
        //! \name Structors and static members
        //@{
        //! Default ctor.
        XlfOper();
        //! Copy ctor.
        XlfOper(const XlfOper& oper);
        //! LPXLOPER/LPXLOPER12 ctor.
        XlfOper(LPXLFOPER lpxloper) : lpxloper4_(reinterpret_cast<LPXLOPER>(lpxloper)) {}
        //! double ctor.
        XlfOper(double value);
        //! short ctor.
        XlfOper(short value);
        //! short or error ctor.
        XlfOper(short value, bool Error);
        //! boolean ctor.
        XlfOper(bool value);
        //! 0 terminated character string ctor.
        XlfOper(const char *value);
        //!  string ctor.
        XlfOper(const std::string& value);
        //!  wstring ctor.
        XlfOper(const std::wstring& value);
        //! CellMatrix ctor.
        XlfOper(const CellMatrix& value);
        //! MyMatrix ctor.
        XlfOper(const MyMatrix& value);
        //! MyArray ctor.
        XlfOper(const MyArray& value);
        //! XlfRef ctor.
        XlfOper(const XlfRef& range);
        //! XlfMulti ctor.
        XlfOper(RW rows, COL cols);
        //! Container ctor.
        template <class FwdIt>
        XlfOper(RW rows, COL cols, FwdIt start)
        {
            Allocate();
            Set(rows, cols, start);
        }
        //! Destructor
        ~XlfOper() { XlfOperImpl::instance().destroy(*this); }
        //! Constructs an Excel error.
        static XlfOper Error(WORD word);
        //@}

        //! \name Memory management
        //@{
        //! Free auxiliary memory associated with the XLOPER
        void FreeAuxiliaryMemory() const  { return XlfOperImpl::instance().FreeAuxiliaryMemory(*this); }
        //@}

        //! \name Operators
        //@{
        //! Assignment operator
        XlfOper& operator=(const XlfOper& xloper)  { return XlfOperImpl::instance().operator_assignment(*this, xloper); }
        //! Cast to XLOPER *.
        operator LPXLOPER() { return XlfOperImpl::instance().operator_LPXLOPER(*this); }
        //! Cast to XLOPER12 *.
        operator LPXLOPER12() { return XlfOperImpl::instance().operator_LPXLOPER12(*this); }
        //! Cast to LPXLFOPER.
        operator LPXLFOPER() { return XlfOperImpl::instance().operator_LPXLFOPER(*this); }
        //@}

        //! \name Inspectors
        //@{
        //! Is the data missing ?
        bool IsMissing() const { return XlfOperImpl::instance().IsMissing(*this); }
        //! Is the data an error ?
        bool IsError() const { return XlfOperImpl::instance().IsError(*this); }
        //! Is the data a reference ?
        bool IsRef() const { return XlfOperImpl::instance().IsRef(*this); }
        //! Is the data a sheet reference ?
        bool IsSRef() const { return XlfOperImpl::instance().IsSRef(*this); }
        //! Is the data an array ?
        bool IsMulti() const { return XlfOperImpl::instance().IsMulti(*this); }
        //! Is the data a number ?
        bool IsNumber() const { return XlfOperImpl::instance().IsNumber(*this); }
        //! Is the data a string ?
        bool IsString() const { return XlfOperImpl::instance().IsString(*this); }
        //! Is the data a null value ?
        bool IsNil() const { return XlfOperImpl::instance().IsNil(*this); }
        //! Is the data a boolean ?
        bool IsBool() const { return XlfOperImpl::instance().IsBool(*this); }
        //! Is the data an integer ?
        bool IsInt() const { return XlfOperImpl::instance().IsInt(*this); }

        //! The Excel code for the underlying datatype
        DWORD xltype() const { return XlfOperImpl::instance().xltype(*this); }
        //! String representation of the Excel code for the underlying datatype
        std::string xltypeName() const;
        //@}

        /*! \name Array Accessors / Operators
        These functions are used to access the elements of an array in an XlfOper 
        whose underlying <tt>LPXLOPER/LPXLOPER12</tt> has <tt>xltype = xltypeMulti</tt>.

        Here is an example of how this interface can be used to inspect
        a value received from Excel as an input to an addin function.
        \code
        LPXLFOPER EXCEL_EXPORT test_sum(XlfOper xlInput) {
            EXCEL_BEGIN;

            double sum = 0.;
            for (RW i = 0; i < xlInput.rows(); i++)
                for (COL j = 0; j < xlInput.columns(); j++)
                    sum += xlInput(i, j).AsDouble();
            return XlfOper(sum);

            EXCEL_END;
        }
        \endcode

        Here is an example of how this interface can be used to populate an array
        to be returned to Excel from an addin function.
        \code
        XlfOper ret((WORD)3, (WORD)2);
        ret.SetElement(0, 0, "abc");
        ret.SetElement(0, 1, (short)42);
        ret.SetElement(1, 0, 1.23);
        ret.SetElement(1, 1, XlfOper::Error(xlerrValue));
        ret.SetElement(2, 0, true);
        \endcode
        */
        //! Number of rows in matrix.
        RW rows() const { return XlfOperImpl::instance().rows(*this); }
        //! Number of columns in matrix.
        COL columns() const { return XlfOperImpl::instance().columns(*this); }
        //! Function call operator, used here to subscript a two dimensional array.
        XlfOper operator()(RW row, COL col) { return XlfOperImpl::instance().operator_subscript(*this, row, col); }
        //! Set the value of array element with specified subscript.
        XlfOper& SetElement(RW r, COL c, const XlfOper &value)
            { return XlfOperImpl::instance().SetElement(*this, r, c, value); }
        //@}

        //! \name Conversions
        //@{
        //! Converts to a double.
        double AsDouble(int *pxlret = 0) const;
        //! Converts to a double with error identifer.
        double AsDouble(const std::string& ErrorId, int *pxlret = 0) const;
        //! Converts to a double with error identifer and default value.
		double AsDoubleWD(const std::string& ErrorId, const double& DefaultValue, int *pxlret = 0) const;

        //! Converts to a std::vector<double>.
        std::vector<double> AsDoubleVector(XlfOperImpl::DoubleVectorConvPolicy policy = XlfOperImpl::UniDimensional,
            int *pxlret = 0) const;
        //! Converts to a std::vector<double>.
        std::vector<double> AsDoubleVector(const std::string& ErrorId,
            XlfOperImpl::DoubleVectorConvPolicy policy = XlfOperImpl::UniDimensional, int *pxlret = 0) const;

        //! Converts to an array.
        MyArray AsArray(XlfOperImpl::DoubleVectorConvPolicy policy = XlfOperImpl::UniDimensional, int *pxlret = 0) const;
        //! Converts to an array.
        MyArray AsArray(const std::string& ErrorId,
            XlfOperImpl::DoubleVectorConvPolicy policy = XlfOperImpl::UniDimensional, int *pxlret = 0) const;

        //! Converts to a short.
        short AsShort(int *pxlret = 0) const;
        //! Converts to a short with error identifer.
        short AsShort(const std::string& ErrorId, int *pxlret = 0) const;
        //! Converts to a short with error identifer and default value.
		short AsShortWD(const std::string& ErrorId, const short& DefaultValue, int *pxlret = 0) const;

        //! Converts to a bool.
        bool AsBool(int *pxlret = 0) const;
        //! Converts to a bool with error identifer.
        bool AsBool(const std::string& ErrorId, int *pxlret = 0) const;
        //! Converts to a bool with error identifer and default value.
		bool AsBoolWD(const std::string& ErrorId, const bool& DefaultValue, int *pxlret = 0) const;

        //! Converts to an int.
        int AsInt(int *pxlret = 0) const;
        //! Converts to an int with error identifer.
        int AsInt(const std::string& ErrorId, int *pxlret = 0) const;
        //! Converts to an int with error identifer and default value.
        int AsIntWD(const std::string& ErrorId, const int& DefaultValue, int *pxlret = 0) const;

        //! Converts to a char *.
        char *AsString(int *pxlret = 0) const;
        //! Converts to a char * with error identifer.
        char *AsString(const std::string& ErrorId, int *pxlret = 0) const;
        //! Converts to a char * with error identifer and default value.
		char *AsStringWD(const std::string& ErrorId, const std::string& DefaultValue, int *pxlret = 0) const;

        //! Converts to a wstring.
        std::wstring AsWstring(int *pxlret = 0) const;

        //! Converts to a cell Matrix.
        CellMatrix AsCellMatrix(int *pxlret=0) const;
        //! Converts to a cell Matrix with error identifer.
        CellMatrix AsCellMatrix(const std::string& ErrorId, int *pxlret=0) const;

        //! Converts to a matrix.
        MyMatrix AsMatrix(int *pxlret=0) const;
        //! Converts to a matrix with error identifer.
        MyMatrix AsMatrix(const std::string& ErrorId, int *pxlret=0) const;

        //! Converts to a XlfRef.
        XlfRef AsRef(int *pxlret = 0) const;

        //! Converts to a date.
        MG::MG_Date AsMGDate(int *pxlret=0) const;
        //! Converts to a date with error identifer.
		MG::MG_Date AsMGDate(const std::string& ErrorId, int *pxlret=0) const;
        //! Converts to a date with error identifer and default value.
		MG::MG_Date AsMGDateWD(const std::string& ErrorId, const std::string& , int *pxlret=0) const;

        //! Converts to a generic date.
        MG::MG_GenericDate AsMGGenDate(int *pxlret=0) const;
        //! Converts to a generic date with error identifer.
		MG::MG_GenericDate AsMGGenDate(const std::string& ErrorId, int *pxlret=0) const;
        //! Converts to a generic date with error identifer and default value.
		MG::MG_GenericDate AsMGGenDateWD(const std::string& ErrorId, const std::string& DefaultValue, int *pxlret=0) const;

        //! Converts to an Excel object.
        MG::MG_XLObjectPtr AsMGXLObject(int *pxlret=0) const;
        //! Converts to an Excel object with error identifer.
		MG::MG_XLObjectPtr AsMGXLObject(const std::string& ErrorId, int *pxlret=0) const;
        //! Converts to an Excel object with error identifer and default value.
		MG::MG_XLObjectPtr AsMGXLObjectWD(const std::string& ErrorId, const std::string& , int *pxlret=0) const;
        //@}

        //! \name Manage reference to underlying XLOPER
        //@{
        //! Gets the internal LPXLFOPER.
        LPXLFOPER GetLPXLFOPER() const { return XlfOperImpl::instance().GetLPXLFOPER(*this); }
        //@}

        //! \name Set the value of the underlying reference
        //@{
        //! Set the underlying XLOPER * to lpxloper.
        XlfOper& Set(LPXLFOPER lpxlfoper) { return XlfOperImpl::instance().Set(*this, lpxlfoper); }
        //! Set to a double.
        XlfOper& Set(double value) { return XlfOperImpl::instance().Set(*this, value); }
        //! Set to a short.
        XlfOper& Set(short value) { return XlfOperImpl::instance().Set(*this, value); }
        //! Set to a boolean.
        XlfOper& Set(bool value) { return XlfOperImpl::instance().Set(*this, value); }
        //! Set to a zero-terminated character string.
        XlfOper& Set(const char *value) { return XlfOperImpl::instance().Set(*this, value); }
        //! Set to a wstring.
        XlfOper& Set(const std::wstring &value) { return XlfOperImpl::instance().Set(*this, value); }
        //! Set to a cell matrix.
        XlfOper& Set(const CellMatrix& cells) { return XlfOperImpl::instance().Set(*this, cells); }
        //! Set to a  matrix.
        XlfOper& Set(const MyMatrix& matrix);
        //! Set to an array.
        XlfOper& Set(const MyArray& values);
        //! Set to a range.
        XlfOper& Set(const XlfRef& range) { return XlfOperImpl::instance().Set(*this, range); }
        //! Set to a short or error, bool for disambiguation.
        XlfOper& Set(short value, bool Error) { return XlfOperImpl::instance().Set(*this, value, Error); }
        //! Set to an array.
        /*!
        \param rows number of rows in the array
        \param cols number of columns in the array
        \param it iterator pointing to the begining of a container
         of size r x c (at least) that contain the data.
        \warning Data are to be stored row-wise.
        */
        template<class FwdIt>
        XlfOper& Set(RW rows, COL cols, FwdIt it)
        {
            return XlfOperImpl::instance().Set(*this, rows, cols, it);
        }
        //! Set to an array of the specified dimensions.
        XlfOper& Set(RW r, COL c) { return XlfOperImpl::instance().Set(*this, r, c); }
        //! Set to an error value.
        XlfOper& SetError(WORD error) { return XlfOperImpl::instance().SetError(*this, error); }
        //@}

    private:
        //! \name Manage reference to the underlying XLOPER
        //@{
        //! Internal LPXLOPER/LPXLOPER12.
        union {
            LPXLOPER lpxloper4_;
            LPXLOPER12 lpxloper12_;
        };
        //@}

        //! \name Coercion
        //@{
        //! Coerce method is called by conversion operators if needed (never by the user).
        int Coerce(short type, XlfOper& res) const { return XlfOperImpl::instance().Coerce(*this, type, res); }
        //@}

        //! \name Memory management
        //@{
        //! Reserves memory in XLL buffer (garbage collected).
        int Allocate() { return XlfOperImpl::instance().Allocate(*this); }
        //@}

        //! \name Error handling
        //@{
        //! Throws an exception when critical errors occur.
        int ThrowOnError(int value) const;
        //! Throws an exception when critical errors occur but passes on an identifier to help track it down.
        int ThrowOnError(int value, const std::string& identifier) const;
        //@}

        //! \name Memory management
        //@{
        //! Internally used to flag XLOPER returned by Excel.
        static int xlbitFreeAuxMem;
        //@}

        //! \name Private implementations of conversion routines
        //@{
        //! Attempts conversion to double and returns Excel error code.
        int ConvertToDoubleVector(std::vector<double>& value,
            XlfOperImpl::DoubleVectorConvPolicy policy = XlfOperImpl::UniDimensional) const
            { return XlfOperImpl::instance().ConvertToDoubleVector(*this, value, policy); }
        //! Attempts conversion to double and returns Excel error code.
        int ConvertToDouble(double& value) const throw() {
            return XlfOperImpl::instance().ConvertToDouble(*this, value); }
        int ConvertToDoubleWD(double& value, const double& def) const throw() {
            return XlfOperImpl::instance().ConvertToDoubleWD(*this, value, def); }
        //! Attempts conversion to short and returns Excel error code.
        int ConvertToShort(short& value) const throw() {
            return XlfOperImpl::instance().ConvertToShort(*this, value); }
        int ConvertToShortWD(short& value, const short& def) const throw() {
            return XlfOperImpl::instance().ConvertToShortWD(*this, value, def); }
        //! Attempts conversion to bool and returns Excel error code.
        int ConvertToBool(bool& value) const throw() {
            return XlfOperImpl::instance().ConvertToBool(*this, value); }
        int ConvertToBoolWD(bool& value, const bool& def) const throw() {
            return XlfOperImpl::instance().ConvertToBoolWD(*this, value, def); }
        // Attempts conversion to int and returns Excel error code.
        //int ConvertToInt(int& value) const throw() {
        //    return XlfOperImpl::instance().ConvertToInt(*this, value); }
        //! Attempts conversion to string and returns Excel error code.
        int ConvertToString(char *& value) const throw() {
            return XlfOperImpl::instance().ConvertToString(*this, value); }
		int ConvertToStringWD(char *& value, const std::string& def) const throw() {
            return XlfOperImpl::instance().ConvertToStringWD(*this, value, def); }
        //! Attempts conversion to wstring and returns Excel error code.
	    int ConvertToWstring(std::wstring &value) const throw() {
            return XlfOperImpl::instance().ConvertToWstring(*this, value); }
        //! Attempts conversion to CellMatrix and returns Excel error code.
        int ConvertToCellMatrix(CellMatrix& output) const {
            return XlfOperImpl::instance().ConvertToCellMatrix(*this, output); }
        //! Attempts conversion to Matrix and returns Excel error code.
        int ConvertToMatrix(MyMatrix& output) const {
            return XlfOperImpl::instance().ConvertToMatrix(*this, output); }
        //! Attempts conversion to XlRef and returns Excel error code.
        int ConvertToRef(XlfRef& ref) const throw() {
            return XlfOperImpl::instance().ConvertToRef(*this, ref); }
        //! Attempts conversion to XlRef and returns Excel error code.
        int ConvertToErr(WORD& e) const throw() {
            return XlfOperImpl::instance().ConvertToErr(*this, e); }
        //@}

    };

}

#ifdef NDEBUG
#include <xlw/XlfOper.inl>
#endif

#endif

