/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liVertebralBody.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------

#ifndef liVERTEBRALBODYCLASS
#define liVERTEBRALBODYCLASS

#include "fltkShape3D.h"
#include <vector>

namespace li {


/**
 *  This class implements a 3D model of a generic Vertebral Body 
 */
class ITK_EXPORT VertebralBody : public fltk::Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef VertebralBody   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltk::Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * ConstSmart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( VertebralBody, fltk::Shape3D );


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro( Self );
    


protected:
        VertebralBody();
        ~VertebralBody();

public:
        void DrawGeometry(void) const;
        void GeneratePoints();
        void SetUpperEndYRadius(double val);
        void SetUpperEndXRadius(double val);
        void SetUpperEndYConcavityDepth(double val);
        void SetUpperEndYConcavityWidth(double val);
        void SetUpperEndInclination(double val);
        void SetLowerEndYRadius(double val);
        void SetLowerEndXRadius(double val);
        void SetLowerEndYConcavityDepth(double val);
        void SetLowerEndYConcavityWidth(double val);
        void SetLowerEndInclination(double val);
        void SetMiddleYRadius(double val);
        void SetMiddleXRadius(double val);
        void SetHeight(double val);
        void SetSectorsNumber(unsigned int);
        void SetLevelsNumber(unsigned int);
        double GetUpperEndYRadius(void) const;
        double GetUpperEndXRadius(void) const;
        double GetUpperEndYConcavityDepth(void) const;
        double GetUpperEndYConcavityWidth(void) const;
        double GetUpperEndInclination(void) const;
        double GetLowerEndYRadius(void) const;
        double GetLowerEndXRadius(void) const;
        double GetLowerEndYConcavityDepth(void) const;
        double GetLowerEndYConcavityWidth(void) const;
        double GetLowerEndInclination(void) const;
        double GetMiddleYRadius(void) const;
        double GetMiddleXRadius(void) const;
        double GetHeight(void) const;
        unsigned int GetSectorsNumber(void) const;
        unsigned int GetLevelsNumber(void) const;

  int Write( fltk::Shape3D::OfstreamType &os, int level ) const;
  int Read( fltk::Shape3D::IfstreamType &is );

private:  
        double                      m_UpperEndYRadius;
        double                      m_UpperEndXRadius;
        double                      m_UpperEndYConcavityDepth;
        double                      m_UpperEndYConcavityWidth;
        double                      m_UpperEndInclination;
        double                      m_LowerEndYRadius;
        double                      m_LowerEndXRadius;
        double                      m_LowerEndYConcavityDepth;
        double                      m_LowerEndYConcavityWidth;
        double                      m_LowerEndInclination;
        double                      m_MiddleYRadius;
        double                      m_MiddleXRadius;
        double                      m_Height;
  std::vector< PointType >    m_Points;
        unsigned int                m_SectorsNumber;
        unsigned int                m_LevelsNumber;

};



} // end namespace li

#endif

