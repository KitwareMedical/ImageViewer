#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    TkSegmentationWindowInteractor.tcl
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
package provide segmenterinteraction 4.0

package require vtk
package require vtkinteraction 

proc BindTkSegmentationViewer {widget resampler dataReader WSmanager} {
   # to avoid queing up multple expose events.
   SetWidgetVariableValue $widget Rendering 0

   set imager [[$widget GetImageViewer] GetRenderer]

   #
   # Get the underlying segmentation label value.
   #
   bind $widget <ButtonPress-1> [subst {  SelectRegion %W %x %y $resampler $dataReader $WSmanager}]

   bind $widget <Shift-ButtonPress-1> [subst {  AppendRegion %W %x %y $resampler $dataReader $WSmanager}]

   #
   #
   bind $widget <Expose> {ExposeTkImageViewer %W %x %y %w %h}
   bind $widget <Enter> {EnterTkViewer %W}
   bind $widget <Leave> {LeaveTkViewer %W}
   bind $widget <KeyPress-u> {wm deiconify .vtkInteract}
   bind $widget <KeyPress-r> {ResetTkImageViewer %W}
}

proc SelectRegion {widget x y resampler dataReader WSmanager} {
    global $resampler $dataReader $WSmanager

    set viewer [$widget GetImageViewer]
    set mapper [$viewer GetImageMapper]
    set input  [$viewer GetInput]
    set magX [$resampler GetAxisMagnificationFactor 0]
    set magY [$resampler GetAxisMagnificationFactor 1]

    set z [$viewer GetZSlice]

    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ $input GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax || \
            $z < $zMin || $z > $zMax} {
        return
    }

    if { $magX != 0 } { set x [expr int ($x / $magX) ]  }
    if { $magY != 0 } { set y [expr int ($y / $magY) ] }

    set val [ [$dataReader GetOutput] GetScalarComponentAsFloat $x $y $z 0]

    $WSmanager CompileEquivalenciesFor $x $y $z [$dataReader GetOutput]

#    $WSmanager CompileEquivalenciesFor $val
    $WSmanager ClearHighlightedValuesToSameColor
    $WSmanager HighlightComputedEquivalencyList

    $viewer Render

#    puts "$x $y $z => $val"

}


proc AppendRegion {widget x y resampler dataReader WSmanager} {
    global $resampler $dataReader $WSmanager

    set viewer [$widget GetImageViewer]
    set mapper [$viewer GetImageMapper]
    set input  [$viewer GetInput]
    set magX [$resampler GetAxisMagnificationFactor 0]
    set magY [$resampler GetAxisMagnificationFactor 1]

    set z [$viewer GetZSlice]

    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ $input GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax || \
            $z < $zMin || $z > $zMax} {
        return
    }

    if { $magX != 0 } { set x [expr int ($x / $magX) ]  }
    if { $magY != 0 } { set y [expr int ($y / $magY) ] }

    set val [ [$dataReader GetOutput] GetScalarComponentAsFloat $x $y $z 0]

    $WSmanager AppendEquivalenciesFor $x $y $z [$dataReader GetOutput]

#    $WSmanager CompileEquivalenciesFor $val
    $WSmanager ClearHighlightedValuesToSameColor
    $WSmanager HighlightComputedEquivalencyList

    $viewer Render

#    puts "$x $y $z => $val"

}


proc BindTkSourceImageViewer {widget resampler dataReader WSmanager labeledViewer binaryVolume binaryViewer} {
   # to avoid queing up multple expose events.
   SetWidgetVariableValue $widget Rendering 0

   set imager [[$widget GetImageViewer] GetRenderer]

   #
   # Get the underlying segmentation label value.
   #
   bind $widget <ButtonPress-2> [subst {  SelectRegion %W %x %y $resampler $dataReader $WSmanager; $labeledViewer Render;} ]

   bind $widget <Shift-ButtonPress-2> [subst {  AppendRegion %W %x %y $resampler $dataReader $WSmanager; $labeledViewer Render;} ]

#   bind $widget <ButtonPress-1> [subst {  AddPixel %W %x %y $resampler $binaryVolume $binaryViewer } ]
#   bind $widget <B1-Motion> [subst {  AddPixel %W %x %y $resampler $binaryVolume $binaryViewer } ]

#   bind $widget <ButtonPress-3> [subst {  SubtractPixel %W %x %y $resampler $binaryVolume $binaryViewer } ]
#   bind $widget <B3-Motion> [subst {  SubtractPixel %W %x %y $resampler $binaryVolume $binaryViewer } ]

   bind $widget <ButtonPress-3> [subst {  UnpaintPixels %W %x %y $resampler $binaryVolume $binaryViewer } ]
   bind $widget <B3-Motion> [subst {  UnpaintPixels %W %x %y $resampler $binaryVolume $binaryViewer } ]
   
   bind $widget <ButtonPress-1> [subst {  PaintPixels %W %x %y $resampler $binaryVolume $binaryViewer } ]
   bind $widget <B1-Motion> [subst {  PaintPixels %W %x %y $resampler $binaryVolume $binaryViewer } ]
   



   #
   #
   bind $widget <Expose> {ExposeTkImageViewer %W %x %y %w %h}
   bind $widget <Enter> {EnterTkViewer %W}
   bind $widget <Leave> {LeaveTkViewer %W}
   bind $widget <KeyPress-u> {wm deiconify .vtkInteract}
   bind $widget <KeyPress-r> {ResetTkImageViewer %W}
}

