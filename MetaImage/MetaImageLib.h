#ifndef METAIMAGE_H
#define METAIMAGE_H

#include "MetaUtils.h"
#include "MetaFileLib.h"
#include "itkFstream.h"

/*!
 *   Just a reminder of the dimension ordering in memory
 *       - metaImageFiles use 1D arrays to represent ND images (innermost loop x, then y, ...)
 */
#define MET_DIM_X                   0
#define MET_DIM_Y                   1
#define MET_DIM_Z                   2
#define MET_DIM_S                   3


typedef enum         {MET_MOD_UNKNOWN,
                      MET_MOD_MRI,
                      MET_MOD_MRA,
                      MET_MOD_CT,
                      MET_MOD_DSA,
                      MET_MOD_CXR,
                      MET_MOD_DRR} MET_ModType;

/*!
 *    Given a string, convert it to the corresponding enumerated MET_ModType
 *    Helps with reading in the Mod type from a text file and convering it to a usable variable
 */
extern MET_ModType   MET_StringToMod(const char *_s);

/*!
 *
 *    Converts a Mod type to a corresponding string (for writing to a text file, for example).
 */
extern char *        MET_ModToString(MET_ModType _mType);


/*!    MetaImageLib (.h and .cpp)
 *
 * Description:
 *    Reads and Writes MetaImageFiles.
 *    MetaImageFiles can be in one of two possible formats:
 *       a combined header/data format, typically designated .mha files
 *       or as separate header and data files, typically designated .mhd and .mda files
 *
 * Features:
 *    Header information is in ascii format - for easy creation, editing, and review.
 *    If separate files are used, a specified number of header-bytes in the datafile can be skipped
 *       - in this manner, different file formats (e.g., bmp, img, and /usr/Image) can 
 *          be converted to metaImageFile format by merely specifying a separate text header
 *          (.mhd) file and in that file specifying how many data-header-bytes should be skipped.
 *          Thereby the data files can serve a dual role (as bitmap files and as metaImage data files)
 *    Supports all pixel formats
 *    Handles byte ordering (MSB/LSB)
 *    Provides the ability to handle associated medical image information (element size, element spacing, etc).
 *    Has required and optional header data (provides rapid formation or extensive documentation).
 *       REQUIRED: NDims, DimSize, ByteOrderMSB, DataFileName
 *
 * \author Stephen R. Aylward
 * 
 * \date August 29, 1999
 * 
 * Depends on:
 *    MetaUtils.h
 *    MetaFileLib.h
 */
