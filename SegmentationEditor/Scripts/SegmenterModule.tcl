#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    SegmenterModule.tcl
#  Language:  C++
#  Date:      $Date$
#  Version:   $Revision$
#
#  Copyright (c) 2002 Insight Consortium. All rights reserved.
#  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#     This software is distributed WITHOUT ANY WARRANTY; without even 
#     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
#     PURPOSE.  See the above copyright notices for more information.
#
#=========================================================================*/
package require vtkinteraction

##
# This file contains methods for implementing the
# watershed segmentation stage of processing.
#

#
#Initialize some global variables for this module
set SegmenterGlobals(frame_title_text) "Watershed Segmenter"
set SegmenterGlobals(source_window_label) "Source Volume"
set SegmenterGlobals(output_window_label) "Segmentation Preview"
set SegmenterGlobals(default_color_window) 255.0
set SegmenterGlobals(default_color_level) 127.5
set SegmenterGlobals(default_lookup_table_low) 0.0
set SegmenterGlobals(default_lookup_table_high) 255.0
set SegmenterGlobals(default_threshold_parameter) 0.007
set SegmenterGlobals(default_maximum_merge) 0.05
set SegmenterGlobals(source_label) "Source data"
set SegmenterGlobals(segmenter_label) "Watershed segmentation"
set SegmenterGlobals(segmentation_filename) "segmented_volume"
set SegmenterGlobals(tree_filename) "segment_tree"
set SegmenterGlobals(segmentation_tag) "segmentation"
set SegmenterGlobals(tree_tag) "segmentation_tree"
set SegmenterGlobals(help_file) "SegmenterHelp.txt"
set SegmenterGlobals(help_text) "Sorry, could not find the file $SegmenterGlobals(help_file)"
set SegmenterGlobals(display_window_is_visible) "false"

proc ConstructSegmenterFrame {f} {
    global Options DataGlobals SegmenterGlobals
    
    #Initialize the module
    SegmenterInitialize
    
    # Create the GUI
    $f configure -background $Options(text_background_color)

    # Some title text
    frame $f.title -background $Options(pleasant_background_color)
    label $f.title.text -textvariable SegmenterGlobals(frame_title_text) \
        -background $Options(pleasant_background_color) \
        -font $Options(h1_font)
    pack $f.title.text -side left

    # Source selector
    frame $f.data_source
    frame $f.data_source.loadframe
    label $f.data_source.label1 -font $Options(h3_font) -text $SegmenterGlobals(source_label)
    listbox $f.data_source.loadframe.listbox -selectmode browse -width 20 \
        -height $Options(data_window_size) \
        -yscrollcommand "$f.data_source.loadframe.scrollbar set"
    scrollbar $f.data_source.loadframe.scrollbar -orient "vert" \
        -command "$f.data_source.loadframe.listbox yview" 
    pack $f.data_source.loadframe.scrollbar -side right -fill y
    pack $f.data_source.loadframe.listbox

    RegisterDataSelector $f.data_source.loadframe.listbox $DataGlobals(scalar_type)
    set SegmenterGlobals(data_selector) $f.data_source.loadframe.listbox

    # Load button for source selector
    button $f.data_source.loadbutton -command SegmenterLoadData -text "Load"

    grid configure $f.data_source.label1 -padx 3 -column 0 -row 0 \
        -columnspan 2 -sticky nw
    grid configure $f.data_source.loadframe -padx 3 -column 0 -row 1 \
        -columnspan 2 -sticky nw
#    grid configure $f.data_source.loadbutton -padx 3 -column 0 -row 2 -sticky nw
  
    # Controls for the segmenter
    frame $f.segmenter
    label $f.segmenter.label1 -font $Options(h3_font) -text $SegmenterGlobals(segmenter_label)
    entry $f.segmenter.threshold -width 6 -justify r
    entry $f.segmenter.maximum_merge -width 6 -justify r
    label $f.segmenter.threshold_label -text "Lower threshold (0.0 - 1.0)"
    label $f.segmenter.maximum_merge_label -text "Maximum flood level (0.0 - 1.0)"

    set SegmenterGlobals(threshold_entry) $f.segmenter.threshold
    set SegmenterGlobals(maximum_merge_entry)  $f.segmenter.maximum_merge

    button $f.segmenter.start -text "Start" -command {SpawnProcess "SegmenterStartSegmentation" "SegmenterRecoverFailedSegmentation"}
    set SegmenterGlobals(start_button) $f.segmenter.start
 
    $f.segmenter.threshold insert 0 $SegmenterGlobals(default_threshold_parameter)
    $f.segmenter.maximum_merge insert 0 $SegmenterGlobals(default_maximum_merge)
    
    grid configure $f.segmenter.label1 -padx 3 -column 0 -row 0 -columnspan 2 -sticky nw
    grid configure $f.segmenter.maximum_merge_label -padx 3 -column 0 -row 1 -sticky nw
    grid configure $f.segmenter.maximum_merge -padx 3 -column 1 -row 1 -sticky nw
    grid configure $f.segmenter.threshold_label -padx 3 -column 0 -row 2 -sticky nw
    grid configure $f.segmenter.threshold -padx 3 -column 1 -row 2 -sticky nw
    grid configure $f.segmenter.start -padx 3 -column 0 -row 3 -sticky nw 
    
    # Create the output display window
    SegmenterCreateOutputDisplayWindow $f


 # Create a little box for instructions
    frame $f.instructions
      text $f.instructions.text \
        -font $Options(mono_font) \
        -width 75 \
        -height 10 \
        -background $Options(help_box_color) \
        -wrap "word" \
        -insertontime 0 \
        -setgrid true  \
        -yscrollcommand "$f.instructions.scrollbar set"

    scrollbar $f.instructions.scrollbar -orient "vert" -command "$f.instructions.text yview"

    if {[file exists $SegmenterGlobals(help_file)] == 1} {
        set fileId [open $SegmenterGlobals(help_file) r]
        
        if {$fileId != ""} {
            set i 0
            while {! [eof $fileId]} {
                set i [expr $i +1 ]
                set tmp [gets $fileId]
                append tmp "\n"
                [subst {$f.instructions.text}] insert $i.0 $tmp
            }
        }
    } else { 
        [subst {$f.instructions.text}] insert 0.0 $SegmenterGlobals(help_text)
    }
    
    pack $f.instructions.scrollbar -side right -fill y
    pack $f.instructions.text -fill both -expand t

    $f.instructions.text configure -state disabled

    # Configure layout
    grid configure $f.title -row 0 -column 0 -columnspan 2 -sticky nw -padx 10
    grid configure $f.data_source -row 1 -column 0 -sticky nw -padx 10 -pady 10
    grid configure $f.segmenter -row 1  -column 1  -sticky nw -padx 10 -pady 10
    grid configure $f.instructions -row 2 -column 0 -sticky nw -padx 3 -pady 3 \
        -columnspan 2
}

