#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#include "MetaUtils.h"
#include "MetaImageLib.h"

//
// Modality Type
//

MET_ModType MET_StringToMod(const char *_s)
   {
   if(!strcmp(_s, "MET_MOD_UNKNOWN"))
		return MET_MOD_UNKNOWN;

  	if(!strcmp(_s, "MET_MOD_MRI"))
		return MET_MOD_MRI;

  	if(!strcmp(_s, "MET_MOD_MRA"))
		return MET_MOD_MRA;

   if(!strcmp(_s, "MET_MOD_CT"))
		return MET_MOD_CT;
	
   if(!strcmp(_s, "MET_MOD_DSA"))
		return MET_MOD_DSA;
	
   if(!strcmp(_s, "MET_MOD_CXR"))
		return MET_MOD_CXR;
	
   if(!strcmp(_s, "MET_MOD_DRR"))
		return MET_MOD_DRR;
	
   MET_Error = true;
   return MET_MOD_UNKNOWN;
   }

char * MET_ModToString(MET_ModType _mType)
   {
   static char s[80];

   switch(_mType)
      {
      default:
         MET_Error = true;
      case MET_MOD_UNKNOWN:
         sprintf(s, "MET_MOD_UNKNOWN");
         break;
      case MET_MOD_MRI:
         sprintf(s, "MET_MOD_MRI");
         break;
      case MET_MOD_MRA:
         sprintf(s, "MET_MOD_MRA");
         break;
      case MET_MOD_CT:
         sprintf(s, "MET_MOD_CT");
         break;
      case MET_MOD_DSA:
         sprintf(s, "MET_MOD_DSA");
         break;
      case MET_MOD_CXR:
         sprintf(s, "MET_MOD_CXR");
         break;
      case MET_MOD_DRR:
         sprintf(s, "MET_MOD_DRCXR");
         break;
      }

   return s;
   }


//
// MedImage Constructors
//
MetaImage::
MetaImage()
  {
  InitMetaMem();
  }

//
MetaImage::
MetaImage(const char *_name, bool _read_and_close)
	{
   InitMetaMem();
   OpenMetaFile(_name, _read_and_close);
	}

//
MetaImage::
MetaImage(MetaImage *_im)
   {
   InitMetaImage(_im->NDims(), _im->DimSize(), _im->ElemType(), _im->ElemSize(), _im->ElemNBits(), MET_SYSTEM_BYTE_ORDER_MSB, NULL);
   }

//
MetaImage::
MetaImage(int _iNDims, const int * _iDimSize, MET_Type _eType, const float *_eSize, int _eNBits, bool _eByteOrderMSB, const void *_eData)
   {
   InitMetaImage(_iNDims, _iDimSize, _eType, _eSize, _eNBits, _eByteOrderMSB, _eData);
   }

//
MetaImage::
MetaImage(int _x, int _y, MET_Type _eType, const float *_eSize, int _eNBits, bool _eByteOrderMSB, const void *_eData)
   {
   int ds[2];
   ds[MET_DIM_X] = _x;
   ds[MET_DIM_Y] = _y;
   InitMetaImage(2, ds, _eType, _eSize, _eNBits, _eByteOrderMSB, _eData);
   }

//
MetaImage::
MetaImage(int _x, int _y, int _z, MET_Type _eType, const float *_eSize, int _eNBits, bool _eByteOrderMSB, const void *_eData)
   {
   int ds[3];
   ds[MET_DIM_X] = _x;
   ds[MET_DIM_Y] = _y;
   ds[MET_DIM_Z] = _z;
   InitMetaImage(3, ds, _eType, _eSize, _eNBits, _eByteOrderMSB, _eData);
   }

//
MetaImage::
~MetaImage()
	{
	FreeMetaMem();
	}

//
// Common Constructor Code
//
void MetaImage::
InitMetaImage(int _iNDims, const int * _iDimSize, MET_Type _eType, const float *_eSize, int _eNBits, bool _eByteOrderMSB, const void *_eData)
   {
   InitMetaMem();

   iNDims = _iNDims;

   int i;
   iQuantity = 1;
   iSubQuantity[0] = 1;
   for(i=0; i<iNDims; i++)
      {
      iDimSize[i] = _iDimSize[i];
      iQuantity *= iDimSize[i];
      if(i>0)
         iSubQuantity[i] = iDimSize[i-1] * iSubQuantity[i-1];
      }

   eType = _eType;

   if(_eSize == NULL)
      for(i=0; i<iNDims; i++)
         eSize[i] = 0;
     else
      for(i=0; i<iNDims; i++)
         eSize[i] = _eSize[i];

   if(_eNBits == 0)
      eNBits = MET_SizeOfType(eType)*8;
     else
      eNBits = _eNBits;

   eNBytes = MET_SizeOfType(eType);

   eByteOrderMSB = _eByteOrderMSB;

   if(_eData == NULL)
      {
      iFreeEData = true;
      eData = MET_Alloc(eType, iQuantity);
      }
     else
      {
      iFreeEData = false;
      eData = (void *)_eData;
      }
   }

