Simple level-set surface modeling example.

I. DESCRIPTION

The sample code in this directory is a simple illustration of how to create
image filters by subclassing level-set solver filters and level set functions.
The filters implemented in these examples are morphing filters.  They morph one
implicitly defined shape into another by moving the zero level set along a
distance transform between the two implicit shapes.


II. HOW TO BUILD THE EXAMPLE

Run CMake in this directory, or make sure that the line

SUBDIRS(SimpleLevelSetsExample)

is included in the Applications/CMakeLists.txt file.


III. COMMAND LINE ARGUMENTS

itk2DSimpleLevelSetsExample needs you to specify only the number of iterations
that the solver should run.  The final output will reflect the position of the
zero level set after that number of iterations.  The square should morph into a
circle within 100 iterations.

itk3DSimpleLevelSetsExample has many more options which specify the initial and
target shapes of the models.

Use:  itk3DSimpleLevelSetsExample number_of_iterations initial_shape target_shape

Shape values are:  1=torus, 2=sphere, 3=two spheres, 4=cylinder, 5=box

Most combinations will give the expected result, but some combinations
do not because the distance transform constructed from the target shape does
not meet the criteria that the simple MorphFilter expects.  The surface  of the
torus, for example, will collapse in upon itself when give a sphere target
because the entire sphere target lies within the hole of the torus.  Adjusting
the sizes of the shapes is necessary in this case.


IV. OUTPUT

The output of each applications (2D and 3D) are images/volumes which represent
the initial image, the distance transform (target image), and the final image.
The initial and final images are floating point representations of the level
sets of the images, where points inside the surface are positive and those
outside are negative.  See the documentation for
itkSparseFieldLevelSetImageFilter for more information.

These output images can be used as input to surface rendering algorithms such
as Marching Cubes.  There are two Tcl/Tk scripts in this directory which can be
used with VTK 4.0 to visualize the results.
