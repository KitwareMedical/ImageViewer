/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Genome.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef Genome_H
#define Genome_H

#if defined(_WIN32)
#include "itkWindows.h"
#endif

#include <map>
#include <string>
#include <math.h>


namespace bio {




/**
 * \class Genome
 * \brief This class implement the abstraction of a biological genome.
 * 
 * The genome is considered to be a sequence of genes each one having
 * a name and a level of expression.
 * 
 */
class Genome  
{

public:

  typedef   std::string                       GeneIdType;
  typedef   std::map< GeneIdType, double >    MapType;


public:

  Genome();
  virtual ~Genome();

  void Copy( const Genome & genome );

  void InsertGene( const GeneIdType & geneId );
  void KnockOutGene( const GeneIdType & geneId );

  double GetExpressionLevel( const GeneIdType & geneId );
  void   SetExpressionLevel( const GeneIdType & geneId, double level );

  /** This methods computes a normalized Sigmoide function that can
      be used for gene network computations.  */
  static double Sigmoide( double threshold, double slant, double value )
    {
    return atan(   ( value - threshold ) / slant   ) / 3.1416 + 0.5001;
    }
    

private:

  MapType         m_Map;
   
};


} // end namespace bio

#endif


