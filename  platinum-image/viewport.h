//////////////////////////////////////////////////////////////////////////
//
//  Viewport
//
//  Abstraction of widget containing controls and image pane
//  (implemented in FLTKviewport) for each viewpoint of data
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

#ifndef __viewport__
#define __viewport__

#include "rendermanager.h"
#include "FLTKviewport.h"
#include "datamanager.h"

#include "global.h"

class threshold_overlay;
class thresholdparvalue;

#define NO_RENDERER_ID 0

class viewport // friend with renderer_base
{
private:
    friend class FLTKviewport;

    // *** custom data ***

    int ID;  //viewport ID
    static int maxviewportID;
    
    static bool renderermenu_built;

	int rendererID; // this guy will render for us (each renderer instance contains an unique ID)
	int rendererIndex; // direct look up to vector array - CACHED INFO - MIGHT CHANGE!

	void draw_cursor(bool filledcenter);    // it's better that viewport draws the cursor
                                            //than cursor itself (drawing is ugly, FLTK-dependent; keep #renderingclasses down
	void  draw_mousemode(int currentmode);
	void  draw_coordinates(int x, int y);
	void  draw_scolling_borders();

	void  reslice();                         // will call renderer_base::reslice(...) and
                                             //supply our bitmap pointer to the 2D- or 3D-renderer which
                                             //in turn will do the actual reslicing

     // *** FLTK-related data ***

    uchar *rgbpixmap;

    int rgbpixmapwidth;
    int rgbpixmapheight;

    void clear_rgbpixmap();             //fill for viewport without renderer

    FLTKviewport *viewport_widget;      //the frame ("viewport") displaying a rendered image
    Fl_Menu_Button * volumemenu_button;   
    Fl_Menu_Button * directionmenu_button;
    Fl_Menu_Button * renderermenu_button;
    Fl_Menu_Button * blendmenu_button;
    Fl_Pack *viewport_buttons;          //group containing per-viewport widgets such as the volume menu

	// functions called by the main callback
	// will often call somehing in images (or, maybe it should be layer's, or cursor's) front end
	// void  move_cursor_relative(position3D offset);
	// void  set_cursor(position3D offset);


    void update_volume_menu();   //set rendering status for volumes
                                  //from rendercombination for this viewport's renderer
    void rebuild_renderer_menu ();//update checkmark for current renderer type
    void rebuild_blendmode_menu ();//update checkmark for current blend mode
public:
	viewport();
    virtual ~viewport() {}

	void viewport_callback(Fl_Widget *callingwidget);                               //callback that handles events
                                                                                    //always redraws
    static void viewport_callback(Fl_Widget *callingwidget, void *thisviewport);    //FLTK callback wrapper

    static void toggle_volume_callback(Fl_Widget *callingwidget, void * params );
    static void set_direction_callback(Fl_Widget *callingwidget, void * params );
    static void set_blendmode_callback(Fl_Widget *callingwidget, void * params );
        
    // *** refresh methods ***
    //called when any update of the visual parts of viewport is affected, i.e.
    //image and/or menu of volumes

    void refresh_from_geometry (int g);     //refresh if it uses the geometry specified by argument
    void refresh_from_combination (int c);  //refresh if it uses the geometry specified by argument
    void refresh ();                        //re-builds menu and makes viewport re-render and redraw eventually
    
    threshold_overlay * get_threshold_overlay (thresholdparvalue *);

    void update_viewsize(int width, int height);

    void update_fbstring (FLTKviewport* f);   //refresh values in number-at-pointer string

    // *** operators ***
	// virtual const viewport &operator=(const viewport &k) { return k; }
	bool virtual operator<<(const viewport &k) { return ID==k.ID; }
	bool virtual operator==(const viewport &k) { return ID==k.ID; }
	bool virtual operator==(const int &k) { return ID==k; }
	bool virtual operator!=(const viewport &k) { return ID!=k.ID; }
	bool virtual operator<(const viewport &k) { return ID<k.ID; }
	bool virtual operator>(const viewport &k) { return ID>k.ID; }
	friend std::istream &operator>>(std::istream &in, viewport &k) { in >> k.ID; return in; }
	friend std::ostream &operator<<(std::ostream &ut, const viewport &k) { ut << "[viewport. ID= " << k.ID << " rendererID: " << k.rendererID << " rendererIndex:  " << k.rendererIndex << "] "; return ut; }

	void initialize_viewport(int xpos, int ypos, int width, int height); 

	int get_id();
	int get_renderer_id();

	void connect_renderer(int rID);

	void  set_timer_delay(int delay = 0); // if zero, remove timer and always render directly
};

#endif