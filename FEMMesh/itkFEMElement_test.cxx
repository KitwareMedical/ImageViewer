/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFEMElement_test.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkFEMElement_test.h"
#include "itkFEMCell.h"

namespace itk {
namespace fem {




void FEMElement::LinkDOFs(void)
{
  // Release any existing dofs that the element may be using.
  ClearDOFs();

  // Step over all elements in the neighborhood.
  for(int el=0;el<2;el++) // FIXME: we need to change this to use proper functions
  {
    // c should be set so that it points to a cell object in the neighborhood
    FEMCell<2>* c=0;
      
    // Same thing except that here we store a pointer to an Element
    // object.This is to limit the the use of dynamic_cast operator.
    FEMElement* e=dynamic_cast<FEMElement*>(c);

    // Cast this pointer to FEMCell. If it doesn't work, somebody used the wrong elements...
    FEMCell<2>* this_cell=dynamic_cast<FEMCell<2>*>(this);

    if( e==0 ) // Check if the cell object was of corerct class.
    {
      throw; // Nope. Start yelling...
    }



    // Find the shared point in the neighboring object
    for( int i=0;i<this_cell->GetNumberOfPoints();i++ ) // Step over all points in current cell
    {
      // check if point j of the neighboring cell is shared with the point i of the current cell
      for( int j=0;j<c->GetNumberOfPoints();j++ ) // Step over all points in the neighbor cell
      {
        if( this_cell->GetPoint(i) == c->GetPoint(j) )
        {
          // Yes. Copy the DOFs from the neighboring cell's point
          // since they have to be the same. Note that neighboring point
          // may contain more or less DOFs. If it has more, we simply ignore
          // the rest, if it has less, we'll get invalid DOF id from GetDOF
          // function.
          for(unsigned int d=0;d<this->NumberOfDegreesOfFreedom();d++)
          {
            // get the DOF from the point at neighboring element
            DOFType dof = e->GetDOFatPoint(j,d);

            // set the corresponding DOF in current element
            this->SetDOF(i,dof);

          } // end for d

          // once we find one valid shared point, we  can exit the loop over points of the neighbor cell
          break;

        } // end if

      } // end for j

      // Now all DOFs in current object for point i are matched with those
      // in the neghboring elements. However, if none of the neighboring
      // objects defines these DOFs, we need to create them.
      for(unsigned int d=0;d<NumberOfDegreesOfFreedom();d++) // step over all DOFs at point i
      {
        if(this->GetDOF(d)<0)
        {
          /* 
           * Found a undefined DOF. We need either to create a new DOF object,
           * or obtain a unique id, which we'll set with the SetDOF function.
           */
          SetDOF(d,1 /* new dof id */);
        }

      }
 
    } // end for i

  } // end for el

}




}} // end namespace itk::fem
