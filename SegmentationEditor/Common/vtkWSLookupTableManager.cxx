/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkWSLookupTableManager.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "vtkWSLookupTableManager.h"
#include "vtkObjectFactory.h"
#include <fstream>

vtkWSLookupTableManager* vtkWSLookupTableManager::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkWSLookupTableManager");
  if(ret)
    {
    return (vtkWSLookupTableManager*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkWSLookupTableManager;
}

vtkWSLookupTableManager::vtkWSLookupTableManager()
{
  // Initialize member variables.
  this->LookupTable = 0;
  //  this->LookupTable = vtkPatchedLookupTable::New();
  this->CurrentThreshold       = 0.0;
  this->CurrentPositionPointer = 0;
  this->MergeList              = 0;
  this->NumberOfLabels         = 0;
  this->MaximumSaliency        = 0.0;
  this->ComputedEquivalencyList= 0;
  this->HighlightColor[0] = 1.0;
  this->HighlightColor[1] = 1.0;
  this->HighlightColor[2] = 1.0;
  this->RepaintHighlights = 0;
}

vtkWSLookupTableManager::~vtkWSLookupTableManager()
{
  if (this->MergeList != 0) { delete[] this->MergeList; }
  if (this->ComputedEquivalencyList !=0 )
    { delete[] this->ComputedEquivalencyList; }
  this->LookupTable->Delete();
}

void vtkWSLookupTableManager::CompileEquivalenciesFor(int x, int y, int z,
                                                      vtkImageData *data)
{
  // Find the value at x,y,z
  unsigned long n = * ( (unsigned long *) (data->GetScalarPointer(x,y,z)) );
  this->CompileEquivalenciesFor(n);
}

void vtkWSLookupTableManager::AppendEquivalenciesFor(int x, int y, int z,
                                                      vtkImageData *data)
{
  // Find the value at x,y,z
  unsigned long n = * ( (unsigned long *) (data->GetScalarPointer(x,y,z)) );
  this->AppendEquivalenciesFor(n);
}

void vtkWSLookupTableManager::RecompileEquivalencies()
{
  if (this->ComputedEquivalencyList ==0 ) return;

  // Get a region label in the current list and recompute the list
  // based on that value.
 this->CompileEquivalenciesFor(ComputedEquivalencyList[1]);
}

void vtkWSLookupTableManager::AppendEquivalenciesFor(unsigned long n)
{
  unsigned_long_list_t list;

  // If there is already an equivalency list,
  //   then initialize the new list with those values.
  if (this->ComputedEquivalencyList != 0)
    { // ignore the first entry, this was the old size of the list
      for (unsigned int i = 1; i < ComputedEquivalencyList[0] +1; ++i)
        {
          list.push_back(ComputedEquivalencyList[i]);
        }
    }

  if (this->ComputedEquivalencyList !=0 )
    { delete[] this->ComputedEquivalencyList; }
  
  // Get the real value we are looking for.
  unsigned long real_n = EquivalencyTable.RecursiveLookup(n);
  list.push_back(real_n);
  
  // Search for equivalencies.
  for (vtkLookupTableEquivalencyHash::ConstIterator hashIt = EquivalencyTable.Begin();
       hashIt != EquivalencyTable.End(); hashIt++)
    {
      if (EquivalencyTable.RecursiveLookup( (*hashIt).first ) == real_n)
        list.push_back( (*hashIt).first );
    }  
  
  // Copy the list into the array
  this->ComputedEquivalencyList = new unsigned long[list.size() + 1];
  ComputedEquivalencyList[0] = (unsigned long) list.size();
  unsigned long i = 1;
  for (unsigned_long_list_t::iterator it = list.begin();
       it != list.end(); it++, i++)
    {
      ComputedEquivalencyList[i] = *it;
    }
  
}


void vtkWSLookupTableManager::CompileEquivalenciesFor(unsigned long n)
{ 
  if (this->ComputedEquivalencyList !=0 )
    { delete[] this->ComputedEquivalencyList; }

  unsigned_long_list_t list;

  // Get the real value we are looking for.
  unsigned long real_n = EquivalencyTable.RecursiveLookup(n);
  list.push_back(real_n);
  
  // Search for equivalencies.
  for (vtkLookupTableEquivalencyHash::ConstIterator hashIt = EquivalencyTable.Begin();
       hashIt != EquivalencyTable.End(); hashIt++)
    {
      if (EquivalencyTable.RecursiveLookup( (*hashIt).first ) == real_n)
        list.push_back( (*hashIt).first );
    }  
  
  // Copy the list into the array
  this->ComputedEquivalencyList = new unsigned long[list.size() + 1];
  ComputedEquivalencyList[0] = (unsigned long) list.size();
  unsigned long i = 1;
  for (unsigned_long_list_t::iterator it = list.begin();
       it != list.end(); it++, i++)
    {
      ComputedEquivalencyList[i] = *it;
    }
}

void vtkWSLookupTableManager::HighlightValue( unsigned long n )
{
  // Add to highlight list
  this->HighlightedValueList.push_back(n);
   
  // Set to selected color
  this->LookupTable->SetTableValue(n,
                                   HighlightColor[0],
                                   HighlightColor[1],
                                   HighlightColor[2]);
}

void vtkWSLookupTableManager::UnHighlightValue(unsigned long n)
{
  this->HighlightedValueList.remove(n);
  this->LookupTable->SetTableValue(n,
                                   (float)(rand() / (RAND_MAX + 1.0)),
                                   (float)(rand() / (RAND_MAX + 1.0)),
                                   (float)(rand() / (RAND_MAX + 1.0)) );
}

void vtkWSLookupTableManager::RepaintHighlightedValues()
{
    unsigned_long_list_t::iterator it;
  for (it = HighlightedValueList.begin();
       it != HighlightedValueList.end();
       it++)
    {
      this->LookupTable->SetTableValue(*it,
                                       HighlightColor[0],
                                       HighlightColor[1],
                                       HighlightColor[2]);
    }

}

void vtkWSLookupTableManager::ClearHighlightedValues()
{
  unsigned_long_list_t::iterator it;
  for (it = HighlightedValueList.begin();
       it != HighlightedValueList.end();
       it++)
    {
      this->LookupTable->SetTableValue(*it,
                                       (float)(rand() / (RAND_MAX + 1.0)),
                                       (float)(rand() / (RAND_MAX + 1.0)),
                                       (float)(rand() / (RAND_MAX + 1.0)) );
    }
  HighlightedValueList.clear();
}

void vtkWSLookupTableManager::ClearHighlightedValuesToSameColor()
{
  float r =  (float)(rand() / (RAND_MAX + 1.0));
  float g =  (float)(rand() / (RAND_MAX + 1.0));
  float b =  (float)(rand() / (RAND_MAX + 1.0));
  
  unsigned_long_list_t::iterator it;
  for (it = HighlightedValueList.begin();
       it != HighlightedValueList.end();
       it++)
    {
      this->LookupTable->SetTableValue(*it, r, g, b);
    }
  HighlightedValueList.clear();
}

void vtkWSLookupTableManager::HighlightComputedEquivalencyList()
{
  if (this->ComputedEquivalencyList == 0)  { return;  }
  
  unsigned long i;
  for (i = 1; i <= ComputedEquivalencyList[0]; i++ )
    {
      this->HighlightValue( ComputedEquivalencyList[i] );
    }
  
}

void vtkWSLookupTableManager::GenerateColorTable()
{
  for (unsigned long i = 0; i < this->LookupTable->GetNumberOfTableValues(); i++)
    {
      this->LookupTable->SetTableValue(i, (float)(rand() / (RAND_MAX + 1.0)),
                                          (float)(rand() / (RAND_MAX + 1.0)),
                                          (float)(rand() / (RAND_MAX + 1.0)) );
    }
  if (this->RepaintHighlights !=0 ) this->RepaintHighlightedValues();
}

void vtkWSLookupTableManager::Initialize()
{
  // Delete old and allocate new table.
  if ( this->LookupTable !=0 )
    {
      this->LookupTable->Delete();
    }
  this->LookupTable = vtkPatchedLookupTable::New();

  // Reset all other values and tables
  if (this->MergeList != 0) delete[] this->MergeList;
  if (this->ComputedEquivalencyList !=0 ) delete[] this->ComputedEquivalencyList;
  this->CurrentPositionPointer = 0;
  HighlightedValueList.clear();
  EquivalencyTable.Clear();

  this->CurrentThreshold       = 0.0;
  this->CurrentPositionPointer = 0;
  this->MergeList              = 0;
  this->NumberOfLabels         = 0;
  this->MaximumSaliency        = 0.0;
  this->ComputedEquivalencyList= 0;
}

void vtkWSLookupTableManager::LoadTreeFile(const char* fn)
{
  ifstream in(fn);
  if (!in)
    {
      vtkErrorMacro (<<"Bad file name: " << fn);
      exit(-1);
    }
  
  // read header
  unsigned long listsz;
  in.read((unsigned char *)&listsz, sizeof(unsigned long));

  // allocate merge list
  if (this->MergeList != 0) delete[] this->MergeList;
  this->MergeList = new merge_t[listsz + 2];
  
  // now read the data
  in.read((unsigned char *)(this->MergeList + 1), listsz * sizeof(merge_t));

  if (in.gcount() != listsz *sizeof(merge_t))
    {
      vtkErrorMacro(<<"Error reading " << fn << ". File size does not match header size.");
      exit(-1);
    }

  in.close();

  // set current position to beginning of list & reset threshold
  this->CurrentPositionPointer = this->MergeList + 1;
  this->CurrentThreshold       = 0.0;

  // set the maximum saliency
  this->MaximumSaliency = (this->MergeList + listsz)->saliency;

  // mark the first and last (empty) elements
  (this->MergeList + listsz + 1)->saliency = -2.0; // end
  this->MergeList->saliency = -1.0; // start  
  
}

void vtkWSLookupTableManager::SetNumberOfLabels(unsigned long n)
{
  this->NumberOfLabels = n;
  this->LookupTable->SetTableRange(0, n);
  this->LookupTable->SetNumberOfColors(n);
  this->LookupTable->SetNumberOfTableValues(n);
  this->Modified();
}

float vtkWSLookupTableManager::UndoLastMerge()
{
  if (UndoPositionPointer == 0 ) return 0.0;
  merge_t *temp;

  float threshold = (float) ( (double) (UndoPositionPointer->saliency) /
    (double) MaximumSaliency );
  
  this->Merge(threshold);

  return CurrentThreshold;
  
}

float vtkWSLookupTableManager::MergeSelected()
{
  if ( ComputedEquivalencyList == 0 ) return CurrentThreshold;

  // Find the current leaf node represented by the ComputedEquivalencyList.
  // Just need to look at the first element in the list (guaranteed to be
  // there).
  unsigned long leaf_node =
    EquivalencyTable.RecursiveLookup(ComputedEquivalencyList[1]);
  return (this->Merge(leaf_node));  
}

float vtkWSLookupTableManager::Merge(unsigned long n)
{
  // Are we at the end of the list?
  if ( (CurrentPositionPointer + 1)->saliency < 0 ) return 1.0;

  UndoPositionPointer = CurrentPositionPointer;
 
  // Find the threshold of the next merge of this leaf node n or the threshold
  // of the next merge of any node with this one.
  if (MergeList == 0 || CurrentPositionPointer == 0 || MaximumSaliency == 0.0)
    {
      vtkErrorMacro("No segment tree has been specified for merging.");
      exit(-1);
    }

  while (CurrentPositionPointer->from != n
         && CurrentPositionPointer->to !=n )
    {
      EquivalencyTable.Add(CurrentPositionPointer->from,
                           CurrentPositionPointer->to);
      
      CurrentPositionPointer++;

      // stop at the end of the list
      if (CurrentPositionPointer->saliency < 0)
        {
          CurrentPositionPointer--;
          break;
        }
    }
 
  // Do the next merge, which is the node we were looking for.
  EquivalencyTable.Add(CurrentPositionPointer->from,
                       CurrentPositionPointer->to);
 
  CurrentPositionPointer++;
  
  // stop at the end of the list
  if (CurrentPositionPointer->saliency < 0)
    {
      CurrentPositionPointer--;
    }
 
  // Merge the equivalent segments' colors in the lookup table.
  this->MergeEquivalencies();
  CurrentThreshold = (float) ( (double)(CurrentPositionPointer->saliency) /
                               (double)MaximumSaliency ); 
  
  return CurrentThreshold;
}

void vtkWSLookupTableManager::Merge(float t)
{
  if (this->LookupTable == 0 ) return;

  float v;

  v = t * MaximumSaliency;
  
  // save current state for undo
  UndoPositionPointer = CurrentPositionPointer;
    
  if ( CurrentThreshold <= t)  // Move up the tree
    {
      while ( CurrentPositionPointer->saliency <= v )
        {
          EquivalencyTable.Add(CurrentPositionPointer->from,
                               CurrentPositionPointer->to);

          CurrentPositionPointer++;

          // stop at the end of the list
          if (CurrentPositionPointer->saliency < 0)
            {
              CurrentPositionPointer--;
              break;
            }
        }

    }

  else if ( CurrentThreshold > t)  // Move down the tree
    {
      while ( CurrentPositionPointer->saliency > v )
        {
          EquivalencyTable.Erase(CurrentPositionPointer->from);
          LookupTable->SetTableValue(CurrentPositionPointer->from,
                                     (float)(rand() / (RAND_MAX + 1.0)),
                                     (float)(rand() / (RAND_MAX + 1.0)),
                                     (float)(rand() / (RAND_MAX + 1.0)) );
          CurrentPositionPointer--;

          // stop at the beginning of the list
          if (CurrentPositionPointer->saliency < 0)
            {
              CurrentPositionPointer++;
              break;
            }
          
        }

    }

  // Merge the equivalent segments' colors in the lookup table.
  this->MergeEquivalencies();
  CurrentThreshold = t;
}

void vtkWSLookupTableManager::MergeEquivalencies()
{

  // Merge colors in the lookup table
  vtkLookupTableEquivalencyHash::Iterator it;
  it = EquivalencyTable.Begin();

  while (it != EquivalencyTable.End())
    {
      LookupTable->SetTableValue((*it).first,
        LookupTable->GetTableValue(EquivalencyTable.RecursiveLookup((*it).second))); 
      it++;
    }

}

