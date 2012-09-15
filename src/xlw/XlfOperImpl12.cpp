
/*
 Copyright (C) 1998, 1999, 2001, 2002 J�r�me Lecomte
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

/*!
\file XlfOperImpl12.cpp
\brief Implements the XlfOperImpl12 class.
*/

// $Id: XlfOper.cpp 335 2007-06-25 03:47:43Z markjoshi $


#pragma warning (disable : 4996)


#include <xlw/XlfOper.h>
#include <xlw/XlfOperImpl12.h>
#include <xlw/XlfException.h>
#include <xlw/XlfRef.h>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <xlw/CellMatrix.h>
#include <algorithm>
#include <xlw/macros.h>
#include <xlw/cache/cached.h>
// Stop header precompilation
#ifdef _MSC_VER
#pragma hdrstop
#endif

void xlw::XlfOperImpl12::destroy(const XlfOper &xlfOper) const
{
    if (!xlfOper.lpxloper12_)
        return;

    int type = xlfOper.lpxloper12_->xltype;

    //  Only the types below can be flagged xlFreeAuxMem, thus the test is
    //  actually redundant: we don't need to re-check the type of the oper.
    //
    //bool canHaveAuxMem = (type & xltypeStr ||
    //                      type & xltypeRef ||
    //                      type & xltypeMulti ||
    //                      type & xltypeBigData);
    if (type & XlfOper::xlbitFreeAuxMem)
    {
        // switch back the bit as it was originally
        xlfOper.lpxloper12_->xltype &= ~XlfOper::xlbitFreeAuxMem;
        FreeAuxiliaryMemory(xlfOper);
    }
    return;
}

int xlw::XlfOperImpl12::Allocate(XlfOper &xlfOper) const
{
    xlfOper.lpxloper12_ = (LPXLOPER12)XlfExcel::Instance().GetMemory(sizeof(XLOPER12));
    if (!xlfOper.lpxloper12_)
        return xlretInvXloper;
    xlfOper.lpxloper12_->xltype = xltypeNil;
    return xlretSuccess;
}

void xlw::XlfOperImpl12::FreeAuxiliaryMemory(const XlfOper &xlfOper) const
{
    // FIXME call Call12() instead?
    int err = XlfExcel::Instance().XlfExcel::Instance().Call(xlFree, NULL, 1, (LPXLFOPER)xlfOper.lpxloper12_);
    if (err != xlretSuccess)
        std::cerr << XLW__HERE__ << "Call to xlFree failed" << std::endl;
    return;
}

int xlw::XlfOperImpl12::Coerce(const XlfOper &xlfOper, short type, XlfOper& result) const
{
    XlfOper xlType(type);
    // FIXME call Call12() instead?
    int xlret = XlfExcel::Instance().Call(xlCoerce, (LPXLFOPER)result, 2, (LPXLFOPER)xlfOper.lpxloper12_, (LPXLFOPER)xlType);
    return xlret;
}

int xlw::XlfOperImpl12::ConvertToDouble(const XlfOper &xlfOper, double& d) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeInt)
    {
        d = xlfOper.lpxloper12_->val.w;
        xlret=xlretSuccess;
    }
    else if (xlfOper.lpxloper12_->xltype & xltypeNum)
    {
        d = xlfOper.lpxloper12_->val.num;
        xlret=xlretSuccess;
    }
    else
    {
        // Allocates tmp on the stack to avoid filling the internal buffer.
        /*XLOPER12 tmp;
        // Creates a XlfOper based on tmp.
        XlfOper cast(&tmp);
        // Coerces to numeric type.
        xlret = Coerce(xlfOper, xltypeNum, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToDouble(d);*/
		xlret = xlretInvXloper;
    }
    return xlret;
};

int xlw::XlfOperImpl12::ConvertToDoubleWD(const XlfOper &xlfOper, double& d, const double& def) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeInt)
    {
        d = xlfOper.lpxloper12_->val.w;
        xlret=xlretSuccess;
    }
    else if (xlfOper.lpxloper12_->xltype & xltypeNum)
    {
        d = xlfOper.lpxloper12_->val.num;
        xlret=xlretSuccess;
    }
    else
    {
		d = def;
        xlret=xlretSuccess;
    }
    return xlret;
};