//
void MetaImage::
InitMetaMem()
   {
   
   errorMessage[0] = '\0';
   iDataFileName[0] = '\0';

   iFreeEData = false;
   iReadFileOpen = false;
   iWriteFileOpen = false;

   iFreeHeader = false;
   iHeaderSize = 0;
   iHeader = NULL;

   iNDims = 0;
   iQuantity = 0;
   eType = MET_OTHER;
   eNBytes = 0;
   eNBits = 0;
   eMin = 0;
   eMax = 0;
   eByteOrderMSB = MET_SYSTEM_BYTE_ORDER_MSB;
   eData = NULL;

   // Optional Vars
   iComment[0] = '\0';
   iDate[0] = '\0';
   iModality = MET_MOD_UNKNOWN;

   int i;
   for(i=0; i<10; i++)
      {
      iDimSize[i] = 0;
      iSubQuantity[i] = 0;
      eSize[i] = 0;
      iPosition[i] = 0;
      pPosition[i] = 0;
      eSpacing[i] = 0;
      }
   for(i=0; i<6; i++)
      iOrientation[i] = 0;
   for(i=0; i<4; i++)
      seqID[i] = 0;
   }

//
void MetaImage::
FreeMetaMem()
	{
	if(iReadFileOpen)
		ReadFileClose();

   if(iWriteFileOpen)
      WriteFileClose();

   if(iFreeHeader)
      {
      iFreeHeader = false;
      delete [] iHeader;
      }

   if(iFreeEData)
      {
      iFreeEData = false;
      delete [] (char *)eData;
      }

   eData = NULL;
   
   }

//
char* MetaImage::
Error()
  {
  if(strlen(errorMessage))
     return errorMessage;
    else
     return NULL;
  }

