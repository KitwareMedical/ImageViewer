# This class reads a VTK data file in either ASCII or BINARY format.
# The viewer provides simple operations to get pixel values.
#
catch {load vtktcl}
if { [catch {set VTK_TCL $env(VTK_TCL)}] != 0} { set VTK_TCL "./" }

source $VTK_TCL/vtkImageInclude.tcl
source $VTK_TCL/TkImageViewerInteractor.tcl

# This script uses a vtkTkRenderWidget to create a
# Tk widget that is associated with a vtkRenderWindow.

# Create the GUI: a file menu button and the viewer widget.
#
set ViewSize 250
set FitImage 1

wm title . "Insight VTK Viewer"

frame .top
frame .top.mbar -relief raised -bd 2
pack .top.mbar -side top -fill x

menubutton .top.mbar.file -text File -menu .top.mbar.file.menu
menubutton .top.mbar.options -text Edit -menu .top.mbar.options.menu
pack .top.mbar.file .top.mbar.options -side left

menu .top.mbar.file.menu
    .top.mbar.file.menu add command -label Open -command OpenFile
    .top.mbar.file.menu add command -label Exit -command exit

menu .top.mbar.options.menu
    .top.mbar.options.menu add checkbutton -label "Fit Image"  \
            -variable FitImage -command SetFitImage

frame .top.f1 

vtkTkImageViewerWidget .top.f1.r1 -width $ViewSize -height $ViewSize
pack .top.f1.r1 -side left -padx 3 -pady 3 -fill both -expand t
pack .top.f1  -fill both -expand t
pack .top -fill both -expand t

# The pipeline
vtkImageResample magValid
  magValid InterpolateOn
  magValid SetDimensionality 2
set Viewer [.top.f1.r1 GetImageViewer]
BindTkImageViewer .top.f1.r1 

## Open a VTK file
proc OpenFile {} {
    global Viewer FitImage ViewSize

    set types {
        {{Visualization Toolkit (polygonal)}    {.vtk}        }
        {{All Files }                           *             }
    }
    set filename [tk_getOpenFile -filetypes $types]
    if { $filename != "" } {
        if { [info commands reader] != "" } {
            reader Delete
            validFileCheck Delete
        }
        if { [string match *.vtk $filename] } {
            vtkStructuredPointsReader reader
            vtkDataReader validFileCheck
            validFileCheck SetFileName $filename
            reader SetFileName $filename
            if { ! [validFileCheck IsFileValid "structured_points"] } {
                puts "Can't read this file"
                return
            }
            reader Update
            SetFitImage
        } else {
            puts "Can't read this file"
            return
        }

	set filename "Insight Viewer: [file tail $filename]"
	wm title . $filename

        ResetTkImageViewer .top.f1.r1
        $Viewer Render
    }
}

proc SetFitImage {} {
    global FitImage Viewer ViewSize


    if { $FitImage } {
        set size [[$Viewer GetImageWindow] GetSize]
        set dims [[reader GetOutput] GetDimensions]
        
        # Now scale the image to fit
        set sx [expr double([lindex $size 0]) / [lindex $dims 0]]
        set sy [expr double([lindex $size 1]) / [lindex $dims 1]]
        if { $sx < $sy } {
            set scale $sx
        } else {
            set scale $sy
        }
        magValid SetAxisMagnificationFactor 0 $scale
        magValid SetAxisMagnificationFactor 1 $scale
        magValid SetInput [reader GetOutput]
        $Viewer SetInput [magValid GetOutput]
    } else {
        $Viewer SetInput [reader GetOutput]
    }
}