int xlw::XlfOperImpl12::ConvertToDoubleVector(const XlfOper &xlfOper, std::vector<double>& v, DoubleVectorConvPolicy policy) const
{
    if (xlfOper.lpxloper12_->xltype == xltypeMissing)
    {
        v.resize(0);
        return xlretSuccess;
    }

    if (xlfOper.lpxloper12_->xltype & xltypeNum)
    {
        // first test if double
        double d=0.0;
        int xlret1 = ConvertToDouble(xlfOper, d);

        if (xlret1 == xlretSuccess)
        {
            v.resize(1);
            v[0] = d;
            return xlret1;
        }
    }

    if (xlfOper.lpxloper12_->xltype & xltypeMulti)
    {
        size_t nbRows = xlfOper.lpxloper12_->val.array.rows;
        size_t nbCols = xlfOper.lpxloper12_->val.array.columns;

        bool isUniDimRange = ( nbRows == 1 || nbCols == 1 );
        if (policy == UniDimensional && ! isUniDimRange)
            // not a vector we return a failure
            return xlretFailed;

        size_t n = nbRows*nbCols;
        v.resize(n);

        for (size_t i = 0; i < nbRows; ++i)
        {
            for (size_t j = 0; j < nbCols; ++j)
            {
                size_t index;
                if (policy == RowMajor)
                    // C-like dense matrix storage
                    index = i*nbCols+j;
                else
                    // Fortran-like dense matrix storage. Does not matter if the policy is UniDimensional
                    index = j*nbRows+i;

                unsigned long thisType = (*xlfOper.lpxloper12_).val.array.lparray[i*nbCols+j].xltype;
                if (thisType == xltypeNum)
                {
                    v[index] = (*xlfOper.lpxloper12_).val.array.lparray[i*nbCols+j].val.num;
                }
                else
                {
                    int xlret =  XlfOper(&(*xlfOper.lpxloper12_).val.array.lparray[i*nbCols+j]).ConvertToDouble(v[index]);
                    if (xlret != xlretSuccess)
                        return xlret;
                }
            }
        }
        return xlretSuccess;
    }

    XlfRef ref;

    int xlret = ConvertToRef(xlfOper, ref);
    if (xlret != xlretSuccess)
        return xlret;

    int nbRows = ref.GetNbRows();
    int nbCols = ref.GetNbCols();

    bool isUniDimRange = ( nbRows == 1 || nbCols == 1 );
    if (policy == UniDimensional && ! isUniDimRange)
        // not a vector we return a failure
        return xlretFailed;

    int n = nbRows*nbCols;
    v.resize(n);

    for (int i = 0; i < nbRows; ++i)
    {
        for (int j = 0; j < nbCols; ++j)
        {
            if (policy == RowMajor)
                // C-like dense matrix storage
                xlret = ref.element<XlfOper>(i,j).ConvertToDouble(v[i*nbCols+j]);
            else
                // Fortran-like dense matrix storage. Does not matter if the policy is UniDimensional
                xlret = ref.element<XlfOper>(i,j).ConvertToDouble(v[j*nbRows+i]);
            if (xlret != xlretSuccess)
                return xlret;
        }
    }
    return xlret;
};

int xlw::XlfOperImpl12::ConvertToShort(const XlfOper &xlfOper, short& s) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeNum)
    {
        s = static_cast<short>(xlfOper.lpxloper12_->val.num);
        xlret=xlretSuccess;
    }
    else
    {
        // Allocates tmp on the stack to avoid filling the internal buffer.
        /*XLOPER12 tmp;
        // Creates a XlfOper based on tmp.
        XlfOper cast(&tmp);
        // Coerces to numeric type.
        xlret = Coerce(xlfOper, xltypeNum, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToShort(s);*/
		xlret = xlretInvXloper;
        }
        return xlret;
};

