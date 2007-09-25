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

#define TFUNCTIONTEST transfer_default

#ifndef __image_storage_hxx__
#define __image_storage_hxx__

#include "image_storage.h"

#include "transfer.hxx"
#include "histogram.hxx"
#include "transfer_interpolated.hxx"
#include "image_storage_iterator.hxx"

template <class fromType, class toType>
void copy_data(image_storage<fromType > * const in,image_storage<toType > * out) //!General data copying
    {
	//MSVC 2005 no like, See following discussion...
	//http://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=259&rl=1
    //std::copy (in->begin(),in->end(),out->begin());

    typename image_storage<fromType >::iterator i = in->begin();
    typename image_storage<toType >::iterator   o = out->begin();

    while (i != in->end() && o != out->end())
    {
    *o = *i;

    ++i; ++o;
    }

    pt_error::error_if_false(i == in->end() && o == out->end(),"Image sizes didn't match when copying data",pt_error::serious);
    }

//template <class fromType>
//void copy_data( image_storage<fromType > * in,image_storage<IMGBINARYTYPE > * out) //!Data copy specialized for copying *to* boolean
//    {
//    typename image_storage<fromType >::iterator i = in->begin();
//    typename image_storage<IMGBINARYTYPE >::iterator   o = out->begin();
//
//    while (i != in->end() && o != out->end())
//        {
//        *o = (*i == true);
//
//        ++i; ++o;
//        }
//
//    /*if (!(i == in->end() && o == out->end()) )
//    {
//    throw ("Image sizes didn't match when copying boolean data");
//    }*/
//    }

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::set_parameters()
    {
    dataptr = NULL;
    stats = NULL;
    tfunction = NULL;

    set_stats_histogram (new histogram_1D<ELEMTYPE >(this));  //hist1D constructor calls resze() and calculate()
	transfer_function();  //set default transfer function
	
    stats->min(std::numeric_limits<ELEMTYPE>::min());
    stats->max(std::numeric_limits<ELEMTYPE>::max());
//	this->min_max_refresh();	//JK2 - The image data is not available yet!
    }

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::transfer_function(std::string functionName)
    {
    transfer_function(transfer_manufactured::factory.Create<ELEMTYPE> (functionName,this));
    }

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::set_stats_histogram(histogram_1D<ELEMTYPE > * h)
    {
    if (stats != NULL)
        {
        delete stats;
        }

    stats = h;
    }

template <class ELEMTYPE >
image_storage<ELEMTYPE >::image_storage() : image_base ()
    {
    set_parameters();
    }

template <class ELEMTYPE >
template<class SOURCETYPE>
image_storage<ELEMTYPE >::image_storage(image_storage<SOURCETYPE> * const s):image_base (s)
    {
    set_parameters();
    }

template <class ELEMTYPE >
image_storage<ELEMTYPE >::~image_storage()
    {
    delete tfunction;

    if (stats != NULL)
        { delete stats; }

    if (imagepointer() != NULL)
        { deallocate(); }

    //minvalue=std::numeric_limits<ELEMTYPE>::min();
    //maxvalue=std::numeric_limits<ELEMTYPE>::max();
    }

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::transfer_function(transfer_base<ELEMTYPE> * t)
    {
    if (t == NULL) //default
        { 
        if (tfunction == NULL)
            {tfunction = new TFUNCTIONTEST<ELEMTYPE >(this); }
        //else, do nothing since a transfer function was assigned elsewhere
        }
    else
        { 
        if (tfunction != NULL)
            {delete tfunction;}

        tfunction = t;
        }
    }

/*
template <> //JK2 image_complex testing
float image_storage<std::complex<float> >::get_min_float()
    {
    return abs(minvalue);
    }
*/
/*
template <class ELEMTYPE >
float image_storage<ELEMTYPE >::get_min_float() const
    {
    return minvalue;
    }
*/

template <class ELEMTYPE >
ELEMTYPE image_storage<ELEMTYPE >::get_min() const
    {
    return stats->min();
    }
