package require vtk

# usage: vtk MultipleCompareAD.tcl reg.tcl

if {$argc == 1} {
    source [lindex $argv 0]
} else {
    source reg.tcl
}
# Read the two studies. Only short data is supported by th example
vtkImageReader reader1
  reader1 SetFilePrefix $study1Prefix
  reader1 SetDataByteOrderToBigEndian
eval reader1 SetDataExtent $study1Extent
eval reader1 SetDataSpacing $study1Spacing
  reader1 SetDataScalarTypeToShort
vtkImageReader reader2
  reader2 SetFilePrefix $study2Prefix
eval reader2 SetDataExtent $study2Extent
eval reader2 SetDataSpacing $study2Spacing
  reader2 SetDataByteOrderToBigEndian
  reader2 SetDataScalarTypeToShort

if {[lindex $study1Extent 1] == "511" } {
    set mag 1
} else {
    set mag 2
}
set mag 1

# compute the means
vtkImageAccumulate stat1
  stat1 SetInput [reader1 GetOutput]
  stat1 Update
vtkImageAccumulate stat2
  stat2 SetInput [reader2 GetOutput]
  stat2 Update

set ratio [expr [lindex [stat1 GetMean] 0] / [lindex [stat2 GetMean] 0]]

# center both datasets
vtkImageChangeInformation centerImage1
  centerImage1 SetInput [reader1 GetOutput]
  centerImage1 CenterImageOn

vtkImageChangeInformation centerImage2
  centerImage2 SetInput [reader2 GetOutput]
  centerImage2 CenterImageOn

# define the tyransform found by MRIRegistration
vtkTransform transform
vtkTransform identity
set identity 1
if {[info exists matrix] == 0} {
eval  transform Translate $translate
      transform RotateZ $rotateZ
      transform RotateX $rotateX
      transform RotateY $rotateY
} else {
eval transform SetMatrix $matrix
}

reader2 Update

# alight the first dataset with the second
vtkImageReslice reslice
  reslice SetInput [centerImage1 GetOutput]
eval   reslice SetOutputExtent $study2Extent
eval   reslice SetOutputExtent [[reader2 GetOutput] GetWholeExtent]
eval reslice SetOutputSpacing [[reader2 GetOutput] GetSpacing]
  reslice SetResliceTransform transform
  reslice InterpolateOn
  reslice OptimizationOn
  reslice SetBackgroundLevel 0
  reslice AutoCropOutputOff

# does nothing for now
vtkImageTranslateExtent translate1
  translate1 SetInput [reslice GetOutput]
  translate1 SetTranslation 0 0 0

# scale the intensities of the second dataset
vtkImageShiftScale scaleMeans
  scaleMeans SetInput [centerImage2 GetOutput]
  scaleMeans SetScale $ratio

# create a checkerboard pattern of the first and second dataset
vtkImageCheckerboard checkers
  checkers SetInput 0 [translate1 GetOutput]
  checkers SetInput 1 [scaleMeans GetOutput]
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

vtkImageMagnify magReslice
  magReslice SetInput [translate1 GetOutput]
  magReslice SetMagnificationFactors $mag $mag 1
  magReslice InterpolateOff

vtkImageMapper map1
  map1 SetInput [magReslice GetOutput]
  map1 SetColorWindow 2000
  map1 SetColorLevel 1100
  map1 SetZSlice 20

vtkImageMagnify magCenterImage2
  magCenterImage2 SetInput [scaleMeans GetOutput]
  magCenterImage2 SetMagnificationFactors $mag $mag 1
  magCenterImage2 InterpolateOff

vtkImageMapper map2
  map2 SetInput [magCenterImage2 GetOutput]
  map2 SetColorWindow 2000
  map2 SetColorLevel 1100
  map2 SetZSlice 20

vtkImageMathematics diff
  diff SetInput1 [translate1 GetOutput]
  diff SetInput2 [scaleMeans GetOutput]
  diff SetOperationToSubtract

vtkImageMagnify magDiff
  magDiff SetInput [diff GetOutput]
  magDiff SetMagnificationFactors $mag $mag 1
  magDiff InterpolateOff

vtkImageMapper mapDiff
  mapDiff SetInput [magDiff GetOutput]
  mapDiff SetColorWindow 100
  mapDiff SetColorLevel -50
  mapDiff SetZSlice 20

vtkActor2D imgCheckers
  imgCheckers SetMapper mapCheckers

set label1 [lrange [file split $study1Prefix] 4 end]
vtkTextMapper mapText1
  mapText1 SetInput "Study: $label1"
  mapText1 SetFontSize 12

vtkActor2D text1
  text1 SetMapper mapText1
  text1 SetPosition 5 20
 [text1 GetProperty] SetColor 0 1 0

vtkActor2D img1
  img1 SetMapper map1

set label2 [lrange [file split $study2Prefix] 4 end]
vtkTextMapper mapText2
mapText2 SetInput "Study: $label2"
  mapText2 SetFontSize 12

vtkActor2D text2
  text2 SetMapper mapText2
  text2 SetPosition 5 20
 [text2 GetProperty] SetColor 0 1 0

vtkActor2D img2
  img2 SetMapper map2

vtkActor2D imgDiff
  imgDiff SetMapper mapDiff


vtkTextMapper mapSummary
  mapSummary SetInput "Position:  $translate\nOrientation:  $rotateX $rotateY $rotateZ"
  mapSummary SetFontSize 12

vtkActor2D summary
  summary SetMapper mapSummary
  summary SetPosition 5 20
 [summary GetProperty] SetColor 0 1 0

vtkRenderer imagerCheckers
  imagerCheckers AddActor2D imgCheckers
  imagerCheckers AddActor2D summary

vtkRenderer imager1
  imager1 AddActor2D img1
  imager1 AddActor2D text1

vtkRenderer imager2
  imager2 AddActor2D img2
  imager2 AddActor2D text2

vtkRenderer imagerDiff
  imagerDiff AddActor2D imgDiff

imager1 SetViewport 0 0 .5 .5
imager2 SetViewport .5 0 1 .5
imagerCheckers SetViewport 0 .5 .5 1
imagerDiff SetViewport .5 .5 1 1

vtkRenderWindow renWin
  renWin AddRenderer imagerCheckers
  renWin AddRenderer imager1
  renWin AddRenderer imager2
  renWin AddRenderer imagerDiff

renWin SetSize 512 512
renWin Render

#make interface
source [file join [file dirname [info script]] WindowLevelInterface2.tcl]
