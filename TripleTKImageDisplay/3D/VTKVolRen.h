/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VTKVolRen.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VTKVolRen_h
#define _VTKVolRen_h
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkStructuredPoints.h>
#include <vtkPiecewiseFunction.h>
#include <vtkUnsignedCharArray.h>
#include "itkImage.h"
#include "itkSimpleImageRegionIterator.h"

/** \class VTKVolRen
 * \brief Class of vtk convenience functions painful to work out.
 *
 * This class does nothing but save code in an easy place to find and use.
 * All functions return true on success and false on failure. Reasons for
 * failure can be retrieved by calling Error(). Error() returns null if no
 * error is present, so check before use.
 */
template <class T>
class VTKVolRen
{
  typedef itk::Image<T,3>  ImageType;
  typedef itk::Image<unsigned char,3>  OverlayType;

  typedef typename ImageType::Pointer    ImagePointer;
  typedef typename OverlayType::Pointer  OverlayPointer;

protected:

  char    mError[200];
  double  mStepSize;//for step volue rendering. Should be
  //set to the minimum of xstep, ystep, zstep. Default
  //is 0.08.

  double  mOpacity;  //for setting global opacity of a volume. Default 1.0.
  //Will not affect sites excluded by opacmask.

  double  mIntensityMin;//voxels of opacask intensity will be set
  //to an opacity of 0. This value can therefore
  //be used as a ask to exclude soe areas of a dataset
  //if unwanted regions are set to opacask intensity.
  //If you don't want this feature, set opacval to
  //256 or -1 or soething out of range of uchar.
  //Default value is 0, which isn't a very useful
  //value for MIP projections anyway.

  ImagePointer     mImData;
  OverlayPointer   mMask;
  bool             mUseMask;

  vtkStructuredPoints      *mVol;
  vtkUnsignedCharArray     *mScalars;
  vtkVolume                *mActor;
  vtkPiecewiseFunction     *mColorFunc;
  vtkPiecewiseFunction     *mOpacityTransferFunc;
  vtkVolumeProperty        *mProperty;
  vtkVolumeRayCastMapper   *mMapper;
        
public:

  VTKVolRen();

  char*    error(void);

    //Volume Rendering Stuff
    //Greyscale volume for MIP volue rendering. Vtk's volume is buggy
    //for unequal axial spacings. Volume will therefore
    //create a volume with stepsizes = 1; the caller must scale 
    //the actor by xstep, ystep, zstep. One also should not use
    //vtkSetOrigin() with these volumes--that's buggy too. If you
    //need an offset, you must provide it by SetUserMatrix().

  bool    SetInputImage( ImageType * newIm );

  void    stepSize(double newStepSize);
  double  stepSize(void);
    
  void    opacity(double newOpacity);  //val should be between 0 and 1
  double  opacity(void);

  void    intensityMin(double newIntensityMin);
  double  intensityMin(void);

  void    maskData(OverlayType::Pointer newMask);
  OverlayType::Pointer maskData(void);

  void  useMask(bool newUseMask);
  bool  useMask(void);

  vtkVolume               * actor(void);
  vtkPiecewiseFunction    * colorFunc(void);
  vtkPiecewiseFunction    * opacityFunc(void);
  vtkVolumeProperty       * property(void);
  vtkVolumeRayCastMapper  * mapper(void);
  vtkStructuredPoints     * volume(void);
  vtkUnsignedCharArray    * scalars(void);

  void update(void);
};

