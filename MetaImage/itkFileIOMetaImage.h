#ifndef __itkFileIOMetaImage_h
#define __itkFileIOMetaImage_h

#include "itkFileIO.h"
#include "MetaImageLib.h"
#include "itkFileIOMetaImageCommon.h"
#include <itkVersion.h>
#include <string>

namespace itk
{

class ITK_EXPORT FileIOMetaImage : public FileIO
{
public:
  /**
   * Smart pointer typedef support.
   */
  typedef FileIOMetaImage Self;
  typedef SmartPointer<Self>  Pointer;

  /**
   * Run-time type information (and related methods).
   */
  itkTypeMacro(FileIOMetaImage, FileIO);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Default load; do whatever is appropriate for the filetype.
   */
  virtual void Load ();

  /**
   * Load a 2D image. If fileName="" (the default), will read from m_FileName
   */
  virtual void Load2D(const std::string fileName="");

  /**
   * Load a 2D slice from a volume dataset.
   * fileName is file to read from. default="", which uses m_FileName instead
   * sliceNum is the slice # to load (starting at 0). default = 0.
   * offset is the offset, in bytes, into fileData at which the data should be loaded
   *  default = 0
   */
  virtual void Load2DSlice(const std::string fileName="",
                           const unsigned int sliceNum=0,
                           const unsigned int offset=0);

  /**
   * Default save; do whatever is appropriate for MetaImages
   * Since MetaImages can be saved as a header/data file pair, we need to be able to pass
   * in these strings as parameters. Most other file formats will simply use m_FullFileName
   */
  virtual void Save(const std::string headerFile="", const std::string dataFile="");

  /**
   * Save a 3D image
   */
  virtual void Save3D(const std::string headerFile="", const std::string dataFile="");

  /**
   * Read a file's header to determine image dimensions, etc.
   */
  virtual void ReadHeader (const std::string fileName="");

  /**
   * Returns the file extension that a particular FileIO subclass
   * is capable of handling (e.g. .jpg, .mhd, etc.)
   * Currently only a single string is returned, but can be modified
   * so that a whole list of strings is returned.
   */
  virtual std::string GetSupportedFileExtensions () const;

  /**
   * Specify the comment string
   */
  itkSetStringMacro(Comment);

  /**
   * Get the comment string
   */
  itkGetStringMacro(Comment);

  /**
   * Specify the date string
   */
  itkSetStringMacro(Date);

  /**
   * Get the date string
   */
  itkGetStringMacro(Date);

  /**
   * Specify the modality
   */
  void SetModality(const MET_ModType newValue);

  /**
   * Get the modality
   */
  MET_ModType GetModality() const;

  /**
   * Specify the image position
   */
  void SetImagePosition(const float* newValue);

  /**
   * Get the image position
   */
  float* GetImagePosition() const;

  /**
   * Specify the patient orientation
   */
  void SetOrientation(const float* newValue);

  /**
   * Get the patient orientation
   */
  float* GetOrientation() const;

  /**
   * Specify the patient position
   */
  void SetPatientPosition(const float* newValue);

  /**
   * Get the patient position
   */
  float* GetPatientPosition() const;

  /**
   * Specify the sequence ID
   */
  void SetSequenceID(const float* newValue);

  /**
   * Get the sequence ID
   */
  float* GetSequenceID() const;

  /**
   * Specify the element size
   */
  void SetElementSize(const float* newValue);

  /**
   * Get the element size
   */
  float* GetElementSize() const;

  /**
   * Specify the element spacing
   */
  void SetElementSpacing(const float* newValue);

  /**
   * Get the element spacing
   */
  float* GetElementSpacing() const;

  /**
   * Print information about myself
   */
  void Print(std::ostream& os);

protected:
  std::string m_Comment;
  std::string m_Date;
  MET_ModType m_Modality;
  float m_ImagePosition[MET_IMAGEPOS_SIZE];
  float m_Orientation[MET_ORIENTATION_SIZE];
  float m_PatientPosition[MET_PATIENTPOS_SIZE];
  float m_SequenceID[MET_SEQUENCEID_SIZE];
  float m_ElementSize[MET_ELEMENTSIZE_SIZE];
  float m_ElementSpacing[MET_ELEMENTSPACING_SIZE];

  MetaImage* m_MetaImage;
  AtomicPixelType ConvertMET_TypeToAtomicPixelType (const MET_Type value) const;
  MET_Type ConvertAtomicPixelTypeToMET_Type (const AtomicPixelType value) const;
  void CopyAcquisitionParamsToFileIO (MetaImage* fromHere);
  void CopyAcquisitionParamsToMetaImage (MetaImage* toHere);
  virtual void Reset();

  /**
   * Default constructor
   */
  FileIOMetaImage();

  /**
   * Destructor
   */
  ~FileIOMetaImage();

private:
};

class ITK_EXPORT FileIOMetaImageFactory : public ObjectFactoryBase
{
public:
  FileIOMetaImageFactory();
  const char* GetITKSourceVersion();
  const char* GetDescription();


protected:
  typedef FileIOMetaImage myProductType;

  const myProductType* m_MyProduct;

};

} // namespace itk

#endif
