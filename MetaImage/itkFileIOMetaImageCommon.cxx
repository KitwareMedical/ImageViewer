#include "itkFileIOMetaImageCommon.h"

namespace itk
{

AtomicPixelType ConvertMET_TypeToTAtomicPixel (const MET_Type value)
{
	AtomicPixelType result;

	switch (value)
	{
		case MET_CHAR:
			result = ITK_CHAR;
			break;
		case MET_UCHAR:
			result = ITK_UCHAR;
			break;
		case MET_SHORT:
			result = ITK_SHORT;
			break;
		case MET_USHORT:
			result = ITK_USHORT;
			break;
		case MET_INT:
			result = ITK_INT;
			break;
		case MET_UINT:
			result = ITK_UINT;
			break;
		case MET_FLOAT:
			result = ITK_FLOAT;
			break;
		case MET_DOUBLE:
			result = ITK_DOUBLE;
			break;
		default:
			result = ITK_DOUBLE;
			break;
	}
	return result;
}

MET_Type ConvertAtomicPixelTypeToMET_Type (const AtomicPixelType value)
{
	MET_Type result;

	switch (value)
	{
		case ITK_CHAR:
			result = MET_CHAR;
			break;
		case ITK_UCHAR:
			result = MET_UCHAR;
			break;
		case ITK_SHORT:
			result = MET_SHORT;
			break;
		case ITK_USHORT:
			result = MET_USHORT;
			break;
		case ITK_INT:
			result = MET_INT;
			break;
		case ITK_UINT:
			result = MET_UINT;
			break;
		case ITK_FLOAT:
			result = MET_FLOAT;
			break;
		case ITK_DOUBLE:
			result = MET_DOUBLE;
			break;
		default:
			result = MET_DOUBLE;
			break;
	}
	return result;
}

} // end namespace itk