#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    PostprocessorModule.tcl
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

set PostprocessorGlobals(has_run_segmentation) 0
set PostprocessorGlobals(flip_y_axis) 0
set PostprocessorGlobals(load_count) 0
set PostprocessorGlobals(frame_title_text)    "Postprocessor"
set PostprocessorGlobals(source_window_label) "Seed Volume"
set PostprocessorGlobals(feature_window_label) "Feature/Output Volume"
set PostprocessorGlobals(source_label)        "Seed Data"
set PostprocessorGlobals(feature_label)        "Feature Data"
set PostprocessorGlobals(levelset_label)        "Segmentation"
set PostprocessorGlobals(save_label)            "Save Output"
set PostprocessorGlobals(default_color_window) 1.0
set PostprocessorGlobals(default_color_level)  127.5
set PostprocessorGlobals(default_lookup_table_low)  0.0
set PostprocessorGlobals(default_lookup_table_high) 255.0
set PostprocessorGlobals(default_binary_lookup_table_low)  0.0
set PostprocessorGlobals(default_binary_lookup_table_high) 1.0
set PostprocessorGlobals(source_window_visible) "false"
set PostprocessorGlobals(feature_window_visible) "false"
set PostprocessorGlobals(zero_level_color) "1 1 0"
set PostprocessorGlobals(negative_level_color) "1 0 1"
set PostprocessorGlobals(positive_level_color) "0 1 1"
set PostprocessorGlobals(background_level_color) "0 0 0"
set PostprocessorGlobals(help_file) "PostprocessorHelp.txt"
set PostprocessorGlobals(help_text) "ATTENTION!  This is an experimental module that is still under development.  No documentation is currently available except as found in the code for the filters used here."
set PostprocessorGlobals(default_lower_threshold) 65.0
set PostprocessorGlobals(default_upper_threshold) 175.0
set PostprocessorGlobals(default_maximum_rms_error) 0.02
set PostprocessorGlobals(default_maximum_iterations) 100
set PostprocessorGlobals(default_isosurface_value) 0.5
set PostprocessorGlobals(levelset_label) "Segmentation"
set PostprocessorGlobals(negative_speed) 0
set PostprocessorGlobals(levelset_type) "threshold"
set PostprocessorGlobals(default_feature_scaling) 1.0
set PostprocessorGlobals(default_curvature_scaling) 1.0