//
void MetaImage::
PrintMetaInfo()
   {
   int i;

   if(strlen(iComment)>0)
      cout << "Comment = " << iComment << endl;

   if(strlen(iDate)>0)
      cout << "Date = " << iDate << endl;

   if(iModality != MET_MOD_UNKNOWN)
      cout << "Modality = " << MET_ModToString(iModality) << endl;

   cout << "NDims = " << iNDims << endl;

   cout << "DimSize = ";
   for(i=0; i<iNDims; i++)
      cout << iDimSize[i] << " ";
   cout << endl;
   
   if(eMin != eMax)
      {
      cout << "Min = " << eMin << endl;
      cout << "Max = " << eMax << endl;
      }

   bool defined = false;
   for(i=0; i<3; i++)
      if(iPosition[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      {
      cout << "ImagePosition = ";
      for(i=0; i<3; i++)
         cout << iPosition[i] << " ";
      cout << endl;
      }

   defined = false;
   for(i=0; i<6; i++)
      if(iOrientation[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      {
      cout << "ImageOrientation = ";
      for(i=0; i<6; i++)
         cout << iOrientation[i] << " ";
      cout << endl;
      }

   defined = false;
   for(i=0; i<3; i++)
      if(pPosition[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      {
      cout << "PatientPosition = ";
      for(i=0; i<3; i++)
         cout << pPosition[i] << " ";
      cout << endl;
      }

   defined = false;
   for(i=0; i<4; i++)
      if(seqID[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      {
      cout << "SequenceID = ";
      for(i=0; i<4; i++)
         cout << seqID[i] << " ";
      cout << endl;
      }

   defined = false;
   for(i=0; i<iNDims; i++)
      if(eSize[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      {
      cout << "ElementSize = ";
      for(i=0; i<iNDims; i++)
         cout << eSize[i] << " ";
      cout << endl;
      }

   defined = false;
   for(i=0; i<iNDims; i++)
      if(eSpacing[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      {
      cout << "ElementSpacing = ";
      for(i=0; i<iNDims; i++)
         cout << eSpacing[i] << " ";
      cout << endl;
      }

   cout << "ElementType = " << MET_TypeToString(eType) << endl;

   if(eNBits>0 && eNBits != MET_SizeOfType(eType)*8)
      cout << "ElementNBits = " << eNBits << endl;
   
   if(eByteOrderMSB)
      cout << "ElementByteOrderMSB = True" << endl;
     else
      cout << "ElementByteOrderMSB = False" << endl;
   }

//
//
//
bool MetaImage::
OpenMetaFile(const char *_fname, bool _read_and_close)
   {
   FreeMetaMem();
   
   InitMetaMem();
   
   iReadFilePointer = fopen(_fname, "r+b");
   if(iReadFilePointer == NULL)
      {
      cout << "Error: Cannot open file: " << _fname << endl;
      strcpy(errorMessage, "Error: Cannot open file: ");
      strcat(errorMessage, _fname);
      return 0;
      }
   iReadFileOpen = true;

   MF_FieldRec f[20];

   int c = 0;
   sprintf(f[c].name, "Comment");
   f[c].type = MF_CHAR_ARRAY;
   f[c].dependsOn = -1;
   f[c].required = false;

   sprintf(f[++c].name, "HeaderSize");
   f[c].type = MF_INT;
   f[c].dependsOn = -1;
   f[c].required = false;

   sprintf(f[++c].name, "AcquisitionDate");
   f[c].type = MF_CHAR_ARRAY;
   f[c].dependsOn = -1;
   f[c].required = false;

   sprintf(f[++c].name, "Modality");
   f[c].type = MF_CHAR_ARRAY;
   f[c].dependsOn = -1;
   f[c].required = false;

   int nDimsC = ++c;
   sprintf(f[c].name, "NDims");
   f[c].type = MF_INT;
   f[c].dependsOn = -1;
   f[c].required = true;

   sprintf(f[++c].name, "DimSize");
   f[c].type = MF_INT_ARRAY;
   f[c].dependsOn = nDimsC;
   f[c].required = true;

   sprintf(f[++c].name, "Min");
   f[c].type = MF_FLOAT;
   f[c].dependsOn = -1;
   f[c].required = false;

   sprintf(f[++c].name, "Max");
   f[c].type = MF_FLOAT;
   f[c].dependsOn = -1;
   f[c].required = false;

   sprintf(f[++c].name, "ImagePosition");
   f[c].type = MF_FLOAT_ARRAY;
   f[c].dependsOn = -1;
   f[c].length = 3;
   f[c].required = false;

   sprintf(f[++c].name, "ImageOrientation");
   f[c].type = MF_FLOAT_ARRAY;
   f[c].dependsOn = -1;
   f[c].length = 6;
   f[c].required = false;

   sprintf(f[++c].name, "PatientPosition");
   f[c].type = MF_FLOAT_ARRAY;
   f[c].dependsOn = -1;
   f[c].length = 3;
   f[c].required = false;

   sprintf(f[++c].name, "SequenceID");
   f[c].type = MF_FLOAT_ARRAY;
   f[c].dependsOn = -1;
   f[c].length = 4;
   f[c].required = false;

   sprintf(f[++c].name, "ElementSize");
   f[c].type = MF_FLOAT_ARRAY;
   f[c].dependsOn = nDimsC;
   f[c].required = false;

   sprintf(f[++c].name, "ElementSpacing");
   f[c].type = MF_FLOAT_ARRAY;
   f[c].dependsOn = nDimsC;
   f[c].required = false;

   sprintf(f[++c].name, "ElementType");
   f[c].type = MF_CHAR_ARRAY;
   f[c].dependsOn = -1;
   f[c].required = true;

   sprintf(f[++c].name, "ElementNBits");
   f[c].type = MF_INT;
   f[c].dependsOn = -1;
   f[c].required = false;

   sprintf(f[++c].name, "ElementByteOrderMSB");
   f[c].type = MF_CHAR_ARRAY;
   f[c].dependsOn = -1;
   f[c].required = true;

   sprintf(f[++c].name, "ElementDataFile");
   f[c].type = MF_CHAR_ARRAY;
   f[c].dependsOn = -1;
   f[c].required = true;

   int lastC = c;
   if(!MF_Read(iReadFilePointer, c+1, f, c))
      {
      cout << "Error parsing header file: _" << _fname << "_" << endl;
      strcpy(errorMessage, "Error parsing header file: ");
      strcat(errorMessage, _fname);
      return false;
      }

   c = 0;
   if(f[c].defined)
      strcpy(iComment, (char *)(f[c].val));

   if(f[++c].defined)
      iHeaderSize = (int)f[c].val[0];

   if(f[++c].defined)
      strcpy(iDate, (char *)(f[c].val));

   if(f[++c].defined)
      iModality = MET_StringToMod((char *)(f[c].val));

   iNDims = (int)(f[++c].val[0]);
   if(c != nDimsC)
      {
      cout << "MetaImageLib Error: field count" << endl;
      exit(0);
      }

   int i;
   iQuantity = 1;
   iSubQuantity[0] = 1;
   if(f[++c].defined)
      for(i=0; i<iNDims; i++)
         {
         iDimSize[i] = (int)(f[c].val[i]);
         iQuantity *= iDimSize[i];
         if(i>0)
            iSubQuantity[i] = iDimSize[i-1] * iSubQuantity[i-1];
         }

   if(f[++c].defined)
      eMin = (float)(f[c].val[0]);
     else
      eMin = 0;

   if(f[++c].defined)
      eMax = (float)(f[c].val[0]);
     else
      eMax = 0;

   if(f[++c].defined)
      for(i=0; i<3; i++)
         iPosition[i] = f[c].val[i];
     else
      for(i=0; i<3; i++)
         iPosition[i] = 0;

   if(f[++c].defined)
      for(i=0; i<6; i++)
         iOrientation[i] = f[c].val[i];
     else
      for(i=0; i<6; i++)
         iOrientation[i] = 0;
   
   if(f[++c].defined)
      for(i=0; i<3; i++)
         pPosition[i] = f[c].val[i];
     else
      for(i=0; i<3; i++)
         pPosition[i] = 0;

   if(f[++c].defined)
      for(i=0; i<4; i++)
         seqID[i] = f[c].val[i];
     else
      for(i=0; i<4; i++)
         seqID[i] = 0;

   if(f[++c].defined)
      for(i=0; i<iNDims; i++)
         eSize[i] = f[c].val[i];
     else
      for(i=0; i<iNDims; i++)
         eSize[i] = 0;

   if(f[++c].defined)
      for(i=0; i<iNDims; i++)
         eSpacing[i] = f[c].val[i];
     else
      for(i=0; i<iNDims; i++)
         eSpacing[i] = 0;

   if(f[++c].defined)
      eType = MET_StringToType((char *)f[c].val);
     else
      eType = MET_UCHAR;

   if(f[++c].defined)
      eNBits = (int)f[c].val[0];
     else
      eNBits = MET_SizeOfType(eType)*8;

   eNBytes = MET_SizeOfType(eType);

   if(f[++c].defined)
      {
      if(((char *)(f[c].val))[0] == 'f' || ((char *)(f[c].val))[0] == 'F')
         eByteOrderMSB = false;
        else
         eByteOrderMSB = true;
      }
     else
      eByteOrderMSB = MET_SYSTEM_BYTE_ORDER_MSB;

   if(f[++c].defined)
      {
      sprintf(iDataFileName, "%s", (char *)f[c].val);
      MET_GetFilePath(_fname, iDataFilePath);
      }

   if(c != lastC)
      {
      cout << "MetaImageLib Error: last field" << endl;
      exit(0);
      }

   //
   if(_read_and_close)
		{
		if(!ReadImageData())
         return false;
      ReadFileClose();
		}

   return true;
   }

//
// Valid if not read_and_close
//
bool MetaImage::
FieldExists(const char *_fieldName)
   {
   MF_FieldRec f[2];

   sprintf(f[0].name, "%s", _fieldName);
   f[0].type = MF_INT;
   f[0].dependsOn = -1;
   f[0].required = true;

   sprintf(f[1].name, "ElementDataFile");
   f[1].type = MF_CHAR_ARRAY;
   f[1].dependsOn = -1;
   f[1].required = true;

   return MF_Read(iReadFilePointer, 2, f, 1);
   }

//
void * MetaImage::
GetFieldValue(const char *_fieldName, MF_ValType _fieldType, int _nDims)
   {
   static MF_FieldRec f[2];

   sprintf(f[0].name, "%s", _fieldName);
   f[0].type = _fieldType;
   f[0].dependsOn = -1;
   f[0].required = true;
   f[0].defined = false;
   f[0].length = _nDims;

   sprintf(f[1].name, "ElementDataFile");
   f[1].type = MF_CHAR_ARRAY;
   f[1].dependsOn = -1;
   f[1].required = true;

   if(!MF_Read(iReadFilePointer, 2, f, 1))
      {
      cout << "Error: Field does not exist: " << _fieldName << endl;
      strcpy(errorMessage, "Error: Field does not exist: ");
      strcat(errorMessage, _fieldName);
      return NULL;
      }

   return (void *)(f[0].val);
   }

//
bool MetaImage::
ReadImageData()
   {
   bool localData = false;
   FILE *fp;

   char fName[800];
   if(!strcmp("LOCAL", iDataFileName))
      {
      localData = true;
      fp = iReadFilePointer;
      }
     else
      {
      //FILE PATH FIX
      if(!MET_GetFilePath(iDataFileName, fName))
          sprintf(fName, "%s%s", iDataFilePath, iDataFileName);
      else
          sprintf(fName, "%s", iDataFileName);
      fp = fopen(fName, "r+b");
      }
   if(fp == NULL)
      {
      strcpy(errorMessage, "Error: cannot open element data file: ");
      strcat(errorMessage, fName);
      cout << "Error: Cannot open element data file: _" << fName << "_" << endl;
      return false;
      }

   int n;
   if(iHeaderSize>0)
      {
      iHeader = (unsigned char *)MET_Alloc(MET_UCHAR, iHeaderSize);
      iFreeHeader = true;
      n = fread(iHeader, sizeof(unsigned char), iHeaderSize, fp);
      if(n != iHeaderSize)
         {
         sprintf(errorMessage, "Error: read header %d but expected %d", n, iHeaderSize);
         cout << "Error: read header " << n << " but expecting " << iHeaderSize << endl;
         return false;
         }
      }

   eData = MET_Alloc(eType, iQuantity);
   if(eData == NULL)
      {
      strcpy(errorMessage, "Error: Cannot allocate memory for image data");
      cout << "Error: Cannot allocate memory for image data" << endl;
      return false;
      }
   iFreeEData = true;

   n = fread(eData, eNBytes, iQuantity, fp);
   if(n != iQuantity)
      {
      sprintf(errorMessage, "Error: read %i but expected %i", n, iQuantity);
      cout << "Error: read " << n << " but expecting " << iQuantity << endl;
      return false;
      }

   fclose(fp);
   if(localData)
      iReadFileOpen = false;

   ElemByteOrderFix();

   return true;
   }

//
bool MetaImage::
ReadFileClose()
   {
   if(iReadFileOpen == true)
      {
      iReadFileOpen = false;
      fclose(iReadFilePointer);
      return true;
      }
   return false;
   }

//
// Read by default
//
int MetaImage::
HeaderSize(int _iHeaderSize)
   {
   if(_iHeaderSize > 0)
      iHeaderSize = _iHeaderSize;
   return iHeaderSize;
   }

char * MetaImage::
Comment(const char *_c)
   {
   if(_c != NULL)
      strcpy(iComment, _c);
   return iComment;
   }

unsigned char * MetaImage::
Header(const unsigned char *_iHeader)
   {
   if(_iHeader != NULL)
      {
      if(iFreeHeader)
         delete [] iHeader;
        else
         iFreeHeader = true;
      iHeader = (unsigned char *)MET_Alloc(MET_UCHAR, iHeaderSize);
      memcpy(iHeader, _iHeader, iHeaderSize);
      }
   return iHeader;
   }

char * MetaImage::
Date(const char *_d)
   {
   if(_d != NULL)
      strcpy(iDate, _d);
   return iDate;
   }

MET_ModType MetaImage::
Modality(MET_ModType _mType)
   {
   if(_mType != MET_MOD_UNKNOWN)
      iModality = _mType;
   return iModality;
   }

int MetaImage::
NDims()
   {
   return iNDims;
   }

int * MetaImage::
DimSize()
   {
   return iDimSize;
   }

int MetaImage::
DimSize(int i)
   {
   if(i<iNDims)
      return iDimSize[i];
   return 0;
   }

int MetaImage::
Quantity()
   {
   return iQuantity;
   }

int * MetaImage::
SubQuantity()
   {
   return iSubQuantity;
   }

int MetaImage::
SubQuantity(int i)
   {
   return iSubQuantity[i];
   }

float * MetaImage::
Position(const float *_iPosition)
   {
   if(_iPosition != NULL)
      {
      int i;
      for(i=0; i<iNDims; i++)
         iPosition[i] = _iPosition[i];
      }
   return iPosition;
   }

float MetaImage::
Position(int i)
   {
   return iPosition[i];
   }

float * MetaImage::
Orientation(const float *_iOrientation)
   {
   if(_iOrientation != NULL)
      {
      int i;
      for(i=0; i<6; i++)
         iOrientation[i] = _iOrientation[i];
      }
   return iOrientation;
   }

float MetaImage::
Orientation(int i)
   {
   return iOrientation[i];
   }

float * MetaImage::
PatPosition(const float *_pPosition)
   {
   if(_pPosition != NULL)
      {
      int i;
      for(i=0; i<iNDims; i++)
         pPosition[i] = _pPosition[i];
      }
   return pPosition;
   }

float MetaImage::
PatPosition(int i)
   {
   return pPosition[i];
   }


float * MetaImage::
SequenceID(const float *_seqID)
   {
   if(_seqID != NULL)
      {
      int i;
      for(i=0; i<4; i++)
         seqID[i] = _seqID[i];
      }
   return seqID;
   }

float MetaImage::
SequenceID(int i)
   {
   return seqID[i];
   }

float * MetaImage::
ElemSize(const float *_eSize)
   {
   if(_eSize != NULL)
      {
      int i;
      for(i=0; i<iNDims; i++)
         eSize[i] = _eSize[i];
      }
   return eSize;
   }

float MetaImage::
ElemSize(int i)
   {
   return eSize[i];
   }

float * MetaImage::
ElemSpacing(const float *_eSpacing)
   {
   if(_eSpacing != NULL)
      {
      int i;
      for(i=0; i<iNDims; i++)
         eSpacing[i] = _eSpacing[i];
      }
   return eSpacing;
   }

float MetaImage::
ElemSpacing(int i)
   {
   return eSpacing[i];
   }


MET_Type MetaImage::
ElemType(MET_Type _eType)
   {
   if(_eType != MET_DEFAULT)
      eType = _eType;
   return eType;
   }

int MetaImage::
ElemNBytes()
   {
   return eNBytes;
   }

int MetaImage::
ElemNBits(int _eNBits)
   {
   if(_eNBits != 0)
      eNBits = _eNBits;
   return eNBits;
   }

bool MetaImage::
ElemByteOrderMSB()
   {
   return eByteOrderMSB;
   }

bool MetaImage::
ElemByteSwapRequired()
   {
   return (eByteOrderMSB != MET_SYSTEM_BYTE_ORDER_MSB);
   }

//
// Available after read and close
//
bool MetaImage::
ElemMakeByteOrderMSB()
   {
   if(eByteOrderMSB)
      return false;
     else
      {
      ElemByteOrderSwap();
      return true;
      }
   }

bool MetaImage::
ElemMakeByteOrderLSB()
   {
   if(eByteOrderMSB)
      {
      ElemByteOrderSwap();
      return true;
      }
     else
      return false;
   }

void MetaImage::
ElemByteOrderSwap()
   {
   int i;
   unsigned short s;
   unsigned int l;
   if(MET_SizeOfType(eType) == 2)
      for(i=0; i<iQuantity; i++)
         {
         s = MET_SWAPS(((unsigned short *)eData)[i]);
         ((unsigned short *)eData)[i] = s;
         }
     else
      if(MET_SizeOfType(eType) == 4)
         for(i=0; i<iQuantity; i++)
            {
            l = MET_SWAPL(((unsigned int *)eData)[i]);
            ((unsigned int *)eData)[i] = l;
            }

   eByteOrderMSB = !eByteOrderMSB;
   }

bool MetaImage::
ElemByteOrderFix()
   {
   if(ElemByteSwapRequired())
      {
      ElemByteOrderSwap();
      return true;
      }
   return false;
   }

//
float MetaImage::
Min(float _eMin, bool _reCalc)
   {
   ElemByteOrderFix();

   int i;

   if ((_eMin != -99 || _reCalc || eMin == eMax) && eData)
      if(_eMin != -99 && !_reCalc)
         eMin = _eMin;
        else
         {
         eMin = MET_ValueToFloat(eType, &((char *)eData)[0]);
         for(i=1; i<iQuantity; i++)
            if(MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]) < eMin)
               eMin = MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         }

   return eMin;
   }

//
float MetaImage::
Max(float _eMax, bool _reCalc)
   {
   ElemByteOrderFix();

   int i;

   if ((_eMax != -99 || _reCalc || eMin >= eMax || eMax == 0) && eData)
      if(_eMax != -99 && !_reCalc)
         eMax = _eMax;
        else
         {
         if(eNBits < 8*eNBytes && !_reCalc && eType != MET_FLOAT && eType != MET_DOUBLE)
            {
            if(eType == MET_UCHAR || eType == MET_USHORT || eType == MET_UINT)
               eMax = (float)(1<<eNBits)-1;
             else
               eMax = (float)(1<<(eNBits-1));
            }
           else
            {
            eMax = MET_ValueToFloat(eType, &((char *)eData)[0]);
            for(i=1; i<iQuantity; i++)
               if(MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]) > eMax)
                  eMax = MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
            }
        }
   return eMax;
   }

//
bool MetaImage::
ConvertTo(MET_Type _eType)
   {
   if(_eType == eType)
      return true;

   ElemByteOrderFix();

   if(eMin == eMax)
      {
      Min();
      Max();
      }

   float mm = (float)fabs(eMax);
   if(mm<fabs(eMin))
      mm = (float)fabs(eMin);

   int i;

   void *nData = MET_Alloc(_eType, iQuantity);
   switch(_eType)
      {
      case MET_UCHAR:
         if(eMin<0 || eMax>255)
            for(i=0;i<iQuantity; i++)
               ((unsigned char *)nData)[i] = (unsigned char)((MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes])-eMin)/eMax * 255);
           else
            for(i=0; i<iQuantity; i++)
               ((unsigned char *)nData)[i] = (unsigned char)MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      case MET_CHAR:
         if(eMin<-128 || eMax>128)
            for(i=0;i<iQuantity; i++)
               ((char *)nData)[i] = (char)(MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes])/mm * 128);
           else
            for(i=0; i<iQuantity; i++)
               ((char *)nData)[i] = (char)MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      case MET_USHORT:
         if(eMin<0 || eMax>65535)
            for(i=0;i<iQuantity; i++)
               ((unsigned short *)nData)[i] = (unsigned short)((MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes])-eMin)/eMax * 65535);
           else
            for(i=0; i<iQuantity; i++)
               ((unsigned short *)nData)[i] = (unsigned short)MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      case MET_SHORT:
         if(eMin<-32767 || eMax>32767)
            for(i=0;i<iQuantity; i++)
               ((short *)nData)[i] = (short)(MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes])/mm * 32767);
           else
            for(i=0; i<iQuantity; i++)
               ((short *)nData)[i] = (short)MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      case MET_UINT:
         for(i=0; i<iQuantity; i++)
            ((unsigned int *)nData)[i] = (unsigned int)MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      case MET_INT:
         for(i=0; i<iQuantity; i++)
            ((int *)nData)[i] = (int)MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      case MET_FLOAT:
         for(i=0;i<iQuantity; i++)
            ((float *)nData)[i] = MET_ValueToFloat(eType, &((char *)eData)[i*eNBytes]);
         break;
      default:
         delete [] (char *)nData;
         return false;
      }
   eType = _eType;
   eNBytes = MET_SizeOfType(eType);
   eNBits = 8*eNBytes;
   eByteOrderMSB = MET_SYSTEM_BYTE_ORDER_MSB;
   eMin = 0;
   eMax = 0;
   if(iFreeEData)
      {
      iFreeEData = false;
      delete [] (char *)eData;
      }
   eData = nData;
   return true;
   }

