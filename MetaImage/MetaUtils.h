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

#include <cstdio>

#include "MetaImageTypes.h"

#ifndef WIN32
#define  MET_SYSTEM_BYTE_ORDER_MSB   true
#else
#define  MET_SYSTEM_BYTE_ORDER_MSB   false
#endif


// Ya never know...
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif


//
// Basic template functions
//
template <class T> int  MET_SIGN(T a) { return ((a>=0)?1:-1); }

template <class T> T    MET_MIN(T a, T b) { return ((a<b) ? a : b); }

template <class T> T    MET_MAX(T a, T b) { return ((a>=b) ? a : b); }

template <class T> T    MET_BND(T a, T b, T c) { return ((a>b)?((a<c)?a:c):b); }

template <class T> T    MET_SWAPS(T x) { return ((unsigned short)(((unsigned short)(((unsigned short)(x))<<8)) | ((unsigned short)(((unsigned short)(x))>>8)))); }

template <class T> T    MET_SWAPL(T x) { return ((unsigned int)(((unsigned int)MET_SWAPS((unsigned short)(((unsigned int)(x))&65535))) | (((unsigned int)MET_SWAPS((unsigned short)(((unsigned int)(x))>>16)))<<16))); }



//
// Read and write shorts and longs using appropriate/arbitrary byte ordering
//
extern bool             MET_fwrites(FILE *fp, unsigned short _v, bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);
extern bool             MET_fwritel(FILE *fp, unsigned long _v, bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);
extern unsigned short   MET_freads(FILE *fp, bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);
extern unsigned long    MET_freadl(FILE *fp, bool _hostMSB=MET_SYSTEM_BYTE_ORDER_MSB);



//
// MET_TYPE
//
#define MET_TYPE_DEFAULT            void
#define MET_TYPE_CHAR               char
#define MET_TYPE_UCHAR              unsigned char
#define MET_TYPE_SHORT              short
#define MET_TYPE_USHORT             unsigned short
#define MET_TYPE_INT                int
#define MET_TYPE_UINT               unsigned int
#define MET_TYPE_FLOAT              float
#define MET_TYPE_DOUBLE             double
#define MET_TYPE_OTHER              void

extern bool          MET_Error;

extern MET_Type	  	MET_StringToType(const char *_s);
extern char *			MET_TypeToString(MET_Type _pType);

extern float			MET_ValueToFloat(MET_Type _pType, const void *_v, bool _byteOrderMSB=MET_SYSTEM_BYTE_ORDER_MSB);

extern int           MET_SizeOfType(MET_Type _pType);


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
template <class T>
bool MET_Init(int _n, T *_data, T _x)
   {
   int i;
   for(i=0; i<_n; i++)
      _data[i] = _x;
   return true;
   }

template <class T>
bool MET_InitDiag(int _n, int _m, T *_data, T _x)
   {
   MET_Init(_n*_m, _data, 0);

   int i, j;
   for(i=0; i<_m; i++)
      for(j=0; j<_n; j++)
         _data[i*_n+j] = _x;
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from, unsigned char *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = (unsigned char)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from,  char *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = ( char)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from, unsigned short *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = (unsigned short)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from,  short *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = ( short)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from, unsigned int *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = (unsigned int)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from,  int *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = (int)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from, float *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = (float)_from[i];
   }

template <class T>
bool MET_CopyTo(int _n, const T *_from, double *_to)
   {
   int i;
   for(i=0; i<_n; i++)
      _to[i] = (double)_from[i];
   }


//
// FUN WITH FILENAMES
//
extern bool          MET_GetFilePath(const char *_fName, char *_fPath);
extern int           MET_GetFileSuffixPtr(const char *_fName);
extern bool          MET_SetFileSuffix(char *_fName, const char *_suf);


#endif