int xlw::XlfOperImpl12::ConvertToShortWD(const XlfOper &xlfOper, short& s, const short& def) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeNum)
    {
        s = static_cast<short>(xlfOper.lpxloper12_->val.num);
        xlret=xlretSuccess;
    }
    else
    {
		s = def;
        xlret=xlretSuccess;
        }
        return xlret;
};

int xlw::XlfOperImpl12::ConvertToBool(const XlfOper &xlfOper, bool& b) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeBool)
    {
        b = (xlfOper.lpxloper12_->val.xbool != 0);
        xlret = xlretSuccess;
    }
    else
    {
        // see ConvertToDouble
        /*XLOPER12 tmp;
        XlfOper cast(&tmp);
        xlret = Coerce(xlfOper, xltypeBool, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToBool(b);*/
		xlret = xlretInvXloper;
    }
    return xlret;
};

int xlw::XlfOperImpl12::ConvertToBoolWD(const XlfOper &xlfOper, bool& b, const bool& def) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeBool)
    {
        b = (xlfOper.lpxloper12_->val.xbool != 0);
        xlret = xlretSuccess;
    }
    else
    {
        b = def;
        xlret=xlretSuccess;
    }
    return xlret;
};

int xlw::XlfOperImpl12::ConvertToMatrix(const XlfOper &xlfOper, MyMatrix& value) const
{
    // deal with empty case first
    if (xlfOper.lpxloper12_->xltype == xltypeMissing || xlfOper.lpxloper12_->xltype == xltypeNil )
    {

       MyMatrix tmp;
       value = tmp;

        return xlretSuccess;
    }

    CellMatrix tmp(1,1);// will be resized anyway
    int xlret = ConvertToCellMatrix(xlfOper, tmp);
    if (xlret != xlretSuccess)
        return xlret;

    value = MyMatrix(tmp.RowsInStructure(), tmp.ColumnsInStructure());
    for (unsigned long i =0; i < tmp.RowsInStructure(); i++)
        for (unsigned long j =0; j < tmp.ColumnsInStructure(); j++)
            if (tmp(i,j).IsANumber())
                ChangingElement(value,i,j) = tmp(i,j).NumericValue();
            else
                return xlretFailed;

    return xlretSuccess;
}