//
//
//
void * MetaImage::
operator()()
   {
   return eData;
   }

float MetaImage::
operator()(int _x)
   {
   return Get(_x);
   }

float MetaImage::
operator()(int _x, int _y)
   {
   return Get(_x, _y);
   }

float MetaImage::
operator()(int _x, int _y, int _z)
   {
   return Get(_x, _y, _z);
   }

float MetaImage::
operator()(const int * _x)
   {
   return Get(_x);
   }

void * MetaImage::
Get()
   {
   return eData;
   }

float MetaImage::
Get(int _x)
   {
   switch(eType)
      {
      case MET_CHAR:
         return MET_ValueToFloat(MET_CHAR, (void*)&(((char *)eData)[_x]));
      case MET_UCHAR:
         return MET_ValueToFloat(MET_UCHAR, (void*)&(((unsigned char *)eData)[_x]));
      case MET_SHORT:
         return MET_ValueToFloat(MET_SHORT, (void*)&(((short *)eData)[_x]));
      case MET_USHORT:
         return MET_ValueToFloat(MET_USHORT, (void*)&(((unsigned short *)eData)[_x]));
      case MET_INT:
         return MET_ValueToFloat(MET_INT, (void*)&(((int *)eData)[_x]));
      case MET_UINT:
         return MET_ValueToFloat(MET_UINT, (void*)&(((unsigned int *)eData)[_x]));
      case MET_FLOAT:
         return MET_ValueToFloat(MET_FLOAT, (void*)&(((float *)eData)[_x]));
      case MET_DOUBLE:
         return MET_ValueToFloat(MET_DOUBLE, (void*)&(((double *)eData)[_x]));
      default:
         return 0;
      }
   }

