/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkImageStatistics.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkImageStatistics.h"
#include "vtkObjectFactory.h"



//------------------------------------------------------------------------------
vtkImageStatistics* vtkImageStatistics::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkImageStatistics");
  if(ret)
    {
    return (vtkImageStatistics*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkImageStatistics;
}

//----------------------------------------------------------------------------
vtkImageStatistics::vtkImageStatistics()
{
  this->MinValue  = 0.0;
  this->MaxValue  = 0.0;
  this->MeanValue = 0.0;
  this->StandardDeviationValue = 0.0;
}

//----------------------------------------------------------------------------
// Description:
void vtkImageStatistics::ComputeInputUpdateExtent(int inExt[6], 
                                                  int outExt[6])
{
  int *wholeExtent;

  outExt = outExt;
  wholeExtent = this->GetInput()->GetWholeExtent();
  memcpy(inExt, wholeExtent, 6*sizeof(int));
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class IT, class OT>
static void vtkImageStatisticsExecute(vtkImageStatistics *self,
                                      vtkImageData *inData, IT *inPtr,
                                      vtkImageData *outData, OT *outPtr)
{
  IT max, min, *inptr1;
  int idxR, idxY, idxZ;
  int maxY, maxZ;
  int inIncX, inIncY, inIncZ;
  int outIncX, outIncY, outIncZ;
  int outExt[6],id;
  int rowLength;
  unsigned long count = 0;
  unsigned long target;
  double mean = 0.0;
  double sumSqr = 0.0;
  double variance;;

  id = 1;
  outData->GetExtent(outExt);
  max = static_cast<IT>(inData->GetScalarTypeMin());
  min = static_cast<IT>(inData->GetScalarTypeMax());

  // find the region to loop over
  rowLength = (outExt[1] - outExt[0]+1)*inData->GetNumberOfScalarComponents();
  maxY = outExt[3] - outExt[2]; 
  maxZ = outExt[5] - outExt[4];
  target = (unsigned long)((maxZ+1)*(maxY+1)/50.0);
  target++;
  
  // Get increments to march through
  inData->GetContinuousIncrements(outExt, inIncX, inIncY, inIncZ);
  outData->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);

  // find max & min first
  // Loop through ouput pixels
  inptr1 = inPtr;
  for (idxZ = 0; idxZ <= maxZ; idxZ++)
    {
    for (idxY = 0; !self->AbortExecute && idxY <= maxY; idxY++)
      {
      if (!id) 
        {
        if (!(count%target))
          {
    self->UpdateProgress(count/(50.0*target));
    }
  count++;
  }
      for (idxR = 0; idxR < rowLength; idxR++)
  {
        // Pixel operation
        if ( *inptr1 > max )
          {
          max = *inptr1;
          }
        if ( *inptr1 < min )
          {
          min = *inptr1;
          }
        mean += (double)(*inptr1);
        sumSqr += ((double)(*inptr1) * (double)(*inptr1));
        inptr1++;
  }
      inptr1 += inIncY;
      }
    inptr1 += inIncZ;
    }

  count = maxZ*maxY*rowLength;
  mean /= (double) count;
  variance = sumSqr / (double) (count - 1) - ((double) count * mean * mean / (double) (count - 1));
  
  self->MinValue = min;
  self->MaxValue = max;
  self->MeanValue = mean;
  self->StandardDeviationValue = sqrt(variance);
}



//----------------------------------------------------------------------------
// This method is passed a input and output region, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the regions data types.
void vtkImageStatistics::ExecuteData(vtkDataObject *vtkNotUsed(out))
{ 
  vtkImageData *outData = this->GetOutput();
  vtkImageData *inData = (vtkImageData *) this->GetInput();
  void *inPtr = inData->GetScalarPointerForExtent(inData->GetUpdateExtent());
  void *outPtr;

  vtkDebugMacro(<< "ExecuteData: inData = " << inData 
                << ", outData = " << outData);
  
  outData->AllocateScalars();
  outData->SetExtent(this->GetOutput()->GetWholeExtent());
  outPtr = outData->GetScalarPointer();

  // this filter expects that input is the same type as output.
  if (inData->GetScalarType() != outData->GetScalarType())
    {
    vtkErrorMacro(<< "Execute: input ScalarType, " << inData->GetScalarType()
                  << ", must match out ScalarType " << outData->GetScalarType());
    return;
    }
  
  switch (inData->GetScalarType())
    {
    case VTK_DOUBLE:
      vtkImageStatisticsExecute(this, 
                                inData, (double *)(inPtr), 
                                outData, (double *)(outPtr));
      break;
    case VTK_FLOAT:
      vtkImageStatisticsExecute(this, 
                                inData, (float *)(inPtr), 
                                outData, (float *)(outPtr));
      break;
    case VTK_LONG:
      vtkImageStatisticsExecute(this, 
                                inData, (long *)(inPtr), 
                                outData, (long *)(outPtr));
      break;
    case VTK_UNSIGNED_LONG:
      vtkImageStatisticsExecute(this, 
                                inData, (unsigned long *)(inPtr), 
                                outData, (unsigned long *)(outPtr));
      break;
    case VTK_INT:
      vtkImageStatisticsExecute(this, 
                                inData, (int *)(inPtr), 
                                outData, (int *)(outPtr));
      break;
    case VTK_UNSIGNED_INT:
      vtkImageStatisticsExecute(this, 
                                inData, (unsigned int *)(inPtr), 
                                outData, (unsigned int *)(outPtr));
      break;
    case VTK_SHORT:
      vtkImageStatisticsExecute(this, 
                                inData, (short *)(inPtr), 
                                outData, (short *)(outPtr));
      break;
    case VTK_UNSIGNED_SHORT:
      vtkImageStatisticsExecute(this, 
                                inData, (unsigned short *)(inPtr), 
                                outData, (unsigned short *)(outPtr));
      break;
    case VTK_CHAR:
      vtkImageStatisticsExecute(this, 
                                inData, (char *)(inPtr), 
                                outData, (char *)(outPtr));
      break;
    case VTK_UNSIGNED_CHAR:
      vtkImageStatisticsExecute(this, 
                                inData, (unsigned char *)(inPtr), 
                                outData, (unsigned char *)(outPtr));
      break;
    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}

//----------------------------------------------------------------------------
// Description:
// PrintSelf function ...
void vtkImageStatistics::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageToImageFilter::PrintSelf(os,indent);
  vtkImageToImageFilter::PrintSelf(os,indent);

  os << indent << "Min Value: " << this->MinValue << "\n";
  os << indent << "Max Value: " << this->MaxValue << "\n";
  os << indent << "Mean Value: " << this->MeanValue << "\n";
  os << indent << "Standard Deviation Value: " << this->StandardDeviationValue << "\n";
}
