/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTreeGeneratorApp.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "chunks.h"
#include "itkWatershedSegmentTreeGenerator.h"
#include "itkSWSSegmentTableReader.h"
#include "itkSWSSegmentTreeWriter.h"
#include "itkSWSSegmentTreeReader.h"
#include "itkSWSEquivalencyTableReader.h"
#include "itkImage.h"
#include "param.h"
#include "itkCommand.h"
#include <string>

namespace itk {
class PrintProgressCommand : public Command
{
public:
  /** Smart pointer declaration methods */
  typedef PrintProgressCommand Self;
  typedef Command Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  itkTypeMacro( PrintProgressCommand, Command );
  itkNewMacro(Self);

  /** Standard Command virtual methods */
  void Execute(Object *caller, const EventObject &event);
  void Execute(const Object *caller, const EventObject &event);

protected:
  PrintProgressCommand() {}
  virtual ~PrintProgressCommand() {}
  
private:
};
  
void PrintProgressCommand
::Execute(Object *caller, const EventObject &event)
{
  ProcessObject *po = dynamic_cast<ProcessObject *>(caller);
  if (! po) return;
  
  if( typeid(event) == typeid ( ProgressEvent)  )
    {
      std::cout << "Progress at " << po->GetProgress() * 100 << "% " <<std::endl;;
    }
}

void PrintProgressCommand
::Execute(const Object *caller, const EventObject &event)
{
  ProcessObject *po = dynamic_cast<ProcessObject *>(const_cast<Object *>(caller));
  if (! po) return;
  
  if( typeid(event) == typeid ( ProgressEvent)  )
    {
      std::cout << "Progress at " << po->GetProgress() * 100<< "% "<< std::endl;;
    }
}
} // namespace itk

inline void die(const char *s)
{
  std::cerr << s << std::endl;
  exit(-1);
}

int main(int argc, char *argv[])
{  
  typedef itk::Image<float, 3> FloatImageType;
  typedef itk::Image<unsigned long, 3> LongImageType;

  char strbuf[255];
  
  std::string fn, segment_file_name, tree_file_name, equivalency_file_name;
  int merge_equivalencies;
  float max_floodlevel;

  char *default_fn = "treegenerator.in";
  if (argc >1) fn = argv[1];
  else fn = default_fn;

  try {

    VPF::ParameterFile pf;
    // Open and parse the parameter file.  Required parameters.
    pf.Initialize(fn.c_str());
    
    if (VPF::set(segment_file_name, pf["segment_file_name"][0]) == VPF::INVALID)
      die ("Cannot find segment_file_name parameter");
    if (VPF::set(equivalency_file_name, pf["equivalency_file_name"][0]) == VPF::INVALID)
      die ("Cannot find equivalency_file_name parameter");
    if (VPF::set(tree_file_name, pf["tree_file_name"][0])   == VPF::INVALID)
      die ("Cannot find output tree parameter");
    if (VPF::set(max_floodlevel, pf["max_floodlevel"][0]) == VPF::INVALID)
      die ("Cannot find the max_threshold parameter");
    if (VPF::set(merge_equivalencies, pf["merge_equivalencies"][0]) == VPF::INVALID)
          die ("Cannot find the merge_equivalencies parameter");

    itk::PrintProgressCommand::Pointer c =itk::PrintProgressCommand::New();
    
    itk::watershed::SegmentTreeGenerator<float>::Pointer generator =
      itk::watershed::SegmentTreeGenerator<float>::New();
    
    itk::sws::SegmentTableReader<float>::Pointer table_reader =
      itk::sws::SegmentTableReader<float>::New();
    
    itk::sws::SegmentTreeReader<float>::Pointer tree_reader =
      itk::sws::SegmentTreeReader<float>::New();
    
    itk::sws::SegmentTreeWriter<float>::Pointer tree_writer =
      itk::sws::SegmentTreeWriter<float>::New();
    
    itk::sws::EquivalencyTableReader::Pointer eq_reader =
      itk::sws::EquivalencyTableReader::New();
    
    eq_reader->SetFileName(equivalency_file_name.c_str());
    
    table_reader->SetFileName(segment_file_name.c_str());
    if (((bool)merge_equivalencies) == true)
      {
        generator->SetInputEquivalencyTable(eq_reader->GetOutput());
      }
    generator->SetConsumeInput(true);
    generator->SetInputSegmentTable(table_reader->GetOutput());
    generator->SetFloodLevel((double)max_floodlevel);
    generator->SetMerge((bool)merge_equivalencies);
    generator->AddObserver(itk::ProgressEvent(), c);

    
    tree_writer->SetInput(generator->GetOutputSegmentTree());
    tree_writer->SetFileName(tree_file_name.c_str());
    tree_writer->Update();
  }
  catch (itk::ExceptionObject & e)
    {
      std::cout << "CAUGHT ITK EXCEPTION" << std::endl;
      std::cout << e << std::endl;
    }

  return 0;
}