proc ConstructPostprocessorFrame {f} {
    global Options DataGlobals PostprocessorGlobals

    # Initialize the module
    PostprocessorInitialize
    
    # Create the GUI
    $f configure -background $Options(text_background_color)

    # Some title text
    frame $f.title -background $Options(pleasant_background_color)
    label $f.title.text -textvariable PostprocessorGlobals(frame_title_text) \
        -background $Options(pleasant_background_color) \
        -font $Options(h1_font)
    pack $f.title.text -side left

    # Source selector
    frame $f.data_source
    frame $f.data_source.loadframe
    label $f.data_source.label1 -font $Options(h3_font) -text $PostprocessorGlobals(source_label)
    listbox $f.data_source.loadframe.listbox -selectmode browse -width 20 \
        -height $Options(data_window_size) \
        -yscrollcommand "$f.data_source.loadframe.scrollbar set"
    scrollbar $f.data_source.loadframe.scrollbar -orient "vert" \
        -command "$f.data_source.loadframe.listbox yview" 
    pack $f.data_source.loadframe.scrollbar -side right -fill y
    pack $f.data_source.loadframe.listbox

    RegisterDataSelector $f.data_source.loadframe.listbox $DataGlobals(scalar_type)
    set PostprocessorGlobals(data_selector) $f.data_source.loadframe.listbox

    # Load button for source selector
    button $f.data_source.loadbutton \
        -command {SpawnProcess "PostprocessorLoadData"  "PostprocessorLoadDataRecovery" } \
        -text "Load"
    checkbutton $f.data_source.flipyaxis  -indicatoron t \
        -text "Flip Y axis" \
        -variable PostprocessorGlobals(flip_y_axis) -onvalue 1 -offvalue 0
    set PostprocessorGlobals(flip_y_axis) 0

    grid configure $f.data_source.label1 -padx 3 -column 0 -row 0 \
        -columnspan 2 -sticky nw
    grid configure $f.data_source.loadframe -padx 3 -column 0 -row 1 \
        -columnspan 2 -sticky nw

    grid configure $f.data_source.loadbutton -padx 3 -column 0 -row 2 -sticky nw
    grid configure $f.data_source.flipyaxis  -padx 3 -column 0 -row 3 -sticky nw

  # Source selector for feature
    frame $f.feature_source
    frame $f.feature_source.loadframe
    label $f.feature_source.label1 -font $Options(h3_font) -text $PostprocessorGlobals(feature_label)
    listbox $f.feature_source.loadframe.listbox -selectmode browse -width 20 \
        -height $Options(data_window_size) \
        -yscrollcommand "$f.feature_source.loadframe.scrollbar set"
    scrollbar $f.feature_source.loadframe.scrollbar -orient "vert" \
        -command "$f.feature_source.loadframe.listbox yview" 
    pack $f.feature_source.loadframe.scrollbar -side right -fill y
    pack $f.feature_source.loadframe.listbox

    RegisterDataSelector $f.feature_source.loadframe.listbox $DataGlobals(scalar_type)
    set PostprocessorGlobals(feature_selector) $f.feature_source.loadframe.listbox

    # Load button for source selector
    button $f.feature_source.loadbutton \
        -command {SpawnProcess "PostprocessorLoadFeature"  "PostprocessorLoadFeatureRecovery" } -text "Load"


    grid configure $f.feature_source.label1 -padx 3 -column 0 -row 0 \
        -columnspan 2 -sticky nw
    grid configure $f.feature_source.loadframe -padx 3 -column 0 -row 1 \
        -columnspan 2 -sticky nw
    grid configure $f.feature_source.loadbutton -padx 3 -column 0 -row 2 -sticky nw


  # Controls for the levelset
    frame $f.levelset
    label $f.levelset.label1 -font $Options(h3_font) -text $PostprocessorGlobals(levelset_label)
    entry $f.levelset.rms -width 6 -justify r
    entry $f.levelset.iterations -width 6 -justify r
    entry $f.levelset.feature_scaling -width 6 -justify r
    entry $f.levelset.curvature_scaling -width 6 -justify r
    label $f.levelset.rms_label -text "Maximum RMS Error of Solution"
    label $f.levelset.iterations_label -text "Maximum Solver Iterations"
    label $f.levelset.feature_scaling_label -text "Scaling value for features"
    label $f.levelset.curvature_scaling_label -text "Scaling value for curvature"


    entry $f.levelset.isovalue -width 6 -justify r
    entry $f.levelset.upper_threshold -width 6 -justify r
    entry $f.levelset.lower_threshold -width 6 -justify r
    label $f.levelset.isovalue_label -text "Isovalue of the seed"


    label $f.levelset.upper_threshold_label -text "Upper threshold"
    label $f.levelset.lower_threshold_label -text "Lower threshold"
    checkbutton $f.levelset.speedisnegative  -indicatoron t \
        -text "Negative speed" \
        -variable PostprocessorGlobals(negative_speed) -onvalue 1 -offvalue 0
    set PostprocessorGlobals(negative_speed) 0


    label $f.levelset.rms_change_label -text "Last RMS Change "
    label $f.levelset.rms_change -width 10 -justify l \
        -textvariable PostprocessorGlobals(last_rms_change)
    set PostprocessorGlobals(last_rms_change) 0

    label $f.levelset.elapsed_iterations_label -text "Elapsed iterations "
    label $f.levelset.elapsed_iterations -width 10 -justify l \
        -textvariable PostprocessorGlobals(elapsed_iterations)
    set PostprocessorGlobals(elapsed_iterations) 0

    set PostprocessorGlobals(rms_entry) $f.levelset.rms
    set PostprocessorGlobals(iterations_entry)  $f.levelset.iterations
    set PostprocessorGlobals(upper_threshold_entry) $f.levelset.upper_threshold
    set PostprocessorGlobals(lower_threshold_entry) $f.levelset.lower_threshold
    set PostprocessorGlobals(isovalue_entry) $f.levelset.isovalue
    set PostprocessorGlobals(negative_speed_checkbutton) $f.levelset.speedisnegative
    set PostprocessorGlobals(feature_scaling) $f.levelset.feature_scaling
    set PostprocessorGlobals(curvature_scaling) $f.levelset.curvature_scaling

    label $f.levelset.typeLabel -text "Levelset type"
    radiobutton $f.levelset.type1 -indicatoron true -text "threshold" \
        -variable PostprocessorGlobals(levelset_type) -value "threshold"
    radiobutton $f.levelset.type2 -indicatoron true -text "laplacian" \
        -variable PostprocessorGlobals(levelset_type) -value "laplacian"

    button $f.levelset.start -text "Start" \
        -command {SpawnProcess "PostprocessorStartSegmentation" "PostprocessorStartSegmentationRecovery" } \
        -state disabled
   set PostprocessorGlobals(levelset_button) $f.levelset.start

    $f.levelset.rms insert 0 $PostprocessorGlobals(default_maximum_rms_error)
    $f.levelset.iterations insert 0 $PostprocessorGlobals(default_maximum_iterations)
    $f.levelset.isovalue insert 0 $PostprocessorGlobals(default_isosurface_value)
    $f.levelset.lower_threshold insert 0 $PostprocessorGlobals(default_lower_threshold)
    $f.levelset.upper_threshold insert 0 $PostprocessorGlobals(default_upper_threshold)
    $f.levelset.feature_scaling insert 0 $PostprocessorGlobals(default_feature_scaling)
    $f.levelset.curvature_scaling insert 0 $PostprocessorGlobals(default_curvature_scaling)

    grid configure $f.levelset.label1 -padx 3 -column 0 -row 0 -columnspan 2 -sticky nw
    grid configure $f.levelset.iterations_label -padx 3 -column 0 -row 1 -sticky nw
    grid configure $f.levelset.iterations -padx 3 -column 1 -row 1 -sticky nw
    grid configure $f.levelset.rms_label -padx 3 -column 0 -row 2 -sticky nw
    grid configure $f.levelset.rms -padx 3 -column 1 -row 2 -sticky nw
    grid configure $f.levelset.speedisnegative -padx 3 -column 0 -row 3 -sticky nw

    grid configure $f.levelset.isovalue_label -padx 3 -column 0 -row 4 -sticky nw
    grid configure $f.levelset.isovalue -padx 3 -column 1 -row 4 -sticky nw
    grid configure $f.levelset.feature_scaling_label -padx 3 -column 0 -row 5 -sticky nw
    grid configure $f.levelset.feature_scaling -padx 3 -column 1 -row 5 -sticky nw
    grid configure $f.levelset.curvature_scaling_label -padx 3 -column 0 -row 6 -sticky nw
    grid configure $f.levelset.curvature_scaling -padx 3 -column 1 -row 6 -sticky nw

    grid configure $f.levelset.upper_threshold_label -padx 3 -column 0 -row 7 -sticky nw
    grid configure $f.levelset.upper_threshold -padx 3 -column 1 -row 7 -sticky nw
    grid configure $f.levelset.lower_threshold_label -padx 3 -column 0 -row 8 -sticky nw
    grid configure $f.levelset.lower_threshold -padx 3 -column 1 -row 8 -sticky nw

    grid configure $f.levelset.typeLabel -column 0 -row 9 -rowspan 2 -sticky nw -padx 3 -pady 3
    grid configure $f.levelset.type1 -padx 3 -column 1 -row 9 -sticky nw 
    grid configure $f.levelset.type2 -padx 3 -column 1 -row 10 -sticky nw

    grid configure $f.levelset.start -padx 3 -column 0 -row 11 -sticky nw

    grid configure $f.levelset.elapsed_iterations_label -padx 3 -column 0 -row 12 -sticky ne
    grid configure $f.levelset.elapsed_iterations -padx 3 -column 1 -row 12 -sticky nw

    grid configure $f.levelset.rms_change_label -padx 3 -column 0 -row 13 -sticky ne
    grid configure $f.levelset.rms_change -padx 3 -column 1 -row 13 -sticky nw

    # Controls to save the binary segmentation
    frame $f.save
    label $f.save.label -font $Options(h3_font) -text $PostprocessorGlobals(save_label)
    label $f.save.filename_label -text "Filename for output"
    entry $f.save.filename -width 40 -justify l
    label $f.save.tag_label -text "Tag for output data"
    entry $f.save.tag -width 40 -justify l
    set PostprocessorGlobals(save_filename) $f.save.filename
    set PostprocessorGlobals(save_tag) $f.save.tag

    button $f.save.save_button -text "Save" -command PostprocessorSaveOutput

    grid configure $f.save.label -column 0 -row 1 -columnspan 2 -sticky nw -padx 3 -pady 3
    grid configure $f.save.filename_label -column 0 -row 1 -sticky nw -padx 3 -pady 3
    grid configure $f.save.filename -column 1 -row 1 -sticky nw -padx 3 -pady 3
    grid configure $f.save.tag_label -column 0 -row 2 -sticky nw -padx 3 -pady 3
    grid configure $f.save.tag -column 1 -row 2 -sticky nw -padx 3 -pady 3
    grid configure $f.save.save_button -column 0 -row 3 -sticky nw -padx 3 -pady 3

    # Source viewer window
    CreatePostprocessorRenderWindow $f "source"

    # Feature image viewer window
    CreatePostprocessorRenderWindow $f "feature"
    frame $f.feature.extras
    checkbutton $f.feature.extras.toggle -indicatoron t -text "Show Features" \
        -command PostprocessorToggleFeatures -variable PostprocessorGlobals(features_toggle) \
        -onvalue 1 -offvalue 0
    set PostprocessorGlobals(features_toggle) 0
    set PostprocessorGlobals(features_toggle_button) $f.feature.extras.toggle
    pack $f.feature.extras.toggle
    grid configure $f.feature.extras -row 3 -columnspan 3 -column 0 -sticky w -padx 4
    $f.feature.extras.toggle configure -state disabled

    wm withdraw $PostprocessorGlobals(source_viewer_window)
    wm withdraw $PostprocessorGlobals(feature_viewer_window)

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

    if {[file exists $PostprocessorGlobals(help_file)] == 1} {
        set fileId [open $PostprocessorGlobals(help_file) r]
        
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
        [subst {$f.instructions.text}] insert 0.0 $PostprocessorGlobals(help_text)
    }
    
    pack $f.instructions.scrollbar -side right -fill y
    pack $f.instructions.text -fill both -expand t

    $f.instructions.text configure -state disabled

    # Configure layout
    grid configure $f.title -row 0 -column 0 -columnspan 3 -sticky nw -padx 10
    grid configure $f.data_source -row 1 -column 1 -sticky nw -padx 10 -pady 10
    grid configure $f.feature_source -row 1 -column 0 -sticky nw -padx 10 -pady 10
    grid configure $f.levelset -row 1 -rowspan 2 -column 2  -sticky nw -padx 10 -pady 10
    grid configure $f.save -row 2 -rowspan 1 -column 0 -columnspan 2 -sticky nw -padx 10 -pady 10
 #   grid configure $f.gradience -row 1  -column 2  -sticky nw -padx 10 -pady 10
    grid configure $f.instructions -row 3 -column 0 -columnspan 3 -padx 3 -pady 3

}