proc SegmenterRecoverFailedSegmentation {} {
    global SegmenterGlobals
    
    $SegmenterGlobals(start_button) configure -state normal
    set SegmenterGlobals(display_window_is_visible) "false"
}

proc SegmenterCreateOutputDisplayWindow {f} {
    global SegmenterGlobals Options

    toplevel $f.viewer
    wm title $f.viewer $SegmenterGlobals(output_window_label)
    vtkTkImageViewerWidget $f.viewer.widget -iv $SegmenterGlobals(segmentation_viewer)
    BindTkImageViewer $f.viewer.widget
    wm protocol $f.viewer WM_DELETE_WINDOW { }
    set SegmenterGlobals(segmentation_widget) $f.viewer.widget

    set SegmenterGlobals(output_window) $f.viewer

    scale $f.viewer.slice -from 0 -to 0 -orient horizontal \
        -command SegmenterSetSlice -variable SegmenterGlobals(slice_number)
    set SegmenterGlobals(slice_scale) $f.viewer.slice

    scale $f.viewer.scale -from 0.0 -to 1.0\
    -length 500 \
    -resolution 0.001 \
    -orient horizontal \
    -variable SegmenterGlobals(scale_level) \
    -label "Scale" -command SegmenterSetMerge

    set SegmenterGlobals(scale_level) 0
    set SegmenterGlobals(previous_scale_level) 0

    grid configure $f.viewer.widget -row 0 -column 0 -columnspan 1 -padx 10
    grid configure $f.viewer.slice  -row 1 -column 0 -columnspan 1 -padx 10
    grid configure $f.viewer.scale  -row 2 -column 0 -columnspan 1 -padx 10

    wm withdraw $SegmenterGlobals(output_window)
    set SegmenterGlobals(display_window_is_visible) "false"
}

proc SegmenterSetMerge {scale} {
    global SegmenterGlobals
    if {$SegmenterGlobals(scale_level) == $SegmenterGlobals(previous_scale_level)} {
        return
    }

    $SegmenterGlobals(lookup_table_manager) ClearHighlightedValuesToSameColor
    $SegmenterGlobals(lookup_table_manager) Merge $scale
    set $SegmenterGlobals(previous_scale_level) $scale;

    $SegmenterGlobals(segmentation_viewer) Render
}