//
//
//
template <class T>
VTKVolRen<T>::VTKVolRen()
{
  *mError = '\0';

  mStepSize = 0.8;        //volume rendering default stepsize
  mOpacity = 0.8;         //global opacity of a volume
  mIntensityMin = 0;      //voxels of this intensity will be set to 0 opacity

  mImData = ImageType::New();
  mVol = NULL;
  mScalars = NULL;

  mUseMask = false;
  mMask = NULL;

  // Create a transfer function mapping scalar value to color (grey)
  mColorFunc = vtkPiecewiseFunction::New();
  mColorFunc->AddSegment(0, 0.0, 255, 1.0);

  /*  
  //turns out the above smaller function is sufficient for greyscale; however
  //save the following function for reference because you might want it later.
  //It is good for color mapping.
 
  vtkColorTransferFunction* ctfun = vtkColorTransferFunction::New();
    ctfun->AddRGBSegment(0, 0.0, 0.0, 0.0, 
                             255, 1.0, 1.0, 1.0 );
  */

  //All voxels of volIntensityMin intensity will have an opacity of 0.
  //The opacity of remaining voxels is volOpacity 
  mOpacityTransferFunc = vtkPiecewiseFunction::New();
  mOpacityTransferFunc->AddPoint(0, 0);
  mOpacityTransferFunc->AddSegment(1, mOpacity, 255, mOpacity);

  mProperty = vtkVolumeProperty::New();
    //mProperty->SetColor(mColorFunc);
    //mProperty->SetInterpolationTypeToLinear();
    //mProperty->SetScalarOpacity(mOpacityTransferFunc);

  vtkVolumeRayCastMIPFunction *rt = vtkVolumeRayCastMIPFunction::New();
  mMapper = vtkVolumeRayCastMapper::New();
  mMapper->SetVolumeRayCastFunction(rt);
  mMapper->SetSampleDistance(mStepSize);    //class data

  rt->Delete();

  mActor = vtkVolume::New();
}

//
//
//
template <class T>
void VTKVolRen<T>::stepSize(double newStepSize)
  {
  mStepSize = newStepSize;
  }

template <class T>
double VTKVolRen<T>::stepSize(void)
  {
  return mStepSize;
  }

//
//
//
template <class T>
void VTKVolRen<T>::opacity(double newOpacity)
  {
  if (newOpacity < 0)
    newOpacity = 0;
  if (newOpacity > 1)
    newOpacity = 1.0;
  mOpacity = newOpacity;

  intensityMin(mIntensityMin);
  }

template <class T>
double VTKVolRen<T>::opacity(void)
  {
  return mOpacity;
  }

//
//
//
template <class T>
void VTKVolRen<T>::intensityMin(double newIntensityMin)
  {
  mIntensityMin = newIntensityMin;
  mOpacityTransferFunc->RemoveAllPoints();
  if(mIntensityMin*255 <= 0) 
    {
    mOpacityTransferFunc->AddPoint(0, 0); 
    mOpacityTransferFunc->AddSegment(1, mOpacity, 255, mOpacity);
    }
  else
  if(mIntensityMin*255 >= 255) 
    {
    mOpacityTransferFunc->AddSegment(0, mOpacity, 254, mOpacity);
    mOpacityTransferFunc->AddPoint(255, 0); 
    }
  else 
    {
    mOpacityTransferFunc->AddSegment(0, 0, mIntensityMin*255 - 1, 0);
    mOpacityTransferFunc->AddSegment(mIntensityMin*255 + 1, mOpacity, 255, mOpacity);
    mOpacityTransferFunc->AddPoint(mIntensityMin*255, 0); 
    }

  mProperty->SetScalarOpacity(mOpacityTransferFunc);
  }

template <class T>
double VTKVolRen<T>::intensityMin(void)
  {
  return mIntensityMin;
  }

//
//
//
template <class T>
void VTKVolRen<T>::maskData(OverlayType::Pointer newMask)
  {
  mMask = newMask;
  }

//
//
//
template <class T>
VTKVolRen<T>::OverlayType::Pointer 
VTKVolRen<T>::maskData(void)
  {
  return mMask;
  }

//
//
//
template <class T>
void VTKVolRen<T>::useMask(bool newUseMask)
  {
  mUseMask = newUseMask;
  }

template <class T>
bool VTKVolRen<T>::useMask(void)
  {
  return mUseMask;
  }


