/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VDSegGrayRun.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <FL/fl_file_chooser.H>
#include "fltkImageViewer.h"
#include "GLSliceView.h"
#include "itkVoronoiSegmentationImageFilter.h"

class VDSegGrayRun
{

public:
  typedef itk::Image<float,3> FloatImage;
  typedef itk::Image<float,2> FloatImage2D;
  typedef FloatImage::IndexType IndexType;
  typedef itk::Image<bool,3> BinImage;
  typedef itk::Image<bool,2> BinImage2D;
  typedef GLSliceView<float,bool>   ViewerType;
  typedef itk::VoronoiSegmentationImageFilter<FloatImage2D,BinImage2D> VDFilter;

  VDSegGrayRun();
  ~VDSegGrayRun();

protected:
  virtual bool LoadImage (void);
  virtual int Load (const char * inputFileName);
  void ReadImage(const char * inputFileName);  
  virtual bool LoadPrior (void);

  void ClearClicked(void); 
  void ComputeStats(void); 
  void GenerateData();
  void GenerateDataPrior();
  void GenerateDataStandard();
  void ShowResult();
  void Save(void);
  void Save(const char *outputFileName);


  unsigned int m_Cols;
  unsigned int m_Rows;
  unsigned int m_Pages;
  float m_Mean;
  float m_STD;
  float m_MeanRatio;
  float m_STDRatio;
  unsigned int m_SampleRegion;

  char m_Datatype;
  unsigned int m_Offset;

  unsigned long m_ImageSize;

  FloatImage::Pointer m_ViewResultImage;
  FloatImage::Pointer m_DataImage;

  FloatImage2D::Pointer m_WorkingImage;

  ViewerType *m_Viewer;
  ViewerType *m_ViewerResult;
  unsigned int m_DisplaySlice;


  FloatImage::RegionType m_ImageRegion;
  FloatImage2D::RegionType m_ImageRegion2D;

  bool m_UsePrior;
  bool m_OutputBnd;
  unsigned int m_InitSeeds;
  unsigned int m_MinRegion;
  char m_PriorFileName[500];
};


