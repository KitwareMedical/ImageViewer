#ifndef QtSlicer_h
#define QtSlicer_h

//Qt include
#include <QDialog>

//itk include
#include "itkImage.h"

//QtImageViewer includes
#include "ui_QtSlicerGUI.h"
#include "ui_QtSlicerHelpGUI.h"


class QtSlicer : public QDialog, public Ui::GuiDialogBase
{
  Q_OBJECT
public:
    
  QtSlicer( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~QtSlicer();
  
  typedef itk::Image<double,3> ImageType;
public slots:
  void updateSliceMaximum();
  void setInputImage(ImageType * newImData);
  void setDisplaySliceNumber(int number);
};

#endif
