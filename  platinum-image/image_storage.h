//////////////////////////////////////////////////////////////////////////
//
//   Image_storage $Revision$
///
///  Abstract base class storing image data as a stream
///
//   $LastChangedBy$
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

#include <typeinfo>

#include "image_base.h"
#include "histogram.h"
#include "transfer.h"

#define IMGLABELTYPE unsigned char
#define IMGBINARYTYPE IMGLABELTYPE

class image_base;


template<class ELEMTYPE>
class image_storage : public image_base
    {
    friend class transfer_base<ELEMTYPE>;

    private:
        void set_parameters ();
 
    protected:
        image_storage();
        template<class SOURCETYPE>
        image_storage(image_storage<SOURCETYPE> * const s);
//        image_storage(const string filepath);

        transfer_base<ELEMTYPE> *tfunction;
        histogram_1D<ELEMTYPE> *stats;

        virtual void transfer_function(transfer_base<ELEMTYPE> * t = NULL);
        //allows subclasses to set a different default transfer function, and
        //to reject unsuitable choices

        virtual void transfer_function(std::string functionName); //! replace transfer function using string identifier
        void set_stats_histogram(histogram_1D<ELEMTYPE > * h);

        // *** Image data pointer ***
        ELEMTYPE *dataptr;			//no forced access via function for speedup...
        ELEMTYPE *imagepointer();
		void set_imagepointer(ELEMTYPE *new_value);
        void deallocate();

		unsigned long num_elements;        //image size in # pixels/voxels


    public:
        virtual ~image_storage();

        virtual float get_max_float() const;
		virtual float get_min_float() const;
        ELEMTYPE get_max() const;
        ELEMTYPE get_min() const;
		float get_mean(image_storage<IMGBINARYTYPE>* mask=NULL);
		float get_standard_deviation(image_storage<IMGBINARYTYPE>* mask=NULL);
        ELEMTYPE get_num_values();
//		unsigned long get_num_elements();
        histogram_1D<ELEMTYPE>* get_histogram();
		histogram_1D<ELEMTYPE>* get_histogram_new_with_same_num_buckets_as_intensities();

		virtual void data_has_changed(bool stats_refresh = true) = 0;   
        void stats_refresh(bool min_max_refresh = false);
        void min_max_refresh();     //! lighter function that _only_ recalculates max/min values,
                                    //! for use inside processing functions
        void get_min_max_values(ELEMTYPE &minimum, ELEMTYPE &maximum);
		double get_sum_of_voxels(ELEMTYPE lower_limit = std::numeric_limits<ELEMTYPE>::min(), bool calc_scalar_abs_value=false, image_storage<IMGBINARYTYPE>* mask=NULL);
		void set_sum_of_voxels_to_value(double value);

        void erase();
		void fill(ELEMTYPE value);
		void invert();
		void add_value_to_all_voxels(ELEMTYPE value, image_storage<IMGBINARYTYPE>* mask=NULL);
        void scale(ELEMTYPE new_min=0, ELEMTYPE new_max=255);   
		void scale_by_factor(float factor, ELEMTYPE old_center=0, ELEMTYPE new_center=0, image_storage<IMGBINARYTYPE>* mask=NULL);
		void map_values(ELEMTYPE map_from=1, ELEMTYPE map_to=255, ELEMTYPE result_value=255);
		void map_negative_values(ELEMTYPE to_value=0);
		int get_number_of_voxels_with_value(ELEMTYPE val);
		int get_number_of_voxels_with_value_greater_than(ELEMTYPE val);

		
		bool same_size(image_storage<ELEMTYPE> *const image2); //checks the data size only... (not the dimension)
		void combine(image_storage<ELEMTYPE> *const image2, COMBINE_MODE mode);

		void print_stats();

		virtual string resolve_tooltip();		//combines tooltip data of this class with data from other classes
		string resolve_tooltip_image_storage(); //resolves tooltip data typical for this class
		string resolve_datasize_in_kb();


        // *** iterator ***        
        class iterator : public std::iterator<std::forward_iterator_tag, ELEMTYPE>
            {
            private:
                ELEMTYPE* ptr;
			public:
				iterator(ELEMTYPE* i);               
				~iterator();// {}
				iterator& operator=(const iterator& other);                
				bool operator==(const iterator& other);                
				bool operator!=(const iterator& other);                
				iterator& operator++();                
				iterator& operator++(int);                
				ELEMTYPE& operator*();                
				ELEMTYPE* operator->();
                ELEMTYPE* pointer();
				iterator& operator--();             
				iterator operator+(unsigned long n);
            };

        iterator begin();        
        iterator end();   
    };

#endif