//////////////////////////////////////////////////////////////////////////
//
//  Pt_event $Revision$
//
/// The pt_event object (with FLTK-implementing subclass) is the interface
/// that allows events to be handled with the future option of migrating from FLTK.
///
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

#ifndef __pt_event__
#define __pt_event__

#include <string>
#include <vector>
#include <FL/Fl_Widget.H>

class FLTKviewport;

class pt_event //! this class should not contain anything FLTK
{
public:
    typedef enum  { no_type,
        hover,
        adjust,  //typically LMB
        create,  //typically RMB
        browse,  //typically MMB
        scroll,  //may be generated by mouse wheel, not necessarily for scrolling     
        key,    
        dragNdrop,
        draw,
        resize,
		focus,	// focus all viewports where the mouse is (typically double LMB) //AF
		rotate
    }pt_event_type;
    typedef enum  { no_state,
        idle,                      // for pressing modifier keys
        begin,                     // mouse/key down
        iterate,                   // drag/key repeat
        end
    } pt_event_state;
    
    typedef enum {            
        //NOTE: some keys are intentionally left out! 
        
        //this is an attempt to increase the usability of Platinum,
        //notably those not accessible for laptop users,
        //or which vary by platform. Think twice before adding "missing"
        //definitions!
        
        space_key = 32, //ASCII value
        backspace_key = FL_BackSpace, //- The backspace key.
        tab_key = FL_Tab, //- The tab key.
        return_key = FL_Enter, //The enter key.
        escape_key = FL_Escape, //- The escape key.
        home_key = FL_Home, //The home key.        
        left_key =  FL_Left, //The left arrow key.
        up_key =  FL_Up, //The up arrow key.
        right_key =  FL_Right, //The right arrow key.
        down_key =  FL_Down, //The down arrow key.
        pageup_key =  FL_Page_Up, //The page-up key.
        pagedown_key =  FL_Page_Down, //The page-down key.
        end_key =  FL_End, //The end key.
        enter_key = FL_KP_Enter, //- The enter key on the keypad.
        //FL_F - One of the function keys; use FL_F + n for function key n.
        //FL_KP - One of the keypad numbers; use FL_KP + n for number n.
        f1_key = FL_F + 1,
        f2_key = FL_F + 2,
        f3_key = FL_F + 3,
        f4_key = FL_F + 4,
        f5_key = FL_F + 5,
        f6_key = FL_F + 6,
        f7_key = FL_F + 7,
        f8_key = FL_F + 8,
        f9_key = FL_F + 9,
        capslock_key = FL_Caps_Lock, //- The caps lock key.
        delete_key = FL_Delete, //- The delete key.
        //FL_Shift_L - The lefthand shift key, FL_Shift_R - The righthand shift key.
        shift_key = 0x1000,
        //FL_Alt_L - The left alt key, FL_Alt_R - The right alt key.
        alt_key = 0x10000,
        //FL_Control_L - The lefthand control key, FL_Control_R - The righthand control key.
        ctrl_key = 0x100000,
        //FL_Meta_L - The left meta/command/Windows key,FL_Meta_R - The right meta/command/Windows key.
        meta_key = 0x1000000 //this may be swapped with ctrl on Macs
    } pt_event_keypress;
    
protected:
        bool handled_;
    int mousePos[2];
    int mouseStart[2];
    int resizeDim[2];
    int wheelDelta;
    int value;                    //key w/o modifiers
    int modifier;
    pt_event_type type_;
    pt_event_state state_;
    pt_event();
public:
    virtual void grab ();
    virtual void ungrab (); //use wisely (i.e. don't :)
                            // *** access functions ***
    bool handled();
    
    void set_resize (int w, int h);
    
    const int scroll_delta();
    const int * drag_start();
    const int * get_resize();
    const pt_event_type type();
    const pt_event_state state();
    bool has_modifier(const int); //!true if the argument mod keys (shift, alt etc.) were pressed - any other mods ignored
    bool key_combo (const int);
    const int * mouse_pos_global();
};

class FLTK_event : public pt_event
{
private:
    static std::string eventnames[];
    void set_type (); //helper to set type variable
    void attach (Fl_Widget *);
protected:
        Fl_Widget * myWidget;
    FLTK_event (int FL_event, FLTKviewport * fvp); //! constructor translates the content of the FLTK event into a neutral format that is stored in the base class
    FLTK_event (FLTKviewport * fvp);
    
public:
        std::vector<int> mouse_pos_local();
};

class viewport_event : public FLTK_event
{
public:
    viewport_event (pt_event_type, FLTKviewport * fvp); //constructor for events not from FLTK event no. (e.g. draw)
    viewport_event (int FL_event, FLTKviewport * fvp);
    viewport_event() : FLTK_event (NULL) {}
    
    FLTKviewport * get_FLTK_viewport();
    void resize_point (int &x,int &y);
};

#endif //__pt_event__

