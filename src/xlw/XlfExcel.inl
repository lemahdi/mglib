
/*
 Copyright (C) 1998, 1999, 2001, 2002, 2003, 2004 J�r�me Lecomte
 Copyright (C) 2007, 2008 Eric Ehlers
 Copyright (C) 2009 Narinder S Claire
 
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
\file XlfExcel.inl
\brief Implements inline methods of XlfExcel.
*/

// $Id: XlfExcel.inl 507 2008-03-27 13:33:20Z ericehlers $

#ifdef NDEBUG
#define INLINE inline
#else
#define INLINE
#endif
#include <iostream>
#include <xlw/XlfMutex.h>

namespace xlw {

    /*!
    The macro EXCEL_BEGIN includes a call to XlfExcel::FreeMemory.

    FreeMemory frees *all* memory previously allocated by the
    framework. Keeps the biggest buffer allocated (front one) so far
    for subsequent calls.

    \sa XlfBuffer.
    */
    INLINE void XlfExcel::FreeMemory(bool finished) {
        XlfMutex xlfMutex;
        size_t nbBuffersToKeep = 1;
        if (finished)
            nbBuffersToKeep = 0;
        while (freeList_.size() > nbBuffersToKeep) {
            delete[] freeList_.back().start;
            freeList_.pop_back();
        }
        offset_ = 0;
    }

    /*!
    Allocates a \c new[] buffer and pushes it in front of the list of buffers.
    \arg size Size of the new buffer in bytes.
    */
    INLINE void XlfExcel::PushNewBuffer(size_t size) {
        XlfBuffer newBuffer;
        newBuffer.size = size;
        newBuffer.start = new char[size];
        freeList_.push_front(newBuffer);
        offset_=0;
    #if !defined(NDEBUG)
        std::cerr << "xlw is allocating a new buffer of " << static_cast<unsigned int>(size) << " bytes" << std::endl;
    #endif
        return;
    }

    /*!
    \param bytes is the size of the chunk required in bytes.
    \return the address of the chunk.

    Check if the buffer has enough memory, and move the offset of the static
    buffer of the ammount of memory requested. If the buffer is full, a new
    buffer is allocated whose size is 150% of the one we just filled.

    \sa XlfBuffer, XlfExcel::PushNewBuffer(size_t)
    */
    INLINE LPSTR XlfExcel::GetMemory(size_t bytes) {
        XlfMutex xlfMutex;

        if (freeList_.empty())
            PushNewBuffer(8192);
        while (1) {
            XlfBuffer& buffer = freeList_.front();
            if (offset_ + bytes < buffer.size) {
                int temp = static_cast<int>(offset_);
                offset_ += bytes;
                LPSTR ret = buffer.start + temp;
                return ret;
            } else {
                PushNewBuffer(size_t(buffer.size*1.5));
            }
        }
        // should never get to this point...
        return 0;
    }

}
