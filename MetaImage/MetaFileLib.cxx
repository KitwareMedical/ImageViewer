#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <ctype.h>


#include "MetaFileLib.h"

char sepChar = '=';

//
bool MF_SkipToVal(std::ifstream &fp)
  {
  char c;
  if( fp.eof() )
  {
    return false;
  }
  c = fp.get();
  while( c != sepChar && !fp.eof() )
  {
    c = fp.get();
  }

  if( fp.eof() )
    {
    std::cout << "Incomplete file record definition" << std::endl;
    return false;
    }
  return true;
  }

//
bool MF_IsComplete(int nFields, MF_FieldRec *field)
   {
   int i;
   for(i=0; i<nFields; i++)
    if(field[i].required && !field[i].defined)
      {
      std::cout << field[i].name << " required and not defined." << std::endl;
      return false;
      }
   return true;
   }

//
//bool MF_Read(FILE *fp, int nFields, MF_FieldRec *field, int fTerm, bool fromTopOfFile, char _sepChar)
bool MF_Read(std::ifstream &fp, int nFields, MF_FieldRec *field, int fTerm, bool fromTopOfFile, char _sepChar)
  {
  char s[255];
  int i, j;
  for(i=0; i<nFields; i++)
  {
    field[i].defined = false;
  }

   sepChar = _sepChar;

  int found;

   if(fromTopOfFile) 
   {
     fp.seekg( 0, std::ios::beg ); //      fseek(fp, 0, SEEK_SET);
   }


  while(1)
    {
    fp.get( s, 255, sepChar );
    if( fp.eof() )
    {
      return MF_IsComplete(nFields, field);
    }

    i = strlen( s )-1;
    while((s[i] == sepChar || s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && i>0)
    {
      s[i--] = '\0';
    }
    if( fp.eof() )
    {
      return MF_IsComplete(nFields, field);
    }

    found = 0;
    for(i=0; i<nFields; i++)
    {
      if(!strcmp(field[i].name, s))
        {
        if(field[i].dependsOn >= 0)
          if(!field[field[i].dependsOn].defined)
            {
            std::cout << field[i].name << " defined prior to defining " << field[field[i].dependsOn].name << std::endl;
            return false;
            }
        switch(field[i].type)
          {
          case MF_NONE:
            if( !fp.eof() )
            {
              char c = fp.get();
            }
            break;
          case MF_CHAR:
            {
            MF_SkipToVal(fp);
            char c = fp.get();
            field[i].val[0] = (float)c;
            fp.getline( s,500 );
            break;
            }
          default:
          case MF_INT:
            {
            MF_SkipToVal(fp);
            fp >> field[i].val[0];
            fp.getline( s,500 );
            break;
            }
          case MF_FLOAT:
            MF_SkipToVal(fp);
            fp >> field[i].val[0];
            fp.getline( s,500 );
            break;
          case MF_CHAR_ARRAY:
            MF_SkipToVal(fp);
            if(field[i].dependsOn >= 0)
                     {
                     field[i].length = (int)(field[field[i].dependsOn].val[0]);
                     fp.read( (char *)field[i].val, field[i].length+1 );
                     }
              else
                     {
                     char * str = (char *)field[i].val;
                     fp.getline(str,500);
                     char * start = str;
                     while( isspace( *start ) ) start++;
                     char * compact = str;
                     while( *start && *start != '\r' )
                     {
                       *compact = *start;
                       start++;
                       compact++;
                     }
                     *compact = '\0';
                     field[i].length = strlen( str );
                     strcpy( (char *)(field[i].val), str );
                     }
            break;
          case MF_INT_ARRAY:
            {
            MF_SkipToVal(fp);
            if(field[i].dependsOn >= 0)
             {
              field[i].length = (int)field[field[i].dependsOn].val[0];
              for(j=0; j<field[i].length; j++) 
              {
                fp >> field[i].val[j];
              }
             }
             else
             {
               if(field[i].length <= 0)
               {
                 std::cout << "Int arrays must have dependency" << std::endl;
                 return false;
               }
               for(j=0; j<field[i].length; j++)
               {
                 fp >> field[i].val[j];
               }
              }
              fp.getline( s,500 );
            break;
            }
          case MF_FLOAT_ARRAY:
            {
            MF_SkipToVal(fp);
            if(field[i].dependsOn >= 0)
             {
              field[i].length = (int)field[field[i].dependsOn].val[0];
              for(j=0; j<field[i].length; j++) 
              {
                fp >> field[i].val[j];
              }
             }
             else
             {
               if(field[i].length <= 0)
               {
                 std::cout << "Float arrays must have dependency" << std::endl;
                 return false;
               }
               for(j=0; j<field[i].length; j++)
               {
                 fp >> field[i].val[j];
               }
              }
              fp.getline( s,500 );
            break;
            }
          }
          found = 1;
          field[i].defined = true;
          if((int)i == fTerm)
          {
            return MF_IsComplete(nFields, field);
          }
        }
        if( found )
        {
          break;
        }
      }



    }

  return true;
  }

//
bool MF_Write(std::ofstream &fp, int nFields, MF_FieldRec *field, char _sepChar)
   {
   sepChar = _sepChar;

   int i, j;
   for(i=0; i<nFields; i++)
      {
      switch(field[i].type)
         {
         case MF_NONE:
            fp << field[i].name << std::endl;
            break;
         case MF_CHAR:
            fp << field[i].name << " " << sepChar << " " << (char)field[i].val[0] << std::endl;
            break;
         case MF_INT:
            fp << field[i].name << " " << sepChar << " " << (int)field[i].val[0] << std::endl;
            break;
         case MF_FLOAT:
            fp << field[i].name << " " << sepChar << " " << (float)field[i].val[0] << std::endl;
            break;
         case MF_CHAR_ARRAY:
            fp << field[i].name << " " << sepChar;
            if(field[i].dependsOn >= 0) 
            {
               if(field[i].length != field[field[i].dependsOn].val[0])
               {
                  std::cout << "Warning: length and dependsOn values not equal in write" << std::endl;
               }
            }
            char temp_str[80] ;
            for (j = 0 ; j < field[i].length ; j++)
              temp_str[j] = (char) field[i].val[j] ;
            fp.write(temp_str, field[i].length );
            fp << std::endl;
            break;
         case MF_INT_ARRAY:
            fp << field[i].name << " " << sepChar;
            if(field[i].dependsOn >= 0)
            {
               if(field[i].length != field[field[i].dependsOn].val[0])
               {
                  std::cout << "Warning: length and dependsOn values not equal in write" << std::endl;
               }
            }
            for(j=0; j<field[i].length; j++)
            {
              fp << " " << (int)field[i].val[j];
            }
            fp << std::endl;
            break;
      case MF_FLOAT_ARRAY:
            fp << field[i].name << " " << sepChar;
            if(field[i].dependsOn >= 0)
            {
               if(field[i].length != field[field[i].dependsOn].val[0])
               {
                  std::cout << "Warning: length and dependsOn values not equal in write" << std::endl;
               }
            }
            for(j=0; j<field[i].length; j++)
            {
              fp << " " << (float)field[i].val[j];
            }
            fp << std::endl;
        break;
         }
      }
   return true;
   }

bool MF_ParseStringToCHAR_ARRAY(char *s, int *n, char ***val)
{
    int l = strlen(s);

    int p = 0;
    while(p<l && s[p] == ' ')
        p++;

    sscanf(&s[p], "%d ", n);
    while(p<l && s[p] != ' ')
        p++;

    *val = new char*[*n];

    int i, j;
    for(i=0; i<*n; i++) {
        if(p == l)
            return false;
        (*val)[i] = new char [80];
        while(p<l && s[p] == ' ')
            p++;
        j = 0;
        while(p<l && s[p] != ' ')
            (*val)[i][j++] = s[p++];
        (*val)[i][j] = '\0';
    }

    return true;

}

bool WriteFieldToFile(std::ofstream & _fp, const char *_fieldName,
                       MF_ValType _pType, int _n, const void *_v)
{
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
      for(i = 0; i < _n; i++)
      {
        f.val[i] = (float)(((char *)_v)[i]);
      }
      break;
    case MF_INT:
    case MF_INT_ARRAY:
      for(i=0; i<_n; i++)
      {
        f.val[i] = (float)(((int *)_v)[i]);
      }
      break;
    case MF_FLOAT:
    case MF_FLOAT_ARRAY:
      for(i=0; i<_n; i++)
      {
        f.val[i] = ((float *)_v)[i];
      }
      break;
    default:
      break;
  }

  MF_Write(_fp, 1, &f);

  return true;
}

bool WriteFieldToFile(std::ofstream & _fp, const char *_fieldName,
                      MF_ValType _pType, float _v)
{
  MF_FieldRec f;

  sprintf(f.name, "%s", _fieldName);
  f.defined = false;
  f.dependsOn = -1;
  f.length = 1;
  f.required = false;
  f.type = _pType;
  f.val[0] = _v;

  MF_Write(_fp, 1, &f);

  return true;
}

