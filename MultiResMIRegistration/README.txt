This example shows how to build a multi-resolution mutual information
registration class using components from the multi-resolution
framework and registration framework.

For more documentation on these framework see:
InsightDocuments/General/Registration.pdf
InsightDocuments/General/MultiResolutionProposal.doc
( a pdf version coming soon ... )

This example defines class MultiResolutionMutualInfromationRegistration
which is a specific instance of MultiResolutionRegistration with
ImageToImageAffineMutualInformationGradientDescentRegistration 
as the internal registrator used at each resolution level.

MulitResolutionImagePyramid is used to create the coarse resolution
images. Simple subsampling is peformed using ShrinkImageFilter.
As this framework develops more sophisticated downsampling stragtey
such as Gaussian, splines and wavelets will be put into place.