float MetaImage::
Get(int _x, int _y)
   {
   return Get(_y*iSubQuantity[1]+_x);
   }

float MetaImage::
Get(int _x, int _y, int _z)
   {
   return Get(_z*iSubQuantity[2]+_y*iSubQuantity[1]+_x);
   }

float MetaImage::
Get(const int * _x)
   {
   int i;
   int q = 0;

   for(i=0; i<iNDims; i++)
      q += _x[i] * iSubQuantity[i];

   return Get(q);
   }

bool MetaImage::
Set(float _v)
   {
   int i;

   for(i=0; i<iQuantity; i++)
      Set(i, _v);

   return true;
   }

bool MetaImage::
Set(int _x, float _v)
   {
   switch(eType)
      {
      case MET_CHAR:
         ((char *)eData)[_x] = (char)_v;
         break;
      case MET_UCHAR:
         ((unsigned char *)eData)[_x] = (unsigned char)_v;
         break;
      case MET_SHORT:
         ((short *)eData)[_x] = (short)_v;
         break;
      case MET_USHORT:
         ((unsigned short *)eData)[_x] = (unsigned short)_v;
         break;
      case MET_INT:
         ((int *)eData)[_x] = (int)_v;
         break;
      case MET_UINT:
         ((unsigned int *)eData)[_x] = (unsigned int)_v;
         break;
      case MET_FLOAT:
         ((float *)eData)[_x] = (float)_v;
         break;
      case MET_DOUBLE:
         ((double *)eData)[_x] = (double)_v;
         break;
      default:
         return false;
      }
   return true;
   }

