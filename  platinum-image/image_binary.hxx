// $Id$

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

#ifndef __image_binary_hxx__
#define __image_binary_hxx__

#include "image_binary.h"
#include "image_label.hxx"


template <int IMAGEDIM>
image_binary<IMAGEDIM>::image_binary(int w, int h, int d, IMGBINARYTYPE *ptr):image_label<IMAGEDIM>(w, h, d, ptr)
{}
/*
template<int IMAGEDIM>
template<class SOURCETYPE>
image_binary<IMAGEDIM>::image_binary(image_general<SOURCETYPE, IMAGEDIM> * old_image, bool copyData): image_label<IMAGEDIM>(old_image, copyData)
{}
*/
/*
template <int IMAGEDIM>
image_binary<IMAGEDIM>::image_binary(std::vector<std::string> files, long width, long height, bool bigEndian, long headerSize, Vector3D voxelSize, unsigned int startFile, unsigned int increment): image_label<IMAGEDIM> (files, width, height, bigEndian, headerSize, voxelSize, startFile,increment) 
{}

*/
template <int IMAGEDIM>
image_binary<IMAGEDIM>::image_binary<IMAGEDIM>(itk::SmartPointer< itk::OrientedImage<IMGBINARYTYPE, IMAGEDIM > > &i):image_label<IMAGEDIM>(i)
{}

template <int IMAGEDIM>
image_binary<IMAGEDIM>::image_binary(const string filepath, const string name):image_label<IMAGEDIM>(filepath, name) 
{}




template <int IMAGEDIM>
image_binary<IMAGEDIM>* image_binary<IMAGEDIM>::get_subvolume_from_slice_3D(int slice, int dir)
{
	cout<<"get_subvolume_from_region_3D..."<<endl;

	int usize=this->get_size_by_dim_and_dir(0,dir);
	int vsize=this->get_size_by_dim_and_dir(1,dir);
	pt_error::error_if_false(slice>=0 && slice<this->get_size_by_dim_and_dir(2,dir)," slice outside image dimensions in image_scalar<ELEMTYPE, IMAGEDIM>::get_subvolume_from_slice_3D",pt_error::debug);

	image_binary<IMAGEDIM>* res;
	if (dir==0) {
		res = new image_binary<IMAGEDIM>(1, usize, vsize);
		res->set_parameters(this);
		res->set_origin(this->get_physical_pos_for_voxel(slice,0,0));
	} else if (dir==1) {
		res = new image_binary<IMAGEDIM>(vsize, 1, usize);
		res->set_parameters(this);
		res->set_origin(this->get_physical_pos_for_voxel(0,slice,0));
	} else {
		res = new image_binary<IMAGEDIM>(usize, vsize, 1);
		res->set_parameters(this);
		res->set_origin(this->get_physical_pos_for_voxel(0,0,slice));
	}

	for (int u=0; u<usize; u++){
		for (int v=0; v<vsize; v++){
			res->set_voxel_by_dir(u,v,0, this->get_voxel_by_dir(u,v,slice,dir), dir);
		}
	}
	return res;
}

template <int IMAGEDIM>
image_binary<IMAGEDIM>* image_binary<IMAGEDIM>::get_subvolume_from_region_3D(Vector3Dint vox_pos, Vector3Dint vox_size)
{
	return get_subvolume_from_region_3D(vox_pos[0],vox_pos[1],vox_pos[2],vox_pos[0]+vox_size[0],vox_pos[1]+vox_size[1],vox_pos[2]+vox_size[2]);
}