//
//
//
template <class T>
bool VTKVolRen<T>::SetInputImage(ImageType * image)
  {

  ImagePointer newIm( image );
      
  if (newIm->GetLargestPossibleRegion().GetSize()[0] <=0 
      || newIm->GetLargestPossibleRegion().GetSize()[1]  <=0 
      || newIm->GetLargestPossibleRegion().GetSize()[2]  <=0) 
    {
    strcpy(mError, "Fvtk: Invalid dimensions\n"); 
    return false;
    }

  mImData = newIm;
    
  mVol = vtkStructuredPoints::New();
  mVol->SetDimensions(mImData->GetLargestPossibleRegion().GetSize()[0],
                      mImData->GetLargestPossibleRegion().GetSize()[1],
                      mImData->GetLargestPossibleRegion().GetSize()[2]);
  mVol->SetSpacing(1.0, 1.0, 1.0);

  //if(!mImData->dataRangeDefined())
    //    mImData->calcDataRange();

  mScalars = vtkUnsignedCharArray::New();

  mActor->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);

  mUseMask = false;

  //double imMin = mImData->dataMin();
  //double imRange = mImData->dataMax()-imMin;

  
  itk::SimpleImageRegionIterator<ImageType> it(mImData, mImData->GetLargestPossibleRegion());
  it = it.Begin();
  
  double imMin = 100000 ;
  double imMax = 0;

  long i = 0;
  while (!it.IsAtEnd())
    {
    if (it.Get()<imMin) imMin = it.Get();
    if (it.Get()>imMax) imMax = it.Get();
    ++it;
    i++;
    }
  double imRange = imMax -imMin; 

  it = it.Begin();
  i=0 ;
  //for (i = 0; i < (long)mImData->quantity(); i++)
  while (!it.IsAtEnd())
    {
    mScalars->InsertTuple1(i, (unsigned char)(((it.Get()-imMin)/imRange)*255));
    i++;
    ++it;
    }  

  //try to put the scalars into the box and render directly
  mVol->GetPointData()->SetScalars(mScalars);

  mMapper->SetInput(mVol);

  mActor->SetMapper(mMapper);
  mActor->SetProperty(mProperty);

  return true;
  }

//
//
//
template <class T>
vtkVolume * VTKVolRen<T>::actor(void)
  {
  return mActor;
  }

//
//
//
template <class T>
vtkPiecewiseFunction * VTKVolRen<T>::colorFunc(void)
  {
  return mColorFunc;
  }

//
//
//
template <class T>
vtkPiecewiseFunction * VTKVolRen<T>::opacityFunc(void)
  {
  return mOpacityFunc;
  }

//
//
//
template <class T>
vtkVolumeProperty * VTKVolRen<T>::property(void)
  {
  return mProperty;
  }

//
//
//
template <class T>
vtkVolumeRayCastMapper * VTKVolRen<T>::mapper(void)
  {
  return mMapper;
  }

template <class T>
vtkStructuredPoints * VTKVolRen<T>::volume(void)
  {
  return mVol;
  }

template <class T>
vtkUnsignedCharArray * VTKVolRen<T>::scalars(void)
  {
  return mScalars;
  }

template <class T>
void VTKVolRen<T>::update(void)
  {
  if( !mImData )
    {
    return;
    }

  //create a structured points dataset with
  //scalar values equal to the vals in the MRA. Set the
  //spacing to 1,1,1. The caller must scale the actor by
  //(xstep, ystep, zstep).
  
  itk::SimpleImageRegionIterator<ImageType> it(mImData,mImData->GetRequestedRegion());
  itk::SimpleImageRegionIterator<OverlayType> itMask(mMask,mMask->GetRequestedRegion());
  it = it.Begin();
  
  double imMin = 100000 ;
  double imMax = 0;

  while (!it.IsAtEnd())
    {
    if (it.Get()<imMin) imMin = it.Get();
    if (it.Get()>imMax) imMax = it.Get();
    ++it;
    }
  
  double imRange = imMax-imMin;
  
  //double imMin = mImData->dataMin();
  //double imRange = mImData->dataMax()-imMin;

  it = it.Begin();
  itMask = itMask.Begin();
  long i=0;
  
  if(mUseMask) 
    {
    int j = 0;
    //for (i = 0; i < (long)mImData->quantity(); i++)
    while (!it.IsAtEnd())
      {
      if(itMask.Get()>0)
        {
        mScalars->InsertTuple1(i, (unsigned char)(((it.Get()-imMin)/imRange)*255));
        j++;
        } 
      else
        mScalars->InsertTuple1(i, 0);
      i++; 
      ++it;
      ++itMask;
      }
    }
  else
    while (!it.IsAtEnd())
      {
      //for (i = 0; i < (long)mImData->quantity(); i++)
      mScalars->InsertTuple1(i, (unsigned char)(((it.Get()-imMin)/imRange)*255));
      ++it;
      i++;
      }
      
  //try to put the scalars into the box and render directly
  mVol->GetPointData()->SetScalars(mScalars);

  mMapper->SetInput(mVol);

  mActor->SetMapper(mMapper);
  mActor->SetProperty(mProperty);

  }

#endif
