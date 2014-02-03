#ifndef QtSlicer_h
#define QtSlicer_h

//#include <QtGui>
#include <QDialog>

#include <itkImage.h>
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

  void DisplayPosition(int x,int y ,int z,float value);
//  void Help();
  void SetInputImage(ImageType * newImData);
  void DisplaySliceNumber(int number);
  void DisplayIMin(int value);
  void DisplayIMax(int value);

private:
//  Ui::HelpWindow *helpWindow;

};

#endif
