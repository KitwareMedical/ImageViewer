#ifndef __itkFileIOMetaImageCommon_h
#define __itkFileIOMetaImageCommon_h

#include "MetaImageLib.h"
#include <itkFileIOCommon.h>

namespace itk
{

const short int MET_IMAGEPOS_SIZE = 10;
const short int MET_ORIENTATION_SIZE = 10;
const short int MET_PATIENTPOS_SIZE = 10;
const short int MET_SEQUENCEID_SIZE = 4;
const short int MET_ELEMENTSIZE_SIZE = 10;
const short int MET_ELEMENTSPACING_SIZE = 10;

AtomicPixelType ConvertMET_TypeToTAtomicPixel (const MET_Type value);
MET_Type ConvertTAtomicPixelToMET_Type (const AtomicPixelType value);

} // end namespace itk

#endif // __itkFileIOMetaImageCommon_h
