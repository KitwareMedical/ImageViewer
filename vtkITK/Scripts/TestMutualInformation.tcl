package require vtk

#
# Create the RenderWindow, Renderer and Interactor
#
vtkRenderer ren1
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin

#
# Set up some parameters.
#
set DEF_V_VAR 4.0
set DEF_UU_VAR 2.0
set DEF_VV_VAR 2.0
set DEF_ROT_STEP 0.005
set DEF_TRA_STEP 100

set GEVTK_DATA_ROOT c:/lorensen/GEVTKData
#
# Read in the test data.
#
vtkImageReader reader
     reader SetDataByteOrderToLittleEndian
     reader SetDataScalarTypeToUnsignedShort
     reader SetDataSpacing 0.8  0.8  1.5
     reader SetDataExtent 0 255 0 255 1 10
     reader SetFilePrefix "$GEVTK_DATA_ROOT/Data/fullHead/headsq"
     reader SetDataMask 0x7fff

vtkImageCast castToFloat
 castToFloat SetOutputScalarTypeToFloat
 castToFloat SetInput [reader GetOutput]

set LowerZSlice 1
set UpperZSlice 94
set DefaultZSlice [expr {round(($LowerZSlice+$UpperZSlice)/2.0)}]

vtkTransform trans
    trans Translate -16 -20.0 10.0
    trans RotateX 10
    trans Update

vtkImageReslice slicer
    slicer SetInterpolationModeToCubic
    slicer SetInput [castToFloat GetOutput]
    slicer SetBackgroundLevel 0
    slicer SetResliceTransform trans
    slicer Update

vtkImageChangeInformation changeSource
  changeSource SetInput [slicer GetOutput]
  changeSource CenterImageOn

vtkITKNormalizeImageFilter normalizeSource
    normalizeSource SetInput [changeSource GetOutput]

vtkImageShrink3D shrinkSource
  shrinkSource SetInput [normalizeSource GetOutput]
  shrinkSource SetShrinkFactors 4 4 1

vtkImageChangeInformation changeTarget
  changeTarget SetInput [castToFloat GetOutput]
  changeTarget CenterImageOn

vtkITKNormalizeImageFilter normalizeTarget
    normalizeTarget SetInput [changeTarget GetOutput]

vtkImageShrink3D shrinkTarget
  shrinkTarget SetShrinkFactors 4 4 1
  shrinkTarget SetInput [normalizeTarget GetOutput]

vtkITKMutualInformationTransform mutual
    mutual SetSourceImage [shrinkSource GetOutput]
    mutual SetTargetImage [shrinkTarget GetOutput]
    mutual SetNumberOfIterations 200
    mutual SetNumberOfSamples 50
    mutual SetLearningRate .001
    mutual SetTranslateScale 64

vtkImageReslice newSlicer
    newSlicer SetInterpolationModeToCubic
    newSlicer SetInput [changeSource GetOutput]
    newSlicer SetResliceTransform mutual
    newSlicer SetBackgroundLevel 0

set colorLevel 100
set colorWindow 1

vtkImageCheckerboard checkers
    checkers SetInput 0 [reader GetOutput]
    checkers SetInput 1 [reader GetOutput]
    checkers SetNumberOfDivisions 5 5 5

vtkImageMapper imageMapperSource
    imageMapperSource SetInput [reader GetOutput]
    imageMapperSource SetZSlice 10
    imageMapperSource SetColorWindow $colorWindow
    imageMapperSource SetColorLevel $colorLevel

vtkActor2D actor1 
    actor1 SetMapper imageMapperSource

#
# Add the actor to the renderer, set the background and size
ren1 AddActor actor1
#ren1 SetBackground 0.8 0.8 0.8

renWin SetSize 256 256


#
# Render the image
iren SetUserMethod {wm deiconify .vtkInteract}
iren Initialize
renWin Render
# Prevent the tk window from showing up then start the event loop
wm withdraw .