int xlw::XlfOperImpl12::ConvertToCellMatrix(const XlfOper &xlfOper, CellMatrix& output) const
{

    if (xlfOper.lpxloper12_->xltype == xltypeMissing || xlfOper.lpxloper12_->xltype == xltypeNil)
    {

        CellMatrix tmp(1,1);
        output = tmp;

        return xlretSuccess;
    }

    if (xlfOper.lpxloper12_->xltype == xltypeNum)
    {
        CellMatrix tmp(1,1);

        tmp(0,0) = xlfOper.lpxloper12_->val.num;

        output = tmp;

        return xlretSuccess;
    }

    if (xlfOper.lpxloper12_->xltype == xltypeBool)
    {
        CellMatrix tmp(1,1);

        tmp(0,0) = (xlfOper.lpxloper12_->val.xbool >0);

        output = tmp;

        return xlretSuccess;
    }

    if (xlfOper.lpxloper12_->xltype == xltypeErr)
    {
        CellMatrix tmp(1,1);

        tmp(0,0) = CellValue(xlfOper.lpxloper12_->val.err, true);

        output = tmp;

        return xlretSuccess;
    }

    if (xlfOper.lpxloper12_->xltype == xltypeStr)
    {
        CellMatrix tmpCell(1,1);

        unsigned long len = *((*xlfOper.lpxloper12_).val.str);

        std::wstring tmp;
        tmp.resize(len);

        for(unsigned long k=0; k<len; ++k)
            tmp[k]= static_cast<char>(((*xlfOper.lpxloper12_).val.str)[k+1]);

	    std::string vDesc(tmp.begin(), tmp.end());
	    bool vIsMGObj = MG::MG_SCache::Instance()->IsMGObjectDescriptor(vDesc);
	    if (vIsMGObj)
	    {
		    std::string vErr;
		    MG::MG_XLObjectPtr vObj(NULL);
		    MG::MG_SCache::Instance()->PersistentGet(vDesc, vObj, vErr);
		    if (vObj == NULL)
			    tmpCell(0,0) = vObj;
		    else
			    tmpCell(0,0) = vErr;
	    }
	    else
		    tmpCell(0,0) = tmp;

        output = tmpCell;

        return xlretSuccess;
    }

    if (xlfOper.lpxloper12_->xltype == xltypeMulti)
    {
        unsigned long rows = xlfOper.lpxloper12_->val.array.rows;
        unsigned long columns = xlfOper.lpxloper12_->val.array.columns;
        CellMatrix result(rows,columns);

        for (unsigned long i=0; i < rows; i++)
            for (unsigned long j=0; j < columns; j++)
            {
                unsigned long thisType = (*xlfOper.lpxloper12_).val.array.lparray[i*columns+j].xltype;
                if (thisType == xltypeNum)
                {
                    double x = (*xlfOper.lpxloper12_).val.array.lparray[i*columns+j].val.num;
                    result(i,j) = x;
                }
                else
                {
                    if (thisType==xltypeStr)
                    {
                          unsigned long len = *((*xlfOper.lpxloper12_).val.array.lparray[i*columns+j].val.str);

                          std::wstring tmp;
                          tmp.resize(len);

                          for(unsigned long k=0; k<len; ++k)
                            tmp[k]=
                            ((*xlfOper.lpxloper12_).val.array.lparray[i*columns+j].val.str)[k+1];

						  std::string vDesc(tmp.begin(), tmp.end());
						  bool vIsMGObj = MG::MG_SCache::Instance()->IsMGObjectDescriptor(vDesc);
						  if (vIsMGObj)
						  {
							  std::string vErr;
							  MG::MG_XLObjectPtr vObj(NULL);
							  MG::MG_SCache::Instance()->PersistentGet(vDesc, vObj, vErr);
							  if (vObj == NULL)
								  result(i,j) = vObj;
							  else
								  result(i,j) = vErr;
						  }
						  else
	                          result(i,j) = tmp;
                    }

                    else
                        if (thisType == xltypeBool)
                        {
                            result(i,j) = ((*xlfOper.lpxloper12_).val.array.lparray[i*columns+j].val.xbool > 0);
                        }
                        else
                            if (thisType== xltypeErr)
                            {
                                result(i,j) = CellValue(((*xlfOper.lpxloper12_).val.array.lparray[i*columns+j].val.err),true);
                            }
                            else
                                if (thisType!=xltypeMissing && thisType != xltypeNil)
                                    throw("cell contains neither number nor string nor boolean nor empty");
                }

            }

        output= result;

        return xlretSuccess;

    }

    XlfRef ref;

    int xlret = ConvertToRef(xlfOper, ref);
    if (xlret != xlretSuccess)
        return xlret;

    unsigned long nbRows = ref.GetNbRows();
    unsigned long nbCols = ref.GetNbCols();

    output = CellMatrix(nbRows,nbCols);
    for (unsigned long i = 0; i < nbRows; ++i)
    {
        for (unsigned long j = 0; j < nbCols; ++j)
        {
            XlfOper element = ref.element<XlfOper>(static_cast<WORD>(i),static_cast<BYTE>(j));
            unsigned long type = element.lpxloper12_->xltype;

            if (type == xltypeRef)
            {
                XlfRef xlrefij;

                int xlretij = element.ConvertToRef(xlrefij);

                if (xlretij != xlretSuccess)
                    return xlretij;

                XlfOper refElement = xlrefij.element<XlfOper>(0UL,0UL);
                type = refElement.lpxloper12_->xltype;

                if (type == xltypeNum)
                {
                    double tmp;
                    xlret = refElement.ConvertToDouble(tmp);

                    output(i,j) = tmp;

                    if (xlret != xlretSuccess)
                        return xlret;
                }
                else
                    if (type == xltypeErr)
                    {
                        WORD tmp;

                        xlret = refElement.ConvertToErr(tmp);

                        output(i,j) = CellValue(tmp,true);

                        if (xlret != xlretSuccess)
                            return xlret;
                    }
                    else
                    if (type == xltypeBool)
                    {
                        bool tmp;

                        xlret = refElement.ConvertToBool(tmp);

                        output(i,j) = tmp;

                        if (xlret != xlretSuccess)
                            return xlret;
                    }
                    else
                    {
                        if (type == xltypeStr || type == xltypeSRef)
                        {
                            char* tmp;
                            xlret = refElement.ConvertToString(tmp);
                            output(i,j) = std::string(tmp);

                            if (xlret != xlretSuccess)
                                return xlret;
                        }
                        else
                        {

                            if (type != xltypeMissing)
                                return xlretInvXloper;

                        }

                    }

            }
            else
                if (type == xltypeNum)
                {
                    double tmp;
                    xlret = element.ConvertToDouble(tmp);

                    output(i,j) = tmp;

                    if (xlret != xlretSuccess)
                        return xlret;
                }
                else
                {
                    if (type == xltypeStr || type == xltypeSRef)
                    {
                        char* tmp;
                        xlret = element.ConvertToString(tmp);
                        output(i,j) = std::string(tmp);

                        if (xlret != xlretSuccess)
                            return xlret;
                    }
                    else
                    {
                        if (element.lpxloper12_->xltype != xltypeMissing)
                            return xlretInvXloper;

                    }

                }
        }

    }
    return xlret;

}

