package require vtk
package require vtkinteraction

set X 299
set Y 215

# A reader for the original image
vtkPNGReader sReader
sReader SetFileName "burundi_beef.png"

# A reader for the level set image
vtkImageReader lsReader
lsReader SetFileName "output.raw"
lsReader SetDataExtent 0 $X 0 $Y 0 0
lsReader SetFileDimensionality 2
lsReader SetDataScalarTypeToFloat
lsReader SetDataByteOrderToBigEndian

# A reader for the feature image
vtkImageReader fReader
fReader SetFileName "features.raw"
fReader SetDataExtent 0 $X 0 $Y 0 0
fReader SetFileDimensionality 2
fReader SetDataScalarTypeToFloat
fReader SetDataByteOrderToBigEndian

# A color lookup table for the level sets
vtkLookupTable lsLookup
lsLookup SetNumberOfTableValues 2
lsLookup SetNumberOfColors 2
lsLookup SetTableValue 1 1.0 1.0 0.0 1.0
lsLookup SetTableValue 0 0.0 0.0 0.0 0.0

# A color lookup table for the image features
vtkColorTransferFunction fLookup
fLookup AddRGBPoint 0.0001 1 0 0   ;# Red are positive INSIDE values
fLookup AddRGBPoint -0.0001 0 1 0  ;# Green are negative OUTSIDE values

# A threshold filter to approximate zero crossings of level sets
vtkImageThreshold lsThreshold
lsThreshold ThresholdBetween -0.55 0.55
lsThreshold ReplaceInOn
lsThreshold ReplaceOutOn
lsThreshold SetInValue 1
lsThreshold SetOutValue 0
lsThreshold SetOutputScalarTypeToUnsignedChar
lsThreshold SetInput [lsReader GetOutput]

# A color mapper for the level sets
vtkImageMapToColors lsMapper
lsMapper SetLookupTable lsLookup
lsMapper SetInput [lsThreshold GetOutput]

# A color mapper for the features
vtkImageMapToColors fMapper
fMapper SetLookupTable fLookup
fMapper SetInput [fReader GetOutput]

# A viewer
vtkImageViewer viewer
viewer SetColorWindow 255
viewer SetColorLevel  127.5
viewer SetInput [sReader GetOutput]
#viewer SetInput [fMapper GetOutput]

# A widget for the viewer
vtkTkImageViewerWidget .viewer_widget -iv viewer -height $Y -width $X
::vtk::bind_tk_imageviewer_widget .viewer_widget
pack .viewer_widget
