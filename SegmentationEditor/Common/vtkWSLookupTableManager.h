/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkWSLookupTableManager.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
// .Name vtkWSLookupTableManager
// .Section Description
//  This class is used to manage the display watershed segmentation data via
//  a color lookup table.  It provides methods and data structures to keep
//  track of the state of merges among segments in a labeled image and reflects 
//  those merges through a color lookup table.
#ifndef __vtkWSLookupTableManager_
#define __vtkWSLookupTableManager_

#include "vtkPatchedLookupTable.h"
#include "vtkLookupTableEquivalencyHash.h"
#include "vtkImageData.h"

class VTK_EXPORT vtkWSLookupTableManager : public vtkObject
{
public:
  static vtkWSLookupTableManager *New();

  vtkTypeMacro(vtkWSLookupTableManager,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) {}
  
  // Description:
  // Fills the table with a random color for each label.
  void GenerateColorTable();


  // Perform a label-based merge.  This merge method performs all merges up 
  // to and including the next merge of label l, starting from position
  // CurrentPositionPointer in the list.
  float Merge(unsigned long l);

  // Performs a label-based merge by finding the current leaf node label for
  // the selected region and propagating the merge threshold to the next merge
  // by this region using Merge(unsigned long).
  float MergeSelected();

  // Perform a threshold-based merge.
  // If CurrentThreshold < t: Perform merges after CurrentPositionPointer in
  //                          the MergeList until CurrentThreshold > t.
  // If CurrentThreshold > t: Undo all merges before CurrentPositionPointer
  //                          in the MergeList until CurrentThreshold < t.
  // If CurrentThreshold = t: Do nothing.
  void Merge(float t);

  // Resets the merge state to the UndoPositionPointer.
  float UndoLastMerge();

  // Loads a merge tree from a data file with filename fn.
  void LoadTreeFile(const char* fn);  

  // Sets the various table parameters based on the number of labels
  // in the image.  Number of labels must be specified by the user.
  void SetNumberOfLabels(unsigned long n);
  vtkGetMacro(NumberOfLabels, unsigned long);

  vtkPatchedLookupTable *GetLookupTable()
    { return this->LookupTable; }

  void MergeEquivalencies();

  void RandomizeColors()
    {
      this->GenerateColorTable();
      this->MergeEquivalencies();
    }

  // Searches the equivalency table for labels equivalent to n.  The list of
  // equivalencies is then stored internally in this object because of the
  // complexities associated with interfacing with Tcl.  FOR THIS REASON THE
  // METHOD IS NOT THREAD SAFE.
  void CompileEquivalenciesFor(unsigned long n);

  // Appends equivalency table with labels equivalent to n.  This can be used
  // to select multiple regions.
  void AppendEquivalenciesFor(unsigned long n);

  // Searches the equivalency table for labels equivalent to the value at x, y, 
  // z.  The list of
  // equivalencies is then stored internally in this object because of the
  // complexities associated with interfacing with Tcl.  FOR THIS REASON THE
  // METHOD IS NOT THREAD SAFE.
  void CompileEquivalenciesFor(int x, int y, int z, vtkImageData *);

  void AppendEquivalenciesFor(int x, int y, int z, vtkImageData *);

  void RecompileEquivalencies();
  
  // Returns the last list of equivalencies computed by
  // CompileEquivalenciesFor.  The first element in the list denotes the number 
  // of elements that follow. (i.e. the list has size: list[0] + 1).
  const unsigned long *GetEquivalencies() const
    { return ComputedEquivalencyList; }
  
  unsigned long GetMergedToLabel(unsigned long n)
    { return EquivalencyTable.RecursiveLookup(n); }

  void HighlightComputedEquivalencyList();
  void ClearComputedEquivalencyList()
    {
      if (ComputedEquivalencyList != 0) delete [] ComputedEquivalencyList;
      ComputedEquivalencyList = 0;
    }

  void ClearHighlightedValuesToSameColor();
  void ClearHighlightedValues();
  void HighlightValue( unsigned long n );
  void UnHighlightValue( unsigned long n );
  void RepaintHighlightedValues();

  vtkSetMacro(RepaintHighlights, int);
  vtkGetMacro(RepaintHighlights, int);
  
  void SetHighlightColor(float r, float g, float b)
    { HighlightColor[0] = r;
      HighlightColor[1] = g;
      HighlightColor[2] = b; }
  const float *GetHighlightColor() const
    { return HighlightColor; }
  
  vtkSetMacro(CurrentThreshold, float);
  vtkGetMacro(CurrentThreshold, float);

  void Initialize();
  
protected:
  vtkWSLookupTableManager();
  ~vtkWSLookupTableManager();

private:
  vtkPatchedLookupTable *LookupTable;
  vtkLookupTableEquivalencyHash EquivalencyTable;
  
  float    MaximumSaliency;        // Maximum value the saliency can take.
  float    CurrentThreshold;       // Current saliency level of the merge.
  merge_t *CurrentPositionPointer; // Current position in the MergeList.
                                   // Represents the state of the exposed leaf nodes.
  merge_t *MergeList;              // Ordered list of all possible merges in the
                                   // binary merge tree.
  merge_t *UndoPositionPointer;    // The last value that CurrentPositionPointer held.
  
  unsigned long NumberOfLabels;    // Number of labels needed in the lookup table.

  unsigned long *ComputedEquivalencyList;   // See CompileEquivalenciesFor and
                                            // GetEquivalencies above.  THIS
                                            // SHOULD PROBABLY BE CALLED THE
                                            // SelectedLabelsList 

  unsigned_long_list_t HighlightedValueList;

  int RepaintHighlights;
  float HighlightColor[3];
  
  vtkWSLookupTableManager(const vtkWSLookupTableManager&);
  void operator=(const vtkWSLookupTableManager&); // Not implemented
};

#endif
