//
// File:
//    MetaFileLib (.h and .cpp)
//
// Description:
//    This file provides generic ascii file parsing capabilities.
//    It assumes that the files consist of a set of fields
//    Each field is list of variable = value pairs
//
// Features:
//    There can be dependencies between fields, required fields, and optional fields.
//    Undefined fields are skipped.
//    Values must conform to expected types.   There can be default values for fields.
//
// Author:
//    Stephen R. Aylward
//
// Date:
//    August 29, 1999
//
// Depends on:
//    MetaUtils.h
//
#ifndef METAFILELIB_H
#define METAFILELIB_H

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "MetaUtils.h"     // Generic Memory/IO/Debug/Misc Routines


// Given an array of fieldRec that define a metaFile format, parse that file
//    fTerm specifies what field (if any) marks the field (variable name)
//       at which file parsing should stop (e.g., if binary data follows)
//    fromTopOfFile indicates whether the File pointer fp should be reset
//       to topOfFile before parsing begins

//extern bool MF_Read(FILE *fp,
extern bool MF_Read(std::ifstream &fp,
                    int nFields,
                    MF_FieldRec *field,
                    int fTerm=-1,
                    bool fromTopOfFile=true,
                    char _sepChar='=');

// Given an array of fieldRecs, creates a metaFile.
//extern bool MF_Write(FILE *fp,
extern bool MF_Write(std::ofstream &fp,
                     int nFields,
                     MF_FieldRec *field,
                     char _sepChar='=');

extern bool MF_ParseStringToCHAR_ARRAY(char *s, int *n, char ***val);

extern bool WriteFieldToFile(FILE* _fp, const char *_fieldName,
              	 						 MF_ValType _pType, int _n, const void *_v);

extern bool WriteFieldToFile(FILE* _fp, const char *_fieldName,
														 MF_ValType _pType, float _v);

#endif