proc ActivatePostprocessorModule {} {
    global PostprocessorGlobals
    
    if { $PostprocessorGlobals(source_window_visible) == "true" } {
        wm deiconify $PostprocessorGlobals(source_viewer_window)
    }
    if { $PostprocessorGlobals(feature_window_visible) == "true" } {
        wm deiconify $PostprocessorGlobals(feature_viewer_window)
    }
}

proc DeactivatePostprocessorModule {} {
    global PostprocessorGlobals
    wm withdraw $PostprocessorGlobals(source_viewer_window)
    wm withdraw $PostprocessorGlobals(feature_viewer_window)

}

proc PostprocessorLoadDataRecovery {} {
    global PostprocessorGlobals

    $PostprocessorGlobals(levelset_button) configure -state disabled
    set PostprocessorGlobals(source_window_visible) "false"
    wm withdraw $PostprocessorGlobals(source_viewer_window)
}
 
proc PostprocessorLoadFeatureRecovery {} {
    global PostprocessorGlobals

    $PostprocessorGlobals(levelset_button) configure -state disabled
    set PostprocessorGlobals(feature_window_visible) "false"
    wm withdraw $PostprocessorGlobals(feature_viewer_window)
}
 
###
# proc PostprocessorInitialize
#
# Set up the various filters for this module.
#
proc PostprocessorInitialize {} {
    global PostprocessorGlobals DataGlobals

    # A reader for the input to the module
    vtkImageReader PostprocessorSourceReader
    PostprocessorSourceReader SetHeaderSize 0
    set PostprocessorGlobals(source_reader) PostprocessorSourceReader

    #A reader for the feature image
    vtkImageReader PostprocessorFeatureReader
    PostprocessorFeatureReader SetHeaderSize 0
    set PostprocessorGlobals(feature_reader) PostprocessorFeatureReader

    # A color lookup table for the source
    vtkColorTransferFunction PostprocessorLookupTable
    PostprocessorLookupTable AddRGBPoint  $PostprocessorGlobals(default_binary_lookup_table_low)\
        0.0 0.0 0.0 
    PostprocessorLookupTable AddRGBPoint  $PostprocessorGlobals(default_binary_lookup_table_high)\
        1.0 1.0 1.0
    set PostprocessorGlobals(source_lookup_table) PostprocessorLookupTable

    # A color lookup table for the feature image
    vtkColorTransferFunction PostprocessorFeatureLookupTable
    PostprocessorFeatureLookupTable AddRGBPoint  $PostprocessorGlobals(default_lookup_table_low)\
        0.0 0.0 0.0 
    PostprocessorFeatureLookupTable AddRGBPoint  $PostprocessorGlobals(default_lookup_table_high)\
        1.0 1.0 1.0
    set PostprocessorGlobals(feature_lookup_table) PostprocessorFeatureLookupTable

    # To flip the y axis
    vtkImageFlip PostprocessorImageFlip
    PostprocessorImageFlip SetFilteredAxis 1
    set PostprocessorGlobals(image_flip) PostprocessorImageFlip

    # A color lookup table for the level sets
#    vtkColorTransferFunction PostprocessorLevelSetsLookupTable
#    PostprocessorLevelSetsLookupTable AddRGBPoint -4.0 0 0 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint -4.0 0 0 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint -4.0 0 0 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint -4.0 0 0 0

    vtkLookupTable PostprocessorLevelSetsLookupTable
    PostprocessorLevelSetsLookupTable SetNumberOfTableValues 2
    PostprocessorLevelSetsLookupTable SetTableValue 1 1.0 1.0 0.0 1.0
    PostprocessorLevelSetsLookupTable SetTableValue 0 0.0 0.0 0.0 0.0

#    PostprocessorLevelSetsLookupTable SetTableValue 1 0.0 0.0 0.0 0.0
#    PostprocessorLevelSetsLookupTable SetTableValue 2 0.0 0.0 0.0 0.0
#    PostprocessorLevelSetsLookupTable SetTableValue 3 0.0 0.0 0.0 0.0
#    PostprocessorLevelSetsLookupTable SetTableValue 4 0.0 0.0 0.0 0.0

#    vtkColorTransferFunction PostprocessorLevelSetsLookupTable
#    PostprocessorLevelSetsLookupTable AddRGBPoint  0.0 1 1 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint -1.0 0 1 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint  1.0 1 0 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint  2.0 1 0 1
#    PostprocessorLevelSetsLookupTable AddRGBPoint -2.0 0 1 1
#    PostprocessorLevelSetsLookupTable AddRGBPoint  3.0 0 0 0
#    PostprocessorLevelSetsLookupTable AddRGBPoint -3.0 1 1 1
#    set PostprocessorGlobals(levelset_lookup_table) PostprocessorLevelSetsLookupTable

   # A color lookup table for the speed image
    vtkColorTransferFunction PostprocessorSpeedLookupTable
    PostprocessorSpeedLookupTable AddRGBPoint  0.001 1 1 1
    PostprocessorSpeedLookupTable AddRGBPoint  0.001 1 0 0
    PostprocessorSpeedLookupTable AddRGBPoint  -0.001 0 1 0
#    PostprocessorSpeedLookupTable AddRGBPoint -1.0 0 1 0
#    PostprocessorSpeedLookupTable AddRGBPoint  1.0 1 0 0
    set PostprocessorGlobals(speed_lookup_table) PostprocessorSpeedLookupTable


    #
    # Threshold filter to approximate zero crossings of level sets for visualization
    #
    vtkImageThreshold PostprocessorThresholder
    PostprocessorThresholder ThresholdBetween -0.55 0.55
    PostprocessorThresholder ReplaceInOn
    PostprocessorThresholder ReplaceOutOn
    PostprocessorThresholder SetInValue 1
    PostprocessorThresholder SetOutValue 0
    PostprocessorThresholder SetOutputScalarTypeToUnsignedChar
    set PostprocessorGlobals(thresholder) PostprocessorThresholder
   
    #
    # Color mapper for the source data viewer
    #
    vtkImageMapToColors PostprocessorMapToRGBA
    PostprocessorMapToRGBA SetInput [PostprocessorSourceReader GetOutput]
    PostprocessorMapToRGBA SetOutputFormatToRGBA
    PostprocessorMapToRGBA SetLookupTable PostprocessorLookupTable
    set PostprocessorGlobals(source_color_mapper) PostprocessorMapToRGBA
    #
    # Color mapper for the level sets
    #
    vtkImageMapToColors PostprocessorLevelsetMapToRGBA
    PostprocessorLevelsetMapToRGBA SetLookupTable PostprocessorLevelSetsLookupTable
    set PostprocessorGlobals(levelset_color_mapper) PostprocessorLevelsetMapToRGBA

    #
    # Color mapper for the feature data viewer
    #
    vtkImageMapToColors PostprocessorFeatureMapToRGBA
    PostprocessorFeatureMapToRGBA SetOutputFormatToRGBA
    PostprocessorFeatureMapToRGBA SetLookupTable PostprocessorFeatureLookupTable
    set PostprocessorGlobals(feature_color_mapper) PostprocessorFeatureMapToRGBA

    #
    # Color mapper for the speed image
    #
    vtkImageMapToColors PostprocessorSpeedColorMapper
    PostprocessorSpeedColorMapper SetOutputFormatToRGBA
    PostprocessorSpeedColorMapper SetLookupTable PostprocessorSpeedLookupTable
    set PostprocessorGlobals(speed_color_mapper) PostprocessorSpeedColorMapper

    #
    # Overlay the level sets  volume on the feature image
    #
    vtkImageMapper PostprocessorOverlayMapper
    PostprocessorOverlayMapper SetInput [PostprocessorLevelsetMapToRGBA GetOutput]
    PostprocessorOverlayMapper SetColorWindow 255
    PostprocessorOverlayMapper SetColorLevel 129
    PostprocessorOverlayMapper SetZSlice 0
    set PostprocessorGlobals(levelset_overlay_mapper) PostprocessorOverlayMapper

    vtkActor2D PostprocessorOverlayActor
    PostprocessorOverlayActor SetMapper PostprocessorOverlayMapper
    set PostprocessorGlobals(levelset_overlay_actor) PostprocessorOverlayActor
#


    #
    # Color mapper for the gradient data viewer
    #
#    vtkImageMapToColors PostprocessorGradientMapToRGBA
#    PostprocessorGradientMapToRGBA SetOutputFormatToRGBA
#    PostprocessorGradientMapToRGBA SetLookupTable PostprocessorGradientLookupTable
#    set PostprocessorGlobals(gradient_color_mapper) PostprocessorGradientMapToRGBA
    
    #
    # Viewer for source data.
    #
    vtkImageViewer PostprocessorImageViewer
    PostprocessorImageViewer SetColorWindow $PostprocessorGlobals(default_color_window)
    PostprocessorImageViewer SetColorLevel  $PostprocessorGlobals(default_color_level)
    PostprocessorImageViewer SetZSlice 0
    set PostprocessorGlobals(source_viewer) PostprocessorImageViewer

    ExitRegisterViewer PostprocessorImageViewer

    #
    # Viewer for  processed data
    #
    vtkImageViewer PostprocessorOutputImageViewer
    PostprocessorOutputImageViewer SetColorWindow 255
    PostprocessorOutputImageViewer SetColorLevel  127.5
    PostprocessorOutputImageViewer SetZSlice 0
    set PostprocessorGlobals(feature_viewer) PostprocessorOutputImageViewer

    ExitRegisterViewer PostprocessorOutputImageViewer

    #
    # Viewer for gradient processed data
    #
    #    vtkImageViewer PostprocessorGradientImageViewer
#    PostprocessorGradientImageViewer SetColorWindow \
#        $PostprocessorGlobals(default_color_window)
#    PostprocessorGradientImageViewer SetColorLevel  \
#        $PostprocessorGlobals(default_color_level)
#    PostprocessorGradientImageViewer SetZSlice 0
#    set PostprocessorGlobals(gradient_viewer) PostprocessorGradientImageViewer

#    ExitRegisterViewer PostprocessorGradientImageViewer

    #
    # Casting filter to convert to float
    #
    vtkImageCast PostprocessorCaster
    PostprocessorCaster SetOutputScalarTypeToFloat
#    PostprocessorCaster ReleaseDataFlagOn
#    PostprocessorCaster SetInput [PostprocessorSourceReader GetOutput]
    set PostprocessorGlobals(source_caster) PostprocessorCaster

    vtkImageCast PostprocessorCaster2
    PostprocessorCaster2 SetOutputScalarTypeToFloat
 #   PostprocessorCaster2 ReleaseDataFlagOn
    PostprocessorCaster2 SetInput [PostprocessorFeatureReader GetOutput]
    set PostprocessorGlobals(feature_caster) PostprocessorCaster2

    #
    # Threshold based level set style segmentation
    #
    vtkITKThresholdSegmentationLevelSetImageFilter PostprocessorThresholdSegmenter
    PostprocessorThresholdSegmenter SetLowerThreshold $PostprocessorGlobals(default_lower_threshold)
    PostprocessorThresholdSegmenter SetUpperThreshold $PostprocessorGlobals(default_upper_threshold)
    PostprocessorThresholdSegmenter SetMaximumIterations $PostprocessorGlobals(default_maximum_iterations)
    PostprocessorThresholdSegmenter SetMaximumRMSError $PostprocessorGlobals(default_maximum_rms_error)
    set PostprocessorGlobals(threshold_segmenter) PostprocessorThresholdSegmenter

    #
    # Laplacian based level set style segmentation
    #
    vtkITKLaplacianSegmentationLevelSetImageFilter PostprocessorLaplacianSegmenter
    PostprocessorLaplacianSegmenter SetMaximumIterations $PostprocessorGlobals(default_maximum_iterations)
    PostprocessorLaplacianSegmenter SetMaximumRMSError $PostprocessorGlobals(default_maximum_rms_error)
    set PostprocessorGlobals(laplacian_segmenter) PostprocessorLaplacianSegmenter

    #
    # File writer for the output of the anisotropic diffusion
    #
    vtkImageWriter PostprocessorWriter
    PostprocessorWriter SetFileDimensionality \
        $DataGlobals(default_file_dimensionality)
    PostprocessorWriter SetFilePattern \
        $DataGlobals(default_file_pattern)
    set PostprocessorGlobals(writer) PostprocessorWriter
}


