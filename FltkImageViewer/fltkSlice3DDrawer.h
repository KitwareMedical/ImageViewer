/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    fltkSlice3DDrawer.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/

#ifndef __fltkSlice3DDrawer_h
#define __fltkSlice3DDrawer_h

#include <fltkSlice3DDrawerGUI.h>
#include <fltkGlDrawer.h>
#include <itkImage.h>
#include <fltkUpdater.h>


namespace fltk {


template <class ImagePixelType>
class Slice3DDrawer : public fltkSlice3DDrawerGUI, 
                      public GlDrawer, 
                      public liUpdater
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef Slice3DDrawer<ImagePixelType>       Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;

 
  typedef itk::Image< ImagePixelType, 3 >   ImageType;
 

  /**
   *  Creation through the factory
   */
   itkNewMacro( Self );


  /**
   * Default constructor
   */
   Slice3DDrawer();


  /**
   * Destructor
   */
  ~Slice3DDrawer();


  /**
   * Set Input Image
   */
  void SetInput( ImageType * );

  /**
   * Update GUI with image data
   */
  void RefreshGUI( void );

  /**
   * Show
   */
  void Show(void);


  /**
   * Hide
   */
  void Hide(void);


  /**
   * Select slice X
   */
   void SelectSliceX(void);


   /**
   * Select slice Y
   */
   void SelectSliceY(void);


   /**
   * Select slice Z
   */
   void SelectSliceZ(void);


  /**
   * Draw the target in Open Gl
   */
   void glDraw(void);


  /**
   * Bind texture for Slice X
   */
   void BindTextureX(void);

   
  /**
   * Bind texture for Slice Y
   */
   void BindTextureY(void);


  /**
   * Bind texture for Slice Z
   */
   void BindTextureZ(void);



private:

   ImageType::Pointer      m_Volume;
   
   int                    m_Nx;
   int                    m_Ny;
   int                    m_Nz;
   
   int                    m_Nxr;
   int                    m_Nyr;
   int                    m_Nzr;
   
   float                  m_Dx;
   float                  m_Dy;
   float                  m_Dz;

   float                  m_Cx;
   float                  m_Cy;
   float                  m_Cz;
   
   unsigned char        * m_SliceX;
   unsigned char        * m_SliceY;
   unsigned char        * m_SliceZ;

   unsigned char        * m_TextureX;
   unsigned char        * m_TextureY;  
   unsigned char        * m_TextureZ;
   GLuint                  m_TextureName[3];

   ImagePixelType         m_Max_Value;

   bool                   texturesGenerated;

};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkSlice3DDrawer.txx>
#endif




#endif




