#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    PreprocessorModule.tcl
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
# preprocessing stages of segmentations.
#
# Initialize some global variables for this module
set PreprocessorGlobals(frame_title_text)    "Preprocessor"
set PreprocessorGlobals(source_window_label) "Source Volume"
set PreprocessorGlobals(gradient_window_label)   "Gradient Volume"
set PreprocessorGlobals(diffused_window_label) "Diffused Volume"
set PreprocessorGlobals(default_color_window) 255.0
set PreprocessorGlobals(default_color_level)  127.5
set PreprocessorGlobals(default_lookup_table_low)  0.0
set PreprocessorGlobals(default_lookup_table_high) 255.0
set PreprocessorGlobals(default_conductance_parameter) 1.0
set PreprocessorGlobals(default_iterations)  1
set PreprocessorGlobals(default_time_step)   0.125
set PreprocessorGlobals(default_variance_parameter)   0.0
set PreprocessorGlobals(diffusion_label)     "Noise reduction"
set PreprocessorGlobals(source_label)        "Source data"
set PreprocessorGlobals(gradient_label)      "Gradient image"
set PreprocessorGlobals(loaded_data_tag)     ""
set PreprocessorGlobals(diffusion_volume_filename) "WST_Diffused"
set PreprocessorGlobals(gradient_volume_filename) "WST_Gradient"
set PreprocessorGlobals(diffused_tag) "diffused_volume"
set PreprocessorGlobals(gradient_tag) "gradient_volume"
set PreprocessorGlobals(source_window_visible) "false"
set PreprocessorGlobals(diffused_window_visible) "false"
set PreprocessorGlobals(gradient_window_visible) "false"

set PreprocessorGlobals(help_file) "PreprocessorHelp.txt"
set PreprocessorGlobals(help_text) "Sorry, could not find the file $PreprocessorGlobals(help_file)"

set PreprocessorGlobals(diffusion_type) "gradient"

###
#
# 
proc DeactivatePreprocessorModule {} {
    global PreprocessorGlobals
    wm withdraw $PreprocessorGlobals(source_viewer_window)
    wm withdraw $PreprocessorGlobals(diffused_viewer_window)
    wm withdraw $PreprocessorGlobals(gradient_viewer_window)

}

proc ActivatePreprocessorModule {} {
    global PreprocessorGlobals

    if { $PreprocessorGlobals(source_window_visible) == "true" } {
        wm deiconify $PreprocessorGlobals(source_viewer_window)
    }
    if { $PreprocessorGlobals(diffused_window_visible) == "true" } {
        wm deiconify $PreprocessorGlobals(diffused_viewer_window)
    }
    if { $PreprocessorGlobals(gradient_window_visible) == "true" } {
        wm deiconify $PreprocessorGlobals(gradient_viewer_window)
    }
}

