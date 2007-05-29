//////////////////////////////////////////////////////////////////////////
//
//  Rendergeometry $Revision$
///
/// Rendergeometry stores rendering coordinates (look at, orientation
/// and zoom) for renderers
///
//  $LastChangedBy$

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

#ifndef __rendergeometry__
#define __rendergeometry__

#include "ptmath.h"

#include "global.h"

class rendergeometry 
    {
    private:
        int id;

        static int new_rg_ID;                   //ID counter for new geometry objects
    public:
        rendergeometry();

        Matrix3D view_to_unit(int viewminsize);

        // *** NOTE: some render parameter constraints (like not zooming in/out at insane levels)
        //     are implemented in renderer_base::move

        Matrix3D dir;               //view to composite image matrix
        //only direction (no scaling) = normal of slice plane

        float zoom;                 //user-determined magnification, multiplied with scale to
        //obtain the actual rendering scale

        Vector3D look_at;           //center viewpoint, this will be the middle of the
        //rendering and pivot point for the slice direction.
        int get_id();

        void refresh_viewports();   //refresh viewports using this combination
    };

#endif