
#include <qbutton.h>
#include "itkCommand.h"

namespace itk {

  class QtLightIndicator : public QButton
{

  Q_OBJECT

public:

  /** Constructor */
  QtLightIndicator( QWidget *parent, char * name):QButton( parent, name ) {}

public slots:

  void Start()
    {
    QColor yellow(255,255,0);
    this->setBackgroundColor( yellow );
    }

  void Modified()
    {
    QColor red(255,0,0);
    this->setBackgroundColor( red );
    }

  void End()
    {
    QColor green(0,255,0);
    this->setBackgroundColor( green );
    }

};


}  // end of namespace
