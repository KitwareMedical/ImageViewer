/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVVPluginAPI.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 1998-1999 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
#ifndef vtkVVPluginAPI_h
#define vtkVVPluginAPI_h

#ifdef  __cplusplus
extern "C" {
#endif

/*=========================================================================

The following are the different types of parameters that the plugin API
  supports 
  
VVP_ERROR 

VVP_NAME - a short name for the plugin to be used in the menu to select it 

VVP_TERSE_DOCUMENTATION - a short one sentence description of the plugin 

VVP_FULL_DOCUMENTATION 
  a long description of the plugin and how it works, can be very long 

VVP_SUPPORTS_IN_PLACE_PROCESSING
  does this plugin support processing the data in place. Normally the output
  volume is a separate block of memory from the input data. But for some
  filters they can write the output directly onto the input volume. This is
  called InPlace processing

VVP_SUPPORTS_PROCESSING_PIECES 
  does this plugin support procesing the data in pieces (slabs) 

VVP_NUMBER_OF_GUI_ITEMS 
  specify the GUI for this plugin. How many elements are there? 

VVP_PRODUCES_MESH_ONLY - does this plugin only produce a mesh? 

VVP_REQUIRED_Z_OVERLAP
  for plugins that support processing pieces, does it require any Z axis
  overlap. If so how many slices of overlap are required 

VVP_PER_VOXEL_MEMORY_REQUIRED
  an estimate of how much memory per input voxel component will be
  consumed by the filter in bytes. This does not include the input and
  output volumes but does include intermediate results. 

VVP_ABORT_PROCESSING  
  a flag indicating that the plugin should abort processing

=========================================================================*/

  
/* These types are returned by GetDataType to indicate pixel type. */
#ifndef VTK_VOID  
#define VTK_VOID            0
#define VTK_BIT             1 
#define VTK_CHAR            2
#define VTK_UNSIGNED_CHAR   3
#define VTK_SHORT           4
#define VTK_UNSIGNED_SHORT  5
#define VTK_INT             6
#define VTK_UNSIGNED_INT    7
#define VTK_LONG            8
#define VTK_UNSIGNED_LONG   9
#define VTK_FLOAT          10
#define VTK_DOUBLE         11 
#define VTK_ID_TYPE        12
#endif
  
/* the following defines enumerate the different GUI components supported by
 * plugins.  */
#define VV_GUI_SCALE     0
#define VVP_GUI_SCALE    "scale"
#define VV_GUI_CHOICE    1
#define VVP_GUI_CHOICE   "choice"
#define VV_GUI_CHECKBOX  2
#define VVP_GUI_CHECKBOX "checkbox"

#define VVP_ERROR                         0
#define VVP_NAME                          1
#define VVP_TERSE_DOCUMENTATION           2
#define VVP_FULL_DOCUMENTATION            3
#define VVP_SUPPORTS_IN_PLACE_PROCESSING  4
#define VVP_SUPPORTS_PROCESSING_PIECES    5 
#define VVP_NUMBER_OF_GUI_ITEMS           6
#define VVP_PRODUCES_MESH_ONLY            7
#define VVP_REQUIRED_Z_OVERLAP            8
#define VVP_PER_VOXEL_MEMORY_REQUIRED     9
#define VVP_ABORT_PROCESSING             10  
  
#define VVP_GUI_LABEL   0
#define VVP_GUI_TYPE    1
#define VVP_GUI_DEFAULT 2
#define VVP_GUI_HELP    3
#define VVP_GUI_HINTS   4
#define VVP_GUI_VALUE   5
  
  
/*=========================================================================
CM_PLUGIN_EXPORT should be used by the Init function of all plugins
=========================================================================*/
#ifdef _WIN32
#define VV_PLUGIN_EXPORT  __declspec( dllexport )
#else
#define VV_PLUGIN_EXPORT 
#endif

/*=========================================================================
Finally we define the key data structures and function prototypes
The first struture here is the vtkVVProcessDataStruct. This structure
gets passed into the ProcessData function.
=========================================================================*/
  typedef struct {
    /* the input data pointer */
    void *inData; 
    /* the output data pointer */
    void *outData; 
    /* what slice to start processing on (used for pieces) */    
    int StartSlice;         
    /* how many slices to process right now */
    int NumberOfSlicesToProcess;
    /* if this plugin produces a mesh / surface store it */
    /* in the following entries, these are all set by the plugin */
    /* and allocated by the plugin. VolView will copy them into */
    /* its own data structures, the format of these arrays follows */
    /* VTKs basic vtkPolyData structure */
    int NumberOfMeshPoints;
    float *MeshPoints;
    int NumberOfMeshCells;
    int *MeshCells;
    float *MeshNormals;
    float *MeshScalars;
  } vtkVVProcessDataStruct;

  typedef struct {
    /* these three members should not used by the plugin */
    unsigned char magic1;
    unsigned char magic2;
    void *Self;

    /* these are the characteristics of the input data */
    int InputVolumeScalarType;
    int InputVolumeNumberOfComponents;
    int InputVolumeDimensions[3];
    float InputVolumeSpacing[3];
    float InputVolumeOrigin[3];
    double InputVolumeScalarRange[2]; /* actual scalar range */
    double InputVolumeScalarTypeRange[2]; /* possible scalar range */
    
    /* specify the charateristics of the output data */
    int OutputVolumeScalarType;
    int OutputVolumeNumberOfComponents;
    int OutputVolumeDimensions[3];
    float OutputVolumeSpacing[3];
    float OutputVolumeOrigin[3];

    /* these are the methods that a plugin can invoke on VolView */
    /* the following function can be called to update progress */
    void  (*UpdateProgress) (void *info, float progress, const char *msg);

    /* for plugins that generate polygonal data they should call the */
    /* following function to assign the polygonal data to VolView */
    /* after which they can free any local memory that was being used */
    void  (*AssignPolygonalData) (void *info, vtkVVProcessDataStruct *pds);

    /* memory safe way to set/get properties */
    void        (*SetProperty) (void *info, int property, const char *value);
    const char *(*GetProperty) (void *info, int property);
    void        (*SetGUIProperty) (void *info, int num, int property, 
                                   const char *value);
    const char *(*GetGUIProperty) (void *info, int num, int property);

    /* you must implement the two following functions */
    int (*ProcessData) (void *info, vtkVVProcessDataStruct *pds);
    int (*UpdateGUI)   (void *info);

  } vtkVVPluginInfo;

  /* this is the type of the init function for the plugin. It must be
   * declared as a static "C" function with VV_PLUGIN_EXPORT. The name must
   * be "vv" plus the name of the plugin plus "Init". */
  typedef void (*VV_INIT_FUNCTION)(vtkVVPluginInfo *);
  
#ifdef  __cplusplus
}
#endif

#endif
