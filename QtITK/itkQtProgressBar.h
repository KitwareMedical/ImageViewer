#include <qslider.h>
#include "itkCommand.h"

namespace itk {


class QtProgressBar : public QSlider
{

public:

  /** Command Class invoked for button redraw */
  typedef itk::MemberCommand< QtProgressBar >  RedrawCommandType;

  /** Constructor */
  QtProgressBar( Orientation orientation, QWidget *parent, char * name);

  /** Get Command */
  RedrawCommandType * GetRedrawCommand( void ) const;

  
  /** Manage a Progress event */
  void ProcessEvent(itk::Object * caller, const itk::EventObject & event );
  void ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event );
 

  /** Manage a Progress event */
  void Observe( itk::Object *caller );

private:

  RedrawCommandType::Pointer m_RedrawCommand;
 


};


}  // end of namespace
