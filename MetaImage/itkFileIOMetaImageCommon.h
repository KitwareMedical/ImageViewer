#ifndef __itkFileIOMetaImageCommon_h
#define __itkFileIOMetaImageCommon_h

#include "MetaImageLib.h"
#include <itkFileIOCommon.h>

namespace itk
{

AtomicPixelType ConvertMET_TypeToTAtomicPixel (const MET_Type value);
MET_Type ConvertTAtomicPixelToMET_Type (const AtomicPixelType value);

} // end namespace itk

#endif // __itkFileIOMetaImageCommon_h