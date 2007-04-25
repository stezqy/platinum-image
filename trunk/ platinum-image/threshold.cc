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

#include "threshold.h"

#include "FLTKviewport.h"
#include "rendermanager.h"

#include "image_label.h"

#include "rawimporter.h"

extern datamanager datamanagement;
extern rendermanager rendermanagement;

thresholdparvalue::thresholdparvalue ()
    {
    mode= THRESHOLD_2D_MODE_RECT;

    for (unsigned int d=0;d < THRESHOLDMAXCHANNELS; d++)
        {
        id[d]=NOT_FOUND_ID;
        high[d]=low[d]=0;
        }
    }

int  thresholdparvalue::get_id(int axis)
    {
    int a;
    for (a = 0; a < axis && a < THRESHOLDMAXCHANNELS && id[a] != NOT_FOUND_ID; a++ )
        {}

    if (a == axis)
        {
        return id[axis];
        }

    return NOT_FOUND_ID;
    }

int thresholdparvalue::make_threshold_volume ()
    {
    short size[3];
    int num_volumes=0;
    int result_vol_ID;

    image_base * the_inputs [THRESHOLDMAXCHANNELS];

    image_label<3>  * the_result;

    for (int v=0;id[v] != NOT_FOUND_ID;v++)
        {
        the_inputs [v] = datamanagement.get_image(id[v]);
        num_volumes = v+1;
        }

    if (num_volumes == 0)
        {
#ifdef _DEBUG
        std::cout << "Segmentation: no volumes selected in threshold" << std::endl;
#endif
        return NOT_FOUND_ID;
        }

    for (int d=0; d < 3; d++)
        {size[d]=the_inputs[0]->get_size_by_dim(d);}

    //check that all volumes have same dimensions
    for (int v = 1; v < num_volumes; v++)
        {
        for (int d=0; d < 3; d++)
            {
            if (size[d]!=the_inputs[v]->get_size_by_dim(d))
                {
#ifdef _DEBUG
                std::cout << "Segmentation: volume sizes do not match" << std::endl;
#endif
                return NOT_FOUND_ID;
                }
            }
        }

    //input volumes are expected to all have the same dimensions, just use the first
    //as blueprint for an empty one

    result_vol_ID= datamanagement.create_empty_volume(the_inputs[0], VOLDATA_UCHAR);
    the_result=(image_label<3> *)datamanagement.get_image(result_vol_ID);

    for (short z = 0; z < size[2];z++)
        {
        for (short y = 0; y < size[1];y++)
            {
            for (short x = 0; x < size[0];x++)
                {
                float rightey;
                bool value=true;
                float t_value [THRESHOLDMAXCHANNELS];

                for (int v = 0; v < num_volumes && value; v++)
                    {
                    t_value[v] = the_inputs [v]->get_number_voxel(x,y,z);

                    //rect threshold
                    value = value && t_value[v] > low[v] && t_value[v] < high[v];
                    }

                if (value && num_volumes == 2 && mode==THRESHOLD_2D_MODE_OVAL)
                    {
                    //oval threshold
                    value = value && (std::sqrt(powf((t_value[0]-((high[0]+low[0])/2.0))/((high[0]-low[0])/(high[1]-low[1])),2.0)+powf(t_value[1]-((high[1]+low[1])/2.0),2.0) ) <= (high[1]+low[1])/2.0);
                    }

                the_result->set_voxel(x,y,z,value ? 1 : 0);
                rightey = the_result->get_number_voxel(x,y,z);
                rightey = the_result->get_number_voxel(x,y,z);
                }
            }
        }

    datamanagement.image_has_changed( result_vol_ID, true);

    return result_vol_ID;
    }

threshold_overlay::threshold_overlay(FLTKviewport * o, int r_index)
    {
    owner=o;

    threshold=NULL;

    width=owner->w();
    height=owner->h();

    overlay_image_data = NULL;
    overlay_image_data = new unsigned char [width*height*RGBApixmap_bytesperpixel];

    overlay_image = NULL;
    overlay_image = new Fl_RGB_Image(overlay_image_data, width, height, RGBApixmap_bytesperpixel, 0);
    rendererIndex= r_index;
    }

threshold_overlay::~threshold_overlay()
    {
    delete overlay_image;
    delete []overlay_image_data;
    }

void threshold_overlay::render (thresholdparvalue * t)
    {
    if (t!= NULL)
        {
        //previous threshold can be re-rendered (eg. when panning)
        threshold=t;
        }

    if (threshold !=NULL)
        {
        rendermanagement.render_threshold (rendererIndex, overlay_image_data, width, height, threshold);
        overlay_image->uncache();

        owner->damage (FL_DAMAGE_ALL);
        }
    }

void threshold_overlay::FLTK_draw()
    {    
    if (threshold !=NULL)
        {        
        overlay_image->draw(owner->x(),owner->y());
        }
    }

void threshold_overlay::expire()
    {
    threshold=NULL;
    }

void threshold_overlay::renderer_index (int index)
    {
    rendererIndex=index;
    }