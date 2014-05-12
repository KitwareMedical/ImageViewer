#ifndef QtSlicer_h
#define QtSlicer_h

//Qt include
#include <QDialog>

//itk include
#include "itkImage.h"
#include "itkImageFileReader.h"

//QtImageViewer includes
#include "ui_QtSlicerGUI.h"
#include "ui_QtSlicerHelpGUI.h"


class QtSlicer : public QDialog, public Ui::GuiDialogBase
{
  Q_OBJECT
public:
    
  QtSlicer( QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~QtSlicer();
  
  typedef itk::Image<double,3>                ImageType;
  typedef unsigned char                       OverlayPixelType;
  typedef itk::Image<OverlayPixelType,3>      OverlayType;

  bool loadOverlayImage(QString overlayImagePath = QString());
  bool loadInputImage(QString filePathToLoad = QString());
public slots:
  void updateSliceMaximum();
  void setInputImage(ImageType * newImData);
  void setOverlayImage(OverlayType * newImData);
  void setDisplaySliceNumber(int number);
};

#endif