###
# proc ConstructPreprocessorFrame
#
# Sets up the GUI interface for this module
#
proc ConstructPreprocessorFrame {f} {
    global Options DataGlobals PreprocessorGlobals

    # Initialize the module
    PreprocessorInitialize
    
    # Create the GUI
    $f configure -background $Options(text_background_color)

    # Some title text
    frame $f.title -background $Options(pleasant_background_color)
    label $f.title.text -textvariable PreprocessorGlobals(frame_title_text) \
        -background $Options(pleasant_background_color) \
        -font $Options(h1_font)
    pack $f.title.text -side left

    # Source selector
    frame $f.data_source
    frame $f.data_source.loadframe
    label $f.data_source.label1 -font $Options(h3_font) -text $PreprocessorGlobals(source_label)
    listbox $f.data_source.loadframe.listbox -selectmode browse -width 20 \
        -height $Options(data_window_size) \
        -yscrollcommand "$f.data_source.loadframe.scrollbar set"
    scrollbar $f.data_source.loadframe.scrollbar -orient "vert" \
        -command "$f.data_source.loadframe.listbox yview" 
    pack $f.data_source.loadframe.scrollbar -side right -fill y
    pack $f.data_source.loadframe.listbox

    RegisterDataSelector $f.data_source.loadframe.listbox $DataGlobals(scalar_type)
    set PreprocessorGlobals(data_selector) $f.data_source.loadframe.listbox

    # Load button for source selector
    button $f.data_source.loadbutton \
        -command {SpawnProcess "PreprocessorLoadData"  "PreprocessorLoadDataRecovery" } -text "Load"

    grid configure $f.data_source.label1 -padx 3 -column 0 -row 0 \
        -columnspan 2 -sticky nw
    grid configure $f.data_source.loadframe -padx 3 -column 0 -row 1 \
        -columnspan 2 -sticky nw
    grid configure $f.data_source.loadbutton -padx 3 -column 0 -row 2 -sticky nw

    # Controls for the diffusion
    frame $f.diffusion
    label $f.diffusion.label1 -font $Options(h3_font) -text $PreprocessorGlobals(diffusion_label)
    entry $f.diffusion.conductance -width 6 -justify r
    entry $f.diffusion.iterations -width 6 -justify r
    label $f.diffusion.conductance_label -text "Conductance"
    label $f.diffusion.iterations_label -text "Iterations"

    set PreprocessorGlobals(conductance_entry) $f.diffusion.conductance
    set PreprocessorGlobals(iterations_entry)  $f.diffusion.iterations

    label $f.diffusion.typeLabel -text "Diffusion type"
    radiobutton $f.diffusion.type1 -indicatoron true -text "gradient based" \
        -variable PreprocessorGlobals(diffusion_type) -value "gradient"
    radiobutton $f.diffusion.type2 -indicatoron true -text "curvature based" \
        -variable PreprocessorGlobals(diffusion_type) -value "curvature"

    button $f.diffusion.start -text "Start" \
        -command {SpawnProcess "PreprocessorStartDiffusion" "PreprocessorStartDiffusionRecovery" } \
        -state disabled
    set PreprocessorGlobals(diffusion_button) $f.diffusion.start

    $f.diffusion.conductance insert 0 $PreprocessorGlobals(default_conductance_parameter)
    $f.diffusion.iterations insert 0 $PreprocessorGlobals(default_iterations)
    
    grid configure $f.diffusion.label1 -padx 3 -column 0 -row 0 -columnspan 2 -sticky nw
    grid configure $f.diffusion.iterations_label -padx 3 -column 0 -row 1 -sticky nw
    grid configure $f.diffusion.iterations -padx 3 -column 1 -row 1 -sticky nw
    grid configure $f.diffusion.conductance_label -padx 3 -column 0 -row 2 -sticky nw
    grid configure $f.diffusion.conductance -padx 3 -column 1 -row 2 -sticky nw
    grid configure $f.diffusion.typeLabel -column 0 -row 3 -rowspan 2 -sticky nw -padx 3 -pady 3
    grid configure $f.diffusion.type1 -padx 3 -column 1 -row 3 -sticky nw 
    grid configure $f.diffusion.type2 -padx 3 -column 1 -row 4 -sticky nw
    grid configure $f.diffusion.start -padx 3 -column 0 -row 5 -sticky nw

    # Controls for the gradient
    frame $f.gradience
    label $f.gradience.label1 -font $Options(h3_font) -text $PreprocessorGlobals(gradient_label)
    entry $f.gradience.variance -width 6 -justify r
    label $f.gradience.variance_label -text "Smoothing variance"

    set PreprocessorGlobals(variance_entry) $f.gradience.variance
    button $f.gradience.start -text "Start" -command {SpawnProcess "PreprocessorStartGradient" "PreprocessorStartGradientRecovery" } -state disabled
    set PreprocessorGlobals(gradient_button) $f.gradience.start
    $f.gradience.variance insert 0 $PreprocessorGlobals(default_variance_parameter)

    grid configure $f.gradience.label1 -padx 3 -column 0 -row 0 -columnspan 2 -sticky nw
    grid configure $f.gradience.variance_label -padx 3 -column 0 -row 1 -sticky nw
    grid configure $f.gradience.variance -padx 3 -column 1 -row 1 -sticky nw
    grid configure $f.gradience.start -padx 3 -column 0 -row 3 -sticky nw
 
    # Source viewer window
    CreatePreprocessorRenderWindow $f "source"

    # Diffused image viewer window
    CreatePreprocessorRenderWindow $f "diffused"

    # Gradient image viewer window
    CreatePreprocessorRenderWindow $f "gradient"

    wm withdraw $PreprocessorGlobals(source_viewer_window)
    wm withdraw $PreprocessorGlobals(diffused_viewer_window)
    wm withdraw $PreprocessorGlobals(gradient_viewer_window)


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

    if {[file exists $PreprocessorGlobals(help_file)] == 1} {
        set fileId [open $PreprocessorGlobals(help_file) r]
        
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
        [subst {$f.instructions.text}] insert 0.0 $PreprocessorGlobals(help_text)
    }
    
    pack $f.instructions.scrollbar -side right -fill y
    pack $f.instructions.text -fill both -expand t

    $f.instructions.text configure -state disabled


    # Configure layout
    grid configure $f.title -row 0 -column 0 -columnspan 3 -sticky nw -padx 10
    grid configure $f.data_source -row 1 -column 0 -sticky nw -padx 10 -pady 10
    grid configure $f.diffusion -row 1  -column 1  -sticky nw -padx 10 -pady 10
    grid configure $f.gradience -row 1  -column 2  -sticky nw -padx 10 -pady 10
    grid configure $f.instructions -row 2 -column 0 -columnspan 3 -padx 3 -pady 3

}