template <int IMAGEDIM>
image_binary<IMAGEDIM>* image_binary<IMAGEDIM>::get_subvolume_from_region_3D(int x1, int y1, int z1, int x2, int y2, int z2)
{
	cout<<"image_binary-get_subvolume_from_region_3D..."<<endl;

	int nx=this->nx();
	int ny=this->ny();
	int nz=this->nz();
	x1 = max(x1,0);	x2 = min(x2,nx);
	y1 = max(y1,0);	y2 = min(y2,ny);
	z1 = max(z1,0);	z2 = min(z2,nz);

	image_binary<IMAGEDIM>* res = new image_binary<IMAGEDIM>(x2-x1+1, y2-y1+1, z2-z1+1);
	res->set_parameters(this);
	res->set_origin(this->get_physical_pos_for_voxel(x1,y1,z1));

	for (int z=z1, res_z=0; z<=z2; z++, res_z++){
		for (int y=y1, res_y=0; y<=y2; y++,res_y++){
			for (int x=x1, res_x=0; x<=x2; x++,res_x++){
				res->set_voxel(res_x,res_y,res_z, this->get_voxel(x,y,z));
			}
		}
	}
	return res;
}

// *** Logical operations ***
    
template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_or(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value)
    {
    image_binary<IMAGEDIM> * output = new image_binary (this,false);
    
    image_storage<IMGBINARYTYPE >::iterator i1 = this->begin();
    image_storage<IMGBINARYTYPE >::iterator i2 = input->begin();
    image_storage<IMGBINARYTYPE >::iterator o = output->begin();
    while (i1 != this->end()) //images are same size and should necessarily end at the same time
        {
        if(*i1 == object_value || *i2 == object_value)
            {*o=object_value;}
        else
            {*o=!object_value;}
        ++i1; ++i2; ++o;
        }
	//output->image_has_changed();
    return output;
    }
    
template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_and(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value)
    {
    image_binary<IMAGEDIM> * output = new image_binary (this,false);
    
    image_storage<IMGBINARYTYPE >::iterator i1 = this->begin();
    image_storage<IMGBINARYTYPE >::iterator i2 = input->begin();
    image_storage<IMGBINARYTYPE >::iterator o = output->begin();
    while (i1 != this->end()) //images are same size and should necessarily end at the same time
        {
        if(*i1 == object_value && *i2 == object_value)
            {*o=object_value;}
        else
            {*o=!object_value;}
        ++i1; ++i2; ++o;
        }
	//output->image_has_changed();
    return output;
    }
    
template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_xor(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value)
    {
    image_binary<IMAGEDIM> * output = new image_binary (this,false);
    
    image_storage<IMGBINARYTYPE >::iterator i1 = this->begin();
    image_storage<IMGBINARYTYPE >::iterator i2 = input->begin();
    image_storage<IMGBINARYTYPE >::iterator o = output->begin();
    while (i1 != this->end()) //images are same size and should necessarily end at the same time
        {
        if((*i1 == object_value && *i2 != object_value) || (*i1 != object_value && *i2 == object_value))
            {*o=object_value;}
        else
            {*o=!object_value;}
        ++i1; ++i2; ++o;
        }
	//output->image_has_changed();
    return output;
    }
template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_or_not(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value)
    {
    image_binary<IMAGEDIM> * output = new image_binary (this,false);
    
    image_storage<IMGBINARYTYPE >::iterator i1 = this->begin();
    image_storage<IMGBINARYTYPE >::iterator i2 = input->begin();
    image_storage<IMGBINARYTYPE >::iterator o = output->begin();
    while (i1 != this->end()) //images are same size and should necessarily end at the same time
        {
        if(*i1 == object_value || *i2 != object_value)
            {*o=object_value;}
        else
            {*o=!object_value;}
        ++i1; ++i2; ++o;
        }
	//output->image_has_changed();
    return output;
    }
    
template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_and_not(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value)
    {
    image_binary<IMAGEDIM> * output = new image_binary (this,false);
    
    image_storage<IMGBINARYTYPE >::iterator i1 = this->begin();
    image_storage<IMGBINARYTYPE >::iterator i2 = input->begin();
    image_storage<IMGBINARYTYPE >::iterator o = output->begin();
    while (i1 != this->end()) //images are same size and should necessarily end at the same time
        {
        if(*i1 == object_value && *i2 != object_value)
            {*o=object_value;}
        else
            {*o=!object_value;}
        ++i1; ++i2; ++o;
        }
	//output->image_has_changed();
    return output;
    }