/*
template <> // image_complex testing
float image_storage<std::complex<float> >::get_max_float()
    {
    return abs(maxvalue);
    }
*/
template <class ELEMTYPE >
float image_storage<ELEMTYPE >::get_max_float() const
    {
		return abs(float(stats->max()));
    }


template <class ELEMTYPE >
ELEMTYPE image_storage<ELEMTYPE >::get_max() const
    {
    return stats->max();
    }


template <class ELEMTYPE >
void image_storage<ELEMTYPE >::erase()
    {
    memset (imagepointer(), 0, sizeof(ELEMTYPE) * num_elements);
    }

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::fill(ELEMTYPE value)
	{
	typename image_storage<ELEMTYPE>::iterator i = this->begin();
	while (i != this->end())
		{
		*i = value;
		++i;
		}
	}


template <class ELEMTYPE >
void image_storage<ELEMTYPE >::scale(ELEMTYPE new_min, ELEMTYPE new_max)
	{
	this->min_max_refresh();

	if(get_min()==get_max())
		{
		fill(0);
		}
	else
		{
		typename image_storage<ELEMTYPE>::iterator i = this->begin();
		while (i != this->end())
			{
			*i = new_min + (ELEMTYPE) (((*i)-get_min()) * ((new_max-new_min)/(double)(get_max()-get_min())));
			++i;
			}
		}
	}

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::scale(float factor, ELEMTYPE old_center, ELEMTYPE new_center)
	{
	typename image_storage<ELEMTYPE>::iterator i = this->begin();
	while (i != this->end())
		{
		*i = new_center + ELEMTYPE(float((*i)-old_center)*factor);
		++i;
		}
	}


template <class ELEMTYPE >
float image_storage<ELEMTYPE >::get_number_of_voxels_with_value(ELEMTYPE val)
	{
		iterator i = this->begin();
		float nr=0;
		while(i != this->end())
		{
			if(*i == val){nr++;}
			i++;
		}
		return nr;
	}


template <class ELEMTYPE >
void image_storage<ELEMTYPE >::stats_refresh(bool min_max_refresh)
    {
	if(min_max_refresh){
		this->min_max_refresh();
	}
    stats->calculate(); 

    //TODO:
    //store #distinct values locally 
   
    //don't change if values don't make sense - 
    //that would be an empty/zero image
/*
    if (stats->min() < stats->max())
        {
        this->maxvalue=stats->max();
        this->minvalue=stats->min();
        }
*/

    }

template <class ELEMTYPE >
void image_storage<ELEMTYPE >::min_max_refresh()
    {
    ELEMTYPE val;

    ELEMTYPE pre_max=std::numeric_limits<ELEMTYPE>::min();
    ELEMTYPE pre_min=std::numeric_limits<ELEMTYPE>::max();
    
    typename image_storage<ELEMTYPE>::iterator itr = this->begin();
    while (itr != this->end())
        {
        val=*itr;
        
        pre_max = max (val, pre_max);
        pre_min = min (val, pre_min);
        
        ++itr;
        }
   
    //don't change if values don't make sense - 
    //that would be an empty/zero image
    if (pre_min < pre_max)
        {
        this->stats->max(pre_max);
        this->stats->min(pre_min);

//        this->maxvalue=pre_max;
  //      this->minvalue=pre_min;
        }
    }
/*
template <class ELEMTYPE >
void image_storage<ELEMTYPE >::save_histogram_to_txt_file(std::string filepath, std::string separator)
	{
		cout<<"save_histogram_to_txt_file..."<<endl;
		cout<<this->stats<<endl;
//		this->min_max_refresh();
//		this->stats->calculate();
//		this->stats->print_histogram_content();
		pt_error::error_if_null(this->stats,"image_storage<ELEMTYPE >::save_histogram_to_txt_file - stats==NULL",pt_error::debug);
		this->stats->save_histogram_to_txt_file(filepath,separator);
	}
*/

#endif