proc SegmenterSetSlice {slice} {
    global SegmenterGlobals
    $SegmenterGlobals(segmentation_viewer) SetZSlice $slice
    $SegmenterGlobals(segmentation_viewer) Render
}

proc SegmenterLoadData {} {
    global SegmenterGlobals

    set tagName [ $SegmenterGlobals(data_selector) get \
                      [$SegmenterGlobals(data_selector) curselection]]
    set cacheEntry [GetDataCacheEntry $tagName]

    set endianString "SetDataByteOrderTo"
    append endianString [GetCacheEntryValue $cacheEntry "endianness"]
    append endianString "Endian"
    $SegmenterGlobals(source_reader) $endianString

    set datatypeString "SetDataScalarTypeTo"
    append datatypeString [GetCacheEntryValue $cacheEntry "data_type"]

    $SegmenterGlobals(source_reader) SetNumberOfScalarComponents \
        [GetCacheEntryValue $cacheEntry "number_of_components"]
    $SegmenterGlobals(source_reader) $datatypeString
    $SegmenterGlobals(source_reader) SetDataExtent \
        0 [GetCacheEntryValue $cacheEntry "size_x"] \
        0 [GetCacheEntryValue $cacheEntry "size_y"] \
        0 [GetCacheEntryValue $cacheEntry "size_z"]

    if { [regexp {^[yY]} [GetCacheEntryValue $cacheEntry "multiple_files"] ] } {
        $SegmenterGlobals(source_reader) SetFileDimensionality 2
        $SegmenterGlobals(source_reader) SetFilePrefix \
            [GetCacheEntryValue $cacheEntry "filename"] 
        $SegmenterGlobals(source_reader) SetFilePattern \
            [GetCacheEntryValue $cacheEntry "file_pattern"]
    } else {
        $SegmenterGlobals(source_reader) SetFileDimensionality 3
        $SegmenterGlobals(source_reader) SetFileName \
            [GetCacheEntryValue $cacheEntry "filename"]
    }

    set SegmenterGlobals(loaded_data_tag) $cacheEntry

    $SegmenterGlobals(source_reader) Modified

    [$SegmenterGlobals(source_reader) GetOutput] SetUpdateExtentToWholeExtent
    $SegmenterGlobals(source_reader) Update
}

proc SegmenterInitialize {} {
    global SegmenterGlobals DataGlobals
    
    # A watershed image filter
    vtkITKWatershedFilterAndWriter SegmenterWatershed
    SegmenterWatershed SetThreshold $SegmenterGlobals(default_threshold_parameter)
    SegmenterWatershed SetLevel     $SegmenterGlobals(default_maximum_merge)
    set SegmenterGlobals(watershed_filter) SegmenterWatershed

    # A reader for the source data
    vtkImageReader SegmenterSourceReader
    SegmenterSourceReader SetHeaderSize 0
    set SegmenterGlobals(source_reader) SegmenterSourceReader

    # A filter to flip the y axis
    vtkImageFlip SegmenterFlipper
    SegmenterFlipper SetFilteredAxis 1
    set SegmenterGlobals(source_flipper) SegmenterFlipper
    
    # A reader for the segmentation data
    vtkPatchedImageReader SegmentationReader
    SegmentationReader SetHeaderSize 0
    SegmentationReader SetDataByteOrderToLittleEndian
    SegmentationReader SetDataScalarTypeToUnsignedLong
    set SegmenterGlobals(segmentation_reader) SegmentationReader

    # A lookup table manager for the segmentation viewer
    vtkWSLookupTableManager SegmentationLookupTableManager
    SegmentationLookupTableManager SetHighlightColor 1.0 1.0 1.0
    SegmentationLookupTableManager SetRepaintHighlights 1
    set SegmenterGlobals(lookup_table_manager) SegmentationLookupTableManager

    # A color mapper for colorizing segmented data
    vtkImageMapToColors SegmentationMapToRGBA
    SegmentationMapToRGBA SetOutputFormatToRGBA
    set SegmenterGlobals(color_mapper) SegmentationMapToRGBA
    
    # A viewer for segmented data
    vtkImageViewer SegmentationViewer
    SegmentationViewer SetColorWindow 255.0
    SegmentationViewer SetColorLevel 127.5
    SegmentationViewer SetZSlice 0
    set SegmenterGlobals(segmentation_viewer) SegmentationViewer
}

proc ActivateSegmenterModule {} {
    global SegmenterGlobals
    if { $SegmenterGlobals(display_window_is_visible) == "true" } {
        wm deiconify $SegmenterGlobals(output_window)
    }
}

proc DeactivateSegmenterModule {} {
    global SegmenterGlobals
    wm withdraw $SegmenterGlobals(output_window)
}

