#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    Utility.tcl
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
# Set up a progress bar window


proc WSTErrorMacro {m} {
    tk_messageBox -message $m -icon "error"
}

proc ConstructProgressWindow {f} {
    global Globals Options

    toplevel .$f

    label .$f.label -font $Options(h1_font) -text "Progress"
    canvas .$f.canvas
    .$f.canvas create rectangle 0 0 1 \
        $Options(progress_bar_height) -fill $Options(progress_bar_color) \
        -tags "bar"

    grid configure .$f.label  -row 0
    grid configure .$f.canvas -row 0
    
    set geomStr $Options(progress_bar_length)
    append geomStr "x"
    append geomStr $Options(progress_bar_height)
    
    wm geometry .$f $geomStr
    wm resizable .$f 0 0
    wm title .$f "Please wait"
    wm protocol .$f WM_DELETE_WINDOW { }
    update
}


proc ProgressProc {filter progress_bar} {
    global Options

    set v [ expr [$filter GetProgress] * $Options(progress_bar_length) ]

    $progress_bar.canvas delete bar
    $progress_bar.canvas create rectangle 0 0 $v \
        $Options(progress_bar_height) -fill $Options(progress_bar_color) \
    -tags "bar"
    
    update
}


proc SpawnProcess { procname recovery_procname} {

    catch $procname result
    if {$result != ""} {
        WSTErrorMacro $result
        $recovery_procname
    }
}