###
#
#
proc PostprocessorLoadData {} {
    global PostprocessorGlobals
    
    $PostprocessorGlobals(levelset_button) configure -state disabled

    wm withdraw $PostprocessorGlobals(source_viewer_window)
    set PostprocessorGlobals(source_window_visible) "false"
    
    set tagName [ $PostprocessorGlobals(data_selector) get \
                      [$PostprocessorGlobals(data_selector) curselection]]
    set cacheEntry [GetDataCacheEntry $tagName]

    set endianString "SetDataByteOrderTo"
    append endianString [GetCacheEntryValue $cacheEntry "endianness"]
    append endianString "Endian"
    $PostprocessorGlobals(source_reader) $endianString

    set datatypeString "SetDataScalarTypeTo"
    append datatypeString [GetCacheEntryValue $cacheEntry "data_type"]

    $PostprocessorGlobals(source_reader) SetNumberOfScalarComponents \
        [GetCacheEntryValue $cacheEntry "number_of_components"]
    $PostprocessorGlobals(source_reader) $datatypeString
    $PostprocessorGlobals(source_reader) SetDataExtent \
        0 [GetCacheEntryValue $cacheEntry "size_x"] \
        0 [GetCacheEntryValue $cacheEntry "size_y"] \
        0 [GetCacheEntryValue $cacheEntry "size_z"]
    $PostprocessorGlobals(source_reader) SetFileNameSliceOffset \
        [GetCacheEntryValue $cacheEntry "slice_offset"]
    $PostprocessorGlobals(source_reader) SetHeaderSize \
        [GetCacheEntryValue $cacheEntry "header_size"]

    if { [regexp {^[yY]} [GetCacheEntryValue $cacheEntry "multiple_files"] ] } {
        $PostprocessorGlobals(source_reader) SetFileDimensionality 2
        $PostprocessorGlobals(source_reader) SetFilePrefix \
            [GetCacheEntryValue $cacheEntry "filename"] 
        $PostprocessorGlobals(source_reader) SetFilePattern \
            [GetCacheEntryValue $cacheEntry "file_pattern"]
    } else {
        $PostprocessorGlobals(source_reader) SetFileDimensionality 3
        $PostprocessorGlobals(source_reader) SetFileName \
            [GetCacheEntryValue $cacheEntry "filename"]
    }

    set PostprocessorGlobals(loaded_data_tag) $cacheEntry

    $PostprocessorGlobals(source_slice_scale) configure -from 0 \
        -to [GetCacheEntryValue $cacheEntry "size_z"] \
        -length [expr 2 * [GetCacheEntryValue $cacheEntry "size_z"] ]

    $PostprocessorGlobals(source_viewer_widget) configure \
        -height [GetCacheEntryValue $cacheEntry "size_y"] \
        -width [GetCacheEntryValue $cacheEntry "size_x"]

    $PostprocessorGlobals(source_reader) Modified

    ConstructProgressWindow sourceReaderProgressWindow
    $PostprocessorGlobals(source_reader) SetProgressMethod \
        "ProgressProc $PostprocessorGlobals(source_reader) .sourceReaderProgressWindow"

    [$PostprocessorGlobals(source_reader) GetOutput] SetUpdateExtentToWholeExtent
    $PostprocessorGlobals(source_reader) Update
    $PostprocessorGlobals(image_flip) SetInput [$PostprocessorGlobals(source_reader) GetOutput]


    if { $PostprocessorGlobals(flip_y_axis) == 1 } {
        $PostprocessorGlobals(source_caster) SetInput [$PostprocessorGlobals(image_flip) GetOutput]
    $PostprocessorGlobals(source_color_mapper) SetInput [$PostprocessorGlobals(image_flip) GetOutput]
    } else {
        $PostprocessorGlobals(source_color_mapper) SetInput [$PostprocessorGlobals(source_reader) GetOutput]
        $PostprocessorGlobals(source_caster) SetInput [$PostprocessorGlobals(source_reader) GetOutput]
    }


    $PostprocessorGlobals(source_viewer) SetInput [$PostprocessorGlobals(source_color_mapper) GetOutput]

    $PostprocessorGlobals(source_viewer) Render
    destroy .sourceReaderProgressWindow
    set PostprocessorGlobals(source_window_visible) "true"   
    wm deiconify $PostprocessorGlobals(source_viewer_window)
    set PostprocessorGlobals(load_count) [expr $PostprocessorGlobals(load_count) + 1]
    if {$PostprocessorGlobals(load_count) > 1} {
        $PostprocessorGlobals(levelset_button) configure -state normal 
    }
}

