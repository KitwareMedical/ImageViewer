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

set GEVTK_DATA_ROOT /projects/vtk/GEVTKproduction/GEVTKData
#
# Read in the test data.
#
vtkImageReader reader
     reader SetDataByteOrderToLittleEndian
     reader SetDataSpacing 0.8  0.8  1.5
     reader SetDataExtent 0 255 0 255 1 94
     reader SetFilePrefix "$GEVTK_DATA_ROOT/Data/fullHead/headsq"
     reader SetDataMask 0x7fff
     reader UpdateInformation
     reader Update
 
set bounds [[reader GetOutput] GetBounds]
reader SetDataOrigin [expr {-1*([lindex $bounds 0]+[lindex $bounds 1])/2.0}] \
    [expr {-1*([lindex $bounds 2]+[lindex $bounds 3])/2.0}] \
    [expr {-1*([lindex $bounds 4]+[lindex $bounds 5])/2.0}]
set extent [[reader GetOutput] GetWholeExtent]

set LowerZSlice [lindex $extent 4]
set UpperZSlice [lindex $extent 5]
set DefaultZSlice [expr {round(($LowerZSlice+$UpperZSlice)/2.0)}]

vtkTransform trans
    trans Translate -16 -20.0 10.0
    trans RotateX 10
    trans Update

vtkImageReslice slicer
    slicer SetInterpolationModeToCubic
    slicer SetInput [reader GetOutput]
    slicer SetBackgroundLevel 0
    slicer SetResliceTransform trans
    slicer Update


vtkITKMutualInformationTransform mutual
    mutual SetSourceImage [reader GetOutput]
    mutual SetTargetImage [slicer GetOutput]
    mutual SetNumberOfIterations 500
    mutual SetNumberOfSamples 50
    mutual SetLearningRate .001
    mutual SetTranslateScale 64


vtkImageReslice newSlicer
    newSlicer SetInterpolationModeToCubic
    newSlicer SetInput [reader GetOutput]
    newSlicer SetResliceTransform mutual
    newSlicer SetBackgroundLevel 0

set colorLevel 1000
set colorWindow 2000

vtkImageCheckerboard checkers
    checkers SetInput 0 [slicer GetOutput]
    checkers SetInput 1 [newSlicer GetOutput]
    checkers SetNumberOfDivisions 5 5 5

vtkImageMapper imageMapperSource
    imageMapperSource SetInput [checkers GetOutput]
    imageMapperSource SetZSlice $DefaultZSlice
    imageMapperSource SetColorWindow $colorWindow
    imageMapperSource SetColorLevel $colorLevel

vtkActor2D actor1 
    actor1 SetMapper imageMapperSource

#
# Add the actor to the renderer, set the background and size
ren1 AddActor actor1
#ren1 SetBackground 0.8 0.8 0.8

renWin SetSize [lindex $bounds 1] [lindex $bounds 3]
renWin Render

if { [info command rtExMath] != ""} {
  set numIters 10
} else {
  set numIters 250

}
for {set iters 0} {$iters<3} {incr iters} {
    mutual SetNumberOfIterations $numIters
    for {set ctr 0} {$ctr<1} {incr ctr} {
         mutual Modified
         mutual Update
         renWin Render
     }
 }

#
# Render the image
iren SetUserMethod {wm deiconify .vtkInteract}
iren Initialize

# Prevent the tk window from showing up then start the event loop
wm withdraw .






