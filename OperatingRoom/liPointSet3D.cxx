/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liPointSet3D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <liPointSet3D.h>



namespace li {

//--------------------------------------------------
//
//    Constructor
//
//--------------------------------------------------
PointSet3D::PointSet3D(void)  
{
  m_PointSet = PointSetType::New();
  SetDrawingMode( points );
}




//--------------------------------------------------
//
//    Get PointSet
//
//--------------------------------------------------
PointSet3D::PointSetType::ConstPointer
PointSet3D::GetPointSet(void)  
{
  return m_PointSet.GetPointer(); 
}



//--------------------------------------------------
//
//    Set PointSet
//
//--------------------------------------------------
void
PointSet3D::SetPointSet( PointSetType * mesh)  
{
  m_PointSet = mesh; 
}




//--------------------------------------------------
//
//    draw Geometric representation
//
//--------------------------------------------------
void 
PointSet3D::DrawGeometry(void)  const
{

  if( !m_PointSet )
  {
    return;
  }


    if( GetDrawingMode() == triangles ) 
    {
    }

    if( GetDrawingMode()== lines ) 
    {
    }

    if( GetDrawingMode()== points ) 
    {

      PointsContainerType::Pointer    meshPoints = m_PointSet->GetPoints();
      PointDataContainerType::Pointer meshData   = m_PointSet->GetPointData();

      if( !meshPoints || !meshData )
      {
        return;
      }

      typedef PointsContainerType::ConstIterator      PointIteratorType;   
      typedef PointDataContainerType::ConstIterator   DataIteratorType;   

      glDisable(GL_LIGHTING);
      glBegin(GL_POINTS);

      PointIteratorType p = meshPoints->Begin();
      DataIteratorType  d = meshData->Begin();

      while( p != meshPoints->End() )
      {
        
        const PointSetType::PointType point = p.Value();
       
        if( d.Value() > 0 ) 
        { // color in blue
          glColor3f( (GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0 ); 
        }
        else 
        { // color in red
          glColor3f( (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0 ); 
        }
        
        glVertex3d( point[0], point[1], point[2] );
        ++p;
        ++d;
      }

      glEnd();
    }

}



} // end namespace li


