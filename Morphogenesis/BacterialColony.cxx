
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

  // Create the first individual
  Bacteria * bacteria = new Bacteria;
  SetCells( bacteria->GetAggregate() );
  
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
  
  os << "Cellular aggregate " << GetCells() << std::endl;

}





}; // end namespace bio