proc PreprocessorLoadDataRecovery {} {
    global PreprocessorGlobals

    $PreprocessorGlobals(diffusion_button) configure -state disabled
    $PreprocessorGlobals(gradient_button) configure -state disabled
}
 
proc PreprocessorStartDiffusionRecovery {} {
    global PreprocessorGlobals
    $PreprocessorGlobals(diffusion_button) configure -state normal
}

proc PreprocessorStartGradientRecovery {} {
    global PreprocessorGlobals
    $PreprocessorGlobals(gradient_button) configure -state normal
}

proc CreatePreprocessorRenderWindow {f fr} {
    global PreprocessorGlobals Options

    toplevel $f.$fr

    set keyWord $fr
    append keyWord "_window_label"
    wm title $f.$fr $PreprocessorGlobals($keyWord)

    set keyWord $fr
    append keyWord "_viewer_window"
    set PreprocessorGlobals($keyWord) $f.$fr

    set keyWord $fr
    append keyWord "_viewer_widget"
    set PreprocessorGlobals($keyWord) $f.$fr.viewer

    set keyWord $fr
    append keyWord "_viewer"
    vtkTkImageViewerWidget $f.$fr.viewer -iv $PreprocessorGlobals($keyWord)
#    BindTkImageViewer        $f.$fr.viewer
    ::vtk::bind_tk_imageviewer_widget $f.$fr.viewer

    wm protocol $f.$fr  WM_DELETE_WINDOW { }

    set keyWord $fr
    append keyWord "_slice_number"
    set commandWord "PreprocessorSetSlice_"
    append commandWord $fr

    scale $f.$fr.slice -from 0 -to 0 -orient horizontal \
        -command $commandWord -variable PreprocessorGlobals($keyWord)
    set PreprocessorGlobals($keyWord) 0

    set keyWord $fr
    append keyWord "_slice_scale"
    set PreprocessorGlobals($keyWord) $f.$fr.slice

    frame $f.$fr.colormap
    entry $f.$fr.colormap.colormapLow  -width 6 -justify r
    entry $f.$fr.colormap.colormapHigh -width 6 -justify r

    $f.$fr.colormap.colormapHigh insert 0 $PreprocessorGlobals(default_lookup_table_high)
    $f.$fr.colormap.colormapLow insert 0 $PreprocessorGlobals(default_lookup_table_low)

    set cmdStr [subst -nocommands  { ResetLookupTable $fr [$f.$fr.colormap.colormapLow get] [$f.$fr.colormap.colormapHigh get] } ]

    button $f.$fr.colormap.colormapButton -text "scalar colormap" \
        -command $cmdStr -font $Options(small_font)

    pack $f.$fr.colormap.colormapLow $f.$fr.colormap.colormapHigh \
        $f.$fr.colormap.colormapButton -side left -padx 3

    grid configure $f.$fr.viewer   -row 0 -column 0 -columnspan 3 -padx 10
    grid configure $f.$fr.slice    -row 1 -column 0 -columnspan 3 -padx 10
    grid configure $f.$fr.colormap -row 2 -columnspan 3 -column 0 -sticky w -padx 10
}