class MetaImage
  {
   ////
   //
   // PROTECTED
   //
   ////
  protected:

      char           errorMessage[800];

      // Image Variables
      char           iDataFileName[800];
      char           iDataFilePath[800];
      bool           iFreeEData;

      bool           iReadFileOpen;
      itk::Ifstream  iReadFilePointer;
      bool           iWriteFileOpen;
      itk::Ofstream  iWriteFilePointer;

      //
      int           iNDims;

      int            iDimSize[10];
      int           iQuantity;
      int            iSubQuantity[10];


      // Image Element Variables
      MET_Type      eType;
      int           eNBytes;

      void *       eData;

      // Optional Variables
      int            iHeaderSize;
      unsigned char *iHeader;
      bool           iFreeHeader;

      char           iComment[800];

      char           iDate[800];

      MET_ModType    iModality;

      float          iPosition[10];

      float          iOrientation[10];

      float          pPosition[10];

      float          seqID[4];

      int           eNBits;

      bool         eByteOrderMSB;

      float          eMin, eMax;

      float       eSize[10];

      float          eSpacing[10];

      // Constructor Common Code
      void           InitMetaMem();
      void           FreeMetaMem();

      void           InitMetaImage(int _iNDims, const int * _iDimSize, MET_Type _eType, const float *_eSize=NULL, int _eNBits=0, bool _eByteOrderMSB=MET_SYSTEM_BYTE_ORDER_MSB, const void *_eData=NULL);

   /////
   //
   // PUBLIC
   //
   ////
  public:

      ////
      //
      // Constructors & Destructor
      //
      ////
      MetaImage();
      MetaImage(const char *_fname, bool _read_and_close=true);   // Read the file and possibly keep open - if kept open, pixel data not read
      MetaImage(MetaImage *_im);                                  // Duplicate structure but not data of another metaImage
      MetaImage(int _iNDims, const int * _iDimSize, MET_Type _eType, const float *_eSize=NULL, int _eNBits=0, bool _eByteOrderMSB=MET_SYSTEM_BYTE_ORDER_MSB, const void *_eData=NULL);
      MetaImage(int _x, int _y, MET_Type _eType, const float *_eSize=NULL, int _eNBits=0, bool _eByteOrderMSB=MET_SYSTEM_BYTE_ORDER_MSB, const void *_eData=NULL);
      MetaImage(int _x, int _y, int _z, MET_Type _eType, const float *_eSize=NULL, int _eNBits=0, bool _eByteOrderMSB=MET_SYSTEM_BYTE_ORDER_MSB, const void *_eData=NULL);

      ~MetaImage();


      ////
      //
      // Utility Code
      //
      ////

      //    Error()
      //       Retrievable error messages
      //       Null if no errors
      char *         Error();                                                 

      //    PrintMetaInfo()
      //       Writes image parameters to stdout
      void           PrintMetaInfo();
      

      ////
      //
      // Read a file
      //
      ////

      //    OpenMetaFile(...)
      //       Reusable open file
      bool           OpenMetaFile(const char *_fname, bool _read_and_close=true);       

      //    FieldExists(...)
      //       Returns true if the filed is defined within the metaFile
    //       Only available if the file was opened with _read_and_close=false
      bool           FieldExists(const char *_fieldName);

      //    GetFieldValue(...)
      //       Returns the value stored in a particular field
      //       Enables the reading of non-pre-define fields (see below)
    //       Only available if the file was opened with _read_and_close=false
      void *        GetFieldValue(const char *_fieldName, MF_ValType _fieldType, int _nDims=-1);

      //    ReadImageData()
      //       Reads the pixel data of a MetaImage.
      //       Only necessary to call this function is file was opened with _read_and_close=false
      bool         ReadImageData();

      //    ReadFileClose()
      //       Closes a MetaImage file.
      //       Only necessary to call this function is file was opened with _read_and_close=false
      bool         ReadFileClose();


      ////
      //
      // Pre-defined MetaImage Fields - will be read and stored by default
      //
      ////

      //    Comment(...)
      //       Optional Field
      //       Arbitrary string
      char *         Comment(const char *_c=NULL);

      //    HeaderSize(...)
      //       Optional Field
      //       Number of header-bytes to skip at the begining of the data file
      int            HeaderSize(int _iHeaderSize=-1);

      //    Header(...)
      //       Optional Field
      //       Data read from or to be placed at the begining of the data file
      unsigned char *Header(const unsigned char *_iHeader=NULL);

      //    Date(...)
      //       Optional Field
      //       Date image was created/recorded/converted
      char *         Date(const char *_d=NULL);

      //    Modality(...)
      //       Optional Field
      //       Modality type of the image (see enum type define at top of this file)
      MET_ModType    Modality(MET_ModType _mType=MET_MOD_UNKNOWN);

      //    NDims()
      //       REQUIRED Field
      //       Number of dimensions to the image
      int           NDims();

      //    DimSize(...)
      //       REQUIRED Field
      //       Number of elements along each dimension
      int *           DimSize();           // returns array of size of each dim - do not create or delete mem; use empty ptr
      int            DimSize(int i);      // returns size of dim (0 for x; 1 for y; ...)

      //    Quantity()
      //       Not a field in file
      //       Total number of elements in image (Prod(dimSize[i]))
      int           Quantity();

      //    SubQuantity(...)
      //       Not a field in file
      //       Number of elements in image spanning sub-dimensions
      //       E.g., elements per line, 2D sub-image, 3D sub-volume,
      int *          SubQuantity();       // returns array of number of elements in each sub-dimension of the image (0=linesize = dimSize[0])
      int            SubQuantity(int i);  // returns number of elements in a particular sub-dimension (e.g., 1-elements per 2D sub-image = dimSize[0]*dimSize[1])

      //    Position(...)
      //       Optional Field
      //       Physical location (in millimeters and wrt machine coordinate system or the patient) of the first element in the image
      float *        Position(const float *_position=NULL);
      float          Position(int i);

      //    Orientation(...)
      //       Optional Field
      //       Physical orientation of the image (up and left directions as a 6 dim array; 2 3D vectors)
      float *        Orientation(const float *_orientation=NULL);
      float          Orientation(int i);

      //    PatPosition(...)
      //       Optional Field
      //       Position of the patient in 3D space relative to machine coordinate system
      float *        PatPosition(const float *_pPosition=NULL);
      float          PatPosition(int i);

      //    SequenceID(...)
      //       Optional Field
      //       DICOM designation of this image relative to other images acquired at the same time
      float *       SequenceID(const float *_seqID=NULL);
      float          SequenceID(int i);

      //    ElemSize(...)
      //       Optional Field
      //       Physical size (in MM) of each element in the image (0 = xSize, 1 = ySize, 2 = zSize)
      float *       ElemSize(const float *_eSize=NULL);
      float          ElemSize(int i);

      //    ElemSpacing(...)
      //       Optional Field
      //       Physical space (in MM) between elements (1mm can mean there is size+1mm space between or just simply 1mm between - DICOM abiguity, sorry
      float *        ElemSpacing(const float *_eSpacing=NULL);
      float          ElemSpacing(int i); 

      //    ElemType(...)
      //       Optional Field
      //       The type (e.g., MET_USHORT, MET_CHAR, MET_FLOAT) of the elements' data
      //       Enumerated type is defined in MetaUtils.h
      MET_Type      ElemType(MET_Type _eType=MET_DEFAULT);

      //    ElemNBytes()
      //       Optional Field
      //       The number of bytes per element (defined as sizeof(ElemType))
      int           ElemNBytes();

      //    ElemNBits(...)
      //       Optional Field
      //       The number of bits used per element (may be less than 8*ElemNBytes, e.g., 12 bit images stored using 2 bytes per element)
      int           ElemNBits(int _eBits=0);

      //    ElemByteOrderMSB()
      //       REQUIRED Field
      //       returns whether byte order is Sun(MSB=TRUE) or IBM PC compatible (MSB=FALSE)
      bool           ElemByteOrderMSB();
      
      //    ElemByteSwapperRequired()
      //       Not a Field in a MetaImageFile
      //       returns whether byte swap is needed to convert to local machine's byte ordering
      bool           ElemByteSwapperRequired();


      //    ElemMakeByteOrderMSB(), ElemMakeByteOrderLSB(), ElemByteOrderSwap(), ElemByteOrderFix()
      //       The following functions are available only after ReadImageData() or if _read_and_close=TRUE when read
      bool          ElemMakeByteOrderMSB();             // Make image data bytes MSB
      bool          ElemMakeByteOrderLSB();             // Make image data bytes LSB
      void           ElemByteOrderSwap();                // Swap image data bytes
      bool           ElemByteOrderFix();                 // Match local system's byte order

      //    Min(...) Max(...)
      //       The default max returned is the largest allowed by ElemNBytes (12 bit uint16_t will give 4096 max).
      //       This may not represent the true max.   Use _reCalc=true to force a calcuation of the actual max element value.
      float          Min(float _eMin=-99, bool _reCalc=false);    // get or set min
      float          Max(float _eMax=-99, bool _reCalc=false);    // get or set max

      //    ConverTo(...)
      //       Converts to a new data type
      //       Rescales using Min and Max (see above)
      bool           ConvertTo(MET_Type _eType=MET_UCHAR);


      ////
      //
      //    Really slow element access
      //
      ////
    void *         operator()();                       // returns pointer to image data
      float       operator()(int _x);                 // returns value of a particular element
    float       operator()(int _x, int _y);
    float       operator()(int _x, int _y, int _z);
    float       operator()(const int * _x);

      void *         Get();                              // returns pointer to image data
    float       Get(int _x);                        // returns value of a particular element
    float       Get(int _x, int _y);
    float       Get(int _x, int _y, int _z);
    float       Get(const int * _x);

      bool           Set(float _v);                      // replaces all element data with this value
      bool           Set(int _x, float _v);              // replace individual element's data
      bool           Set(int _x, int _y, float _v);
      bool           Set(int _x, int _y, int _z, float _v);
      bool           Set(const int *_x, float _v);


    ////
      //
      // Write a MetaImage file
      //
      ////
      
      //    Save(...)
      //       Saves info of pre-defined fields and image data
      //       If only one filename is given, a single (combined header and data) file is written, otherwise separate files are written
      //       If _write_and_close = false, non-standard (not pre-defined) fields can be written.
      bool         Save(const char *_headFname, const char *_dataFname=NULL, bool _write_and_close=true);

      //    SetFieldValue(...)
      //       Only available if _write_and_close=false
      //       Enables the writing of non-pre-defined fields.
      //       See MetaFileLib.h for definition of MF_ValueType - i.e., field value types (e.g., MF_CHAR_ARRAY)
      bool           SetFieldValue(const char *_fieldName, MF_ValType _pType, float _v);
      bool           SetFieldValue(const char *_fieldName, MF_ValType _pType, int _n, const void *_v);

      //    WriteFileClose()
      //       Must be called if Save with _write_and_close=false was called - forces element data to be written
      bool           WriteFileClose();

  };


#endif