###
#
#
proc PostprocessorLoadFeature {} {
    global PostprocessorGlobals
    
    $PostprocessorGlobals(levelset_button) configure -state disabled

#    if {$PostprocessorGlobals(has_run_segmentation) == 1} {
        [$PostprocessorGlobals(feature_viewer) GetRenderer] \
            RemoveActor $PostprocessorGlobals(levelset_overlay_actor)
#    }


    wm withdraw $PostprocessorGlobals(feature_viewer_window)
    set PostprocessorGlobals(feature_window_visible) "false"

    set tagName [ $PostprocessorGlobals(feature_selector) get \
                      [$PostprocessorGlobals(feature_selector) curselection]]
    set cacheEntry [GetDataCacheEntry $tagName]

    set endianString "SetDataByteOrderTo"
    append endianString [GetCacheEntryValue $cacheEntry "endianness"]
    append endianString "Endian"
    $PostprocessorGlobals(feature_reader) $endianString

    set datatypeString "SetDataScalarTypeTo"
    append datatypeString [GetCacheEntryValue $cacheEntry "data_type"]

    $PostprocessorGlobals(feature_reader) SetNumberOfScalarComponents \
        [GetCacheEntryValue $cacheEntry "number_of_components"]
    $PostprocessorGlobals(feature_reader) $datatypeString
    $PostprocessorGlobals(feature_reader) SetDataExtent \
        0 [GetCacheEntryValue $cacheEntry "size_x"] \
        0 [GetCacheEntryValue $cacheEntry "size_y"] \
        0 [GetCacheEntryValue $cacheEntry "size_z"]
    $PostprocessorGlobals(feature_reader) SetFileNameSliceOffset \
        [GetCacheEntryValue $cacheEntry "slice_offset"]
    $PostprocessorGlobals(feature_reader) SetHeaderSize \
        [GetCacheEntryValue $cacheEntry "header_size"]

    if { [regexp {^[yY]} [GetCacheEntryValue $cacheEntry "multiple_files"] ] } {
        $PostprocessorGlobals(feature_reader) SetFileDimensionality 2
        $PostprocessorGlobals(feature_reader) SetFilePrefix \
            [GetCacheEntryValue $cacheEntry "filename"] 
        $PostprocessorGlobals(feature_reader) SetFilePattern \
            [GetCacheEntryValue $cacheEntry "file_pattern"]
    } else {
        $PostprocessorGlobals(feature_reader) SetFileDimensionality 3
        $PostprocessorGlobals(feature_reader) SetFileName \
            [GetCacheEntryValue $cacheEntry "filename"]
    }

    set PostprocessorGlobals(loaded_data_tag) $cacheEntry

    $PostprocessorGlobals(feature_slice_scale) configure -from 0 \
        -to [GetCacheEntryValue $cacheEntry "size_z"] \
        -length [expr 2 * [GetCacheEntryValue $cacheEntry "size_z"] ]

    $PostprocessorGlobals(feature_viewer_widget) configure \
        -height [GetCacheEntryValue $cacheEntry "size_y"] \
        -width [GetCacheEntryValue $cacheEntry "size_x"]

    $PostprocessorGlobals(feature_reader) Modified

    ConstructProgressWindow featureReaderProgressWindow
    $PostprocessorGlobals(feature_reader) SetProgressMethod \
        "ProgressProc $PostprocessorGlobals(feature_reader) .featureReaderProgressWindow"

    [$PostprocessorGlobals(feature_reader) GetOutput] SetUpdateExtentToWholeExtent
    $PostprocessorGlobals(feature_reader) Update
    $PostprocessorGlobals(feature_color_mapper) SetInput [$PostprocessorGlobals(feature_reader) GetOutput]
    $PostprocessorGlobals(feature_viewer) SetInput [$PostprocessorGlobals(feature_color_mapper) GetOutput]
    $PostprocessorGlobals(feature_viewer) Render

    destroy .featureReaderProgressWindow
    set PostprocessorGlobals(feature_window_visible) "true"
    wm deiconify $PostprocessorGlobals(feature_viewer_window)

    set PostprocessorGlobals(load_count) [expr $PostprocessorGlobals(load_count) + 1]
    if {$PostprocessorGlobals(load_count) > 1} { 
        $PostprocessorGlobals(levelset_button) configure -state normal 
    }


}

