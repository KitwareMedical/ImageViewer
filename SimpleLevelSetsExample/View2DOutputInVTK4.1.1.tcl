package require vtkinteraction

set X 255
set Y 255

set img_data "initial_image_2D.raw"
set img_data2 "final_image_2D.raw"

#
# Left-hand image is the initial position of the 0 level set
# the right-hand image is the final position of the 0 level set.
# Middle button queries values in the right-hand image

### Img
vtkImageReader imgReader
imgReader SetDataExtent 0 $X 0 $Y 0 0
imgReader SetFileName $img_data
imgReader SetDataByteOrderToLittleEndian
imgReader SetNumberOfScalarComponents 1
imgReader SetFileDimensionality 2
imgReader SetDataScalarTypeToFloat

vtkColorTransferFunction lut
 lut AddRGBPoint -2.0 1.0 1.0 0.0
 lut AddRGBPoint -1.0 1.0 0.0 0.0
 lut AddRGBPoint 0.0 1.0 1.0 1.0
 lut AddRGBPoint 1.0 1.0 0.0 0.0
 lut AddRGBPoint 2.0 1.0 1.0 0.0

vtkImageMagnify imgMag
 imgMag SetMagnificationFactors 1 1 1
 imgMag SetInput [imgReader GetOutput]

vtkImageMapToColors colorMapImg
colorMapImg SetInput [imgMag GetOutput]
colorMapImg SetLookupTable lut
colorMapImg SetOutputFormatToRGBA

vtkImageViewer imgViewer
imgViewer SetColorWindow 255
imgViewer SetColorLevel 127.5
imgViewer SetZSlice 0
imgViewer SetInput [colorMapImg GetOutput]

### Img2
vtkImageReader imgReader2
imgReader2 SetDataExtent 0 $X 0 $Y 0 0
imgReader2 SetFileName $img_data2
imgReader2 SetDataByteOrderToLittleEndian
imgReader2 SetNumberOfScalarComponents 1
imgReader2 SetFileDimensionality 2
imgReader2 SetDataScalarTypeToFloat

vtkImageMagnify imgMag2
 imgMag2 SetMagnificationFactors 1 1 1
 imgMag2 SetInput [imgReader2 GetOutput]

vtkImageMapToColors colorMapImg2
colorMapImg2 SetInput [imgMag2 GetOutput]
colorMapImg2 SetLookupTable lut
colorMapImg2 SetOutputFormatToRGBA

vtkImageViewer imgViewer2
imgViewer2 SetColorWindow 255
imgViewer2 SetColorLevel 127.5
imgViewer2 SetZSlice 0
imgViewer2 SetInput [colorMapImg2 GetOutput]

vtkTextMapper valLabel
valLabel SetInput "none"
valLabel SetFontFamilyToTimes
valLabel SetFontSize 18
valLabel BoldOn
valLabel ShadowOn

vtkActor2D valLabelActor
valLabelActor SetMapper valLabel
valLabelActor SetLayerNumber 1
valLabelActor SetVisibility 0

[imgViewer2 GetRenderer] AddActor valLabelActor


### GUI
frame .windows
vtkTkImageViewerWidget .windows.r1 -width [expr 1.0 * $X] -height [expr 1.0 * $Y] \
    -iv imgViewer

vtkTkImageViewerWidget .windows.r2 -width [expr 1.0 * $X] -height [expr 1.0 * $Y] \
    -iv imgViewer2

pack .windows.r1 .windows.r2 -side left -padx 2

frame .buttons
button .buttons.quit -text "quit" -command Quit
pack .buttons.quit

pack .windows .buttons -pady 2

::vtk::bind_tk_imageviewer_widget .windows.r1
::vtk::bind_tk_imageviewer_widget .windows.r2
#BindTkImageViewer .windows.r1
#BindTkImageViewer .windows.r2

bind .windows.r2 <ButtonPress-2> [subst { StartQueryValue %W %x %y }]
bind .windows.r2 <B2-Motion> {QueryValue %W %x %y}
bind .windows.r2 <ButtonRelease-2> {EndQueryValue %W}

proc StartQueryValue {widget x y} {
    valLabelActor SetVisibility 1
    QueryValue $widget $x $y
}

proc EndQueryValue {widget} {
    valLabelActor SetVisibility 0
    .windows.r2 Render
}

proc QueryValue {widget x y} {
    # y is flipped upside down
    set height [lindex [$widget configure -height] 4]
    set y [expr $height - $y]

    # make sure point is in the whole extent of the image.
    scan [ [imgViewer2 GetInput] GetWholeExtent] "%d %d %d %d %d %d" \
        xMin xMax yMin yMax zMin zMax
    if {$x < $xMin || $x > $xMax || $y < $yMin || $y > $yMax} {
        return
    }

    # unmagnify
    set x [expr int ($x / 1) ]
    set y [expr int ($y / 1) ]

    set val [ [imgReader2 GetOutput] GetScalarComponentAsFloat $x $y 0 0]
    valLabel SetInput "($x, $y):  $val"
    .windows.r2 Render
}


proc Quit {} {
#    imgViewer Delete
#    imgViewer2 Delete
    exit
}
