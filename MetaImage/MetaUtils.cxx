/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    MetaUtils.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <ctype.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef _WIN32
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include "MetaUtils.h"

bool MET_Error = false;

bool MET_fwrites(FILE *fp, unsigned short _v, bool _hostMSB)
   {
   short t;
   if(_hostMSB)
      t = _v;
     else
      t = MET_SWAPS(_v);
   fwrite(&t, 1, sizeof(short), fp);
   return true;
   }

bool MET_fwritel(FILE *fp, unsigned long _v, bool _hostMSB)
   {
   long t;
   if(_hostMSB)
      t = _v;
     else
      t = MET_SWAPL(_v);
   fwrite(&t, 1, sizeof(long), fp);
   return true;
   }

unsigned short MET_freads(FILE *fp, bool _hostMSB)
   {
   short t;
   fread(&t, 1, sizeof(short), fp);
   if(_hostMSB)
      return t;
     else
      return MET_SWAPS(t);
   }

unsigned long MET_freadl(FILE *fp, bool _hostMSB)
   {
   long t;
   fread(&t, 1, sizeof(long), fp);
   if(_hostMSB)
      return t;
     else
      return MET_SWAPL(t);
   }

//
// String To Type
//
MET_Type MET_StringToType(const char *_s)
        {
        if(!strcmp(_s, "MET_DEFAULT"))
                return MET_DEFAULT;

        if(!strcmp(_s, "MET_CHAR"))
                return MET_CHAR;

   if(!strcmp(_s, "MET_UCHAR"))
                return MET_UCHAR;
        
   if(!strcmp(_s, "MET_SHORT"))
                return MET_SHORT;
        
   if(!strcmp(_s, "MET_USHORT"))
                return MET_USHORT;
        
   if(!strcmp(_s, "MET_INT"))
                return MET_INT;
        
   if(!strcmp(_s, "MET_UINT"))
                return MET_UINT;
        
   if(!strcmp(_s, "MET_FLOAT"))
                return MET_FLOAT;
        
   if(!strcmp(_s, "MET_DOUBLE"))
                return MET_DOUBLE;
        
   if(!strcmp(_s, "MET_OTHER"))
                return MET_OTHER;
        
   MET_Error = true;
   return MET_OTHER;
        }

//
// METType To String
//
char * MET_TypeToString(MET_Type _pType)
        {
        static char s[80];
        switch(_pType)
                {
                case MET_DEFAULT:
                        sprintf(s, "MET_DEFAULT");
                        break;
                case MET_CHAR:
                        sprintf(s, "MET_CHAR");
                        break;
                case MET_UCHAR:
                        sprintf(s, "MET_UCHAR");
                        break;
                case MET_SHORT:
                        sprintf(s, "MET_SHORT");
                        break;
                case MET_USHORT:
                        sprintf(s, "MET_USHORT");
                        break;
                case MET_INT:
                        sprintf(s, "MET_INT");
                        break;
                case MET_UINT:
                        sprintf(s, "MET_UINT");
                        break;
                case MET_FLOAT:
                        sprintf(s, "MET_FLOAT");
                        break;
                case MET_DOUBLE:
                        sprintf(s, "MET_DOUBLE");
                        break;
      default:
         MET_Error = true;
                case MET_OTHER:
                        sprintf(s, "MET_OTHER");
                        break;
                }

        return s;
        }

//
// Value to Float
//
float MET_ValueToFloat(MET_Type _pType, const void * _v, bool _ByteOrderMSB)
        {
   bool s = false;
   if(_ByteOrderMSB != MET_SYSTEM_BYTE_ORDER_MSB)
      s = true;
        switch(_pType)
                {
                case MET_CHAR:
                        return (float)(*((char *)_v));
                case MET_UCHAR:
                        return (float)(*((unsigned char *)_v));
                case MET_SHORT:
         if(s)
            return (float)MET_SWAPS(*((short *)_v));
           else
            return (float)(*((short *)_v));
                case MET_USHORT:
         if(s)
                        return (float)MET_SWAPS(*((unsigned short *)_v));
           else
                        return (float)(*((unsigned short *)_v));
                case MET_INT:
         if(s)
                        return (float)MET_SWAPL(*((int *)_v));
           else
                        return (float)(*((int *)_v));
                case MET_UINT:
                        if(s)
            return (float)MET_SWAPL(*((unsigned int *)_v));
                          else
            return (float)(*((unsigned int *)_v));
                case MET_FLOAT:
                        if(s)
            return (float)MET_SWAPL(*((int *)_v));
                          else
            return (float)(*((float *)_v));
                case MET_DOUBLE:
                        if(s) // Not completed! - Not sure how to swap doubles!
            return (float)(*((double *)_v));
                          else
            return (float)(*((double *)_v));
                default:
         MET_Error = true;
                        return 0;
                }
        }