proc CreatePostprocessorRenderWindow {f fr} {
    global PostprocessorGlobals Options

    toplevel $f.$fr

    set keyWord $fr
    append keyWord "_window_label"
    wm title $f.$fr $PostprocessorGlobals($keyWord)

    set keyWord $fr
    append keyWord "_viewer_window"
    set PostprocessorGlobals($keyWord) $f.$fr

    set keyWord $fr
    append keyWord "_viewer_widget"
    set PostprocessorGlobals($keyWord) $f.$fr.viewer

    set keyWord $fr
    append keyWord "_viewer"
    vtkTkImageViewerWidget $f.$fr.viewer -iv $PostprocessorGlobals($keyWord)

    ::vtk::bind_tk_imageviewer_widget $f.$fr.viewer

    wm protocol $f.$fr  WM_DELETE_WINDOW { }

    set keyWord $fr
    append keyWord "_slice_number"
    set commandWord "PostprocessorSetSlice_"
    append commandWord $fr

    scale $f.$fr.slice -from 0 -to 0 -orient horizontal \
        -command $commandWord -variable PostprocessorGlobals($keyWord)
    set PostprocessorGlobals($keyWord) 0

    set keyWord $fr
    append keyWord "_slice_scale"
    set PostprocessorGlobals($keyWord) $f.$fr.slice

    frame $f.$fr.colormap
    entry $f.$fr.colormap.colormapLow  -width 6 -justify r
    entry $f.$fr.colormap.colormapHigh -width 6 -justify r

    $f.$fr.colormap.colormapHigh insert 0 $PostprocessorGlobals(default_lookup_table_high)
    $f.$fr.colormap.colormapLow insert 0 $PostprocessorGlobals(default_lookup_table_low)

    set cmdStr [subst -nocommands  { PostProcessorResetLookupTable $fr [$f.$fr.colormap.colormapLow get] [$f.$fr.colormap.colormapHigh get] } ]

    button $f.$fr.colormap.colormapButton -text "scalar colormap" \
        -command $cmdStr -font $Options(small_font)

    pack $f.$fr.colormap.colormapLow $f.$fr.colormap.colormapHigh \
        $f.$fr.colormap.colormapButton -side left -padx 3 

    grid configure $f.$fr.viewer   -row 0 -column 0 -columnspan 3 -padx 10 
    grid configure $f.$fr.slice    -row 1 -column 0 -columnspan 3 -padx 10 
    grid configure $f.$fr.colormap -row 2 -columnspan 3 -column 0 -sticky w -padx 10
}

