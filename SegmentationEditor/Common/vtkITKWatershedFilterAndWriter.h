/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkITKWatershedFilterAndWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKWatershedFilterAndWriter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
*/
// .NAME vtkITKWatershedFilterAndWriter - Wrapper class around itk::WatershedFilterAndWriterImageFilter
// .SECTION Description
// vtkITKWatershedFilterAndWriter


#ifndef __vtkITKWatershedFilterAndWriter_h
#define __vtkITKWatershedFilterAndWriter_h

#include <string>
#include "vtkITKImageToImageFilterFUL.h"
#include "itkWatershedFilterAndWriter.h"
#include "vtkObjectFactory.h"

class VTK_EXPORT vtkITKWatershedFilterAndWriter : public vtkITKImageToImageFilterFUL
{
 public:
  static vtkITKWatershedFilterAndWriter *New();
  vtkTypeRevisionMacro(vtkITKWatershedFilterAndWriter, vtkITKImageToImageFilterFUL);

  void SetThreshold ( double d ) { DelegateSetMacro ( Threshold, d ); };
  double GetThreshold () { DelegateGetMacro ( Threshold ); };
  void SetLevel ( double d ) { DelegateSetMacro ( Level, d ); };
  double GetLevel () { DelegateGetMacro ( Level ); };

  void SetTreeFileName ( char *s )
  {
    DelegateSetMacro ( TreeFileName, s);
  }
  void SetSegmentationFileName (char *s)
  {
    DelegateSetMacro( SegmentationFileName, s);
  }
  
protected:
  //BTX
  typedef itk::WatershedFilterAndWriter<Superclass::InputImageType> ImageFilterType;
  vtkITKWatershedFilterAndWriter() : Superclass ( ImageFilterType::New() ){};
  ~vtkITKWatershedFilterAndWriter() {};

  //ETX
  
private:
  vtkITKWatershedFilterAndWriter(const vtkITKWatershedFilterAndWriter&);  // Not implemented.
  void operator=(const vtkITKWatershedFilterAndWriter&);  // Not implemented.
};

vtkCxxRevisionMacro(vtkITKWatershedFilterAndWriter, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkITKWatershedFilterAndWriter);

#endif