###
# proc PreprocessorInitialize
#
# Set up the various filters for this module.
#
proc PreprocessorInitialize {} {
    global PreprocessorGlobals DataGlobals

    # A reader for the input to the module
    vtkImageReader PreprocessorSourceReader
    set PreprocessorGlobals(source_reader) PreprocessorSourceReader

    # A color lookup table for the source
    vtkColorTransferFunction PreprocessorLookupTable
    PreprocessorLookupTable AddRGBPoint  $PreprocessorGlobals(default_lookup_table_low)\
        0.0 0.0 0.0 
    PreprocessorLookupTable AddRGBPoint  $PreprocessorGlobals(default_lookup_table_high)\
        1.0 1.0 1.0
    set PreprocessorGlobals(source_lookup_table) PreprocessorLookupTable

    # A color lookup table for the diffused
    vtkColorTransferFunction PreprocessorDiffusedLookupTable
    PreprocessorDiffusedLookupTable AddRGBPoint  $PreprocessorGlobals(default_lookup_table_low)\
        0.0 0.0 0.0 
    PreprocessorDiffusedLookupTable AddRGBPoint  $PreprocessorGlobals(default_lookup_table_high)\
        1.0 1.0 1.0
    set PreprocessorGlobals(diffused_lookup_table) PreprocessorDiffusedLookupTable


    # A color lookup table for the gradient image
    vtkColorTransferFunction PreprocessorGradientLookupTable
    PreprocessorGradientLookupTable AddRGBPoint  $PreprocessorGlobals(default_lookup_table_low)\
        0.0 0.0 0.0 
    PreprocessorGradientLookupTable AddRGBPoint  $PreprocessorGlobals(default_lookup_table_high)\
        1.0 1.0 1.0
    set PreprocessorGlobals(gradient_lookup_table) PreprocessorGradientLookupTable
   
    #
    # Color mapper for the source data viewer
    #
    vtkImageMapToColors PreprocessorMapToRGBA
    PreprocessorMapToRGBA SetInput [PreprocessorSourceReader GetOutput]
    PreprocessorMapToRGBA SetOutputFormatToRGBA
    PreprocessorMapToRGBA SetLookupTable PreprocessorLookupTable
    set PreprocessorGlobals(source_color_mapper) PreprocessorMapToRGBA

    #
    # Color mapper for the diffused data viewer
    #
    vtkImageMapToColors PreprocessorDiffusedMapToRGBA
    PreprocessorDiffusedMapToRGBA SetOutputFormatToRGBA
    PreprocessorDiffusedMapToRGBA SetLookupTable PreprocessorDiffusedLookupTable
    set PreprocessorGlobals(diffused_color_mapper) PreprocessorDiffusedMapToRGBA

    #
    # Color mapper for the gradient data viewer
    #
    vtkImageMapToColors PreprocessorGradientMapToRGBA
    PreprocessorGradientMapToRGBA SetOutputFormatToRGBA
    PreprocessorGradientMapToRGBA SetLookupTable PreprocessorGradientLookupTable
    set PreprocessorGlobals(gradient_color_mapper) PreprocessorGradientMapToRGBA
    
    #
    # Viewer for source data.
    #
    vtkImageViewer PreprocessorImageViewer
    PreprocessorImageViewer SetColorWindow $PreprocessorGlobals(default_color_window)
    PreprocessorImageViewer SetColorLevel  $PreprocessorGlobals(default_color_level)
    PreprocessorImageViewer SetZSlice 0
    set PreprocessorGlobals(source_viewer) PreprocessorImageViewer

    ExitRegisterViewer PreprocessorImageViewer


    #
    # Viewer for diffusion processed data
    #
    vtkImageViewer PreprocessorOutputImageViewer
    PreprocessorOutputImageViewer SetColorWindow \
        $PreprocessorGlobals(default_color_window)
    PreprocessorOutputImageViewer SetColorLevel  \
        $PreprocessorGlobals(default_color_level)
    PreprocessorOutputImageViewer SetZSlice 0
    set PreprocessorGlobals(diffused_viewer) PreprocessorOutputImageViewer

    ExitRegisterViewer PreprocessorOutputImageViewer

    #
    # Viewer for gradient processed data
    #
    vtkImageViewer PreprocessorGradientImageViewer
    PreprocessorGradientImageViewer SetColorWindow \
        $PreprocessorGlobals(default_color_window)
    PreprocessorGradientImageViewer SetColorLevel  \
        $PreprocessorGlobals(default_color_level)
    PreprocessorGradientImageViewer SetZSlice 0
    set PreprocessorGlobals(gradient_viewer) PreprocessorGradientImageViewer

    ExitRegisterViewer PreprocessorGradientImageViewer

    #
    # Casting filter to convert to float
    #
    vtkImageCast PreprocessorCaster
    PreprocessorCaster SetOutputScalarTypeToFloat
    PreprocessorCaster ReleaseDataFlagOn
    PreprocessorCaster SetInput [PreprocessorSourceReader GetOutput]
    set PreprocessorGlobals(caster) PreprocessorCaster

    #
    # Gradient based anisotropic diffusion filter
    #
    vtkITKGradientAnisotropicDiffusionImageFilter PreprocessorDiffuser
    PreprocessorDiffuser SetTimeStep $PreprocessorGlobals(default_time_step)
    PreprocessorDiffuser SetConductanceParameter \
        $PreprocessorGlobals(default_conductance_parameter)
    PreprocessorDiffuser SetIterations $PreprocessorGlobals(default_iterations)
    set PreprocessorGlobals(diffuser) PreprocessorDiffuser

    #
    # Curvature based anisotropic diffusion filter
    #
    vtkITKCurvatureAnisotropicDiffusionImageFilter CurvaturePreprocessorDiffuser
    CurvaturePreprocessorDiffuser SetTimeStep $PreprocessorGlobals(default_time_step)
    CurvaturePreprocessorDiffuser SetConductanceParameter \
        $PreprocessorGlobals(default_conductance_parameter)
    CurvaturePreprocessorDiffuser SetIterations $PreprocessorGlobals(default_iterations)
    set PreprocessorGlobals(curvature_diffuser) CurvaturePreprocessorDiffuser


    #
    # Gradient magnitude filter
    #
    vtkITKGradientMagnitudeImageFilter GradientMagnitude
    set PreprocessorGlobals(gradient_magnitude) GradientMagnitude


    #
    # Discrete gaussian filter
    #
    vtkITKDiscreteGaussianImageFilter DiscreteGaussian
    DiscreteGaussian SetVariance $PreprocessorGlobals(default_variance_parameter) \
        $PreprocessorGlobals(default_variance_parameter) \
        $PreprocessorGlobals(default_variance_parameter)
    set PreprocessorGlobals(discrete_gaussian) DiscreteGaussian

    #
    # File writer for the output of the anisotropic diffusion
    #
    vtkImageWriter PreprocessorDiffusionWriter
    PreprocessorDiffusionWriter SetFileDimensionality \
        $DataGlobals(default_file_dimensionality)
    PreprocessorDiffusionWriter SetFilePattern \
        $DataGlobals(default_file_pattern)
    set PreprocessorGlobals(diffusion_writer) PreprocessorDiffusionWriter

    #
    # File writer for the output of the gradient calculation
    #
    vtkImageWriter PreprocessorGradientWriter
    PreprocessorGradientWriter SetFileDimensionality \
        $DataGlobals(default_file_dimensionality)
    PreprocessorGradientWriter SetFilePattern \
        $DataGlobals(default_file_pattern)
    set PreprocessorGlobals(gradient_writer) PreprocessorGradientWriter

}

