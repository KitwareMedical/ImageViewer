/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    VanderbiltHeaderParser.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _VanderbiltHeaderParser_txx
#define _VanderbiltHeaderParser_txx


#include "VanderbiltHeaderParser.h"
#include "itkExceptionObject.h"

namespace itk
{

template <unsigned int VImageDimension>
VanderbiltHeaderParser<VImageDimension>
::VanderbiltHeaderParser()
{

  m_ImageSize.Fill( 0 );
  m_ImageSpacing.Fill( 0.0 );
  
  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    m_ImageAxes[j] = ImageDimension;
    }

  m_ImageDirection.Fill( false );
  m_ImageBigEndian = true;

}


template <unsigned int VImageDimension>
void
VanderbiltHeaderParser<VImageDimension>
::Execute()
{

  // open the header file
  std::ifstream inputStream( m_FileName.c_str(), std::ios::in );

  if ( !inputStream.is_open() )
    {
    ExceptionObject err(__FILE__, __LINE__);
    err.SetLocation( "Execute()" );
    err.SetDescription( "Can't open file." );
    throw err;
    }

  const unsigned int maxChar = 512;

  char buffer[maxChar];
 
  while ( !inputStream.eof() )
    {

    inputStream.getline( buffer, maxChar , '\n' );

    std::string currentLine( buffer );
    signed int pos;
    unsigned int uint;
    float flt[2];
    char  ch[3];

    if ( !currentLine.find( "Rows" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, "%u", &uint );
      m_ImageSize[1] = uint;
      }
    else if ( !currentLine.find( "Columns" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, "%u", &uint );
      m_ImageSize[0] = uint;
      }
    else if ( !currentLine.find( "Slices" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, "%u", &uint );
      m_ImageSize[2] = uint;
      }
    else if ( !currentLine.find( "Pixel size" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, "%f : %f", flt, flt+1 );
      m_ImageSpacing[0] = flt[0];
      m_ImageSpacing[1] = flt[1];
      }
    else if ( !currentLine.find( "Slice thickness" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, "%f", flt );
      m_ImageSpacing[2] = flt[0];
      }
    else if ( !currentLine.find( "Patient orientation" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, " %c : %c : %c", ch, ch+1, ch+2 );

      for ( unsigned int j = 0; j < ImageDimension; j++ )
        {
        switch ( ch[j] )
          {
          case 'L': m_ImageAxes[j] = 0; m_ImageDirection[j] = false; break;
          case 'R': m_ImageAxes[j] = 0; m_ImageDirection[j] = true;  break;
          case 'P': m_ImageAxes[j] = 1; m_ImageDirection[j] = false; break;
          case 'A': m_ImageAxes[j] = 1; m_ImageDirection[j] = true;  break;
          case 'H': m_ImageAxes[j] = 2; m_ImageDirection[j] = false; break;
          case 'F': m_ImageAxes[j] = 2; m_ImageDirection[j] = true;  break;
          default: 
            // not a valid axes
            m_ImageAxes[j] = ImageDimension; m_ImageDirection[j] = false;
          }
        }

      }
    else if ( !currentLine.find( "High bit" ) )
      { 
      pos = currentLine.find( ":=" );
      sscanf( currentLine.c_str() + pos + 2, "%u", &uint );
      if ( uint == 0 )
        {
        m_ImageBigEndian = false;
        }
      else
        {
        m_ImageBigEndian = true;
        }
      }
 
    }

   inputStream.close();

 
}


} // namespace itk

#endif