proc SegmenterStartSegmentation {} {
    global SegmenterGlobals DataGlobals Globals

    wm withdraw $SegmenterGlobals(output_window)
    set SegmenterGlobals(display_window_is_visible) "false"
    $SegmenterGlobals(start_button) configure -state disabled
    # Construct the file names
    set tree_fn $DataGlobals(cache_directory)
    set output_fn $DataGlobals(cache_directory)
    append tree_fn "/$DataGlobals(unique_user_prefix)"
    append output_fn "/$DataGlobals(unique_user_prefix)"
    append tree_fn $SegmenterGlobals(tree_filename)
    append output_fn $SegmenterGlobals(segmentation_filename)
    append tree_fn ".tree"
    append output_fn ".raw"

    # Load the source
    SegmenterLoadData

    # Set up the pipeline
    $SegmenterGlobals(source_flipper) SetInput \
        [$SegmenterGlobals(source_reader) GetOutput]
    $SegmenterGlobals(watershed_filter) \
        SetInput [$SegmenterGlobals(source_flipper) GetOutput]
    $SegmenterGlobals(watershed_filter) \
        SetThreshold [$SegmenterGlobals(threshold_entry) get]
    $SegmenterGlobals(watershed_filter) \
        SetLevel [$SegmenterGlobals(maximum_merge_entry) get]

    $SegmenterGlobals(watershed_filter) SetSegmentationFileName $output_fn
    $SegmenterGlobals(watershed_filter) SetTreeFileName $tree_fn

    #Create a progress bar
    ConstructProgressWindow "segmenterProgress"

    #Execute the pipeline
    $SegmenterGlobals(watershed_filter) \
        SetProgressMethod "ProgressProc $SegmenterGlobals(watershed_filter) .segmenterProgress"
    $SegmenterGlobals(watershed_filter) Update
    destroy .segmenterProgress
    update

    set size_z [GetCacheEntryValue $SegmenterGlobals(loaded_data_tag) "size_z"]
    set size_y [GetCacheEntryValue $SegmenterGlobals(loaded_data_tag) "size_y"]
    set size_x [GetCacheEntryValue $SegmenterGlobals(loaded_data_tag) "size_x"]

    #Set up viewer pipeline
    $SegmenterGlobals(segmentation_reader) SetFileName $output_fn
    $SegmenterGlobals(segmentation_reader) SetDataExtent 0 $size_x \
        0 $size_y 0 $size_z
    $SegmenterGlobals(segmentation_reader) SetFileDimensionality 3
    $SegmenterGlobals(segmentation_reader) Modified
    [$SegmenterGlobals(segmentation_reader) GetOutput] \
        SetUpdateExtentToWholeExtent
    $SegmenterGlobals(segmentation_reader) Update
    set maxLabel [$SegmenterGlobals(segmentation_reader) \
                      GetMaximumUnsignedLongValue]

    $SegmenterGlobals(lookup_table_manager) Initialize
    $SegmenterGlobals(lookup_table_manager) LoadTreeFile $tree_fn
    $SegmenterGlobals(lookup_table_manager) SetNumberOfLabels $maxLabel
    $SegmenterGlobals(lookup_table_manager) GenerateColorTable

    $SegmenterGlobals(color_mapper) \
        SetInput [$SegmenterGlobals(segmentation_reader) GetOutput]
    $SegmenterGlobals(color_mapper) SetLookupTable \
        [$SegmenterGlobals(lookup_table_manager) GetLookupTable]

    $SegmenterGlobals(segmentation_viewer) \
        SetInput [$SegmenterGlobals(color_mapper) GetOutput]

    #Update the viewer pipeline
    $SegmenterGlobals(segmentation_widget) configure \
        -height $size_y -width $size_x
    $SegmenterGlobals(slice_scale) configure \
        -length [expr $size_z * 2] -from 0 -to $size_z

#    $SegmenterGlobals(lookup_table_manager) Merge 0.0
    $SegmenterGlobals(segmentation_viewer) Render    
    #Record the data in the cache list
    set entryStr "$output_fn UnsignedLong $size_x $size_y $size_z $DataGlobals(default_file_endianness) No 1 $DataGlobals(default_file_pattern) 0"
    AddDataCacheEntry $SegmenterGlobals(segmentation_tag) $entryStr

    set entryStr "$tree_fn SegmentationTree Null Null Null $DataGlobals(default_file_endianness) Null Null Null Null"
    AddDataCacheEntry $SegmenterGlobals(tree_tag) $entryStr

    wm deiconify $SegmenterGlobals(output_window)
    set SegmenterGlobals(display_window_is_visible) "true"
    $SegmenterGlobals(start_button) configure -state normal
}
