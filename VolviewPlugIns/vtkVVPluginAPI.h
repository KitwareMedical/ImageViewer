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
// .NAME vtkVVPluginAPI - a C API for plugins to add their own algorithms
// .SECTION Description
// This API provides a way for filters to be added to volview at runtime 

#ifndef vtkVVPluginAPI_h
#define vtkVVPluginAPI_h

#ifdef  __cplusplus
extern "C" {
#endif

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
#define VV_GUI_SCALE    0
#define VV_GUI_CHOICE   1
#define VV_GUI_CHECKBOX 2
  
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

  typedef int (*VV_PROCESS_DATA_FUNCTION)(void *info, 
                                          vtkVVProcessDataStruct *pds);
  typedef int (*VV_UPDATE_GUI_FUNCTION)(void *info);
  
  /* this is the data structure describing one GUI element. */
  typedef struct {
    /* the label for this widget */
    const char *Label; 
    /* what type of GUI item should this be, menu, slider etc */    
    int GUIType;         
    /* the initial value for the widget */
    const char *Default;
    /* a help string for baloon help for this widget */
    const char *Help;
    /* this string is where additional information required to setup the GUI
     * element is specified. What goes in the Hints varies depending on the
     * type of the GUI item. For a slider it is the range of the slider
     * e.g. "0 255" */
    const char *Hints;
    /* this is where the current value of the GUI item will be stored */
    char CurrentValue[1024];
  } vtkVVGUIItem;
  
  typedef struct {
    /* these members are passed in by VolView for use by the plugin */
    unsigned char magic1;
    unsigned char magic2;
    /* a pointer to the vtkVVPLugin object */
    void *Self;
    /* characteristics of the input data */
    int InputVolumeScalarType;
    int InputVolumeNumberOfComponents;
    int InputVolumeDimensions[3];
    float InputVolumeSpacing[3];
    float InputVolumeOrigin[3];

    /* a flag indicating that the plugin should abort processing */
    int AbortProcessing;
    
    /* these are the methods that a plugin can invoke on VolView */
    /* when an error occurs, call this function to display the error string */
    void  (*DisplayError) (void *info, const char *err);

    /* the following function can be called to update progress */
    void  (*UpdateProgress) (void *info, float progress, const char *msg);

    /* for plugins that generate polygonal data they should call the */
    /* following function to assign the polygonal data to VolView */
    /* after which they can free any local memory that was being used */
    void  (*AssignPolygonalData) (void *info, vtkVVProcessDataStruct *pds);
    
    /* these members must be set by the plugin */
    /* a short name for the plugin to be used in the menu to select it */
    const char *Name;

    /* a short one sentence description of the plugin */
    const char *TerseDocumentation;

    /* a long description of the plugin and how it works, can be very long */
    const char *FullDocumentation;

    /* does this plugin support procesing the data in pieces (slabs) */
    int SupportsProcessingPieces;

    /* does this plugin support processing the data in place. Normally the
     * output volume is a separate block of memory from the input data. But
     * for some filters they can write the output directly onto the input
     * volume. This is called InPlace processing */
    int SupportsInPlaceProcessing;

    /* an estimate of how much memory per input voxel component will be
     * consumed by the filter in bytes. This does not include the input and
     * output volumes but does include intermediate results. */
    float PerVoxelMemoryRequired;
    
    /* does this plugin only produce a mesh ? */
    int ProducesMeshOnly;
    
    /* for plugins that support processing pieces, does it require any Z axis
     * overlap. If so how many slices of overlap are required */
    int RequiredZOverlap;

    /* specify the charateristics of the output data */
    int OutputVolumeScalarType;
    int OutputVolumeNumberOfComponents;
    int OutputVolumeDimensions[3];
    float OutputVolumeSpacing[3];
    float OutputVolumeOrigin[3];
    
    /* specify the GUI for this plugin. How many elements are there? */
    int NumberOfGUIItems;

    /* allocate an array of vtkVVGUIItem structures and fill them in */
    vtkVVGUIItem *GUIItems;
    
    /* this is the function you must provide to process the data */
    VV_PROCESS_DATA_FUNCTION ProcessData;

    /* this is the function you must provide to update the GUI */
    VV_UPDATE_GUI_FUNCTION UpdateGUI;

  } vtkVVPluginInfo;

  /* this is the type of the init function for the plugin. It must be
   * declared as a static "C" function with VV_PLUGIN_EXPORT. The name must
   * be "vv" plus the name of the plugin plus "Init". */
  typedef void (*VV_INIT_FUNCTION)(vtkVVPluginInfo *);
  
#ifdef  __cplusplus
}
#endif

#endif