proc PostProcessorResetLookupTable {window low high} {
    global PostprocessorGlobals

    set keyWord $window
    append keyWord "_lookup_table"

    $PostprocessorGlobals($keyWord)  RemoveAllPoints
    $PostprocessorGlobals($keyWord)  AddRGBPoint $low  0.0 0.0 0.0
    $PostprocessorGlobals($keyWord)  AddRGBPoint $high 1.0 1.0 1.0

    set keyWord $window
    append keyWord "_viewer"
    $PostprocessorGlobals($keyWord) Render
}

proc PostprocessorSetSlice_source {slice} {
    global PostprocessorGlobals

    $PostprocessorGlobals(source_viewer) SetZSlice $slice
    $PostprocessorGlobals(source_viewer) Render
}
proc PostprocessorSetSlice_feature {slice} {
    global PostprocessorGlobals

    $PostprocessorGlobals(feature_viewer) SetZSlice $slice
    $PostprocessorGlobals(levelset_overlay_mapper) SetZSlice $slice
    $PostprocessorGlobals(feature_viewer) Render
}

proc PostprocessorStartSegmentationRecovery {} {
   global PostprocessorGlobals
   $PostprocessorGlobals(levelset_button) configure -state normal
   $PostprocessorGlobals(features_toggle_button) configure -state disabled
   set PostprocessorGlobals(source_window_visible) "true"
   set PostprocessorGlobals(feature_window_visible) "true"
   ActivatePostprocessorModule
}