###
#
#
proc PreprocessorLoadData {} {
    global PreprocessorGlobals
    
    $PreprocessorGlobals(diffusion_button) configure -state disabled
    $PreprocessorGlobals(gradient_button) configure -state disabled


    wm withdraw $PreprocessorGlobals(source_viewer_window)
    wm withdraw $PreprocessorGlobals(gradient_viewer_window)
    wm withdraw $PreprocessorGlobals(diffused_viewer_window)
    set $PreprocessorGlobals(diffused_window_visible) "false"
    set $PreprocessorGlobals(gradient_window_visible) "false"


    set PreprocessorGlobals(source_window_visible) "false"
    
    set tagName [ $PreprocessorGlobals(data_selector) get \
                      [$PreprocessorGlobals(data_selector) curselection]]
    set cacheEntry [GetDataCacheEntry $tagName]

    set endianString "SetDataByteOrderTo"
    append endianString [GetCacheEntryValue $cacheEntry "endianness"]
    append endianString "Endian"
    $PreprocessorGlobals(source_reader) $endianString

    set datatypeString "SetDataScalarTypeTo"
    append datatypeString [GetCacheEntryValue $cacheEntry "data_type"]

    $PreprocessorGlobals(source_reader) SetNumberOfScalarComponents \
        [GetCacheEntryValue $cacheEntry "number_of_components"]
    $PreprocessorGlobals(source_reader) $datatypeString
    $PreprocessorGlobals(source_reader) SetDataExtent \
        0 [GetCacheEntryValue $cacheEntry "size_x"] \
        0 [GetCacheEntryValue $cacheEntry "size_y"] \
        0 [GetCacheEntryValue $cacheEntry "size_z"]
    $PreprocessorGlobals(source_reader) SetFileNameSliceOffset \
        [GetCacheEntryValue $cacheEntry "slice_offset"]

    if { [regexp {^[yY]} [GetCacheEntryValue $cacheEntry "multiple_files"] ] } {
        $PreprocessorGlobals(source_reader) SetFileDimensionality 2
        $PreprocessorGlobals(source_reader) SetFilePrefix \
            [GetCacheEntryValue $cacheEntry "filename"] 
        $PreprocessorGlobals(source_reader) SetFilePattern \
            [GetCacheEntryValue $cacheEntry "file_pattern"]
    } else {
        $PreprocessorGlobals(source_reader) SetFileDimensionality 3
        $PreprocessorGlobals(source_reader) SetFileName \
            [GetCacheEntryValue $cacheEntry "filename"]
    }

    set PreprocessorGlobals(loaded_data_tag) $cacheEntry

    $PreprocessorGlobals(source_slice_scale) configure -from 0 \
        -to [GetCacheEntryValue $cacheEntry "size_z"] \
        -length [GetCacheEntryValue $cacheEntry "size_z"]

    $PreprocessorGlobals(source_viewer_widget) configure \
        -height [GetCacheEntryValue $cacheEntry "size_y"] \
        -width [GetCacheEntryValue $cacheEntry "size_x"]

    $PreprocessorGlobals(source_reader) Modified

    ConstructProgressWindow sourceReaderProgressWindow
    $PreprocessorGlobals(source_reader) SetProgressMethod \
        "ProgressProc $PreprocessorGlobals(source_reader) .sourceReaderProgressWindow"

    [$PreprocessorGlobals(source_reader) GetOutput] SetUpdateExtentToWholeExtent
    $PreprocessorGlobals(source_reader) Update

    $PreprocessorGlobals(source_color_mapper) SetInput [$PreprocessorGlobals(source_reader) GetOutput]
    $PreprocessorGlobals(source_viewer) SetInput [$PreprocessorGlobals(source_color_mapper) GetOutput]
    $PreprocessorGlobals(source_viewer) Render

    destroy .sourceReaderProgressWindow
    set PreprocessorGlobals(source_window_visible) "true"   
    wm deiconify $PreprocessorGlobals(source_viewer_window)
    $PreprocessorGlobals(diffusion_button) configure -state normal
   
}

