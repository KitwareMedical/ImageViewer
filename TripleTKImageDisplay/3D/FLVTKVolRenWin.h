#ifndef FLVTK_VOLUME_RENDER_WINDOW
#define FLVTK_VOLUME_RENDER_WINDOW

#include "itkImage.h"

#include <vtkExtractVOI.h>
#include <vtkPlaneSource.h>
#include <vtkTexture.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOutlineFilter.h>

#include "VTKVolRen.h"
#include "FLVTKWin.h"



template <class T>
class FLVTKVolRenWin : public FLVTKWin
  {

  typedef itk::Image<T,3>                 ImageType;
  typedef itk::Image<unsigned char,3>     OverlayType;

  typedef typename ImageType::Pointer     ImagePointer;
  typedef typename OverlayType::Pointer   OverlayPointer;


    protected:
       
       ImagePointer               mImData;
       
       OverlayPointer             mMask;
       

       bool                       mMaskAlloc;
       
       vtkRenderer               *mRenderer;
       vtkRenderWindowInteractor *mInteractor;
       
       double              mOpacity;
       
       VTKVolRen<T>        mVolRendProc;
       bool				         mVolRendView;
       
       vtkActor	          *mOutline;
       bool				         mOutlineView;
       
       int                 mSaggitalSlice;
       vtkExtractVOI      *mSaggitalSect;
       vtkPlaneSource     *mSaggitalPlane;
       vtkTexture         *mSaggitalText;
       vtkPolyDataMapper  *mSaggitalMap;
       vtkActor			      *mSaggital;
       bool				         mSaggitalView;
       
       int                 mAxialSlice;
       vtkExtractVOI      *mAxialSect;
       vtkPlaneSource     *mAxialPlane;
       vtkTexture         *mAxialText;
       vtkPolyDataMapper  *mAxialMap;
       vtkActor			      *mAxial;
       bool				         mAxialView;
       
       int                 mCoronalSlice;
       vtkExtractVOI      *mCoronalSect;
       vtkPlaneSource     *mCoronalPlane;
       vtkTexture         *mCoronalText;
       vtkPolyDataMapper  *mCoronalMap;
       vtkActor			      *mCoronal;
       bool				         mCoronalView;
       
    public:
       
       FLVTKVolRenWin(int x, int y, int w, int h, const char* l = 0);
       
       void free(void);
       
       ~FLVTKVolRenWin();
       
       ImagePointer GetInputImage(void);
       void SetInputImage(ImageType * newIm);
       
       void OverlaymaskData(void);
       void maskData(OverlayType * newMaskData);
       
       void useMask(bool newUseMask);
       bool useMask(void);
       
       vtkRenderer * renderer(void);
       
       VTKVolRen<T> * volRendProc(void);
       
       void viewVolRend(bool newViewVolRend);
       bool viewVolRend(void);
       
       void viewOutline(bool newViewOutline);
       bool viewOutline(void);
       
       void viewSaggital(bool newViewSaggital);
       bool viewSaggital(void);
       void sliceSaggital(int newSliceSaggital);
       int  sliceSaggital(void);
       
       void viewAxial(bool newViewAxial);
       bool viewAxial(void);
       void sliceAxial(int newSliceAxial);
       int  sliceAxial(void);
       
       void viewCoronal(bool newViewCoronal);
       bool viewCoronal(void);
       void sliceCoronal(int newSliceCoronal);
       int  sliceCoronal(void);
       
       void    opacity(double newOpacity);
       double  opacity(void);
       
       void update(void);
       
       void draw(void);
  };

/****************************
*
*       Constructor
*
*****************************/
template <class T>
FLVTKVolRenWin<T>::FLVTKVolRenWin(int x, int y, int w, int h, const char* l)
: FLVTKWin(x, y, w, h, l)
  {
  mRenderer = vtkRenderer::New();
  mInteractor = vtkRenderWindowInteractor::New();

  mImData = ImageType::New();
  mMask = OverlayType::New();
  mMaskAlloc = false;

  mOpacity = 0.8;

  mVolRendView = false;

  mOutlineView = false;
  mOutline = NULL;

  mSaggitalSlice = 0;
  mSaggitalView = false;
  mSaggital = NULL;
  mCoronalSlice = 0;
  mCoronalView = false;
  mCoronal = NULL;
  mAxialSlice = 0;
  mAxialView = false;
  mAxial = NULL;

  }