proc AddPixel { widget x y resampler binaryvol binaryviewer } {
    global $resampler $binaryvol

    set viewer [$widget GetImageViewer]
    set mapper [$viewer GetImageMapper]
    set input  [$viewer GetInput]
    set magX [$resampler GetAxisMagnificationFactor 0]
    set magY [$resampler GetAxisMagnificationFactor 1]

    set z [$viewer GetZSlice]

    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ $input GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax || \
            $z < $zMin || $z > $zMax} {
        return
    }

    if { $magX != 0 } { set x [expr int ($x / $magX) ]  }
    if { $magY != 0 } { set y [expr int ($y / $magY) ] }


    $binaryvol Set $x $y $z
    $binaryvol Modified
    $binaryvol SetUpdateExtent $x $x $y $y $z $z

    $binaryviewer Render
    $viewer Render

    $binaryvol SetUpdateExtentToWholeExtent

#    puts "$x $y $z => $val"

}

proc SubtractPixel { widget x y resampler binaryvol binaryviewer } {
    global $resampler $binaryvol

    set viewer [$widget GetImageViewer]
    set mapper [$viewer GetImageMapper]
    set input  [$viewer GetInput]
    set magX [$resampler GetAxisMagnificationFactor 0]
    set magY [$resampler GetAxisMagnificationFactor 1]

    set z [$viewer GetZSlice]

    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ $input GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax || \
            $z < $zMin || $z > $zMax} {
        return
    }

    if { $magX != 0 } { set x [expr int ($x / $magX) ]  }
    if { $magY != 0 } { set y [expr int ($y / $magY) ] }


    $binaryvol Unset $x $y $z
    $binaryvol Modified
    $binaryvol SetUpdateExtent $x $x $y $y $z $z

    $binaryviewer Render
    $viewer Render

    $binaryvol SetUpdateExtentToWholeExtent

#    puts "$x $y $z => $val"

}


proc PaintPixels { widget x y resampler binaryvol binaryviewer } {
    global $resampler $binaryvol

    set viewer [$widget GetImageViewer]
    set mapper [$viewer GetImageMapper]
    set input  [$viewer GetInput]
    set magX [$resampler GetAxisMagnificationFactor 0]
    set magY [$resampler GetAxisMagnificationFactor 1]

    set z [$viewer GetZSlice]

    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ $input GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax || \
            $z < $zMin || $z > $zMax} {
        return
    }

    if { $magX != 0 } { set x [expr int ($x / $magX) ]  }
    if { $magY != 0 } { set y [expr int ($y / $magY) ] }


    $binaryvol SetWithRadius $x $y $z
    $binaryvol Modified

    set paintRadiusValue [$binaryvol GetPaintRadius]

    $binaryvol SetUpdateExtent [expr $x - $paintRadiusValue] \
        [expr $x + $paintRadiusValue]  [expr $y - $paintRadiusValue] \
        [expr $y + $paintRadiusValue]  $z $z

    $binaryviewer Render
    $viewer Render

    $binaryvol SetUpdateExtentToWholeExtent

#    puts "$x $y $z => $val"

}

proc UnpaintPixels { widget x y resampler binaryvol binaryviewer } {
    global $resampler $binaryvol

    set viewer [$widget GetImageViewer]
    set mapper [$viewer GetImageMapper]
    set input  [$viewer GetInput]
    set magX [$resampler GetAxisMagnificationFactor 0]
    set magY [$resampler GetAxisMagnificationFactor 1]

    set z [$viewer GetZSlice]

    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ $input GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax || \
            $z < $zMin || $z > $zMax} {
        return
    }

    if { $magX != 0 } { set x [expr int ($x / $magX) ]  }
    if { $magY != 0 } { set y [expr int ($y / $magY) ] }

    set paintRadiusValue [$binaryvol GetPaintRadius]

    $binaryvol UnsetWithRadius $x $y $z
    $binaryvol Modified
    $binaryvol SetUpdateExtent [expr $x - $paintRadiusValue] \
        [expr $x + $paintRadiusValue]  [expr $y - $paintRadiusValue] \
        [expr $y + $paintRadiusValue]  $z $z

    $binaryviewer Render
    $viewer Render

    $binaryvol SetUpdateExtentToWholeExtent

#    puts "$x $y $z => $val"

}
