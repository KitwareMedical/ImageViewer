package require vtk

# usage: vtk MultipleCompareAD.tcl reg.tcl

if {$argc == 1} {
    source [lindex $argv 0]
} else {
    set STUDY  /home/lorensen2/molecularimaging/moleimage/Kaye/s2_0325/e1
    set EXTENT "0 255 0 255 1 42"
    set SEED "123 140 26"
    set SEED1 "67 158 20"
    set SEED2 "190 47 20"
    set SEEDX "108 147 19"
    set LOWER 800
}

vtkImageReader reader1
  reader1 SetFilePrefix $STUDY
  reader1 SetDataByteOrderToBigEndian
eval reader1 SetDataExtent $EXTENT
  reader1 SetDataScalarTypeToUnsignedShort

if {[lindex $EXTENT 1] == "511" } {
    set mag 1
} else {
    set mag 2
}
set mag 1

#vtkITKConnectedThresholdImageFilter connected
#  connected SetInput [reader1 GetOutput]
#  connected SetReplaceValue 1
#eval  connected SetSeed $SEED1
#  connected SetLower 750
#  connected SetUpper 2000

vtkITKConfidenceConnectedImageFilter connected
  connected SetInput [reader1 GetOutput]
  connected SetReplaceValue 1
eval  connected SetSeed $SEEDX
connected SetMultiplier 10
connected SetNumberOfIterations 30
connected DebugOn


vtkImageShiftScale connectedScale
  connectedScale SetInput [connected GetOutput]
  connectedScale SetScale 2000

#vtkITKIsolatedConnectedImageFilter connected
#  connected SetInput [reader1 GetOutput]
#  connected SetReplaceValue 2000
#eval  connected SetSeed1 $SEED1
#eval  connected SetSeed2 $SEED2

# create a checkerboard pattern of the first and second dataset
vtkImageCheckerboard checkers
  checkers SetInput 0 [reader1 GetOutput]
  checkers SetInput 1 [connectedScale GetOutput]
  checkers SetNumberOfDivisions 2 2 1

# magnify if necessary
vtkImageMagnify magCheckers
  magCheckers SetInput [checkers GetOutput]
  magCheckers SetMagnificationFactors 1 1 1
  magCheckers SetMagnificationFactors $mag $mag 1
  magCheckers InterpolateOff

# set up 4 viewports

vtkImageMapper mapCheckers
  mapCheckers SetInput [magCheckers GetOutput]
  mapCheckers SetColorWindow 2000
  mapCheckers SetColorLevel 1100
  mapCheckers SetZSlice 20

vtkImageMagnify magConnected
  magConnected SetInput [connectedScale GetOutput]
  magConnected SetMagnificationFactors $mag $mag 1
  magConnected InterpolateOff

vtkImageMapper map2
  map2 SetInput [magConnected GetOutput]
  map2 SetColorWindow 2000
  map2 SetColorLevel 1100
  map2 SetZSlice 20

vtkImageMagnify magReader1
  magReader1 SetInput [reader1 GetOutput]
  magReader1 SetMagnificationFactors $mag $mag 1
  magReader1 InterpolateOff

vtkImageMapper map1
  map1 SetInput [magReader1 GetOutput]
  map1 SetColorWindow 2000
  map1 SetColorLevel 1100
  map1 SetZSlice 20

vtkImageMathematics mask
  mask SetInput1 [reader1 GetOutput]
  mask SetInput2 [connected GetOutput]
  mask SetOperationToMultiply

vtkImageMagnify magMask
  magMask SetInput [mask GetOutput]
  magMask SetMagnificationFactors $mag $mag 1
  magMask InterpolateOff

vtkImageMapper mapMask
  mapMask SetInput [magMask GetOutput]
  mapMask SetColorWindow 2000
  mapMask SetColorLevel 1100
  mapMask SetZSlice 20

vtkActor2D imgCheckers
  imgCheckers SetMapper mapCheckers

vtkActor2D img1
  img1 SetMapper map1

vtkActor2D img2
  img2 SetMapper map2

vtkActor2D imgMask
  imgMask SetMapper mapMask

vtkRenderer imagerCheckers
  imagerCheckers AddActor2D imgCheckers

vtkRenderer imager1
  imager1 AddActor2D img1

vtkRenderer imager2
  imager2 AddActor2D img2

vtkRenderer imagerMask
  imagerMask AddActor2D imgMask

imager1 SetViewport 0 0 .5 .5
imager2 SetViewport .5 0 1 .5
imagerCheckers SetViewport 0 .5 .5 1
imagerMask SetViewport .5 .5 1 1

vtkRenderWindow renWin
  renWin AddRenderer imagerCheckers
  renWin AddRenderer imager1
  renWin AddRenderer imager2
  renWin AddRenderer imagerMask

renWin SetSize 512 512
renWin Render

vtkWindowToImageFilter w2if
   w2if ReadFrontBufferOff
   w2if SetInput renWin

vtkPNMWriter pnm
pnm SetInput [w2if GetOutput]
pnm SetFileName a.ppm
pnm Write
#puts "[connected GetIsolatedValue]"

#make interface
source [file join [file dirname [info script]] WindowLevelInterface2.tcl]