/****************************
*
*       Free
*
*****************************/
template <class T>
void FLVTKVolRenWin<T>::free(void )
  {
  /*if(mMask != NULL)
  if(mMaskAlloc)
     delete mMask;*/
  mMask = NULL;
  mMaskAlloc = false;

  if(mRenderer != NULL)
     mRenderer->Delete();
  mRenderer = NULL;

  if(mSaggital != NULL)
     mSaggital->Delete();
  mSaggital = NULL;
  if(mAxial != NULL)
     mAxial->Delete();
  mAxial = NULL;
  if(mCoronal != NULL)
     mCoronal->Delete();
  mCoronal = NULL;
  }

/****************************
*
*       Destructor
*
*****************************/
template <class T>
FLVTKVolRenWin<T>::~FLVTKVolRenWin()
  {
  free();
  }


/****************************
*
*       im
*
*****************************/
template <class T>
typename FLVTKVolRenWin<T>::ImagePointer 
FLVTKVolRenWin<T>::GetInputImage(void)
{
  return mImData;
}

template <class T>
void FLVTKVolRenWin<T>::SetInputImage(ImageType * newIm)
{

  mImData = newIm;

  mVolRendProc.SetInputImage(mImData);
  mVolRendView = false;

  vtkOutlineFilter * of = vtkOutlineFilter::New();
  of->SetInput(mVolRendProc.volume());
  vtkPolyDataMapper * otm = vtkPolyDataMapper::New();
  otm->SetInput(of->GetOutput());
  mOutline = vtkActor::New();
  mOutline->SetMapper(otm);
  mOutline->GetProperty()->SetColor(0, 0, 1);
  mOutline->GetProperty()->SetOpacity(mOpacity);
  mOutline->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);
  mRenderer->AddActor(mOutline);
  mOutlineView = true;

  of->Delete();
  otm->Delete();

  mSaggital = vtkActor::New();
  mSaggitalSect = vtkExtractVOI::New();
  mSaggitalText = vtkTexture::New();
  mSaggitalPlane = vtkPlaneSource::New();
  mSaggitalMap = vtkPolyDataMapper::New();
  mSaggitalMap->SetColorModeToMapScalars();
  mSaggitalText->InterpolateOn();
  mSaggitalPlane->SetXResolution(2);
  mSaggitalPlane->SetYResolution(2);
  mSaggitalMap->SetInput(mSaggitalPlane->GetOutput());
  mSaggitalMap->ImmediateModeRenderingOn();
  mSaggital->SetMapper(mSaggitalMap);
  mSaggital->SetTexture(mSaggitalText);
  mSaggital->GetProperty()->SetOpacity(mOpacity);
  mSaggital->GetProperty()->SetInterpolationToFlat();
  mSaggital->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);
  sliceSaggital(mImData->GetLargestPossibleRegion().GetSize()[0]/2);
  mRenderer->AddActor(mSaggital);
  mSaggitalView = true;

  mAxial = vtkActor::New();
  mAxialSect = vtkExtractVOI::New();
  mAxialPlane = vtkPlaneSource::New();
  mAxialPlane->SetXResolution(2);
  mAxialPlane->SetYResolution(2);
  mAxialText = vtkTexture::New();
  mAxialText->InterpolateOn();
  mAxialMap = vtkPolyDataMapper::New();
  mAxialMap->SetColorModeToMapScalars();
  mAxialMap->SetInput(mAxialPlane->GetOutput());
  mAxialMap->ImmediateModeRenderingOn();
  mAxial->SetMapper(mAxialMap);
  mAxial->SetTexture(mAxialText);
  mAxial->GetProperty()->SetOpacity(mOpacity);
  mAxial->GetProperty()->SetInterpolationToFlat(); //
  mAxial->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);
  sliceAxial(mImData->GetLargestPossibleRegion().GetSize()[2]/2);
  mRenderer->AddActor(mAxial);
  mAxialView = true;

  mCoronal = vtkActor::New();
  mCoronalSect = vtkExtractVOI::New();
  mCoronalPlane = vtkPlaneSource::New();
  mCoronalText = vtkTexture::New();
  mCoronalMap = vtkPolyDataMapper::New();
  mCoronalMap->SetColorModeToMapScalars();
  mCoronalText->InterpolateOn();
  mCoronalPlane->SetXResolution(2);
  mCoronalPlane->SetYResolution(2);
  mCoronalMap->SetInput(mCoronalPlane->GetOutput());
  mCoronalMap->ImmediateModeRenderingOn();
  mCoronal->SetMapper(mCoronalMap);
  mCoronal->SetTexture(mCoronalText);
  mCoronal->GetProperty()->SetOpacity(mOpacity);
  mCoronal->GetProperty()->SetInterpolationToFlat();
  mCoronal->SetScale(1, mImData->GetSpacing()[1]/mImData->GetSpacing()[0], mImData->GetSpacing()[2]/mImData->GetSpacing()[0]);
  sliceCoronal(mImData->GetLargestPossibleRegion().GetSize()[1]/2);
  mRenderer->AddActor(mCoronal);
  mCoronalView = true;

  mMask = OverlayType::New();
  mMask->SetLargestPossibleRegion( mImData->GetLargestPossibleRegion());
  mMask->SetBufferedRegion( mImData->GetBufferedRegion() );
  mMask->SetRequestedRegion( mImData->GetRequestedRegion() );

  mMask->SetSpacing(mImData->GetSpacing());
  mMask->SetOrigin(mImData->GetOrigin());

  mMask->Allocate();

  mVolRendProc.maskData(mMask);
  mVolRendProc.useMask(false);
  mVolRendProc.update();
}