template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_or_prev_and_next(int direction, IMGBINARYTYPE object_value)    
	{
    image_binary<IMAGEDIM> * output = new image_binary (this,false);

	int u,v,w;
	int max_u, max_v, max_w;
	max_u=this->get_size_by_dim_and_dir(0,direction);
	max_v=this->get_size_by_dim_and_dir(1,direction);
	max_w=this->get_size_by_dim_and_dir(2,direction);
		
	IMGBINARYTYPE p,prev,next,res;//pixel value

	//First slice
	w=0;
	for(v=0; v<max_v; v++)
		{
		for(u=0; u<max_u; u++)
			{
			p=this->get_voxel_by_dir(u,v,w,direction);
			output->set_voxel_by_dir(u,v,w,p,direction);
			}
		}

	//Mid slices
	for(w=1; w<max_w-1; w++)
		{
		for(v=0; v<max_v; v++)
			{
			for(u=0; u<max_u; u++)
				{
				p=this->get_voxel_by_dir(u,v,w,direction);
				prev=this->get_voxel_by_dir(u,v,w-1,direction);
				next=this->get_voxel_by_dir(u,v,w+1,direction);
				res=(p==object_value || (prev==object_value && next==object_value))?object_value:!object_value;
				output->set_voxel_by_dir(u,v,w,res,direction);
				}
			}
		}
	//Last slice
	w=max_w-1;
	for(v=0; v<max_v; v++)
		{
		for(u=0; u<max_u; u++)
			{
			p=this->get_voxel_by_dir(u,v,w,direction);
			output->set_voxel_by_dir(u,v,w,p,direction);
			}
		}

	return output;
	}

template <int IMAGEDIM>
image_binary<IMAGEDIM> * image_binary<IMAGEDIM>::logical_and_prev_or_next(int direction, IMGBINARYTYPE object_value)    
	{
    image_binary<IMAGEDIM> * output = new image_binary (this,false);

	int u,v,w;
	int max_u, max_v, max_w;
	max_u=this->get_size_by_dim_and_dir(0,direction);
	max_v=this->get_size_by_dim_and_dir(1,direction);
	max_w=this->get_size_by_dim_and_dir(2,direction);
		
	IMGBINARYTYPE p,prev,next,res;//pixel value

	//First slice
	w=0;
	for(v=0; v<max_v; v++)
		{
		for(u=0; u<max_u; u++)
			{
			p=this->get_voxel_by_dir(u,v,w,direction);
			output->set_voxel_by_dir(u,v,w,p,direction);
			}
		}

	//Mid slices
	for(w=1; w<max_w-1; w++)
		{
		for(v=0; v<max_v; v++)
			{
			for(u=0; u<max_u; u++)
				{
				p=this->get_voxel_by_dir(u,v,w,direction);
				prev=this->get_voxel_by_dir(u,v,w-1,direction);
				next=this->get_voxel_by_dir(u,v,w+1,direction);
				res=(p==object_value && (prev==object_value || next==object_value))?object_value:!object_value;
				output->set_voxel_by_dir(u,v,w,res,direction);
				}
			}
		}
	//Last slice
	w=max_w-1;
	for(v=0; v<max_v; v++)
		{
		for(u=0; u<max_u; u++)
			{
			p=this->get_voxel_by_dir(u,v,w,direction);
			output->set_voxel_by_dir(u,v,w,p,direction);
			}
		}

	return output;
	}

template <int IMAGEDIM>
void image_binary<IMAGEDIM>::invert()
    {
    image_storage<IMGBINARYTYPE >::iterator i = this->begin();
    while (i != this->end())
        {
        *i = !(*i);
        ++i;
        }
	//this->image_has_changed();
    } 

