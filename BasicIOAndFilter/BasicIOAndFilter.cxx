/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BasicIOAndFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// This is an example of a simple program involving filters and I/O, expected
// to be modified and compiled as a project outside the Insight source tree.
// This is a good place to start if you have never used Insight before.
//
// Each filter has an Update() method that forces it to make sure its
// output is up to date and available for reading.  If Update() has
// been called before, and the input has not changed since that call,
// then no action is performed because the correct data is already
// computed and accessible via GetOutput().  Calling
// someFilter->Update() forces it to call the Update() method of any
// previous filters, so that the updating percolates back to the very
// beginning.  The Write() method of a writer calls its Update()
// method.  In this program, for instance, the call to writer->Write()
// forces the whole process to execute.
//
// Notice that you must use the smart pointers (e.g., PNGIOType::Pointer) as
// in the example, since the constructor is protected.  This permits the  
// bookkeeping that allows the Update() calls to work correctly.
// 
// Extensive use of typedefs as in the example is recommended.
//
// To build this project, copy it outside the Insight source tree,
// and then follow the same steps you followed to build itk.

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <itkImage.h>
#include <itkImageFileReader.h> 
#include <itkImageFileWriter.h> 
#include <itkPNGImageIO.h>
#include <itkBinaryThresholdImageFilter.h> 

typedef itk::Image<unsigned char, 2> ImageType;

int main(int argc, char* argv[])
{

    if (argc < 2) 
    {
        std::cout 
            << "usage: SampleFilterProject [inputfile] [outputfile]"
            << std::endl;
        exit(0);
    }

    try
    {

        // Instantiate a reader for the file.
        typedef itk::ImageFileReader<ImageType> ImageReaderType;
        ImageReaderType::Pointer reader = ImageReaderType::New();
        reader->SetFileName(argv[1]);

        // Instantiate a filter and attach it to the reader.
        typedef itk::BinaryThresholdImageFilter<ImageType, ImageType> 
            ThresholdFilterType;
        ThresholdFilterType::Pointer myFilter = ThresholdFilterType::New();
        myFilter->SetInput(reader->GetOutput());
        myFilter->SetUpperThreshold(128);
        myFilter->SetLowerThreshold(0);

        // Instantiate a witer and attach it to the filter.
        typedef itk::ImageFileWriter<ImageType> ImageWriterType;
        ImageWriterType::Pointer writer = ImageWriterType::New();
        writer->SetImageIO(itk::PNGImageIO::New());
        writer->SetInput(myFilter->GetOutput());
        writer->SetFileName(argv[2]);

        // Execute the process.
        writer->Update();

    }
    catch(itk::ExceptionObject &err)
    {
        std::cerr << err << std::endl;
        return 1;
    } 

    return 0;

}