//
//
//
template <class T>
FLVTKVolRenWin<T>::OverlayType::Pointer 
FLVTKVolRenWin<T>::maskData(void)
  {
  return mMask;
  }

template <class T>
void FLVTKVolRenWin<T>::maskData(OverlayType::Pointer newMaskData)
  {
  if(mMask != NULL)
    if(mMaskAlloc)
       delete mMask;
    mMaskAlloc = false;
    mMask = newMaskData;
    mVolRendProc.maskData(newMaskData);
  }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::useMask(bool newUseMask)
  {
  mVolRendProc.useMask(newUseMask);
  }

template <class T>
bool FLVTKVolRenWin<T>::useMask(void)
  {
  return mVolRendProc.useMask();
  }

//
//
//
template <class T>
vtkRenderer * FLVTKVolRenWin<T>::renderer(void)
  {
  return mRenderer;
  }

template <class T>
VTKVolRen<T> * FLVTKVolRenWin<T>::volRendProc(void)
  {
  return & mVolRendProc;
  }


//
//
//
template <class T>
void FLVTKVolRenWin<T>::viewVolRend(bool newViewVolRend)
  {
  if(newViewVolRend == mVolRendView)
    return;

  if(newViewVolRend == false) 
    {
    mRenderer->RemoveActor(mVolRendProc.actor());
    viewSaggital(true);
    viewCoronal(true);
    viewAxial(true);
    }
  else
    {
    mRenderer->AddActor(mVolRendProc.actor());
    viewSaggital(false);
    viewCoronal(false);
    viewAxial(false);
    }

  mVolRendView = newViewVolRend;
  }

template <class T>
bool FLVTKVolRenWin<T>::viewVolRend(void)
   {
   return mVolRendView;
   }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::viewOutline(bool newViewOutline)
  {
  if(!mOutline || newViewOutline == mOutlineView)
    return;

  if(newViewOutline == false)
    mOutline->VisibilityOff();
  else
    mOutline->VisibilityOn();

  mOutlineView = newViewOutline;

  }

template <class T>
bool FLVTKVolRenWin<T>::viewOutline(void)
  {
  return mOutlineView;
  }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::viewAxial(bool newViewAxial)
  {
  if(!mAxial || newViewAxial == mAxialView)
    return;

  if(newViewAxial == false)
    mAxial->VisibilityOff();
  else
    mAxial->VisibilityOn();

  mAxialView = newViewAxial;
  }

template <class T>
bool FLVTKVolRenWin<T>::viewAxial(void)
  {
  return mAxialView;
  }

template <class T>
void FLVTKVolRenWin<T>::sliceAxial(int newSliceAxial)
  {
  mAxialSlice = newSliceAxial;
  mAxialSect->SetVOI(0, mImData->GetLargestPossibleRegion().GetSize()[0]-1,  0, mImData->GetLargestPossibleRegion().GetSize()[1]-1,
    mAxialSlice, mAxialSlice);
  mAxialSect->SetInput(mVolRendProc.mapper()->GetInput());
  mAxialText->SetInput(mAxialSect->GetOutput());
  mAxialPlane->SetOrigin(0, 0, mAxialSlice);
  mAxialPlane->SetPoint1(mImData->GetLargestPossibleRegion().GetSize()[0]-1, 0, mAxialSlice);
  mAxialPlane->SetPoint2(0, mImData->GetLargestPossibleRegion().GetSize()[1]-1, mAxialSlice);
  }

