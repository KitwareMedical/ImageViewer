#ifndef MetaImageTypes_H
#define MetaImageTypes_H

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

/*!
 *    MetaImages can contain medical image information such as the source modality of the image
 *    Known medical image modality types are defined here
 */
typedef enum         {MET_MOD_UNKNOWN,
                      MET_MOD_MRI,
                      MET_MOD_MRA,
                      MET_MOD_CT,
                      MET_MOD_DSA,
                      MET_MOD_CXR,
                      MET_MOD_DRR} MET_ModType;

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

#endif //MetaImageTypes_H