#platinumlib CMakelists rev. 1.2.1

PROJECT(Platinum)
CMAKE_MINIMUM_REQUIRED(VERSION 2.0)

# This file is part of the Platinum library.
# Copyright (c) 2007 Uppsala University.
#
#
#    The Platinum library is free software; you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    The Platinum library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with the Platinum library; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

INCLUDE_REGULAR_EXPRESSION(".*[.](cc|h|cxx|cp)$")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wunused")

FIND_PACKAGE( ITK )
IF ( ITK_FOUND )
  INCLUDE( ${USE_ITK_FILE} )
ENDIF( ITK_FOUND )

INCLUDE_DIRECTORIES( ${ITK_INCLUDE_DIR} )

FIND_PACKAGE ( FLTK )
IF (   USE_FLTK_FILE   )
  INCLUDE(  ${USE_FLTK_FILE}  )
ENDIF(   USE_FLTK_FILE   )

INCLUDE_DIRECTORIES( ${FLTK_INCLUDE_DIR}  )

LINK_DIRECTORIES(${ITK_LIBRARY_DIR} ${FLTK_LIBRARY_DIR})

IF(WIN32)
    SET(WIN32_DIRENT_SRC dirent.c)
    SET(WIN32_DIRENT_HEAD dirent.h)
ENDIF(WIN32)

ADD_LIBRARY(platinumlib STATIC color.h colormap.h cursor3D.h data_base.h ${WIN32_DIRENT_HEAD} datamanager.h datawidget.h fileutils.h FLTK2Dregionofinterest.h FLTKuserIOparameters.h FLTKviewport.h global.h histogram.h image_base.h image_binary.h image_complex.h image_general.h image_integer.h image_label.h image_multi.h image_scalar.h platinumprogram.h point_base.h point3D.h rawimporter.h renderer_base.h renderer_VTK.h rendererMPR.h rendergeometry.h rendercombination.h rendermanager.h ptmath.h threshold.h userIO.h userIOmanager.h viewport.h viewmanager.h color.cc colormap.cc cursor3D.cc data_base.cc datamanager.cc datawidget.cc ${WIN32_DIRENT_SRC} fileutils.cc FLTK2Dregionofinterest.cc FLTKuserIOparameters.cc FLTKviewport.cc histogram.cc image_base.cc image_binary.hxx image_complex.hxx image_general.hxx image_generalfile.hxx image_generaliterator.hxx image_integer.hxx image_label.hxx image_multi.hxx image_scalar.hxx point_base.cc rawimporter.cc renderer_base.cc rendererMPR.cc renderer_VTK.cc  rendergeometry.cc rendercombination.cc rendermanager.cc threshold.cc userIO.cc userIOmanager.cc viewport.cc viewmanager.cc ${WIN32_DIRENT_SRC} CMakeLists.txt)
LINK_DIRECTORIES (${Platinum_BINARY_DIR})

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# _DEBUG flag will be defined when in debug mode
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

IF(WIN32)
  OPTION(FLTK_HAS_DEPENDENT_NAMES "FLtk debug lib is named fltkd" OFF)
  MARK_AS_ADVANCED(FLTK_HAS_DEPENDENT_NAMES)
  TARGET_LINK_LIBRARIES (platinumlib wsock32 comctl32)
  IF (FLTK_HAS_DEPENDENT_NAMES)
    TARGET_LINK_LIBRARIES (platinumlib debug fltkgld optimized fltkgl debug fltkd optimized fltk)
  ELSE(FLTK_HAS_DEPENDENT_NAMES)
    TARGET_LINK_LIBRARIES (platinumlib fltk_gl fltk )
  ENDIF(FLTK_HAS_DEPENDENT_NAMES)
ENDIF(WIN32)

IF(UNIX)
  LINK_DIRECTORIES(/usr/local/lib )
  IF(APPLE)
  ELSE(APPLE)
    LINK_DIRECTORIES (/usr/X11R6/lib)
  ENDIF (APPLE)
  TARGET_LINK_LIBRARIES (platinumlib fltk_gl fltk )
  #TARGET_LINK_LIBRARIES(platinumlib GLU GL m  Xext X11  supc++)
ENDIF(UNIX)

TARGET_LINK_LIBRARIES(platinumlib ITKCommon ITKIO ${FLTK_LIBRARIES})

#test program for quick compile cycles
#MACOSX_BUNDLE only affects OS X builds, and is required for GUI to function
ADD_EXECUTABLE(PlatinumTest MACOSX_BUNDLE test_app.cc)

IF(APPLE)
   FIND_PROGRAM(ITK_APPLE_RESOURCE Rez /Developer/Tools)
   IF(ITK_APPLE_RESOURCE)
      ADD_CUSTOM_COMMAND(
         TARGET PlatinumTest.app/Contents/Resources/PlatinumTest.rsrc POST_BUILD
         COMMAND ${ITK_APPLE_RESOURCE} ARGS -useDF ${FLTK_INCLUDE_DIR}/mac.r
         WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
         )
   ENDIF(ITK_APPLE_RESOURCE)
ENDIF(APPLE)

TARGET_LINK_LIBRARIES(PlatinumTest platinumlib)

ADD_DEPENDENCIES(PlatinumTest platinumlib)

#add header files to project
SOURCE_GROUP(Headers REGULAR_EXPRESSION ".*[.]h$")

SOURCE_GROUP(ImageVarieties FILES image_binary.hxx image_complex.hxx image_integer.hxx image_label.hxx image_multi.hxx image_scalar.hxx)

SOURCE_GROUP(Data FILES datamanager.cc data_base.cc image_base.cc image_general.hxx image_generalfile.hxx image_generaliterator.hxx point_base.cc rawimporter.cc)

SOURCE_GROUP(View FILES viewport.cc FLTKviewport.cc viewmanager.cc renderer_base.cc renderer_VTK.cc rendererMPR.cc rendercombination.cc rendergeometry.cc rendermanager.cc)

SOURCE_GROUP(Interaction FILES datawidget.cc userIO.cc FLTKuserIOparameters.cc userIOmanager.cc test_app.cc)

SOURCE_GROUP(Helpers FILES color.cc colormap.cc cursor3D.cc ${WIN32_DIRENT_SRC} fileutils.cc FLTK2Dregionofinterest.cc histogram.cc threshold.cc)

#add CMakelists for inclusion in version control
SOURCE_GROUP(Scripts CMakeLists.txt)

#Write PlatinumConfig.cmake so client programs can be built with Platinum

#CMAKE_EXPORT_BUILD_SETTINGS(${Platinum_BINARY_DIR}/PlatinumBuildSettings.cmake)
SET(Platinum_LIBRARY_DEPENDS_FILE ${Platinum_BINARY_DIR}/PlatinumLibraryDepends.cmake)
EXPORT_LIBRARY_DEPENDENCIES(${Platinum_BINARY_DIR}/PlatinumLibraryDepends.cmake)
CONFIGURE_FILE("${Platinum_SOURCE_DIR}/CMake/PlatinumConfig.cmake.in"
  "${Platinum_BINARY_DIR}/PlatinumConfig.cmake" @ONLY IMMEDIATE)