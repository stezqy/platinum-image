//////////////////////////////////////////////////////////////////////////
//
//  Image_storage
//
//  Abstract base class storing image data as a stream
//
//

// This file is part of the Platinum library.
// Copyright (c) 2007 Uppsala University.
//
//    The Platinum library is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    The Platinum library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with the Platinum library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef __image_storage__
#define __image_storage__

template<class ELEMTYPE>
class image_storage : public image_base
    {
    protected:
        image_storage() : image_base() {}
        ELEMTYPE *imageptr;

        ELEMTYPE maxvalue;
        ELEMTYPE minvalue;

        unsigned long num_elements;        //volume size in # pixels/voxels

    public:
        void erase ();

        // *** iterator ***        
        class iterator : public std::iterator<std::forward_iterator_tag, ELEMTYPE>
            {
            public:
                iterator(ELEMTYPE* i);               
                ~iterator() {}
            iterator& operator=(const iterator& other);                
            bool operator==(const iterator& other);                
            bool operator!=(const iterator& other);                
            iterator& operator++();                
            iterator& operator++(int);                
            ELEMTYPE& operator*();                
            ELEMTYPE* operator->();                
            private:
                ELEMTYPE* ptr;
            };

        iterator begin();        
        iterator end() ;   
    };


//with C++ templates, declaration and definition go together
#include "image_storage_iterator.hxx"
#include "image_storage.hxx"

#endif