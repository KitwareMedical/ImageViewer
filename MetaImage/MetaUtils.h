/*!
 * File:
 *   MetaUtils (.h and .cpp)
 *
 * Description:
 *    This file provides generic include files, functions (tempates), and type definitions for
 *    handling metaFiles and metaImageFiles.
 *
 * Features:
 *    Most of these functions are quite broadly applicable
 *
 * \author Stephen R. Aylward
 * \date August 29, 1999
 * 
 */

//
#ifndef METUTILS_H
#define METUTILS_H

#include <stdio.h>

//  #ifndef _WIN32
//  #define  MET_SYSTEM_BYTE_ORDER_MSB   true
//  #else
//  #define  MET_SYSTEM_BYTE_ORDER_MSB   false
//  #endif


#if defined(_WIN32) || defined(__CYGWIN__) 
#define  MET_SYSTEM_BYTE_ORDER_MSB   false
#else
#define  MET_SYSTEM_BYTE_ORDER_MSB   true
#endif

// Value types for the variables in a metaFile
// Format for variables defined in a metaFile is
//    <variable> = <value>
//       where <variable> is a designated fieldname/keyword (e.g., NDims)
//          and value is an instance of that fieldname's associated valueType
typedef enum
   {
   MF_NONE,
   MF_CHAR, 
   MF_INT, 
   MF_FLOAT, 
   MF_CHAR_ARRAY, 
   MF_INT_ARRAY, 
   MF_FLOAT_ARRAY
   } MF_ValType;

// Structure used to define a field (variable = value definition) in a MetaFile
typedef struct
   {
   char name[255];      // Fieldname / keyword to designate a variable
   MF_ValType type;     // Expected value type of the field
   bool required;       // Is this field a required field in a metaFile
   int dependsOn;       // If value type is an array, the size of this array can be
                        //    defined by a different field (e.g., DimSize array depends on NDims)
   bool defined;        // Has this field already been defined in the MetaFile being parsed
   int length;          // Actual/expect length of an array
   float val[255];      // Memory and pointers for the field's value(s).
   } MF_FieldRec;

typedef enum         {MET_DEFAULT,
                      MET_CHAR,
                      MET_UCHAR, 
                      MET_SHORT, 
                      MET_USHORT, 
                      MET_INT, 
                      MET_UINT, 
                      MET_FLOAT, 
                      MET_DOUBLE, 
                      MET_OTHER} MET_Type;


template <class T> 
T MET_SWAPS(T x) 
  { 
  return ((unsigned short)(((unsigned short)(((unsigned short)(x))<<8)) | ((unsigned short)(((unsigned short)(x))>>8)))); 
  }

template <class T> 
T MET_SWAPL(T x) 
  { 
  return ((unsigned int)(((unsigned int)MET_SWAPS((unsigned short)(((unsigned int)(x))&65535))) | (((unsigned int)MET_SWAPS((unsigned short)(((unsigned int)(x))>>16)))<<16))); 
  }

//
// Read and write shorts and longs using appropriate/arbitrary byte ordering
//
extern bool MET_fwrites(FILE *fp,
                        unsigned short _v,
                        bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);
extern bool MET_fwritel(FILE *fp,
                        unsigned long _v,
                        bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);
extern unsigned short MET_freads(FILE *fp,
                                 bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);
extern unsigned long  MET_freadl(FILE *fp,
                                 bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);


extern bool     MET_Error;

extern MET_Type	MET_StringToType(const char *_s);
extern char *	MET_TypeToString(MET_Type _pType);

extern float	MET_ValueToFloat(MET_Type _pType,
                                 const void *_v,
                                 bool _byteOrderMSB=MET_SYSTEM_BYTE_ORDER_MSB);

extern int      MET_SizeOfType(MET_Type _pType);


//
// MET_ALLOC
//
extern void *	MET_Alloc(MET_Type _pType, int _x);
extern void *	MET_Alloc(MET_Type _pType, int _y, int _x);
extern void *	MET_Alloc(MET_Type _pType, int _z, int _y, int _x);
extern void *	MET_Alloc(MET_Type _pType, int _s, int _z, int _y, int _x);
extern void *	MET_Alloc(MET_Type _pType, int _nDims, const int *_dimSize);

//
//
//
extern bool          MET_GetFilePath(const char *_fName, char *_fPath);
extern int           MET_GetFileSuffixPtr(const char *_fName);
extern bool          MET_SetFileSuffix(char *_fName, const char *_suf);

#endif
