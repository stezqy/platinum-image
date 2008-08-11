//////////////////////////////////////////////////////////////////////////
//
//  Image_binary $Revision$
//
//  Image type with binary voxel/pixel values
//  Implemented like image_label, only that processing expects 0/1 only
//
//  $LastChangedBy$
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

#ifndef __image_binary__
#define __image_binary__

#include "image_integer.h"

template<int IMAGEDIM = 3>
class image_binary : public image_label <IMAGEDIM>
    {
    public:
        image_binary(int w, int h, int d, IMGBINARYTYPE *ptr = NULL);

	    template<class SOURCETYPE>
        image_binary(image_general<SOURCETYPE, IMAGEDIM> * old_image, bool copyData = true) : image_label<IMAGEDIM>(old_image, copyData)//!copy constructor
		{}

		image_binary(std::vector<std::string> files, long width, long height, bool bigEndian = false, long headerSize = 0, Vector3D voxelSize = Vector3D (1,1,4), unsigned int startFile = 1,unsigned int increment = 1): image_label<IMAGEDIM> (files, width, height, bigEndian, headerSize, voxelSize, startFile,increment) 
		{}

	    image_binary<IMAGEDIM>(itk::SmartPointer< itk::OrientedImage<IMGBINARYTYPE, IMAGEDIM > > &i);
		image_binary(const string filepath, const string name="");
//		image_binary<IMAGEDIM>(itk::SmartPointer< itk::OrientedImage<IMGBINARYTYPE, IMAGEDIM > > &i):image_label<IMAGEDIM>(i){}
//		image_binary(const string filepath, const string name=""):image_label<IMAGEDIM>(filepath, name){}

	image_binary<IMAGEDIM>* get_subvolume_from_slice_3D(int slice, int dir=2);

	image_binary<IMAGEDIM>* get_subvolume_from_region_3D(Vector3Dint vox_pos, Vector3Dint vox_size);
	image_binary<IMAGEDIM>* get_subvolume_from_region_3D(int x1, int y1, int z1, int x2, int y2, int z2);

    // *** operations ***

    image_binary<IMAGEDIM> * logical_or(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A OR B operation
    image_binary<IMAGEDIM> * logical_and(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A AND B operation
    image_binary<IMAGEDIM> * logical_xor(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A XOR B operation
    image_binary<IMAGEDIM> * logical_or_not(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A OR (NOT B) operation
    image_binary<IMAGEDIM> * logical_and_not(image_binary<IMAGEDIM> *input, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A AND (NOT B) operation
    image_binary<IMAGEDIM> * logical_or_prev_and_next(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A OR (PrevSlice AND NextSlice) operation
    image_binary<IMAGEDIM> * logical_and_prev_or_next(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Perform a voxelwise logical A OR (PrevSlice AND NextSlice) operation
    void invert(); ///Perform a voxelwise inversion


	virtual image_base* expand_borders(unsigned int dx, unsigned int dy, unsigned int dz, IMGBINARYTYPE value=0);	
	virtual image_base* expand_borders2D_by_dir(int dir, unsigned int dr=1, IMGBINARYTYPE value=0);	
	virtual image_base* contract_borders(unsigned int dx, unsigned int dy, unsigned int dz);	
	virtual image_base* contract_borders2D_by_dir(int dir, unsigned int dr=1);	

    // *** applications ***

    // ----------- 2D operations in image_binaryprocess ---------------

	image_binary<3>* convex_hull_2D(int dir=2); // Get convex hull in 2D-planes orthogonal to the axis given by direction.
	image_binary<3>* convex_hull_objectwise_2D(int dir=2); // Get convex hull of each 4-connected object in 2D-planes orthogonal to the axis given by direction.
	void convex_hull_in_slice_2D(image_binary<3>* image, int dir=2, int slice=0); // Get convex hull in specified 2D-plane orthogonal to the axis given by direction.
	void convex_hull_objectwise_in_slice_2D(image_label<3>* image, int dir=2, int slice=0); // Get convex hull of each 4-connected object in specified 2D-plane orthogonal to the axis given by direction.
    void fill_holes_2D(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Fill holes in objects defined by object_value in 2D-planes orthogonal to the axis given by direction.
    image_label<3>* label_connected_objects_2D(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Get image_label from connected objects in 2D-planes orthogonal to the axis given by direction.
	image_integer<unsigned long, 3>* label_connected_objects_with_area_2D(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Get image_integer from connected objects in 2D-planes orthogonal to the axis given by direction with labels corresponding to object volume.
	void largest_object_2D(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Keep the largest object (defined by object_value) for each plane in 2D-planes orthogonal to the axis given by direction.
    void threshold_size_2D(int min_size, int direction=2, IMGBINARYTYPE object_value=TRUE); ///Keep all objects (defined by object_value) > min_size voxels for each plane in 2D-planes orthogonal to the axis given by direction.
    void cog_inside_2D(image_binary<IMAGEDIM>* mask, int direction=2, IMGBINARYTYPE object_value=TRUE); ///Keep all objects (defined by object_value) having their cog within mask in 2D-planes orthogonal to the axis given by direction.
	void erode_2D(int thickness=3, int direction=2, IMGBINARYTYPE object_value=TRUE); ///Morphological erode up to distance value=thickness for each plane in 2D-planes orthogonal to the axis given by direction.
	void dilate_2D(int thickness=3, int direction=2, IMGBINARYTYPE object_value=TRUE); ///Morphological dilate up to distance value=thickness for each plane in 2D-planes orthogonal to the axis given by direction.
	void outline_2D(int thickness=3, int direction=2, IMGBINARYTYPE object_value=TRUE); ///Morphological outline up to distance value=thickness for each plane in 2D-planes orthogonal to the axis given by direction.
    void connect_outer_2D(int direction=2, IMGBINARYTYPE object_value=TRUE); ///Connect outer contour of objects (defined by object_value) for each plane in 2D-planes orthogonal to the axis given by direction.
    image_integer<short, IMAGEDIM> * distance_34_2D(bool edge_is_object=false, int direction=2, IMGBINARYTYPE object_value=TRUE); ///Compute 34 chamfer distance map for each plane in 2D-planes orthogonal to the axis given by direction. If edge_is_object=true then everything outside the image is regarded to be object voxels.
	
	void get_num_neighbours_distribution_in_slice_2D_4Nbh(vector<int> &num_vox, int slice, int dir=2, IMGBINARYTYPE object_value=TRUE);
	void get_num_neighbours_distribution_in_slice_2D_8Nbh(vector<int> &num_vox, int slice, int dir=2, IMGBINARYTYPE object_value=TRUE);

    // --------------- 3D operations in image_binaryprocess ---------------

    void fill_holes_3D(IMGBINARYTYPE object_value=TRUE); ///Fill holes in objects defined by object_value.
	image_label<3>* label_connected_objects_3D(IMGBINARYTYPE object_value=TRUE); ///Get image_label from connected objects in binary.
	image_integer<unsigned long,3>* label_connected_objects_with_volume_3D(IMGBINARYTYPE object_value=TRUE); ///Get image_integer from connected objects in binary with labels corresponding to object volume.
	void largest_object_3D(IMGBINARYTYPE object_value=TRUE); ///Keep the largest object (defined by object_value).
	void largest_objects_3D(int num_objects=2, IMGBINARYTYPE object_value=TRUE); ///Keep the x largest objects (defined by object_value).
	void erode_3D(int thickness=3, IMGBINARYTYPE object_value=TRUE);///Morphological erode up to distance value=thickness.
	//JK - there is a bug in dilate3D... An in-slice line (with 2 segments...) is sometimes seen....
	void dilate_3D(int thickness=3, IMGBINARYTYPE object_value=TRUE); ///Morphological dilate up to distance value=thickness.
	void outline_3D(int thickness=3, IMGBINARYTYPE object_value=TRUE); ///Morphological outline up to distance value=thickness.
    image_integer<short, IMAGEDIM> * distance_345_3D(bool edge_is_object=false, IMGBINARYTYPE object_value=TRUE); ///Compute 345 chamfer distance map. If edge_is_object=true then everything outside the image is regarded to be object voxels.
	//TODO: create distancetransform where weights are given as argument (3,4,5)... can then be used to weight spatial resolution....

	void erode_3D_26Nbh(IMGBINARYTYPE object_value=TRUE); ///Morphological erode (26 Neighbourhood)
	void erode_3D_26Nbh(int num_iter, IMGBINARYTYPE object_value); ///Morphological dilate (26 Neighbourhood)
	void dilate_3D_26Nbh(IMGBINARYTYPE object_value=TRUE); ///Morphological dilate (26 Neighbourhood)
	void dilate_3D_26Nbh(int num_iter, IMGBINARYTYPE object_value); ///Morphological dilate (26 Neighbourhood)
	int find_voxel_index_percent_object_content(int dir, int object_content_percent, IMGBINARYTYPE object_value=TRUE); 
	void convex_hull_line_filling_3D(int dir, IMGBINARYTYPE object_value=TRUE); 

	void get_num_neighbours_distribution_3D_26Nbh(vector<int> &num_nb, vector<int> &num_vox, IMGBINARYTYPE object_value=TRUE); //Returns statistics on the number of object neighbours object voxels have.
	float get_border_volume_ratio_3D_26Nbh(int num_nb_inside_limit=17, IMGBINARYTYPE object_value=TRUE); //Returns statistics on the number of object neighbours object voxels have.
	float mutual_overlap_3D(image_binary<IMAGEDIM>* second_image);
    };

template <int DIM>
image_binary<DIM>* binary_copycast (image_base* input); //! Converts might-be binary types into image_binary

#endif