//
// Sizeof METTYPE
//
int MET_SizeOfType(MET_Type _pType)
        {
        switch(_pType)
                {
                case MET_CHAR:
                case MET_UCHAR:
                        return 1;
                case MET_SHORT:
                case MET_USHORT:
                        return 2;
                case MET_INT:
                case MET_UINT:
                case MET_FLOAT:
                        return 4;
                case MET_DOUBLE:
                        return 8;
                default:
         MET_Error = true;
                        return 0;
                }
        }

//
//   MET_ALLOC
//
void * MET_Alloc(MET_Type _pType, int _x)
        {
        void * d;

        d = calloc(_x, MET_SizeOfType(_pType));
        if(d == NULL)
                {
                std::cout << "ERROR: MI_Alloc(" << MET_TypeToString(_pType) << ", " << _x << ") returned NULL" << std::endl;
                MET_Error = true;
      return NULL;
                }

        return d;
        }

//
void * MET_Alloc(MET_Type _pType, int _y, int _x)
        {
        void * d;

        d = calloc(_y*_x, MET_SizeOfType(_pType));
        if(d == NULL)
                {
                std::cout << "ERROR: MI_Alloc(" << MET_TypeToString(_pType);
      std::cout << ", " << _y << ", " << _x;
      std::cout << ") returned NULL" << std::endl;
                MET_Error = true;
                return NULL;
                }

        return d;
        }

//
void * MET_Alloc(MET_Type _pType, int _z, int _y, int _x)
        {
        void * d;

        d = calloc(_z*_y*_x, MET_SizeOfType(_pType));
        if(d == NULL)
                {
                std::cout << "ERROR: MI_Alloc(" << MET_TypeToString(_pType);
                std::cout << ", " << _z << ", " << _y << ", " << _x;
                std::cout << ") returned NULL" << std::endl;
                MET_Error = true;
                return NULL;
                }

        return d;
        }

//
void * MET_Alloc(MET_Type _pType, int _s, int _z, int _y, int _x)
        {
        void * d;

        d = calloc(_s*_z*_y*_x, MET_SizeOfType(_pType));
        if(d == NULL)
                {
                std::cout << "ERROR: MI_Alloc(" << MET_TypeToString(_pType);
                std::cout << ", " << _s << ", " << _z << ", " << _y << ", " << _x;
      std::cout << ") returned NULL" << std::endl;
        MET_Error = true;
                return NULL;
                }

        return d;
        }

//
void * MET_Alloc(MET_Type _pType, int _nDims, const int *_dimSize)
        {
        int i, q=1;
        for(i=0; i<_nDims; i++)
                q *= _dimSize[i];

        return MET_Alloc(_pType, q);
        }

//
//
//
bool MET_GetFilePath(const char *_fName, char *_fPath)
   {
   int i;

   int l = strlen(_fName);

   for(i=l-1; i>=0; i--)
      if(_fName[i] == '\\' || _fName[i] == '/')
         break;

   if(i >= 0 && (_fName[i] == '/' || _fName[i] == '\\'))
      {
      strcpy(_fPath, _fName);
      _fPath[i+1] = '\0';
      return true;
      }
   else
      {
      _fPath[0] = '\0';
      return false;
      }
   }

//
//
//
int  MET_GetFileSuffixPtr(const char *_fName)
   {
   int i = strlen(_fName);
   while(i>0)
      if(_fName[i-1] == '.')
         break;
        else
         i--;
   return i;
   }

//
//
//
bool    MET_SetFileSuffix(char *_fName, const char *_suf)
   {
   int i = MET_GetFileSuffixPtr(_fName);
   if(i>0)
      {
      if(_suf[0] == '.')
         _fName[i-1] = '\0';
        else
         _fName[i] = '\0';
      strcat(_fName, _suf);
      return true;
      }
     else
      return false;
   }
