package require vtkinteraction

set X 63
set Y 63
set Z 63

#
#  This script renders the initial and the final images from
#  itk3DSimpleLevelSetsExample.  The zero level set is the 
#  surface of interest.
#
#

set img_data1 "initial_image_3D.raw"
set img_data2 "final_image_3D.raw"

vtkImageReader imgReader1
imgReader1 SetDataExtent 0 $X 0 $Y 0 $Z
imgReader1 SetFileName $img_data1
imgReader1 SetDataByteOrderToLittleEndian
imgReader1 SetNumberOfScalarComponents 1
imgReader1 SetFileDimensionality 3
imgReader1 SetDataScalarTypeToFloat

vtkMarchingCubes marcher1
 marcher1 SetNumberOfContours 1
 marcher1 SetValue 0 0.0
 marcher1 SetInput [imgReader1 GetOutput]

vtkOpenGLPolyDataMapper map1
 map1 ScalarVisibilityOff
 map1 ImmediateModeRenderingOn
 map1 SetInput [marcher1 GetOutput]

vtkRenderer ren1
  ren1 SetViewport 0 0 1 1

vtkRenderWindow renWin1
  renWin1 AddRenderer ren1
  renWin1 SetSize 256 256

vtkActor blob1
 blob1 SetMapper map1
 eval [blob1 GetProperty] SetColor 1.0 0.8 0.5

ren1 AddActor blob1


vtkImageReader imgReader2
imgReader2 SetDataExtent 0 $X 0 $Y 0 $Z
imgReader2 SetFileName $img_data2
imgReader2 SetDataByteOrderToLittleEndian
imgReader2 SetNumberOfScalarComponents 1
imgReader2 SetFileDimensionality 3
imgReader2 SetDataScalarTypeToFloat

vtkMarchingCubes marcher2
 marcher2 SetNumberOfContours 1
 marcher2 SetValue 0 0.0
 marcher2 SetInput [imgReader2 GetOutput]

vtkOpenGLPolyDataMapper map2
 map2 ScalarVisibilityOff
 map2 ImmediateModeRenderingOn
 map2 SetInput [marcher2 GetOutput]

vtkRenderer ren2
  ren2 SetViewport 0 0 1 1

vtkRenderWindow renWin2
  renWin2 AddRenderer ren2
  renWin2 SetSize 256 256

vtkActor blob2
 blob2 SetMapper map2
 eval [blob2 GetProperty] SetColor 1.0 0.8 0.5
ren2 AddActor blob2

frame .windows
frame .buttons

vtkTkRenderWidget .windows.r1 -width 256 -height 256 -rw renWin1
vtkTkRenderWidget .windows.r2 -width 256 -height 256 -rw renWin2
pack .windows.r1 .windows.r2 -side left -expand true -fill both -padx 1 -pady 1

pack .windows .buttons -expand true -fill both

BindTkRenderWidget .windows.r1
BindTkRenderWidget .windows.r2

