//////////////////////////////////////////////////////////////////////////
//
//  rendererVTK $Revision$
//
//  Abstract class for VTK-based renderers
//
//  $LastChangedBy$

// This file is part of the Platinum library.
// Copyright (c) 2007 Uppsala University.
//
//    The Platinum library is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    The Platinum library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with the Platinum library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef __rendererVTK__
#define __rendererVTK__

#include "renderer_base.h"

class rendererVTK : public renderer_base
    {
public:
        rendererVTK();
        void draw(void);
        
        virtual void connect_data(int dataID){}
    };

#endif