template <class T>
int FLVTKVolRenWin<T>::sliceAxial(void)
  {
  return mAxialSlice;
  }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::viewSaggital(bool newViewSaggital)
  {
  if(!mSaggital || newViewSaggital == mSaggitalView)
    return;

  if(newViewSaggital == false)
    mSaggital->VisibilityOff();
  else
    mSaggital->VisibilityOn();

  mSaggitalView = newViewSaggital;
  }

template <class T>
bool FLVTKVolRenWin<T>::viewSaggital(void)
  {
  return mSaggitalView;
  }

template <class T>
void FLVTKVolRenWin<T>::sliceSaggital(int newSliceSaggital)
  {
  mSaggitalSlice = newSliceSaggital;

  mSaggitalSect->SetVOI(0, mImData->GetLargestPossibleRegion().GetSize()[0]-1, 
                        mSaggitalSlice, mSaggitalSlice, 0, mImData->GetLargestPossibleRegion().GetSize()[2]-1);
  mSaggitalSect->SetInput(mVolRendProc.volume()); //mapper()->GetInput());
  mSaggitalText->SetInput(mSaggitalSect->GetOutput());
  mSaggitalPlane->SetOrigin(0, mSaggitalSlice, 0);
  mSaggitalPlane->SetPoint1(mImData->GetLargestPossibleRegion().GetSize()[0]-1, mSaggitalSlice, 0);
  mSaggitalPlane->SetPoint2(0, mSaggitalSlice, mImData->GetLargestPossibleRegion().GetSize()[2]-1);
  }

template <class T>
int FLVTKVolRenWin<T>::sliceSaggital(void)
  {
  return mSaggitalSlice;
  }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::viewCoronal(bool newViewCoronal)
  {
  if(!mCoronal || newViewCoronal == mCoronalView)
    return;

  if(newViewCoronal == false)
    mCoronal->VisibilityOff();
  else
    mCoronal->VisibilityOn();

  mCoronalView = newViewCoronal;

  }

template <class T>
bool FLVTKVolRenWin<T>::viewCoronal(void)
  {
  return mCoronalView;
  }

template <class T>
void FLVTKVolRenWin<T>::sliceCoronal(int newSliceCoronal)
  {
  mCoronalSlice = newSliceCoronal;
  mCoronalSect->SetVOI(mCoronalSlice, mCoronalSlice, 0, mImData->GetLargestPossibleRegion().GetSize()[1]-1, 0, mImData->GetLargestPossibleRegion().GetSize()[2]-1);
  mCoronalSect->SetInput(mVolRendProc.mapper()->GetInput());
  mCoronalText->SetInput(mCoronalSect->GetOutput());
  mCoronalPlane->SetOrigin(mCoronalSlice, 0, 0);
  mCoronalPlane->SetPoint1(mCoronalSlice, mImData->GetLargestPossibleRegion().GetSize()[1]-1, 0);
  mCoronalPlane->SetPoint2(mCoronalSlice, 0, mImData->GetLargestPossibleRegion().GetSize()[2]-1);
  }

template <class T>
int FLVTKVolRenWin<T>::sliceCoronal(void)
  {
  return mCoronalSlice;
  }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::opacity(double newOpacity)
  {
  if (newOpacity < 0)
    newOpacity = 0;
  if (newOpacity > 1)
    newOpacity = 1.0;
  mOpacity = newOpacity;

  if(mOutline != NULL)
    mOutline->GetProperty()->SetOpacity(mOpacity);
  if(mAxial != NULL)
    mAxial->GetProperty()->SetOpacity(mOpacity);
  if(mCoronal != NULL)
    mCoronal->GetProperty()->SetOpacity(mOpacity);
  if(mSaggital != NULL)
    mSaggital->GetProperty()->SetOpacity(mOpacity);
  mVolRendProc.opacity(mOpacity);
  }

template <class T>
double FLVTKVolRenWin<T>::opacity(void)
  {
  return mOpacity;
  }

//
//
//
template <class T>
void FLVTKVolRenWin<T>::update(void)
  {
  mVolRendProc.update();
  redraw();
  }

template <class T>
void FLVTKVolRenWin<T>::draw(void)
  {
  FLVTKWin::draw();

  static bool localfirst = true;
  if(localfirst)
    {
    FLVTKWin::win()->AddRenderer(mRenderer);
  
    mInteractor->SetRenderWindow(FLVTKWin::win());
    localfirst = false;
    }
  }

#endif
