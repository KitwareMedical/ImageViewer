#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <ctype.h>


#include "MetaFileLib.h"

char sepChar = '=';

//
bool MF_SkipToVal(FILE *fp)
	{
	char c;
   if(feof(fp))
      return false;

	fscanf(fp, "%c", &c);
	while(c != sepChar && !feof(fp))
		fscanf(fp, "%c", &c);

	if(feof(fp))
		{
		std::cout << "Incomplete file record definition" << std::endl;
		return false;
		}
	while((c == sepChar || c == ' ' || c == '\t') && !feof(fp))
		fscanf(fp, "%c", &c);
	fseek(fp, -1, SEEK_CUR);
	if(feof(fp))
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
bool MF_Read(FILE *fp, int nFields, MF_FieldRec *field, int fTerm, bool fromTopOfFile, char _sepChar)
	{
	char s[255];
	char c;
	int i, j;
	for(i=0; i<nFields; i++)
		field[i].defined = false;

   sepChar = _sepChar;

	int found;

   if(fromTopOfFile)
      fseek(fp, 0, SEEK_SET);

	while(1)
		{
		c = ' ';
		while((c == ' ' || c == '\t' || c == '\n') && !feof(fp))
			fscanf(fp, "%c", &c);
		if(feof(fp))
			return MF_IsComplete(nFields, field);
		fseek(fp, -1, SEEK_CUR);

      i = 0;
      fscanf(fp, " %c", &s[i]);
      while(i<255 && s[i]!=sepChar && !feof(fp))
         s[++i] = getc(fp);
      if(i>=255)
         return MF_IsComplete(nFields, field);
      while((s[i] == sepChar || s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && i>0)
         s[i--] = '\0';
      if(feof(fp))
         return MF_IsComplete(nFields, field);
		fseek(fp, -1, SEEK_CUR);

		found = 0;
		for(i=0; i<nFields; i++)
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
						if(!feof(fp))
                     fscanf(fp, "%c", &c);
						break;
					case MF_CHAR:
                  char c;
						MF_SkipToVal(fp);
						fscanf(fp, "%c", &c);
                  field[i].val[0] = (float)c;
						break;
					default:
					case MF_INT:
					case MF_FLOAT:
						MF_SkipToVal(fp);
						fscanf(fp, " %e ", field[i].val);
						break;
					case MF_CHAR_ARRAY:
						MF_SkipToVal(fp);
						if(field[i].dependsOn >= 0)
                     {
                     field[i].length = (int)(field[field[i].dependsOn].val[0]);
                     fgets((char *)field[i].val, field[i].length+1, fp);
                     }
						  else
                     {
                     fgets((char *)field[i].val, 800, fp);
                     int j = 0;
                     while(isprint(((char *)field[i].val)[j]))
                        j++;
                     while(isspace(((char *)field[i].val)[j]))
                        j--;
                     ((char *)(field[i].val))[j+1] = '\0';
                     field[i].length = strlen((char *)field[i].val);
                     }
						break;
					case MF_INT_ARRAY:
					case MF_FLOAT_ARRAY:
						MF_SkipToVal(fp);
						if(field[i].dependsOn >= 0)
                     {
                     field[i].length = (int)field[field[i].dependsOn].val[0];
							for(j=0; j<field[i].length; j++)
								fscanf(fp, " %e ", &field[i].val[j]);
                     }
						  else
							{
                     if(field[i].length <= 0)
                        {
							   std::cout << "Float and Int arrays must have dependency" << std::endl;
						      return false;
                        }
							for(j=0; j<field[i].length; j++)
								fscanf(fp, " %e ", &field[i].val[j]);
							}
						break;
					}
				found = 1;
				field[i].defined = true;
				if((int)i == fTerm)
					return MF_IsComplete(nFields, field);
				}
		if(!found)
         fgets(s, 255, fp);
		}

	return true;
	}

//
bool MF_Write(FILE *fp, int nFields, MF_FieldRec *field, char _sepChar)
   {
   sepChar = _sepChar;

   int i, j;
   for(i=0; i<nFields; i++)
      {
      switch(field[i].type)
         {
         case MF_NONE:
            fprintf(fp, "%s\n", field[i].name);
            break;
         case MF_CHAR:
            fprintf(fp, "%s %c %c\n", field[i].name, sepChar, (char)field[i].val[0]);
            break;
         case MF_INT:
            fprintf(fp, "%s %c %d\n", field[i].name, sepChar, (int)field[i].val[0]);
            break;
         case MF_FLOAT:
            fprintf(fp, "%s %c %e\n", field[i].name, sepChar, field[i].val[0]);
            break;
         case MF_CHAR_ARRAY:
            fprintf(fp, "%s %c ", field[i].name, sepChar);
            if(field[i].dependsOn >= 0)
               if(field[i].length != field[field[i].dependsOn].val[0])
                  std::cout << "Warning: length and dependsOn values not equal in write" << std::endl;
            for(j=0; j<field[i].length; j++)
               fprintf(fp, "%c", (char)field[i].val[j]);
            fprintf(fp, "\n");
            break;
         case MF_INT_ARRAY:
            fprintf(fp, "%s %c", field[i].name, sepChar);
            if(field[i].dependsOn >= 0)
               if(field[i].length != field[field[i].dependsOn].val[0])
                  std::cout << "Warning: length and dependsOn values not equal in write" << std::endl;
            for(j=0; j<field[i].length; j++)
               fprintf(fp, " %d", (int)field[i].val[j]);
            fprintf(fp, "\n");
            break;
			case MF_FLOAT_ARRAY:
            fprintf(fp, "%s %c", field[i].name, sepChar);
            if(field[i].dependsOn >= 0)
               if(field[i].length != field[field[i].dependsOn].val[0])
                  std::cout << "Warning: length and dependsOn values not equal in write" << std::endl;
            for(j=0; j<field[i].length; j++)
               fprintf(fp, " %e", (float)field[i].val[j]);
            fprintf(fp, "\n");
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
