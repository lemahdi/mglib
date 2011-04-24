
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

/*!
\file XlfRef.inl
\brief Implements inline methods of class XlfRef.
*/

// $Id: XlfRef.inl 474 2008-03-05 15:40:40Z ericehlers $

#ifdef NDEBUG
#define INLINE inline
#else
#define INLINE
#endif

namespace xlw {

    INLINE XlfRef::XlfRef()
        :rowbegin_(0), rowend_(0), colbegin_(0), colend_(0), sheetId_(0)
    {}

    INLINE XlfRef::XlfRef(INT32 top, INT32 left, INT32 bottom, INT32 right, DWORD sheetId)
        :rowbegin_(top), rowend_(bottom + 1), colbegin_(left), colend_(right + 1), sheetId_(sheetId)
    {}

    INLINE XlfRef::XlfRef(INT32 row, INT32 col, DWORD sheetId)
        :rowbegin_(row), rowend_(row + 1), colbegin_(col), colend_(col + 1), sheetId_(sheetId)
    {}

    INLINE INT32 XlfRef::GetRowBegin() const
    {
        return rowbegin_;
    }

    INLINE INT32 XlfRef::GetRowEnd() const
    {
        return rowend_;
    }

    INLINE INT32 XlfRef::GetColBegin() const
    {
        return colbegin_;
    }

    INLINE INT32 XlfRef::GetColEnd() const
    {
        return colend_;
    }

    INLINE DWORD XlfRef::GetSheetId() const
    {
        return sheetId_;
    }

    INLINE INT32 XlfRef::GetNbCols() const
    {
        return colend_-colbegin_;
    }

    INLINE INT32 XlfRef::GetNbRows() const
    {
        return rowend_-rowbegin_;
    }

    INLINE void XlfRef::SetRowBegin(INT32 rowbegin)
    {
        rowbegin_ = rowbegin;
    }

    INLINE void XlfRef::SetRowEnd(INT32 rowend)
    {
        rowend_ = rowend;
    }

    INLINE void XlfRef::SetColBegin(INT32 colbegin)
    {
        colbegin_ = colbegin;
    }

    INLINE void XlfRef::SetColEnd(INT32 colend)
    {
        colend_ = colend;
    }

    INLINE void XlfRef::SetSheetId(DWORD sheetId)
    {
        sheetId_ = sheetId;
    }

}
