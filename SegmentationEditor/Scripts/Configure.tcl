#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    Configure.tcl
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

###
#
# This script is only run if the file WSM.conf is not found
#

## Create a data entry window
proc GenerateConfigurationFile { } {
    global Options
    toplevel .configureWindow
    wm title .configureWindow "$Options(program_name) Configuration"
    
    label .configureWindow.title -text "Configuration" -font $Options(h1_font) \
        -background $Options(pleasant_background_color)

    label .configureWindow.instructions -font $Options(h3_font) \
        -text "IMPORTANT: \nThis script generates a small configuration file $Options(configuration_file_name). Once it has been created, you can edit the values in $Options(configuration_file_name) directly. Deleting the file will cause this dialog to be appear again.  Also see the file \"Options.tcl\" for more configurable options." \
        -wraplength 350 -anchor nw -justify left -background $Options(warning_color)
    
    label .configureWindow.vtkpathLabel -text "Path to VTK source: "
    label .configureWindow.wstpathLabel -text "Path to \"$Options(program_name)\" source: "
    entry .configureWindow.vtkpath 
    entry .configureWindow.wstpath 

    button .configureWindow.save -text "save these settings" -command WriteConfigurationFile
    button .configureWindow.cancel -text "cancel" -command exit
        
    grid configure .configureWindow.title -row 0 -column 0 -columnspan 2 \
        -pady 10
    grid configure .configureWindow.instructions -row 1 -column 0 -columnspan 2 \
        -padx 3 -pady 8

    grid configure .configureWindow.vtkpathLabel -row 2 -column 0 -pady 3 -padx 3 \
        -sticky e
    grid configure .configureWindow.wstpathLabel -row 3 -column 0 -pady 3 -padx 3 \
        -sticky e
    grid configure .configureWindow.vtkpath -row 2 -column 1 -pady 3 -padx 3 \
        -sticky w
    grid configure .configureWindow.wstpath -row 3 -column 1 -pady 3 -padx 3 \
        -sticky w
    grid configure .configureWindow.save    -row 4 -column 0 -pady 3 -padx 3
    grid configure .configureWindow.cancel    -row 4 -column 1 -pady 3 -padx 3

    wm protocol .configureWindow WM_DELETE_WINDOW { exit }

    tkwait window .configureWindow
}


proc WriteConfigurationFile { } {
    global Options

    set str {set Options(vtk_path) }
    append str [.configureWindow.vtkpath get]
    set testfile [.configureWindow.vtkpath get]
    append testfile "/Wrapping/Tcl/vtkinteraction"

    if { [file exists $testfile] != 1 } {
        tk_messageBox -message "Bad VTK path or incompatible VTK version.  This application may only support VTK 4.0 or later.  Could not find the directory $testfile" \
            -icon "error" 
        return
    }
    lappend settings_list $str

    set str {set Options(wst_path) }
    append str [.configureWindow.wstpath get] 
    set testfile [.configureWindow.wstpath get]
    append testfile "/Scripts/Main.tcl"

    if { [file exists $testfile] != 1 } {
        tk_messageBox -message "Bad \"$Options(program_name)\" path. Could not find $testfile" \
            -icon "error" 
        return
    }

    lappend settings_list $str

    ## All OK so write the file
    set f [open $Options(configuration_file_name) w]
    foreach line $settings_list { puts $f $line }
    close $f

    destroy .configureWindow
}
