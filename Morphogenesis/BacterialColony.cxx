#include "BacterialColony.h"


namespace bio {




BacterialColony
::BacterialColony()
{
}




BacterialColony
::~BacterialColony()
{
}



unsigned int
BacterialColony
::GetNumberOfCells(void) const
{
  return m_Bacteria.size();
}



void
BacterialColony
::Draw(void) const
{
	BacteriaListType::const_iterator bacteria = m_Bacteria.begin();
	while( bacteria != m_Bacteria.end() )
    {
		(*bacteria)->Draw();
    bacteria++;
    }
}





void
BacterialColony
::SetGrowthRadiusLimit( double value ) 
{
  Cell::SetGrowthRadiusLimit( value );
}




void
BacterialColony
::SetGrowthRadiusIncrement( double value ) 
{
  Cell::SetGrowthRadiusIncrement( value );
}




void
BacterialColony
::Grow(void)
{
  BacteriaListType::const_iterator bacteria = m_Bacteria.begin();
	while( bacteria != m_Bacteria.end() )
	{
		(*bacteria)->Grow();
    bacteria++;
	}
}




void
BacterialColony
::KillAll(void)
{
  m_Bacteria.clear();
}



BacterialColony::VectorType
BacterialColony
::WellPotentialGradient(const VectorType &  relativePosition) const
{
  double distance      = relativePosition.GetNorm();
  VectorType direction = relativePosition / distance;
  double factor        = 
  if( distance >  )
   {
     factor =
   }
  factor *= 1e-5;

  return direction * factor;
}


void
BacterialColony
::Spread(void)
{

  BacteriaListType::const_iterator bacteria1 = m_Cells->begin();
  BacteriaListType::const_iterator end       = m_Cells->end();
  
	while( bacteria1 != end )
    {
    const Cell::PointType pA = (*bacteria1)->GetPosition();
    BacteriaListType::const_iterator bacteria2 = bacteria1;
    bacteria1++;
    
    while( bacteria2 != end )
      {
      const Cell::PointType pB = (*bacteria2)->GetPosition();
      Cell::VectorType relativePosition = pA - pB;
      const double distance = relativePosition.GetNorm();
      
      const Cell::VectorType force = WellPotentialGradient( relativePosition );

      (*bacteria2)->AddForce(  force );
      (*bacteria1)->AddForce( -force );

      bacteria2++;
      }
    bacteria1++;
    }

  
  BacteriaListType::const_iterator bacteria   = m_Bacteria->begin();
	while( bacteria != end )
	{
    (*bacteria)->UpdatePosition();
    bacteria++;
	}
}

}; // end namespace bio

