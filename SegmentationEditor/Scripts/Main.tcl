#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    Main.tcl
#  Language:  C++
#  Date:      $Date$
#  Version:   $Revision$
#
#  Copyright (c) 2002 Insight Consortium. All rights reserved.
#  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#     This software is distributed WITHOUT ANY WARRANTY; without even 
#     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
#     PURPOSE.  See the above copyright notices for more information.
#
#=========================================================================*/

wm withdraw .

# Load all the "header files" and modules
source "./Fonts.tcl"
source "./Graphics.tcl"
source "./Options.tcl"

set path_to_vtk $Options(vtk_path)
append path_to_vtk "/Wrapping/Tcl"
set path_to_wst $Options(wst_path)
append path_to_wst "/Scripts"

lappend auto_path $path_to_vtk
lappend auto_path $path_to_wst

source "./Globals.tcl"
source "./Utility.tcl"
foreach m $Globals(module_names) {
    append m "Module.tcl"
    source "./$m"
}

###
# This file contains all of the functionality for
# the main console window, including the menu bar
# and switching between modules.  Individual modules
# are defined in either Initialize.tcl and/or own files.
# 
# To add a module, create an initialization function
# in Initialize.tcl and add the name of the module
# to the Globals(module_names) list.
#

###
# proc ConstructMainMenu
#
# Sets up the main menu for the main console and enables switching
# among the various frames.
#
proc ConstructMainMenu {f} {
    global Options Globals
    
    menu $f.menubar -relief "raised" -font $Options(menu_font) \
        -background $Options(menu_bar_color) \
        -foreground $Options(menu_text_color) \
        -activebackground $Options(menu_active_background_color)
    . config -menu $f.menubar

    menu $f.menubar.module -tearoff false -background $Options(menu_bar_color) \
        -foreground $Options(menu_text_color) -font $Options(menu_font)
    menu $f.menubar.options -tearoff false -background $Options(menu_bar_color) \
        -foreground $Options(menu_text_color) -font $Options(menu_font)

    $f.menubar add cascade -label "Module"  -menu $f.menubar.module
    $f.menubar add cascade -label "Options" -menu $f.menubar.options
    
    foreach m $Globals(functional_module_names) {
        $f.menubar.module add command \
            -label $m \
            -activebackground $Options(menu_active_background_color)\
            -command "SwitchWindow $m" 
    }

    foreach m $Globals(options_module_names) {
        $f.menubar.options add command \
            -label $m \
            -activebackground $Options(menu_active_background_color)\
            -command "SwitchWindow $m"
    }
}

####
# proc SwitchWindows 
#
# Changes the window displayed in the main console
#
proc SwitchWindow { name } {
    global Globals

    foreach m $Globals(module_names) {
        set suffix [string tolower $m]
        append suffix "Frame"           

        set activateString "Activate"
        append activateString $m
        append activateString "Module"

        set deactivateString "Deactivate"
        append deactivateString $m
        append deactivateString "Module"

        if { $name == $m } {
            pack .$suffix -side left -fill both -expand true
           $activateString
        } else {
            pack forget .$suffix
            $deactivateString
        }
    }
}


###
#
# main() procedure
#

# Set up the console window
wm title    . $Options(console_title)
wm minsize  . $Options(minimum_console_window_width) $Options(minimum_console_window_height)
wm resizable . $Options(console_is_resizable)  $Options(console_is_resizable) 

# Catch window manager kills
wm protocol . WM_DELETE_WINDOW { SwitchWindow "Exit" }

frame .menuFrame
ConstructMainMenu .menuFrame
pack .menuFrame  -side top

foreach m $Globals(module_names) {
    set suffix [string tolower $m]
    append suffix "Frame"
    frame .$suffix
    set function_name "Construct"
    append function_name $m
    append function_name "Frame"
    eval [subst $function_name] .$suffix
}

# Raise the default window (defined in Globals)
wm deiconify .
SwitchWindow $Globals(default_window);
