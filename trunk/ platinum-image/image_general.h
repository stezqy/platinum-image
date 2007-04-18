//////////////////////////////////////////////////////////////////////////
//
//  Image_general
//
//  Abstract base class for the various image classes, templated over
//  dimension and voxel data type
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

#ifndef __image_general__
#define __image_general__

#include "image_base.h"

template<int IMAGEDIM>
    class image_binary;

#include <string>
#include <vector>
#include <iterator>

#include "itkImage.h"
#include "itkImportImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "global.h"
#include "color.h"

template<class ELEMTYPE, int IMAGEDIM = 3>
class image_general : public image_base
    {
    private:
        image_general<ELEMTYPE, IMAGEDIM>(int w, int h, int d, ELEMTYPE *ptr = NULL);

    protected:
        ELEMTYPE *imageptr;
        unsigned short datasize[IMAGEDIM]; //volume size
        unsigned long num_elements;        //volume size in # pixels/voxels

        ELEMTYPE maxvalue;
        ELEMTYPE minvalue;

        typename itk::ImportImageFilter<ELEMTYPE, IMAGEDIM>::Pointer   ITKimportfilter;
        typename itk::Image<ELEMTYPE, IMAGEDIM >::Pointer                ITKimportimage;

        //typename itk::ImageFileWriter<theImageType>::Pointer       ITKwriterfilter; //get the writer to write in this here array

        // *** Constructors & factories ***
        image_general<ELEMTYPE, IMAGEDIM>();
        image_general<ELEMTYPE, IMAGEDIM>(itk::SmartPointer< itk::Image<ELEMTYPE, IMAGEDIM > > &i);
        template<class SOURCETYPE> 
            image_general(image_general<SOURCETYPE, IMAGEDIM> * old_volume, bool copyData = true);

        void set_parameters ();                                                     //reset & calculate parameters
        void set_parameters (itk::SmartPointer< itk::Image<ELEMTYPE, IMAGEDIM > > &i);   //set parameters from ITK metadata
        void set_parameters (image_general<ELEMTYPE, IMAGEDIM> * from_volume);            //clone parameters from another image
        void calc_transforms ();                                            //used by set_parameters(...) functions
                                                                            //to recalculate cached transform(s)

    public:                                                    
        //virtual image_base * alike ();
        image_base * alike (imageDataType);

        //image_general<ELEMTYPE, IMAGEDIM> * alike ();               //using same type as well

       /* image_base * alike (imageDataType unit);*/                    //overloaded to allow
                                                                    //alike(...) call from base class
        /*image_base * alike ();*/

        image_general<ELEMTYPE, IMAGEDIM> (std::vector<std::string>, long width, long height, bool bigEndian = false, long headerSize = 0, Vector3D voxelSize = Vector3D (1,1,4), unsigned int startFile = 1,unsigned int increment = 1);
        //create image from file set. Notes:
        //A. uses POSIX paths ( / for directories) on all platforms - inlcuding Windows
        //B. Three modes of operation:
        //   1. single file - get multiple slices as specified in arguments
        //   2. multiple files in vector - each file contains one slice
        //   3. multiple files in directory, specified by path in first (only)
        //      element of vector. Reads all files (directories ignored) in argument dir. 

        void initialize_dataset(int w, int h, int d);                           //overloading from image_base
        void initialize_dataset(int w, int h, int d, ELEMTYPE *ptr);          //load might happen outside class

        template <class inType>
            void copy_image (image_general<inType, IMAGEDIM> * in);   //copy image data with type conversion

        //initialize volume from ITK image
        void replicate_itk_to_volume();     //use object's own ITK image pointer
        void replicate_itk_to_volume(itk::SmartPointer< itk::Image<ELEMTYPE, IMAGEDIM > > &i);

        void image_has_changed(bool min_max_refresh = false);          //called when volume data has been changed

        static image_base * type_from_DICOM_file (std::string file_path);
        static image_base * type_from_VTK_file (std::string file_path);

        // *** element access methods ***

        ELEMTYPE get_voxel(int x, int y, int z=0);  
        ELEMTYPE get_voxel(unsigned long offset); //deprecated: use iterator!

        RGBvalue get_display_voxel(itk::Vector<int,IMAGEDIM>);
        //const RGBvalue get_display_voxel(int x, int y, int z=0);
        unsigned char get_display_voxel(int x, int y, int z=0);  
        virtual void get_display_voxel(RGBvalue &val,int x, int y, int z=0);

        ELEMTYPE get_voxel_by_dir(int u, int v, int w, int direction=2);

        //ELEMTYPE get_number_voxel(itk::Vector<int,IMAGEDIM>);
        float get_number_voxel(int x, int y, int z);
        float get_max();
        float get_min();

        void set_voxel(int x, int y, int z, ELEMTYPE);
        void set_voxel(unsigned long offset, ELEMTYPE); //deprecated: use iterator!
		void set_voxel_by_dir(int u, int v, int w, ELEMTYPE value, int direction=2);

        void give_parametersXYplane(int renderstartX, int renderstartY, int renderwidth, int renderheight, int &startoffset, int &patchXoffset );
        void testpattern();
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
        iterator end();
        
        // *** size functions ***
        unsigned short get_size_by_dim(int dim);
        unsigned short get_size_by_dim_and_dir(int dim, int direction);
        bool same_size (image_base * other);    //test whether other volume
                                                //has same voxel dimensions

        void make_image_an_itk_reader();               //initialize ITKimportfilter

        //return ITKimportfilter
        typename itk::ImportImageFilter< ELEMTYPE, IMAGEDIM >::Pointer   itk_import_filter();

        //return ITK image from volume     
        typename itk::Image<ELEMTYPE, IMAGEDIM >::Pointer                itk_image();

        void load_dataset_from_VTK_file(std::string file_path);
        void load_dataset_from_DICOM_files(std::string dir_path,std::string seriesIdentifier);

        void save_image_to_VTK_file(std::string file_path);

        // *** processing ***
        image_binary<IMAGEDIM> * threshold(ELEMTYPE low, ELEMTYPE high, bool true_inside_threshold=true);
    };

//with C++ templates, declaration and definition go together
#include "image_generaliterator.hxx"
#include "image_general.hxx"
#include "image_generalfile.hxx"

#endif