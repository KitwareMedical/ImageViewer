package require vtk
package require vtkinteraction
#
# Create the RenderWindow, Renderer and Interactor
#
vtkRenderer ren1
vtkRenderer ren2
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindow renWin2
    renWin2 AddRenderer ren2
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin
vtkRenderWindowInteractor iren2
    iren2 SetRenderWindow renWin2

#
# Set up some parameters.
#
set DEF_V_VAR 4.0
set DEF_UU_VAR 2.0
set DEF_VV_VAR 2.0
set DEF_ROT_STEP 0.005
set DEF_TRA_STEP 100

set GEVTK_DATA_ROOT /projects/vtk/GEVTKproduction/GEVTKData
set GEVTK_DATA_ROOT c:/lorensen/GEVTKData

#
# Read in the test data.
#
vtkImageReader reader
     reader SetDataByteOrderToLittleEndian
     reader SetDataScalarTypeToUnsignedShort
     reader SetDataSpacing 0.8  0.8  1.5
     reader SetDataExtent 0 255 0 255 1 93
     reader SetFilePrefix "$GEVTK_DATA_ROOT/Data/fullHead/headsq"
     reader SetDataMask 0x7fff

vtkImageChangeInformation center
  center SetInput [reader GetOutput]
  center CenterImageOn

set mag 2
vtkImageShrink3D shrink
  shrink SetShrinkFactors $mag $mag 1
  shrink SetInput [center GetOutput]
  shrink AveragingOn

vtkImageCast castToFloat
 castToFloat SetOutputScalarTypeToFloat
 castToFloat SetInput [shrink GetOutput]

set LowerZSlice 1
set UpperZSlice 93
set DefaultZSlice [expr {round(($LowerZSlice+$UpperZSlice)/2.0)}]

vtkTransform trans
    trans RotateZ 10
    trans RotateY 5
    trans Translate -15 5 10
    trans Update

vtkImageReslice slicer
    slicer SetInterpolationModeToCubic
    slicer SetInput [castToFloat GetOutput]
    slicer SetBackgroundLevel 0
    slicer SetResliceTransform trans

vtkImageChangeInformation changeSource
  changeSource SetInput [slicer GetOutput]
  changeSource CenterImageOn

vtkImageShrink3D shrinkSource
  shrinkSource SetInput [changeSource GetOutput]
  shrinkSource SetShrinkFactors 4 4 1
  shrinkSource AveragingOn

vtkITKNormalizeImageFilter normalizeSource
    normalizeSource SetInput [shrinkSource GetOutput]

vtkImageChangeInformation changeTarget
  changeTarget SetInput [castToFloat GetOutput]
  changeTarget CenterImageOn

vtkImageShrink3D shrinkTarget
  shrinkTarget SetShrinkFactors 4 4 1
  shrinkTarget SetInput [changeTarget GetOutput]
  shrinkTarget AveragingOn

vtkITKNormalizeImageFilter normalizeTarget
    normalizeTarget SetInput [shrinkTarget GetOutput]
    normalizeTarget Update

vtkITKMutualInformationTransform mutual
    mutual SetSourceImage [normalizeSource GetOutput]
    mutual SetTargetImage [normalizeTarget GetOutput]
    mutual SetNumberOfIterations 50
    mutual SetNumberOfSamples 50
    mutual SetLearningRate .01
    mutual SetTranslateScale 64

vtkImageReslice newSlicer
    newSlicer SetInterpolationModeToCubic
    newSlicer SetInput [changeSource GetOutput]
    newSlicer SetResliceTransform mutual
    newSlicer SetBackgroundLevel 0

set colorLevel 1000
set colorWindow 2000

vtkMarchingContourFilter marchTarget
  marchTarget SetInput [changeTarget GetOutput]
  marchTarget SetValue 0 600
 
vtkMarchingContourFilter marchSource
  marchSource SetInput [changeSource GetOutput]
  marchSource SetValue 0 600
 
vtkTransformPolyDataFilter transformSource
  transformSource SetTransform mutual
  transformSource SetInput [marchSource GetOutput]

vtkImageCheckerboard checkers
    checkers SetInput 0 [changeTarget GetOutput]
    checkers SetInput 1 [newSlicer GetOutput]
    checkers SetNumberOfDivisions 2 2 1

set mag 4
vtkImageMagnify magCheckers
  magCheckers SetInput [checkers GetOutput]
  magCheckers SetMagnificationFactors $mag $mag 1
  magCheckers InterpolateOn

vtkImageMapper imageMapperSource
    imageMapperSource SetInput [magCheckers GetOutput]
    imageMapperSource SetZSlice 20
    imageMapperSource SetColorWindow $colorWindow
    imageMapperSource SetColorLevel $colorLevel

vtkActor2D actor1 
    actor1 SetMapper imageMapperSource

vtkPolyDataMapper sourceMapper
  sourceMapper SetInput [marchSource GetOutput]
  sourceMapper ScalarVisibilityOff

vtkLODActor actorSource
  actorSource SetMapper sourceMapper
eval   [actorSource GetProperty] SetDiffuseColor .2 .8 .3

vtkPolyDataMapper targetMapper
  targetMapper SetInput [marchTarget GetOutput]
  targetMapper ScalarVisibilityOff

vtkActor actorTarget
  actorTarget SetMapper targetMapper

#
# Add the actor to the renderer, set the background and size
ren1 AddActor actor1
ren2 AddActor actorSource
ren2 AddActor actorTarget

#ren1 SetBackground 0.8 0.8 0.8

renWin SetSize 512 512
renWin2 SetSize 512 512

#
# Render the image
iren SetUserMethod {wm deiconify .vtkInteract}
iren Initialize
iren2 Initialize

renWin2 Render
renWin Render

# Prevent the tk window from showing up then start the event loop
wm withdraw .
vtkTransform a
vtkTransform b
proc slice {n} {
  imageMapperSource SetZSlice $n
  renWin Render
}

proc go {m n} {
  shrinkSource SetShrinkFactors $n $n 1
  shrinkTarget SetShrinkFactors $n $n 1
    if {$n >= 4} { set l .01 }
    if {$n == 3} { set l .05 }
    if {$n == 2} { set l .001 }
    if {$n == 1} { set l .005 }
    mutual SetLearningRate $l
    for {set i 0} { $i < $m } {incr i} {
    mutual Modified
    eval b SetMatrix [mutual GetMatrix]
    b Inverse
    transformSource SetTransform b
        actorSource SetUserMatrix [b GetMatrix]
    renWin Render
    renWin2 Render
        eval a SetMatrix [mutual GetMatrix]
        puts "[a GetOrientation] [a GetPosition]"
    }
}
