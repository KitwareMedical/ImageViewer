#  Copyright (c) 2002 Insight Consortium. All rights reserved.
#  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#     This software is distributed WITHOUT ANY WARRANTY; without even 
#     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
#     PURPOSE.  See the above copyright notices for more information.

package require vtkinteraction

set X 63
set Y 63
set Z 63

set img_data1 "sample_input.64x64x64.char"
set img_data2 "sample_output.64x64x64.float"

vtkImageReader imgReader1
imgReader1 SetDataExtent 0 $X 0 $Y 0 $Z
imgReader1 SetFileName $img_data1
imgReader1 SetDataByteOrderToLittleEndian
imgReader1 SetNumberOfScalarComponents 1
imgReader1 SetFileDimensionality 3
imgReader1 SetDataScalarType 2

vtkMarchingCubes marcher1
 marcher1 SetNumberOfContours 1
 marcher1 SetValue 0 1.0
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
ren2 SetActiveCamera [ren1 GetActiveCamera]


frame .windows

vtkTkRenderWidget .windows.r1 -width 256 -height 256 -rw renWin1
vtkTkRenderWidget .windows.r2 -width 256 -height 256 -rw renWin2
pack .windows.r1 .windows.r2 -side left -expand true -fill both

pack .windows -expand true -fill both

BindTkRenderWidget .windows.r1
BindTkRenderWidget .windows.r2

