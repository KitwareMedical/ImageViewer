/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liCommandTubesGeneratePointSet.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef liCOMMANDTUBESGENERATEPOINTSET
#define liCOMMANDTUBESGENERATEPOINTSET

#include "liVectorTubes3D.h"
#include "liFluoroscopyCArmMobile.h"
#include "itkCommand.h"
#include "liCommandEvents.h"

namespace li {

/** Implementation of the Command Pattern for 
 *  the generation of points in the tube surface 
 *  according to the line of sight defined in 
 *  the fluoroscopy unit. */
class ITK_EXPORT CommandTubesGeneratePointSet : public itk::Command 
{
public:
  /** Standard class typedefs. */
  typedef CommandTubesGeneratePointSet   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Execute method will call redraw in the GL Window. */
  void Execute(itk::Object *caller, const itk::EventObject & event);
  void Execute(const itk::Object *caller, const itk::EventObject & event);

  /*** Run-time type information (and related methods). */
  itkTypeMacro( CommandTubesGeneratePointSet, ::itk::Command );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );
  
  /** Type defining the vector of Tubes. */
  typedef   VectorTubes3D    VectorTubesType;

  /** Type defining the Fluoroscopy Unit type. */
  typedef   FluoroscopyCArmMobile    FluoroscopyUnitType;

  /** Set the Fluorosocpy unit from which to get the line of sight. */
  void SetFluoroscopyUnit( FluoroscopyUnitType * unit )
    { m_FluoroscopyUnit = unit; }

  /** Set the vector of Tubes for which points have to be generated. */
  void SetTubes( VectorTubesType * tubes )
    { m_Tubes = tubes; }

protected:
  /** Constructor */
  CommandTubesGeneratePointSet();

private:
  /** The fluoroscopy unit from which the point of view is extracted. */
  FluoroscopyUnitType::ConstPointer     m_FluoroscopyUnit;
 
  /** The vector of Tubes3D for which points have to be generated. */
  VectorTubesType::Pointer         m_Tubes;

};


} // end namespace li


#endif
