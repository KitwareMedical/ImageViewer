#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    ExitModule.tcl
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
#  This file defines routines and GUI for the Exit module.


proc DeactivateExitModule {} {
}

proc ActivateExitModule {} {
}


proc ConstructExitFrame {f} {
    global Options 

    $f configure -background $Options(pleasant_background_color)

    label $f.message \
        -text $Options(farewell_message) \
        -font $Options(fancy_font) \
        -background $Options(pleasant_background_color)

    # Splash all the logos listed in $Options(credits_logos) on the screen
    canvas $f.logos -borderwidth 2 \
        -background $Options(pleasant_background_color) \
        -relief flat

    set x 5
    set y 0
    foreach l $Options(credits_logos) {
        $f.logos create image $x 10 -image $Options($l) -anchor nw
        set x [expr $x + [image width $Options($l)] + 5 ]
        if { [image height $Options($l)] > $y } { 
            set y [image height $Options($l)]
        }
    }

    $f.logos configure -width $x -height [expr $y + 20]

    # 
    $f.logos create image [expr [$f.logos cget -width] / 2] \
        [expr [$f.logos cget -height] /2] \
        -image $Options(geek_logo) -tags freak
    $f.logos lower freak
    if { $Options(animate_logo) == "true" } {
        bind $f.logos <Motion> { AnimateLogo %W %x %y "freak"}
    }

    button $f.exit_button -text "Quit" \
        -command ExitDialogue \
        -borderwidth 4 

    label $f.credits \
        -text "$Options(credits_author) . $Options(credits_date)" \
        -font $Options(normal_font) \
        -background $Options(pleasant_background_color)

    pack $f.message $f.logos $f.credits -side top -pady 10 -padx 10
    pack $f.exit_button -side right -pady 10 -padx 10
}


###
# proc ExitDialogue
#
# This is the callback which shuts down the program.  It is
# invoked through the exit menu.
#
proc ExitDialogue {} {
    global Options

    if { $Options(require_confirm_on_exit) == "true" } {

        set ans [ tk_messageBox -message "Are you sure you want to exit?" \
                      -icon "question" -default "no" -type "yesno" \
                      -title "Exit" ]
        if { $ans == "no" } {
            return
        }
    }


    # Check for data modifications.  Prompt to save.

    # Clean any data from the temporary cache directory?


    # Call some sort of global clean-up function here.
    # This means that each VTK viewer must register itself
    # in a global list so we can step through and destroy each one
    # properly.
    ExitDestroyViewers

    # Finally exit
    exit
}


##
#
proc ExitRegisterViewer { viewer } {
    global ExitGlobals
    
    lappend ExitGlobals(viewers) $viewer
}


##
#
proc ExitDestroyViewers { } {
    global ExitGlobals

    foreach i $ExitGlobals(viewers) {
        $i Delete
    }
}


###
# Experimental fluff
#
proc AnimateLogo { widget x y tag } {
    set tag_coords [$widget coords $tag]
    set tagx [lindex $tag_coords 0]
    set tagy [lindex $tag_coords 1]

    if { $x > $tagx } { 
        set deltaX 2 
    } else { set deltaX -1 }
    if { $y > $tagy } {
        set deltaY 2 
    } else { set deltaY -1 }

    $widget move $tag $deltaX $deltaY
}

