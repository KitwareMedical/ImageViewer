#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    LicenseModule.tcl
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
# proc ConstructLicenseFrame
#

proc ConstructLicenseFrame {f} {
    global Options

    text $f.text \
        -font $Options(mono_font) \
        -width 100 \
        -background $Options(text_background_color) \
        -wrap "word" \
        -insertontime 0 \
        -setgrid true  \
        -yscrollcommand "$f.scrollbar set"

    scrollbar $f.scrollbar -orient "vert" -command "$f.text yview"

    if { [file exists $Options(disclaimer_file)] } {

        set fileId [open $Options(disclaimer_file) r]

        if {$fileId != ""} {
            set i 0
            while {! [eof $fileId]} {
                set i [expr $i +1 ]
                set tmp [gets $fileId]
                append tmp "\n"
                [subst {$f.text}] insert $i.0 $tmp
            }
        }
    } else { 
        [subst {$f.text}] insert 0.0 $Options(disclaimer_text)
    }
    
    pack $f.scrollbar -side right -fill y
    pack $f.text -fill both -expand t

    $f.text configure -state disabled
}

proc ActivateLicenseModule {} {
}

proc DeactivateLicenseModule {} {
}
