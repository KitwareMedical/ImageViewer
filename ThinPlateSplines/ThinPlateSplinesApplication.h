/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ThinPlateSplinesApplication.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef ThinPlateSplinesApplication_H
#define ThinPlateSplinesApplication_H



#include "ThinPlateSplinesApplicationGUI.h"



// This is the classe for the Application

class ThinPlateSplinesApplication : public ThinPlateSplinesApplicationGUI
{

public:

  typedef ThinPlateSplinesApplicationBase::PointType        PointType;
  typedef ThinPlateSplinesApplicationBase::PointArrayType   PointArrayType;

  virtual void DisplayLandMarks(void);
  virtual void CreateLandMarks(void);
  virtual void SelectSourceLandMark(void);
  virtual void SelectTargetLandMark(void);
  virtual void UpdateSelectedSourceLandMark(void);
  virtual void UpdateSelectedTargetLandMark(void);
      
public:

  ThinPlateSplinesApplication();
  ~ThinPlateSplinesApplication();
  

protected:
  virtual void MapPoints(void);

private:


};




#endif
