
 IBSR Classification:
 ==================

 This directory contains source code, input parameter files and ouput files
 for the gray and white matter segmentation of the IBSR T1 brain data validation study.

 The study evaluates the effectivenessof the ITK implementation of the
 image classification algorithm for the application of gray, white matter 
 the whole brain T1 volume from MR images. The cerebrospinal fluid is
 also classified. However, the algorithms are not suitable for CSF
 classification with just a single channel T1 brain data. However,
 if T2/PD data is available the CSFdiscrimination significantly improves.

 The accompanying validation report is yet to be generated; once completed
 they will be posted at 
 InsightDocuments/Validation/IBSRClassification/IBSRClassificationStudy.pdf

 Code:
 =====
 
 This sub-directory contains the source code to build the 
 IBSRClassificationApp application used for the study described
 above.

 Inputs:
 =======

 This sub-directory contains the input parameters files used to
 produce the results described in the report above. 


 Results:
 ========

 This sub-directory contains the output files produced as part
 of the validation study described in the report above.

