/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltValidationApp.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltValidationApp_h
#define _VanderbiltValidationApp_h

#include "MIMApplicationBase.h"
#include "itkImage.h"
#include "VanderbiltValidationInputParser.h"
#include "MIMPreprocessor.h"
#include "MIMRegistrator.h"
#include "VanderbiltValidationOutput.h"

#include <string>
#include <time.h>

namespace itk
{

/** \class VanderbiltValidationApp
 * 
 * This class is a simple application that reads in a parameter
 * file, perform an MIM registration and outputs the 
 * target (fixed), source (moving) and registered images as PGM 
 * files.
 *
 * This class is activatived by method Execute().
 *
 * Inputs:
 * - path to the Vanderbilt "images" directory
 * - patient id number { 0 = practice, 1-9, 101-109 }
 * - the "From" modality
 * - the "To" modality
 * - the algorithm parameter filename
 * - flag to indicate if the output transform should be inverted
 * - the output transform filename
 * - flag to indicate whether or not to append to output file
 * - string containing the investigator(s)
 * - string containing the site
 *
 */
template < typename TInputPixel, 
           typename TInputImage = Image<TInputPixel, 3>,
           typename TImage = Image<float,3> >
class VanderbiltValidationApp : 
public MIMApplicationBase < 
  TInputImage, TImage,
  VanderbiltValidationInputParser<TInputImage>,
  MIMPreprocessor<TInputImage,TImage>,
  MIMRegistrator<TImage,TImage>,
  VanderbiltValidationOutput<TInputImage> >   
{
public:

  /** Standard class typedefs. */
  typedef VanderbiltValidationApp Self;
  typedef MIMApplicationBase < 
              TInputImage, TImage,
              VanderbiltValidationInputParser<TInputImage>,
              MIMPreprocessor<TInputImage,TImage>,
              MIMRegistrator<TImage,TImage>,
              VanderbiltValidationOutput<TInputImage> >       Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VanderbiltValidationApp, MIMApplicationBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /*** Modality type. */
  typedef VanderbiltModality::ModalityType ModalityType;

  /** Set the Vanderbilt "images" directory path. */
  itkSetStringMacro( ImageDirectoryName );

  /** Set the patient number. */
  itkSetMacro( PatientNumber, unsigned int );

  /** Set the from modality. */
  itkSetMacro( FromModality, ModalityType );

  /** Set the to modality. */
  itkSetMacro( ToModality, ModalityType );

  /** Set input parameter file */
  itkSetStringMacro( ParameterFileName );

  /** Set flag to indicate if output transform should be inverted. */
  itkSetMacro( InvertTransform, bool );
  itkGetMacro( InvertTransform, bool );
  itkBooleanMacro( InvertTransform );

  /** Set output transformation filename. */
  itkSetStringMacro( OutputFileName );

  /** Set append output file boolean. */
  itkSetMacro( AppendOutputFile, bool );
  itkGetMacro( AppendOutputFile, bool );
  itkBooleanMacro( AppendOutputFile );

  /** Set the investigator string. */
  itkSetStringMacro( InvestigatorString );

  /** Set the site string. */
  itkSetStringMacro( SiteString );

  /** Set the method string. */
  itkSetStringMacro( MethodString );

protected:

  VanderbiltValidationApp();

  virtual ~VanderbiltValidationApp(){};

  /** Initialize the input parser. */
  virtual void InitializeParser();

  /*** Initialize the output generator. */
  virtual void InitializeGenerator();

private:

   std::string          m_ImageDirectoryName;
   unsigned int         m_PatientNumber;
   ModalityType         m_FromModality;
   ModalityType         m_ToModality;
   std::string          m_ParameterFileName;
   bool                 m_InvertTransform;
   std::string          m_OutputFileName;
   bool                 m_AppendOutputFile;
   std::string          m_InvestigatorString;
   std::string          m_SiteString;
   std::string          m_MethodString;

};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "VanderbiltValidationApp.txx"
#endif

#endif