template <int IMAGEDIM>
image_base* image_binary<IMAGEDIM>::expand_borders(unsigned int dx, unsigned int dy, unsigned int dz, IMGBINARYTYPE value)
{
	cout<<"expanding image borders (image_binary)..."<<endl;
	int old_size_x=this->get_size_by_dim(0);
	int old_size_y=this->get_size_by_dim(1);
	int old_size_z=this->get_size_by_dim(2);
	image_binary<IMAGEDIM>* res = new image_binary<IMAGEDIM>(old_size_x+2*dx, old_size_y+2*dy, old_size_z+2*dz);
	res->fill(value);
	res->set_parameters(this);

	for (int z=0; z<old_size_z; z++){
		for (int y=0; y<old_size_y; y++){
			for (int x=0; x<old_size_x; x++){
				res->set_voxel(x+dx,y+dy,z+dz, this->get_voxel(x,y,z));
			}
		}
	}
	res->set_origin(this->get_physical_pos_for_voxel(-dx,-dy,-dz));
	return res;
}

template <int IMAGEDIM>
image_base* image_binary<IMAGEDIM>::expand_borders2D_by_dir(int dir, unsigned int dr, IMGBINARYTYPE value)
{
	image_binary<IMAGEDIM>* res;
	if(dir==2){
		res = binary_copycast<IMAGEDIM>( expand_borders(dr,dr,0,value) );
	}else if(dir==1){
		res = binary_copycast<IMAGEDIM>( expand_borders(dr,0,dr,value) );
	}else{
		res = binary_copycast<IMAGEDIM>( expand_borders(0,dr,dr,value) );
	}
	return res;
}

template <int IMAGEDIM>
image_base* image_binary<IMAGEDIM>::contract_borders(unsigned int dx, unsigned int dy, unsigned int dz)
{
	cout<<"contracting image borders (image_binary)..."<<endl;
	int old_size_x=this->get_size_by_dim(0);
	int old_size_y=this->get_size_by_dim(1);
	int old_size_z=this->get_size_by_dim(2);
	image_binary<IMAGEDIM>* res = new image_binary<IMAGEDIM>(old_size_x-2*dx, old_size_y-2*dy, old_size_z-2*dz);
	res->set_parameters(this);

	for (int z=0; z<res->nz(); z++){
		for (int y=0; y<res->ny(); y++){
			for (int x=0; x<res->nx(); x++){
				res->set_voxel(x,y,z, this->get_voxel(x+dx,y+dy,z+dz));
			}
		}
	}
	res->set_origin(this->get_physical_pos_for_voxel(0+dx,0+dy,0+dz));
	return res;
}

template <int IMAGEDIM>
image_base* image_binary<IMAGEDIM>::contract_borders2D_by_dir(int dir, unsigned int dr)
{
	image_binary<IMAGEDIM>* res;
	if(dir==2){
		res = binary_copycast<IMAGEDIM>( contract_borders(dr,dr,0) );
	}else if(dir==1){
		res = binary_copycast<IMAGEDIM>( contract_borders(dr,0,dr) );
	}else{
		res = binary_copycast<IMAGEDIM>( contract_borders(0,dr,dr) );
	}
	return res;
}



template <int DIM>
image_binary<DIM>* binary_copycast (image_base* input) 
{
    image_binary<DIM > * output = NULL;

        {
        image_integer <unsigned char, DIM>* input_general =
            dynamic_cast<image_integer <unsigned char, DIM> *> (input) ; 

        if (input_general != NULL) //! If cast was successful, input had the tried type and input_general can be used in a call to new class' copy constructor
            {
            output = new image_binary<DIM> (input_general,true);
            }
        }

        {
        image_integer <unsigned short,DIM>* input_general =
            dynamic_cast<image_integer <unsigned short, DIM> *> (input) ;

        if (input_general != NULL) //! If cast was successful, input had the tried type and input_general can be used in a call to new class' copy constructor
            {
            output = new image_binary<DIM> (input_general,true);
            //delete input;
            } 
        }

    return output;
    }

#include "image_binaryprocess.hxx"

#endif