bool MetaImage::
Set(int _x, int _y, float _v)
   {
   return Set(_y*iSubQuantity[1]+_x, _v);
   }

bool MetaImage::
Set(int _x, int _y, int _z, float _v)
   {
   return Set(_z*iSubQuantity[2]+_y*iSubQuantity[1]+_x, _v);
   }

bool MetaImage::
Set(const int * _x, float _v)
   {
   int i;
   int q = 0;
   for(i=0; i<iNDims; i++)
      q += _x[i] * iSubQuantity[i];
   return Set(q, _v);
   }


//
//
//
bool MetaImage::
Save(const char *_headname, const char *_dataname, bool _write_and_close)
   {
   char pName[255];

   iWriteFilePointer = fopen(_headname, "w+b");
   if(iWriteFilePointer == NULL)
      {
      strcpy(errorMessage, "Error: cannot write to file: ");
      strcat(errorMessage, _headname);
      cout << "Error: Cannot write to file: " << _headname << endl;
      return false;
      }
   iWriteFileOpen = true;

   MET_GetFilePath(_headname, pName);
   char s[80];

   if(strlen(iComment)>0)
      SetFieldValue("Comment", MF_CHAR_ARRAY, strlen(iComment), (void *)iComment);

   if(iHeaderSize>0)
      SetFieldValue("HeaderSize", MF_INT, (float)iHeaderSize);

   if(strlen(iDate)>0)
      SetFieldValue("AcquisitionDate", MF_CHAR_ARRAY, strlen(iDate), (void *)iDate);

   if(iModality != MET_MOD_UNKNOWN)
      {
      sprintf(s, "%s", MET_ModToString(iModality));
      SetFieldValue("Modality", MF_CHAR_ARRAY, strlen(s), (void *)s);
      }

   SetFieldValue("NDims", MF_INT, (float)iNDims);

   SetFieldValue("DimSize", MF_INT_ARRAY, iNDims, (void *)iDimSize);

   if(eMin != eMax)
      {
      SetFieldValue("Min", MF_FLOAT, eMin);
      SetFieldValue("Max", MF_FLOAT, eMax);
      }

   int i;
   bool defined = false;
   for(i=0; i<iNDims; i++)
      if(iPosition[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      SetFieldValue("ImagePosition", MF_FLOAT_ARRAY, iNDims, (void *)iPosition);

   defined = false;
   for(i=0; i<6; i++)
      if(iOrientation[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      SetFieldValue("ImageOrientation", MF_FLOAT_ARRAY, 6, (void *)iOrientation);

   defined = false;
   for(i=0; i<iNDims; i++)
      if(pPosition[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      SetFieldValue("PatientPosition", MF_FLOAT_ARRAY, iNDims, (void *)pPosition);

   defined = false;
   for(i=0; i<4; i++)
      if(seqID[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      SetFieldValue("SequenceID", MF_FLOAT_ARRAY, 4, (void *)seqID);

   defined = false;
   for(i=0; i<iNDims; i++)
      if(eSize[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      SetFieldValue("ElementSize", MF_FLOAT_ARRAY, iNDims, (void *)eSize);

   defined = false;
   for(i=0; i<iNDims; i++)
      if(eSpacing[i] != 0)
         {
         defined = true;
         break;
         }
   if(defined)
      SetFieldValue("ElementSpacing", MF_FLOAT_ARRAY, iNDims, (void *)eSpacing);

   sprintf(s, "%s", MET_TypeToString(eType));
   SetFieldValue("ElementType", MF_CHAR_ARRAY, strlen(s), (void *)s);

   if(eNBits>0 && eNBits != MET_SizeOfType(eType)*8)
      SetFieldValue("ElementNBits", MF_INT, (float)eNBits);

   sprintf(s, "%s", (eByteOrderMSB)?"True":"False");
   SetFieldValue("ElementByteOrderMSB", MF_CHAR_ARRAY, strlen(s), (void *)s);
   
   if(_dataname == NULL)
      {
      sprintf(s, "LOCAL");
      SetFieldValue("ElementDataFile", MF_CHAR_ARRAY, strlen(s), (void *)s);
      fwrite(eData, eNBytes, iQuantity, (FILE *)iWriteFilePointer);
      WriteFileClose();
      return true;
      }
     else
      {
      char s[255];
      //
      //FILE PATH FIX
      // Closer to what we need regarding when to append paths to files
      //   - ideally, it should convert absolute to relative paths for
      //     split files.
      //
      if(!MET_GetFilePath(_dataname, s))
          strcpy(s, _dataname);
      else
	  if(!strcmp(s, pName))
              strcpy(s, &(_dataname[strlen(s)]));
	  else
	      strcpy(s, _dataname);
      SetFieldValue("ElementDataFile", MF_CHAR_ARRAY, strlen(s), (void *)s);
      strcpy(pName, s);
      }

   if(_write_and_close)
      WriteFileClose();

   FILE *fp = fopen(pName, "w+b");
   if(fp == NULL)
      {
      strcpy(errorMessage, "Error: Cannot write image data to file: ");
      strcat(errorMessage, _dataname);
      cout << "Error: Cannot write image data to file: " << pName << endl;
      return false;
      }
   int n = fwrite(eData, eNBytes, iQuantity, fp);
   fclose(fp);

   if (!n)
     {
     cout<<"Something went wrong in MetaImage::Save()";
     strcpy(errorMessage, "Write failed");
     return 0;
     }

   return true;
   }

bool MetaImage::
SetFieldValue(const char *_fieldName, MF_ValType _pType, float _v)
   {
   if(!iWriteFileOpen)
      return false;

   MF_FieldRec f;

   sprintf(f.name, "%s", _fieldName);
   f.defined = false;
   f.dependsOn = -1;
   f.length = 1;
   f.required = false;
   f.type = _pType;
   f.val[0] = _v;
   
   MF_Write(iWriteFilePointer, 1, &f);

   return true;
   }

bool MetaImage::
SetFieldValue(const char *_fieldName, MF_ValType _pType, int _n, const void *_v)
   {
   if(!iWriteFileOpen)
      return false;

   int i;
   MF_FieldRec f;

   sprintf(f.name, "%s", _fieldName);
   f.defined = false;
   f.dependsOn = -1;
   f.length = _n;
   f.required = false;
   f.type = _pType;
   switch(_pType)
      {
      case MF_CHAR_ARRAY:
      case MF_CHAR:
         for(i=0; i<_n; i++)
            f.val[i] = (float)(((char *)_v)[i]);
         break;
      case MF_INT:
      case MF_INT_ARRAY:
         for(i=0; i<_n; i++)
            f.val[i] = (float)(((int *)_v)[i]);
         break;
      case MF_FLOAT:
      case MF_FLOAT_ARRAY:
         for(i=0; i<_n; i++)
            f.val[i] = ((float *)_v)[i];
         break;
      default:
         break;
      }

   MF_Write(iWriteFilePointer, 1, &f);

   return true;
   }

bool MetaImage::
WriteFileClose()
   {
   if(iWriteFileOpen)
      {
      iWriteFileOpen = false;
      fclose(iWriteFilePointer);
      return true;
      }
   return false;
   }
