#include "itkFileIOMetaImage.h"

namespace itk
{

FileIOMetaImageFactory::FileIOMetaImageFactory()
{
	myProductType::Pointer m_MyProduct = FileIOMetaImage::New();

  RegisterOverride(m_MyProduct->GetSupportedFileExtensions().c_str(),
                   "FileIOMetaImage", "Create FileIOMetaImage", true,
                   CreateObjectFunction<FileIOMetaImage>::New());
}

const char* FileIOMetaImageFactory::GetITKSourceVersion()
{
	return ITK_SOURCE_VERSION;
}

const char* FileIOMetaImageFactory::GetDescription()
{
	return "FileIOMetaImageFactory - TObject factory with registry";
}

const char META_EXT1[] = "mha";
const char META_EXT2[] = "mhd";
const char META_EXT3[] = "mda";

FileIOMetaImage::FileIOMetaImage() : ImageIO()
{
  Reset();
}

FileIOMetaImage::~FileIOMetaImage()
{
}

void FileIOMetaImage::Reset ()
{
  int i;

  ImageIO::Reset();
  m_Modality = MET_MOD_UNKNOWN;
  m_Comment = "";
  m_Date = "";
  for (i = 0; i < MET_IMAGEPOS_SIZE; i++)
  {
    m_ImagePosition[i] = 0.0;
  }
  for (i = 0; i < MET_ORIENTATION_SIZE; i++)
  {
    m_Orientation[i] = 0.0;
  }
  for (i = 0; i < MET_PATIENTPOS_SIZE; i++)
  {
    m_PatientPosition[i] = 0.0;
  }
  for (i = 0; i < MET_SEQUENCEID_SIZE; i++)
  {
    m_SequenceID[i] = 0.0;
  }
  for (i = 0; i < MET_ELEMENTSIZE_SIZE; i++)
  {
    m_ElementSize[i] = 0.0;
  }
  for (i = 0; i < MET_ELEMENTSPACING_SIZE; i++)
  {
    m_ElementSpacing[i] = 0.0;
  }
  m_MetaImage = NULL;
}

void FileIOMetaImage::Load()
{
  ReadHeader();

  m_MetaImage = new MetaImage();
  m_MetaImage->OpenMetaFile(m_FullFileName.c_str());
  Resize(m_NumDimensions, m_Dimensions);
  memmove(m_FileData, m_MetaImage->Get(), m_MetaImage->Quantity()*GetComponentStride());
  CopyAcquisitionParamsToImageIO(m_MetaImage);
  delete m_MetaImage;
  m_MetaImage = NULL;
}

void FileIOMetaImage::Load2D(const std::string fileName)
{
  FileIOMetaImage::Load();
}

void FileIOMetaImage::Load2DSlice(const std::string fileName,
                                  const unsigned int sliceNum,
                                  const unsigned int offset)
{
  ReadHeader();
  m_NumDimensions = 3;
  m_Dimensions[2] = 1;
  Resize(m_NumDimensions, m_Dimensions);

  m_MetaImage = new MetaImage();
  if (fileName == "")
  {
    m_MetaImage->OpenMetaFile(m_FullFileName.c_str());
  }
  else
  {
    m_MetaImage->OpenMetaFile(fileName.c_str());
  }

  memmove(m_FileData, ((char *) m_MetaImage->Get()) + sliceNum*GetSliceStride(),
          GetSliceStride());
  CopyAcquisitionParamsToImageIO(m_MetaImage);
  delete m_MetaImage;
  m_MetaImage = NULL;
}

void FileIOMetaImage::Save(const std::string headerFile, const std::string dataFile)
{
  FileIOMetaImage::Save3D(headerFile, dataFile);
}

void FileIOMetaImage::Save3D(const std::string headerFile, const std::string dataFile)
{
  char* hFile = new char[300];
  char* dFile = NULL;
  char *path = NULL, *name = NULL, *extension = NULL;

  m_MetaImage = new MetaImage(m_NumDimensions, (const int *) m_Dimensions,
                              ConvertAtomicPixelTypeToMET_Type(m_PixelType), NULL, 0,
                              MET_SYSTEM_BYTE_ORDER_MSB, m_FileData);
  CopyAcquisitionParamsToMetaImage(m_MetaImage);

  if ((headerFile == "") && (dataFile == ""))
  { // use m_FullFileName to determine
    extension = ExtractFileExtension(m_FullFileName.c_str());
    name = ExtractFileName(m_FullFileName.c_str());
    path = ExtractFilePath(m_FullFileName.c_str());
    if (Strucmp(extension, META_EXT1) == 0)
    { // combined header/data file
      sprintf(hFile, "%s%s.%s", path, name, META_EXT1);
      dFile = NULL;
    }
    else
    { // separate header & data files
      sprintf(hFile, "%s%s.%s", path, name, META_EXT2);
      dFile = new char[300];
      sprintf(dFile, "%s.%s", name, META_EXT3);
    }
  }
  else
  { // use parameters to determine
    path = ExtractFilePath(headerFile.c_str());
    if (path == NULL)
    {
      name = ExtractFileName(headerFile.c_str());
      extension = ExtractFileExtension(headerFile.c_str());
    }
    else
    {
      name = ExtractFileName(headerFile.c_str()+strlen(path));
      extension = ExtractFileExtension(headerFile.c_str()+strlen(path));
    }
    if ((extension == NULL) || (Strucmp(extension, META_EXT1) == 0))
    {
      if (path == NULL)
      {
        sprintf(hFile, "%s.%s", name, META_EXT1);
      }
      else
      {
        sprintf(hFile, "%s%s.%s", path, name, META_EXT1);
      }
      dFile = NULL;
    }
    else {
      if (path == NULL)
      {
        sprintf(hFile, "%s.%s", name, META_EXT2);
      }
      else
      {
        sprintf(hFile, "%s%s.%s", path, name, META_EXT2);
      }
      dFile = new char[300];
      if (path == NULL)
      {
        sprintf(dFile, "%s.%s", name, META_EXT3);
      }
      else
      {
        sprintf(dFile, "%s%s.%s", path, name, META_EXT3);
      }
    }
  }

  m_MetaImage->Save(hFile, dFile);
  delete hFile;
  delete dFile;
  delete path;
  delete name;
  delete extension;
  delete m_MetaImage;
}

void FileIOMetaImage::ReadHeader(const std::string fileName)
{
  m_MetaImage = new MetaImage();
  if (fileName == "")
  {
    m_MetaImage->OpenMetaFile(m_FullFileName.c_str(), false);
  }
  else
  {
    m_MetaImage->OpenMetaFile(fileName.c_str(), false);
  }

  SetNumDimensions(m_MetaImage->NDims());
  for (unsigned int i = 0; i < m_NumDimensions; i++)
  {
    m_Dimensions[i] = m_MetaImage->DimSize(i);
  }
  SetComponentsPerPixel(1);
  m_PixelType = ConvertMET_TypeToAtomicPixelType(m_MetaImage->ElemType());
  CopyAcquisitionParamsToImageIO(m_MetaImage);
  CalcStrides();

  m_MetaImage->ReadFileClose();
  delete m_MetaImage;
  m_MetaImage = NULL;
}

std::string FileIOMetaImage::GetSupportedFileExtensions() const
{
//  return "mhd";
  return "mha";
}

void FileIOMetaImage::Print(std::ostream& os)
{
  ImageIO::Print(os);
}

void FileIOMetaImage::CopyAcquisitionParamsToImageIO (MetaImage* fromHere)
{
  SetComment(fromHere->Comment());
  SetDate(fromHere->Date());
  SetModality(fromHere->Modality());
  SetOrientation(fromHere->Orientation());
  SetImagePosition(fromHere->Position());
  SetPatientPosition(fromHere->PatPosition());
  SetSequenceID(fromHere->SequenceID());
  SetElementSize(fromHere->ElemSize());
  SetElementSpacing(fromHere->ElemSpacing());
}

void FileIOMetaImage::CopyAcquisitionParamsToMetaImage (MetaImage* toHere)
{
  toHere->Comment(GetComment());
  toHere->Date(GetDate());
  toHere->Modality(GetModality());
  toHere->Orientation(GetOrientation());
  toHere->PatPosition(GetPatientPosition());
  toHere->SequenceID(GetSequenceID());
  toHere->ElemSize(GetElementSize());
  toHere->ElemSpacing(GetElementSpacing());
}

AtomicPixelType FileIOMetaImage::ConvertMET_TypeToAtomicPixelType (const MET_Type value) const
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

MET_Type FileIOMetaImage::ConvertAtomicPixelTypeToMET_Type (const AtomicPixelType value) const
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

void FileIOMetaImage::SetModality(const MET_ModType newValue)
{
  m_Modality = newValue;
}

MET_ModType FileIOMetaImage::GetModality() const
{
  return m_Modality;
}

void FileIOMetaImage::SetOrientation(const float* newValue)
{
  int i;

  for (i = 0; i < MET_ORIENTATION_SIZE; i++)
  {
    if (newValue == NULL)
    {
      m_Orientation[i] = 0.0;
    }
    else
    {
      m_Orientation[i] = newValue[i];
    }
  }
}

float* FileIOMetaImage::GetOrientation() const
{
  return (float*) m_Orientation;
}

void FileIOMetaImage::SetImagePosition(const float* newValue)
{
  int i;

  for (i = 0; i < MET_IMAGEPOS_SIZE; i++)
  {
    if (newValue == NULL)
    {
      m_ImagePosition[i] = 0.0;
    }
    else
    {
      m_ImagePosition[i] = newValue[i];
    }
  }
}

float* FileIOMetaImage::GetImagePosition() const
{
  return (float*) m_ImagePosition;
}

void FileIOMetaImage::SetPatientPosition(const float* newValue)
{
  int i;

  for (i = 0; i < MET_PATIENTPOS_SIZE; i++)
  {
    if (newValue == NULL)
    {
      m_PatientPosition[i] = 0.0;
    }
    else
    {
      m_PatientPosition[i] = newValue[i];
    }
  }
}

float* FileIOMetaImage::GetPatientPosition() const
{
  return (float*) m_PatientPosition;
}

void FileIOMetaImage::SetSequenceID(const float* newValue)
{
  int i;

  for (i = 0; i < MET_SEQUENCEID_SIZE; i++)
  {
    if (newValue == NULL)
    {
      m_SequenceID[i] = 0.0;
    }
    else
    {
      m_SequenceID[i] = newValue[i];
    }
  }
}

float* FileIOMetaImage::GetSequenceID() const
{
  return (float*) m_SequenceID;
}

void FileIOMetaImage::SetElementSize(const float* newValue)
{
  int i;

  for (i = 0; i < MET_ELEMENTSIZE_SIZE; i++)
  {
    if (newValue == NULL)
    {
      m_ElementSize[i] = 0.0;
    }
    else
    {
      m_ElementSize[i] = newValue[i];
    }
  }
}

float* FileIOMetaImage::GetElementSize() const
{
  return (float*) m_ElementSize;
}

void FileIOMetaImage::SetElementSpacing(const float* newValue)
{
  int i;

  for (i = 0; i < MET_ELEMENTSPACING_SIZE; i++)
  {
    if (newValue == NULL)
    {
      m_ElementSpacing[i] = 0.0;
    }
    else
    {
      m_ElementSpacing[i] = newValue[i];
    }
  }
}

float* FileIOMetaImage::GetElementSpacing() const
{
  return (float*) m_ElementSpacing;
}

} // namespace itk
