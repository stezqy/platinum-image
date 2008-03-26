//////////////////////////////////////////////////////////////////////////
//
//  ptconfig $Revision:$
//
//  Platinum configuration file handling: "last_path", "view_port_setup", "load_this_image_on_startup"
//
//  $LastChangedBy: joel.kullberg $
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

#ifndef __ptconfig__
#define __ptconfig__

#include <string>
//#include <stdio.h>
#include <fstream>
//#include "configfile.h"
#include "Utilities/configfile/configfile.h"
#include "fileutils.h"
#include "error.h"

#define DEFAULT_CONFIG_FILE "pt_settings.inp"

using namespace std;

class pt_config{
    private:
//		ConfigFile cf;

	public:
        static void initialize(string filename=DEFAULT_CONFIG_FILE);
		template<class T> 
			static T read(string key, string filename=DEFAULT_CONFIG_FILE);  // call as read<T>
		template<class T> 
			static void write(string key, T value, string filename=DEFAULT_CONFIG_FILE);  // call as read<T>
/*
    private:
        std::string _M_msg;
        errorLevel level;

    public:
        static bool error_if_true (bool condition, std::string message, pt_error::errorLevel l = serious);
        static bool error_if_false (bool condition, std::string message, pt_error::errorLevel l = serious)
            {return error_if_true (!condition, message, l);}
        template <class PTR>
        static PTR * error_if_null (PTR * p, std::string message, pt_error::errorLevel l = fatal)
            {
            error_if_true (p == NULL, message, l);
            return p;
            }
        static void error ( std::string message, pt_error::errorLevel l = serious);

        static const std::string level_name (pt_error::errorLevel l);

        explicit //avoid throwing this directly, rather use error (...) above
            pt_error(const std::string& __arg,errorLevel l = serious);

        virtual 
            ~pt_error() throw() { }

        virtual const char* 
            what() const throw();
*/
    };

template<class T> 
T pt_config::read(string key, string filename){
	ConfigFile cf = ConfigFile(filename,"=","//");	
	T res;
	try{
		res = cf.read<T>(key);
	}catch(ConfigFile::key_not_found k){
		pt_error::error("Exception - pt_config::read(key=\""+key+"\" filename=\""+filename+"\")",pt_error::debug);
		return NULL;
	}
	return res;
}

template<class T> 
void pt_config::write(string key, T value, string filename){
	ConfigFile cf = ConfigFile(filename,"=","//");	
	cf.update_value_in_file(filename,key,value);
}


#endif __ptconfig__