/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    liSampleGeneratorCollection.h
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
//              Project Registrator
//
//              Author:  Luis Ibanez
//      
//              Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------

#ifndef liSAMPLEGENERATORCOLLECTION
#define liSAMPLEGENERATORCOLLECTION

#include "liSampleGeneratorCollectionGUI.h"


class liSampleGeneratorCollection : public liSampleGeneratorCollectionGUI {

public:
  liSampleGeneratorCollection();
  virtual ~liSampleGeneratorCollection();
  virtual void Show( void );
  virtual void Hide( void );
  virtual void Add( liSampleGenerator * );
  virtual void ShowSampleGenerator( void );

};



#endif

