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
set GEVTK_DATA_ROOT /projects/vtk/GEVTKproduction/GEVTKData
#
# Read in the test data.
#
vtkImageReader reader
     reader SetDataByteOrderToLittleEndian
     reader SetDataScalarTypeToUnsignedShort
     reader SetDataSpacing 0.8  0.8  1.5
     reader SetDataExtent 0 255 0 255 1 94
     reader SetFilePrefix "$GEVTK_DATA_ROOT/Data/fullHead/headsq"
     reader SetDataMask 0x7fff

vtkImageCast castToFloat
 castToFloat SetOutputScalarTypeToFloat
 castToFloat SetInput [reader GetOutput]

set LowerZSlice 1
set UpperZSlice 94
set DefaultZSlice [expr {round(($LowerZSlice+$UpperZSlice)/2.0)}]

vtkTransform trans
#    trans Translate -5 -5 5
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

vtkImageShrink3D shrinkSource
  shrinkSource SetInput [changeSource GetOutput]
  shrinkSource SetShrinkFactors 4 4 1

vtkITKNormalizeImageFilter normalizeSource
    normalizeSource SetInput [shrinkSource GetOutput]

vtkImageChangeInformation changeTarget
  changeTarget SetInput [castToFloat GetOutput]
  changeTarget CenterImageOn

vtkImageShrink3D shrinkTarget
  shrinkTarget SetShrinkFactors 4 4 1
  shrinkTarget SetInput [changeTarget GetOutput]

vtkITKNormalizeImageFilter normalizeTarget
    normalizeTarget SetInput [shrinkTarget GetOutput]

vtkITKMutualInformationTransform mutual
    mutual SetSourceImage [normalizeSource GetOutput]
    mutual SetTargetImage [normalizeTarget GetOutput]
    mutual SetNumberOfIterations 10
    mutual SetNumberOfSamples 50
    mutual SetLearningRate .001
    mutual SetTranslateScale 64

vtkImageReslice newSlicer
    newSlicer SetInterpolationModeToCubic
    newSlicer SetInput [changeSource GetOutput]
    newSlicer SetResliceTransform mutual
    newSlicer SetBackgroundLevel 0

set colorLevel 1000
set colorWindow 2000

vtkImageCheckerboard checkers
    checkers SetInput 0 [changeTarget GetOutput]
    checkers SetInput 1 [newSlicer GetOutput]
    checkers SetNumberOfDivisions 2 2 1

vtkImageMapper imageMapperSource
    imageMapperSource SetInput [checkers GetOutput]
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
renWin Render

#
# Render the image
iren SetUserMethod {wm deiconify .vtkInteract}
iren Initialize
renWin Render
# Prevent the tk window from showing up then start the event loop
wm withdraw .