proc PostprocessorStartSegmentation {} {
    global PostprocessorGlobals DataGlobals
    set PostprocessorGlobals(source_window_visible) "false"
    set PostprocessorGlobals(feature_window_visible) "false"
    DeactivatePostprocessorModule
    $PostprocessorGlobals(levelset_button) configure -state disabled
  
    ConstructProgressWindow preprocessorSegmentationProgressWindow

    ## Set up the segmentation filters

    $PostprocessorGlobals(threshold_segmenter) SetMaximumIterations \
        [expr int([$PostprocessorGlobals(iterations_entry) get]) ]
    $PostprocessorGlobals(threshold_segmenter) SetMaximumRMSError \
        [$PostprocessorGlobals(rms_entry) get]
    $PostprocessorGlobals(threshold_segmenter) SetUpperThreshold \
        [$PostprocessorGlobals(upper_threshold_entry) get]
    $PostprocessorGlobals(threshold_segmenter) SetLowerThreshold \
        [$PostprocessorGlobals(lower_threshold_entry) get]
    $PostprocessorGlobals(threshold_segmenter) SetIsoSurfaceValue \
        [$PostprocessorGlobals(isovalue_entry) get]
    if { $PostprocessorGlobals(negative_speed) == 1 } {
        $PostprocessorGlobals(threshold_segmenter) SetUseNegativeFeatures 1
    } else {
        $PostprocessorGlobals(threshold_segmenter) SetUseNegativeFeatures 0
    }
    $PostprocessorGlobals(threshold_segmenter) SetFeatureScaling \
        [$PostprocessorGlobals(feature_scaling) get]

    $PostprocessorGlobals(threshold_segmenter) SetCurvatureScaling \
        [$PostprocessorGlobals(curvature_scaling) get]

    $PostprocessorGlobals(laplacian_segmenter) SetMaximumIterations \
        [expr int([$PostprocessorGlobals(iterations_entry) get]) ]
    $PostprocessorGlobals(laplacian_segmenter) SetMaximumRMSError \
        [$PostprocessorGlobals(rms_entry) get]
    $PostprocessorGlobals(laplacian_segmenter) SetIsoSurfaceValue \
        [$PostprocessorGlobals(isovalue_entry) get]
    if { $PostprocessorGlobals(negative_speed) == 1 } {
        $PostprocessorGlobals(laplacian_segmenter) SetUseNegativeFeatures 1
    } else {
        $PostprocessorGlobals(laplacian_segmenter) SetUseNegativeFeatures 0
    }
    $PostprocessorGlobals(laplacian_segmenter) SetFeatureScaling \
        [$PostprocessorGlobals(feature_scaling) get]

    $PostprocessorGlobals(laplacian_segmenter) SetCurvatureScaling \
        [$PostprocessorGlobals(curvature_scaling) get]

    [$PostprocessorGlobals(feature_caster) GetOutput] SetUpdateExtentToWholeExtent
    [$PostprocessorGlobals(source_caster) GetOutput] SetUpdateExtentToWholeExtent
    $PostprocessorGlobals(feature_caster) Update
    $PostprocessorGlobals(source_caster) Update



    if {$PostprocessorGlobals(levelset_type) == "threshold"} {
        $PostprocessorGlobals(threshold_segmenter) SetFeatureImage \
            [$PostprocessorGlobals(feature_caster) GetOutput]
        $PostprocessorGlobals(threshold_segmenter) SetInput \
            [$PostprocessorGlobals(source_caster) GetOutput]
        $PostprocessorGlobals(threshold_segmenter) SetProgressMethod \
            "PostprocessorProgressProc $PostprocessorGlobals(threshold_segmenter) .preprocessorSegmentationProgressWindow"
        
        [$PostprocessorGlobals(threshold_segmenter) GetOutput] SetUpdateExtentToWholeExtent
        $PostprocessorGlobals(threshold_segmenter) Update 
        
        $PostprocessorGlobals(thresholder) SetInput \
            [$PostprocessorGlobals(threshold_segmenter) GetOutput]
        
        $PostprocessorGlobals(speed_color_mapper) SetInput \
            [$PostprocessorGlobals(threshold_segmenter) GetSpeedImage]
    } elseif { $PostprocessorGlobals(levelset_type) == "laplacian" } {
        $PostprocessorGlobals(laplacian_segmenter) SetFeatureImage \
            [$PostprocessorGlobals(feature_caster) GetOutput]
        $PostprocessorGlobals(laplacian_segmenter) SetInput \
            [$PostprocessorGlobals(source_caster) GetOutput]
        $PostprocessorGlobals(laplacian_segmenter) SetProgressMethod \
            "PostprocessorProgressProc $PostprocessorGlobals(laplacian_segmenter) .preprocessorSegmentationProgressWindow"
        
        [$PostprocessorGlobals(laplacian_segmenter) GetOutput] SetUpdateExtentToWholeExtent
        $PostprocessorGlobals(laplacian_segmenter) Update 
        
        $PostprocessorGlobals(thresholder) SetInput \
            [$PostprocessorGlobals(laplacian_segmenter) GetOutput]
        
        $PostprocessorGlobals(speed_color_mapper) SetInput \
            [$PostprocessorGlobals(laplacian_segmenter) GetSpeedImage]
    }
    

    $PostprocessorGlobals(levelset_color_mapper) SetInput \
        [$PostprocessorGlobals(thresholder) GetOutput]

    [$PostprocessorGlobals(feature_viewer) GetRenderer] \
        AddActor2D $PostprocessorGlobals(levelset_overlay_actor)

    $PostprocessorGlobals(feature_viewer) Render
    destroy .preprocessorSegmentationProgressWindow
    set PostprocessorGlobals(feature_window_visible) "true"
    wm deiconify $PostprocessorGlobals(feature_viewer_window)
    
    $PostprocessorGlobals(levelset_button) configure -state normal
    $PostprocessorGlobals(features_toggle_button) configure -state normal
    set PostprocessorGlobals(source_window_visible) "true"
    set PostprocessorGlobals(feature_window_visible) "true"
    ActivatePostprocessorModule
#    set PostprocessorGlobals(has_run_segmentation) 1


}


proc PostprocessorToggleFeatures {} {
    global PostprocessorGlobals

    if {$PostprocessorGlobals(features_toggle) == 1}    {
        $PostprocessorGlobals(feature_viewer) SetInput [$PostprocessorGlobals(speed_color_mapper) GetOutput]
    } else {
        $PostprocessorGlobals(feature_viewer) SetInput [$PostprocessorGlobals(feature_color_mapper) GetOutput]
    }
    $PostprocessorGlobals(feature_viewer) Render
}


proc PostprocessorProgressProc {filter progress_bar} {
    global Options PostprocessorGlobals

    set v [ expr [$filter GetProgress] * $Options(progress_bar_length) ]

#    puts  [$filter GetRMSChange]

    set PostprocessorGlobals(last_rms_change) [$filter GetRMSChange]
    set PostprocessorGlobals(elapsed_iterations) [$filter GetElapsedIterations]

    $progress_bar.canvas delete bar
    $progress_bar.canvas create rectangle 0 0 $v \
        $Options(progress_bar_height) -fill $Options(progress_bar_color) \
    -tags "bar"
    
    update
}

proc PostprocessorSaveOutput {} {
    global Options PostprocessorGlobals DataGlobals

    set fn $DataGlobals(cache_directory)
    append fn "/$DataGlobals(unique_user_prefix)"
    append fn [$PostprocessorGlobals(save_filename) get]
    
#    if {$DataGlobals(default_file_dimensionality) == 2} {
#        $PostprocessorGlobals(writer) SetFilePrefix $fn
#    } else {
    $PostprocessorGlobals(writer) SetFileName $fn
    $PostprocessorGlobals(writer) SetFileDimensionality 3
#    }

  if {$PostprocessorGlobals(levelset_type) == "threshold"} {
      $PostprocessorGlobals(writer) SetInput [$PostprocessorGlobals(threshold_segmenter) GetOutput]
  } elseif   { $PostprocessorGlobals(levelset_type) == "laplacian" } {
      $PostprocessorGlobals(writer) SetInput [$PostprocessorGlobals(laplacian_segmenter) GetOutput]
  }

    $PostprocessorGlobals(writer) Write

    set size_z [GetCacheEntryValue $PostprocessorGlobals(loaded_data_tag) "size_z"]
    set size_y [GetCacheEntryValue $PostprocessorGlobals(loaded_data_tag) "size_y"]
    set size_x [GetCacheEntryValue $PostprocessorGlobals(loaded_data_tag) "size_x"]
    
#    if {$DataGlobals(default_file_dimensionality) == 2} { 
#        set multFiles "Yes" 
#    } else {
        set multFiles "No"
#    }
    set noComponents 1
    
    set entryStr "$fn Float $size_x $size_y $size_z $DataGlobals(default_file_endianness) $multFiles $noComponents $DataGlobals(default_file_pattern) 0 0"
    AddDataCacheEntry [$PostprocessorGlobals(save_tag) get] $entryStr
    
}
