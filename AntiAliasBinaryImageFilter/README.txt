I. DESCRIPTION

This example demonstrates how to apply the AntiAliasBinaryImageFilter to a
binary volume to generate a surface estimate of that volume.  For information on
the AntiAliasBinaryImageFilter, including what it does and how it is
constructed, please see the documentation in the file
itkAntiAliasBinaryImageFilter.txx and in the documentation section of
www.itk.org.

Before continuing with this example, you might want to take the time to read
the documentation found in itkAntiAliasBinaryImageFilter and online at
www.itk.org in the Documentation section.


II. HOW TO BUILD

If BUILD_EXAMPLES has been turned on during the CMake configuration of the
Insight build, then this example should build automatically.

Note:
This example uses the
Applications/StreamedWatershedSegmentation/ParameterFileParser library. There is a
dependency switch in CMakeLists.txt which ensures that the ParameterFileParser
library is build before this example is built.


III. HOW TO RUN

Create a file containing the parameters you want to use.  See the sample
parameter file provided for more information.  Invoke the application with the
name of the parameter file as a command line argument.


IV. INPUT

The input should be a binary volume.  The actual values are unimportant, as
long as there are only two of them.

The input file that you use _must_ be a raw (no header) binary file of type
char.  Any other input will be interpreted as garbage and will not give you
meaningful results.  That said, you can easily change the input type by
changing a single typedef in the source code of the application.  See
itkAntiAliasBinaryImageFilterExample.cxx for details.  By changing this typedef
and recompiling, you should be able to read files of any native type (short,
unsigned short, float, etc.).

The input is expected to be a three dimensional volume.  You can modify the
example code to accept images of 2 (or N) dimensions by changing the
instantiations of filters and images from 3 dimensions to 2 (or N) dimensions
and reading 2 (or N) dimension size parameters.


V. OUTPUT

The output is an estimate of the surface of the input.  The surface is embedded
as the zero level-set of a new image, which is the same size and dimensionality
as the input, but is of floating point type. Negative values indicate that a
pixel is outside of the surface and positive values indicate that that pixel is
inside the surface.  Because the solution is calculated in a very narrow band
around the level-set of interest, only values close to the zero level set have
any meaning other than their sign.  (The output image is a distance transform
from the zero level set, but that distance transform only holds for those pixels
close to the zero level set, where calculations were done.)


VI. HOW TO VISUALIZE THE OUTPUT

The surface can be rendered directly from the output image using a suitable
iso-surface rendering technique such as Marching Cubes.  For example, you could
load the output into a VTK application that uses vtkMarchingCubes 
and the vtkRenderWindow to display the surface.  See the file
ViewOutputVTK.tcl.


VII. TUNING THE PARAMETERS

Because the solution may not converge smoothly to 0.0 RMS Error, you will
probably have to tune your parameters so that the solver does not do
unneccessary iterations or become stuck in an infinite loop.  Most volumes
require only a few iterations (less than 10)  to achieve good anti-aliasing.

There are two parameters that can be adjusted on the solver: MaximumRMSError
and MaximimumIterations.  You can control the halting criteria of the solver
with either one of these parameters, but the application will issue a warning if
MaximumIterations is reached before the MaximumRMSError threshold has been
crossed.

One strategy is to run the application several times with MaximumIterations set
to increasingly higher numbers and MaximumRMSError set very low.  The
application will output the last RMS change calculated, which will give you
some indication of a range of RMS error values that can be used with your
volume.


VIII.  SAMPLE DATA AND PARAMETER FILE

The executable "itkSampleDataGenerator" built in this directory can be used to
create a sample input file for this application.  The sample input is a
64x64x64 image containing a low-resolution binary sphere.  The sample parameter
file "sample.parameter" can be used to process this data.

If you are familiar with Tcl/Tk scripting VTK 4.0 applications, then the file
ViewOutputVTK.tcl may also be of interest to you.  It will render side-by-side
views of the binary input and the floating point output.


