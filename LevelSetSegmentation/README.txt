This example shows how to perform segmentation of an image using
level set segmentation filters.

You should read the online documentation for the appropriate filter
(also found in the header files) before attempting to any of these examples.


itk2DThresholdSegmentationLevelSetImageFilter
---------------------------------------------
There are 12 command line parameters as follows

itk2DThresholdSegmentationLevelSetImageFilter input_image input_model output_model 
output_features iterations max_rms_change lower_threshold upper_threshold 
feature_scaling curvature_scaling isovalue negative_features

input_image     The name of the "feature" image, typically the image to segment.  Must be 
                in PNG format (grayscale).

input_model     The name of the "seed" image which contains the initial model.  Must be
                in PNG format (grayscale).

output_model    The name of the output file which will contain the solution. The final
                position of the front is denoted by zero crossings in this image.  The
                image is a raw (no header) floating point image of same size as the input.

output_features The name of the output file which contains the "feature" image 
                calculated by the filter.  This was the image used as a speed function
                for the solution.

iterations      The maximum number of iterations that the solver will perform.

max_rms         The minimum RMS change value of the level-set front allowed for the
                solver to continue iterating (maximum RMS of the solution).

lower_threshold Lower threshold value for the filter to use in calculating speed function.

upper_threshold Upper threshold value for the filter to use in calculating speed function.

feature_scaling Scaling value for the speed term of the equation.

curvature_scaling Scaling value for the curvature term of the equation.

isovalue          The isovalue to use as an initial front in the input model image.

negative_features 1=Use 0=Do not use.  Turning negative features on will swap the inside/outside
                  positive/negative convention. (Front will shrink instead of grow and vice-versa)



Note that this example does NOT handle COLOR PNG images.

vtkViewOutput.tcl 
-----------------
This VTK script can be used to view the output of the example application(s).
You will need to edit the file to point to the correct data files and set the
X and Y values correctly.

When viewing the "feature" image, the green values correspond to negative 
speeds and the red to positive speeds.