proc ResetLookupTable {window low high} {
    global PreprocessorGlobals

    set keyWord $window
    append keyWord "_lookup_table"

    $PreprocessorGlobals($keyWord)  RemoveAllPoints
    $PreprocessorGlobals($keyWord)  AddRGBPoint $low  0.0 0.0 0.0
    $PreprocessorGlobals($keyWord)  AddRGBPoint $high 1.0 1.0 1.0

    set keyWord $window
    append keyWord "_viewer"
    $PreprocessorGlobals($keyWord) Render
}

proc PreprocessorSetSlice_source {slice} {
    global PreprocessorGlobals

    $PreprocessorGlobals(source_viewer) SetZSlice $slice
    $PreprocessorGlobals(source_viewer) Render
}

proc PreprocessorSetSlice_diffused {slice} {
    global PreprocessorGlobals

    $PreprocessorGlobals(diffused_viewer) SetZSlice $slice
    $PreprocessorGlobals(diffused_viewer) Render
}

proc PreprocessorSetSlice_gradient {slice} {
    global PreprocessorGlobals

    $PreprocessorGlobals(gradient_viewer) SetZSlice $slice
    $PreprocessorGlobals(gradient_viewer) Render
}

proc PreprocessorStartDiffusion {} {
    global PreprocessorGlobals DataGlobals
    
    $PreprocessorGlobals(diffusion_button) configure -state disabled
    $PreprocessorGlobals(gradient_button) configure -state disabled
    wm withdraw $PreprocessorGlobals(diffused_viewer_window)
    set PreprocessorGlobals(diffused_window_visible) "false"

    wm withdraw $PreprocessorGlobals(gradient_viewer_window)
    set PreprocessorGlobals(gradient_window_visible) "false"

    set fn $DataGlobals(cache_directory)
    append fn "/$DataGlobals(unique_user_prefix)"
    append fn $PreprocessorGlobals(diffusion_volume_filename)
  
    ConstructProgressWindow preprocessorDiffusionProgressWindow

    $PreprocessorGlobals(diffuser) SetIterations \
        [expr int([$PreprocessorGlobals(iterations_entry) get]) ]
    $PreprocessorGlobals(diffuser) SetConductanceParameter \
        [$PreprocessorGlobals(conductance_entry) get]

    $PreprocessorGlobals(curvature_diffuser) SetIterations \
        [expr int([$PreprocessorGlobals(iterations_entry) get]) ]
    $PreprocessorGlobals(curvature_diffuser) SetConductanceParameter \
        [$PreprocessorGlobals(conductance_entry) get]

    if { $PreprocessorGlobals(diffusion_type) == "gradient" } {
        $PreprocessorGlobals(diffuser) SetInput \
            [$PreprocessorGlobals(caster) GetOutput]
        $PreprocessorGlobals(diffusion_writer) SetInput \
            [$PreprocessorGlobals(diffuser) GetOutput]
        $PreprocessorGlobals(diffuser) SetProgressMethod \
            "ProgressProc $PreprocessorGlobals(diffuser) .preprocessorDiffusionProgressWindow"
    } else {
        $PreprocessorGlobals(curvature_diffuser) SetInput \
            [$PreprocessorGlobals(caster) GetOutput]
        $PreprocessorGlobals(diffusion_writer) SetInput \
            [$PreprocessorGlobals(curvature_diffuser) GetOutput]
        $PreprocessorGlobals(curvature_diffuser) SetProgressMethod \
            "ProgressProc $PreprocessorGlobals(curvature_diffuser) .preprocessorDiffusionProgressWindow"
    }
    
    if {$DataGlobals(default_file_dimensionality) == 2} {
        $PreprocessorGlobals(diffusion_writer) SetFilePrefix $fn
    } else {
        $PreprocessorGlobals(diffusion_writer) SetFileName $fn
    }

    set size_z [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "size_z"]
    set size_y [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "size_y"]
    set size_x [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "size_x"]

    $PreprocessorGlobals(diffused_slice_scale) configure -from 0 -to $size_z \
        -length $size_z
    
    $PreprocessorGlobals(diffused_viewer_widget) configure \
        -height $size_y -width $size_x
    
    # Make sure we diffuse the whole volume, not just one slice at a time.
    if { $PreprocessorGlobals(diffusion_type) == "gradient" } {
        [$PreprocessorGlobals(diffuser) GetOutput] SetUpdateExtentToWholeExtent
        $PreprocessorGlobals(diffuser) Update 
    } else {
        [$PreprocessorGlobals(curvature_diffuser) GetOutput] SetUpdateExtentToWholeExtent
        $PreprocessorGlobals(curvature_diffuser) Update 
    }

    $PreprocessorGlobals(diffusion_writer) Write

    if { $PreprocessorGlobals(diffusion_type) == "gradient" } {
        $PreprocessorGlobals(diffused_color_mapper) SetInput \
            [$PreprocessorGlobals(diffuser) GetOutput]
    } else {
        $PreprocessorGlobals(diffused_color_mapper) SetInput \
            [$PreprocessorGlobals(curvature_diffuser) GetOutput]
    }
    $PreprocessorGlobals(diffused_viewer) SetInput \
        [$PreprocessorGlobals(diffused_color_mapper) GetOutput]
    
    if {$DataGlobals(default_file_dimensionality) == 2} { 
        set multFiles "Yes" 
    } else {
        set multFiles "No"
    }
    set noComponents [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "number_of_components"]
    
    set entryStr "$fn Float $size_x $size_y $size_z $DataGlobals(default_file_endianness) $multFiles $noComponents $DataGlobals(default_file_pattern) 0"
    AddDataCacheEntry $PreprocessorGlobals(diffused_tag) $entryStr
    
    $PreprocessorGlobals(diffused_viewer) Render
    destroy .preprocessorDiffusionProgressWindow
    set PreprocessorGlobals(diffused_window_visible) "true"
    wm deiconify $PreprocessorGlobals(diffused_viewer_window)
    $PreprocessorGlobals(diffusion_button) configure -state normal
    $PreprocessorGlobals(gradient_button) configure -state normal
}