int xlw::XlfOperImpl12::ConvertToErr(const XlfOper &xlfOper, WORD& e) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeErr)
    {
        e = xlfOper.lpxloper12_->val.err ;
        xlret = xlretSuccess;
    }
    else
    {
        // see ConvertToDouble
        XLOPER12 tmp;
        XlfOper cast(&tmp);
        xlret = Coerce(xlfOper, xltypeErr, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToErr(e);
    }
    return xlret;
};

int xlw::XlfOperImpl12::ConvertToString(const XlfOper &xlfOper, char *& s) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeStr)
    {
        size_t n = xlfOper.lpxloper12_->val.str[0];
        s = XlfExcel::Instance().GetMemory(n + 1);
        wcstombs(s, xlfOper.lpxloper12_->val.str + 1, n);
        s[n] = 0;
        xlret = xlretSuccess;
    }
    else
    {
        // see AsDouble
        /*XLOPER12 tmp;
        // Function Coerce calls function Call which sets bit xlbitFreeAuxMem of variable cast,
        // so that the memory which Excel allocates to that variable (the string) is freed
        // when the variable goes out of scope.
        XlfOper cast(&tmp);
        xlret = Coerce(xlfOper, xltypeStr, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToString(s);*/
		xlret = xlretInvXloper;
    }
    return xlret;
}

int xlw::XlfOperImpl12::ConvertToStringWD(const XlfOper &xlfOper, char *& s, const std::string& def) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeStr)
    {
        size_t n = xlfOper.lpxloper12_->val.str[0];
        s = XlfExcel::Instance().GetMemory(n + 1);
        wcstombs(s, xlfOper.lpxloper12_->val.str + 1, n);
        s[n] = 0;
        xlret = xlretSuccess;
    }
    else
    {
		size_t len = def.length()+1;
        s = XlfExcel::Instance().GetMemory(len);
		memcpy(s, def.c_str(), len);
		s[len-1] = '\0';
        xlret = xlretSuccess;
    }
    return xlret;
}

int xlw::XlfOperImpl12::ConvertToWstring(const XlfOper &xlfOper, std::wstring &s) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeStr)
    {
        size_t n = xlfOper.lpxloper12_->val.str[0];
        wchar_t *w = reinterpret_cast<wchar_t*>(XlfExcel::Instance().GetMemory(n*2+1));
        memcpy(w, xlfOper.lpxloper12_->val.str + 1, n*2);
        w[n]=0;
        s = std::wstring(w);
        xlret = xlretSuccess;
    }
    else
    {
        // see AsDouble
        XLOPER12 tmp;
        // Function Coerce calls function Call which sets bit xlbitFreeAuxMem of variable cast,
        // so that the memory which Excel allocates to that variable (the string) is freed
        // when the variable goes out of scope.
        XlfOper cast(&tmp);
        xlret = Coerce(xlfOper, xltypeStr, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToWstring(s);
    }
    return xlret;
}

