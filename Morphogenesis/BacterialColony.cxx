
#include "BacterialColony.h"
#include "FL/gl.h"


namespace bio {




BacterialColony
::BacterialColony()
{

  Cell::ColorType color;
  color.SetRed(0.9);
  color.SetGreen(0.0);
  color.SetBlue(0.0);
  Cell::SetDefaultColor( color );

}




BacterialColony
::~BacterialColony()
{
}




void
BacterialColony
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}





}; // end namespace bio