proc PreprocessorStartGradient {} {
    global PreprocessorGlobals DataGlobals
    $PreprocessorGlobals(gradient_button) configure -state disabled
    wm withdraw $PreprocessorGlobals(gradient_viewer_window)
    set PreprocessorGlobals(gradient_window_visible) "false"

    set fn $DataGlobals(cache_directory)
    append fn "/$DataGlobals(unique_user_prefix)"
    append fn $PreprocessorGlobals(gradient_volume_filename)
  
    ConstructProgressWindow gradientProgressBar
$PreprocessorGlobals(gradient_magnitude) SetProgressMethod \
        "ProgressProc $PreprocessorGlobals(gradient_magnitude) .gradientProgressBar"
    
    if { $PreprocessorGlobals(diffusion_type) == "gradient" } {
        $PreprocessorGlobals(gradient_magnitude) SetInput [$PreprocessorGlobals(diffuser) GetOutput]
    } else {
        $PreprocessorGlobals(gradient_magnitude) SetInput [$PreprocessorGlobals(curvature_diffuser) GetOutput]
    }
    $PreprocessorGlobals(gradient_magnitude) Update

    $PreprocessorGlobals(discrete_gaussian) SetInput [$PreprocessorGlobals(gradient_magnitude) GetOutput]

    $PreprocessorGlobals(discrete_gaussian) Update

    $PreprocessorGlobals(gradient_writer) SetInput [$PreprocessorGlobals(discrete_gaussian) GetOutput] 

    $PreprocessorGlobals(discrete_gaussian) SetVariance [$PreprocessorGlobals(variance_entry) get] \
        [$PreprocessorGlobals(variance_entry) get] \
        [$PreprocessorGlobals(variance_entry) get] 

    if {$DataGlobals(default_file_dimensionality) == 2} {
        $PreprocessorGlobals(gradient_writer) SetFilePrefix $fn
    } else {
        $PreprocessorGlobals(gradient_writer) SetFileName $fn
    }
    
    set size_z [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "size_z"]
    set size_y [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "size_y"]
    set size_x [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "size_x"]

    $PreprocessorGlobals(gradient_slice_scale) configure -from 0 -to $size_z \
        -length $size_z
    
    $PreprocessorGlobals(gradient_viewer_widget) configure \
        -height $size_y -width $size_x
    

    $PreprocessorGlobals(gradient_color_mapper) SetInput [$PreprocessorGlobals(discrete_gaussian) GetOutput]
    $PreprocessorGlobals(gradient_viewer) SetInput [$PreprocessorGlobals(gradient_color_mapper) GetOutput]


    # Make sure we diffuse the whole volume, not just one slice at a time.
    [$PreprocessorGlobals(discrete_gaussian) GetOutput] SetUpdateExtentToWholeExtent
    $PreprocessorGlobals(discrete_gaussian) Update   
    $PreprocessorGlobals(gradient_writer) Write


    if {$DataGlobals(default_file_dimensionality) == 2} { 
        set multFiles "Yes" 
    } else {
        set multFiles "No"
    }
    set noComponents [GetCacheEntryValue $PreprocessorGlobals(loaded_data_tag) "number_of_components"]
    
    set entryStr "$fn Float $size_x $size_y $size_z $DataGlobals(default_file_endianness) $multFiles $noComponents $DataGlobals(default_file_pattern) 0"
    AddDataCacheEntry $PreprocessorGlobals(gradient_tag) $entryStr
    $PreprocessorGlobals(gradient_viewer) Render
    destroy .gradientProgressBar

    set PreprocessorGlobals(gradient_window_visible) "true"
    wm deiconify $PreprocessorGlobals(gradient_viewer_window)
    $PreprocessorGlobals(gradient_button) configure -state normal
}