int xlw::XlfOperImpl12::ConvertToRef(const XlfOper &xlfOper, XlfRef& r) const throw()
{
    int xlret;

    if (xlfOper.lpxloper12_ == 0)
        return xlretInvXloper;

    if (xlfOper.lpxloper12_->xltype & xltypeRef)
    {
        const XLREF12& ref=xlfOper.lpxloper12_->val.mref.lpmref->reftbl[0];
        r = XlfRef (ref.rwFirst,  // top
                    ref.colFirst, // left
                    ref.rwLast,   // bottom
                    ref.colLast,  // right
                    xlfOper.lpxloper12_->val.mref.idSheet); // sheet id
        xlret = xlretSuccess;
    }
    else
    {
        // see AsDouble
        XLOPER12 tmp;
        // Function Coerce calls function Call which sets bit xlbitFreeAuxMem of variable cast,
        // so that the memory which Excel allocates to that variable (the reference) is freed
        // when the variable goes out of scope.
        XlfOper cast(&tmp);
        xlret = Coerce(xlfOper, xltypeRef, cast);
        if (xlret == xlretSuccess)
            xlret = cast.ConvertToRef(r);
    }
    return xlret;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, const CellMatrix& cells) const
{
    size_t r = cells.RowsInStructure();
    size_t c = cells.ColumnsInStructure();

    c = c < 255 ? c : 255;

    xlfOper.lpxloper12_->xltype = xltypeMulti;
    xlfOper.lpxloper12_->val.array.rows = static_cast<RW>(r);
    xlfOper.lpxloper12_->val.array.columns = static_cast<COL>(c);

    xlfOper.lpxloper12_->val.array.lparray
            = (LPXLOPER12)XlfExcel::Instance().GetMemory(r*c*sizeof(XLOPER12));

    for (size_t i=0; i < r; i++)
        for (size_t j=0; j < c; j++)
        {
            size_t k = i*c +j;
            if (cells(i,j).IsANumber())
                xlfOper.lpxloper12_->val.array.lparray[k] = *(LPXLOPER12)XlfOper(cells(i,j).NumericValue());
            else
                if (cells(i,j).IsAString())
                    xlfOper.lpxloper12_->val.array.lparray[k] = *(LPXLOPER12)XlfOper(cells(i,j).StringValue());
                else
                if (cells(i,j).IsAWstring())
                    xlfOper.lpxloper12_->val.array.lparray[k] = *(LPXLOPER12)XlfOper(cells(i,j).WstringValue());
                else
                    if (cells(i,j).IsBoolean())
                            xlfOper.lpxloper12_->val.array.lparray[k] = *(LPXLOPER12)XlfOper(cells(i,j).BooleanValue());
                    else
                        if (cells(i,j).IsError())
                             xlfOper.lpxloper12_->val.array.lparray[k] = *(LPXLOPER12)XlfOper(static_cast<short>(cells(i,j).ErrorValue()),true);
                        else
                                xlfOper.lpxloper12_->val.array.lparray[k] = *(LPXLOPER12)XlfOper("");
        }

    return xlfOper;

}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, LPXLFOPER lpxlfoper) const
{
    assert(lpxlfoper != 0);
    xlfOper.lpxloper12_ = reinterpret_cast<LPXLOPER12>(lpxlfoper);
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, double value) const
{
    if (xlfOper.lpxloper12_)
    {
        xlfOper.lpxloper12_->xltype = xltypeNum;
        xlfOper.lpxloper12_->val.num = value;
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, short value) const
{
    if (xlfOper.lpxloper12_)
    {
        xlfOper.lpxloper12_->xltype = xltypeInt;
        xlfOper.lpxloper12_->val.w = value;
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, short value, bool Error) const
{
    if (xlfOper.lpxloper12_)
    {
        if (Error)
        {
            xlfOper.lpxloper12_->xltype = xltypeErr;
            xlfOper.lpxloper12_->val.err =value;
        }
        else
        {
            xlfOper.lpxloper12_->xltype = xltypeInt;
            xlfOper.lpxloper12_->val.w = value;
        }
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, bool value) const
{
    if (xlfOper.lpxloper12_)
    {
        xlfOper.lpxloper12_->xltype = xltypeBool;
        xlfOper.lpxloper12_->val.xbool = value;
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, const XlfRef& range) const
{
    if (xlfOper.lpxloper12_)
    {
        xlfOper.lpxloper12_->xltype = xltypeRef;
        XLMREF12 * pmRef = reinterpret_cast<XLMREF12 *>(XlfExcel::Instance().GetMemory(sizeof(XLMREF12)));
        // if no memory is available
        if (pmRef == 0)
        {
            // set XlfOper to an invalid state
            xlfOper.lpxloper12_=0;
        }
        else
        {
            pmRef->count=1;
            pmRef->reftbl[0].rwFirst = range.GetRowBegin();
            pmRef->reftbl[0].rwLast = range.GetRowEnd()-1;
            pmRef->reftbl[0].colFirst = range.GetColBegin();
            pmRef->reftbl[0].colLast = range.GetColEnd()-1;
            xlfOper.lpxloper12_->val.mref.lpmref = pmRef;
            xlfOper.lpxloper12_->val.mref.idSheet = range.GetSheetId();
        }
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, const char *value) const
{
    if (xlfOper.lpxloper12_)
    {
        size_t len = strlen(value);
        xlfOper.lpxloper12_->val.str = (XCHAR*)XlfExcel::Instance().GetMemory(len*2+2);
        if (xlfOper.lpxloper12_->val.str) {
            xlfOper.lpxloper12_->xltype = xltypeStr;
            mbstowcs(xlfOper.lpxloper12_->val.str + 1, value, len*2);
            xlfOper.lpxloper12_->val.str[0] = static_cast<XCHAR>(len);
        } else {
            xlfOper.lpxloper12_ = 0;
        }
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, const std::wstring &value) const
{
    if (xlfOper.lpxloper12_)
    {
        xlfOper.lpxloper12_->xltype = xltypeStr;
        xlfOper.lpxloper12_->val.str = (XCHAR*)XlfExcel::Instance().GetMemory(value.length()*2+2);
        wcsncpy(xlfOper.lpxloper12_->val.str + 1, value.c_str(), value.length());
        xlfOper.lpxloper12_->val.str[0] = static_cast<XCHAR>(value.length());
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::Set(XlfOper &xlfOper, RW r, COL c) const
{
    xlfOper.lpxloper12_->xltype = xltypeMulti;
    xlfOper.lpxloper12_->val.array.rows = r;
    xlfOper.lpxloper12_->val.array.columns = c;
    xlfOper.lpxloper12_->val.array.lparray = (LPXLOPER12)XlfExcel::Instance().GetMemory(r * c * sizeof(XLOPER12));
    for (size_t i = 0; i < static_cast<size_t>(r * c); ++i)
        xlfOper.lpxloper12_->val.array.lparray[i].xltype = xltypeNil;
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::SetElement(XlfOper &xlfOper, RW r, COL c, const XlfOper &value) const
{
    if (!(xlfOper.lpxloper12_->xltype & xltypeMulti))
        throw XlfException("attempt to perform an xltypeMulti operation on a value of type " + xlfOper.lpxloper12_->xltype);
    xloper12 &element = xlfOper.lpxloper12_->val.array.lparray[r * xlfOper.lpxloper12_->val.array.columns + c];
    if (value.lpxloper12_->xltype == xltypeNum) {
        element.val.num = value.lpxloper12_->val.num;
    } else if (value.lpxloper12_->xltype == xltypeStr) {
        size_t n = (value.lpxloper12_->val.str[0] + 1) * 2;
        element.val.str = reinterpret_cast<wchar_t*>(XlfExcel::Instance().GetMemory(n));
        memcpy(element.val.str, value.lpxloper12_->val.str, n);
    } else if (value.lpxloper12_->xltype == xltypeBool) {
        element.val.xbool = value.lpxloper12_->val.xbool;
    } else if (value.lpxloper12_->xltype == xltypeErr) {
        element.val.err = value.lpxloper12_->val.err;
    } else if (value.lpxloper12_->xltype == xltypeInt) {
        element.val.w = value.lpxloper12_->val.w;
    } else {
        throw XlfException("attempt to set a matrix element to a value of type " + value.lpxloper12_->xltype);
    }
    element.xltype = value.lpxloper12_->xltype;
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::SetError(XlfOper &xlfOper, WORD error) const
{
    if (xlfOper.lpxloper12_)
    {
        xlfOper.lpxloper12_->xltype = xltypeErr;
        xlfOper.lpxloper12_->val.err = error;
    }
    return xlfOper;
}

xlw::XlfOper& xlw::XlfOperImpl12::operator_assignment(XlfOper &xlfOper, const XlfOper &rhs) const
{
    //if (xlfOper != &rhs) FIXME
        xlfOper.lpxloper12_ = rhs.lpxloper12_;
    return xlfOper;
}

xlw::XlfOper xlw::XlfOperImpl12::operator_subscript(XlfOper &xlfOper, RW row, COL col) const
{
    if (!(xlfOper.lpxloper12_->xltype & xltypeMulti))
        throw XlfException("attempt to perform an xltypeMulti operation on a value of type " + xlfOper.lpxloper12_->xltype);
    return XlfOper(&xlfOper.lpxloper12_->val.array.lparray[row * xlfOper.lpxloper12_->val.array.columns + col]);
}

LPXLOPER xlw::XlfOperImpl12::operator_LPXLOPER(const XlfOper &xlfOper) const
{
    throw("Unable to convert Excel 12 datatype to Excel 4 datatype - operation not yet implemented");
}

LPXLOPER12 xlw::XlfOperImpl12::operator_LPXLOPER12(const XlfOper &xlfOper) const
{
    return xlfOper.lpxloper12_;
}

xlw::LPXLFOPER xlw::XlfOperImpl12::operator_LPXLFOPER(const XlfOper &xlfOper) const
{
    return xlfOper.lpxloper12_;
}

bool xlw::XlfOperImpl12::IsMissing(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeMissing) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsError(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeErr) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsRef(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeRef) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsSRef(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeSRef) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsMulti(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeMulti) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsNumber(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeNum) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsString(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeStr) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsNil(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeNil) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsBool(const XlfOper &xlfOper) const
{
    return (xlfOper.lpxloper12_->xltype & xltypeBool) == 0 ? false : true ;
}

bool xlw::XlfOperImpl12::IsInt(const XlfOper &xlfOper) const
{
	return (xlfOper.lpxloper12_->xltype & xltypeInt) == 0 ? false : true ;
}

xlw::LPXLFOPER xlw::XlfOperImpl12::GetLPXLFOPER(const XlfOper &xlfOper) const
{
    return xlfOper.lpxloper12_;
}

DWORD xlw::XlfOperImpl12::xltype(const XlfOper &xlfOper) const {
    if (xlfOper.lpxloper12_)
        return xlfOper.lpxloper12_->xltype;
    else
        return 0;
}

RW xlw::XlfOperImpl12::rows(const XlfOper &xlfOper) const
{
    if (!(xlfOper.lpxloper12_->xltype & xltypeMulti))
        throw XlfException("attempt to perform an xltypeMulti operation on a value of type " + xlfOper.lpxloper12_->xltype);
    return xlfOper.lpxloper12_->val.array.rows;
}

COL xlw::XlfOperImpl12::columns(const XlfOper &xlfOper) const
{
    if (!(xlfOper.lpxloper12_->xltype & xltypeMulti))
        throw XlfException("attempt to perform an xltypeMulti operation on a value of type " + xlfOper.lpxloper12_->xltype);
    return xlfOper.lpxloper12_->val.array.columns;
}


