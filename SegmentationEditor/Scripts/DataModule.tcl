#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    DataModule.tcl
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

###
# This file contains methods and definitions for managing
# data objects.  A data "cache" is constructed which is
# just a pool of filenames and header information.
#

# Initialize the global data variables
global DataCache
global DataSelectorRegistry

set DataGlobals(frame_title_text)  "Data"
set DataGlobals(delete_cache_files_on_exit) "yes"
set DataGlobals(unique_user_prefix) ""
set DataGlobals(cache_directory) ""
set DataGlobals(load_cache_entry_fields) "tag filename data_type size_x size_y size_z endianness multiple_files number_of_components file_pattern slice_offset header_size"
set DataGlobals(null_data_string)  "/dev/null null_type"
set DataGlobals(user_configurable_options) "unique_user_prefix cache_directory"
set DataGlobals(labeled_image_type) "labeled_image"
set DataGlobals(tree_type) "tree"
set DataGlobals(scalar_type) "scalar"
set DataGlobals(defined_tree_types) "SegmentationTree"
set DataGlobals(defined_labeled_image_types) "UnsignedLong"
set DataGlobals(defined_scalar_types) "Float Short UnsignedShort Char UnsignedChar Int UnsignedInt"
set DataGlobals(default_file_pattern) "%s.%d"
set DataGlobals(default_file_dimensionality) 2
set DataGlobals(default_file_endianness) "Little"
foreach val [split $DataGlobals(load_cache_entry_fields)] {
    set DataLoadValues($val) ""
}


set DataGlobals(help_file) "DataHelp.txt"
set DataGlobals(help_text) "Sorry, could not find $DataGlobals(help_file)."
set DataLabels(tag) "Tag"
set DataLabels(filename) "File prefix/name"
set DataLabels(data_type) "Data type"
set DataLabels(size_x) "Size X"
set DataLabels(size_y) "Size Y"
set DataLabels(size_z) "Size Z"
set DataLabels(endianness) "Byte order"
set DataLabels(multiple_files) "Stored in multiple files?"
set DataLabels(number_of_components) "Number of components"
set DataLabels(file_pattern) "VTK file extension pattern"
set DataLabels(slice_offset) "Number of first slice"
set DataLabels(header_size) "Size of any header to skip"

set DataLabels(unique_user_prefix) "Session prefix"
set DataLabels(cache_directory) "Session directory"


#
# This method registers a listbox widget to be updated each time
# the cache contents change.
#
proc RegisterDataSelector { widget allowed_type } {
    global DataGlobals DataSelectorRegistry
    
    set DataSelectorRegistry($widget) $allowed_type
}


#
# This method returns the value of the requested field in a cache entry
#
proc GetCacheEntryValue { cache_entry field } {
    global DataGlobals

    set idx [lsearch -exact [split $DataGlobals(load_cache_entry_fields)] $field]
    set idx [expr $idx - 1] ;# cache entries do not include the tag field
    return [lindex [split $cache_entry] $idx]
}

#
# Compares a cache entry's data type with a standard type
#
proc CompareDataCacheType { cache_entry type } {
    global DataGlobals
    
    set cache_type [GetCacheEntryValue $cache_entry "data_type"]

    if { $cache_type == $type } {
        return "true"
    }

    set type_string "defined_"
    append type_string $type
    append type_string "_types"

    foreach m [split $DataGlobals($type_string)] {
        if { $cache_type == $m } {
            return "true"
        }
    }

    return "false"
}


#
# Updates the contents of all registered data selector widgets
#
proc UpdateSelectors { }  {
    global DataGlobals DataCache DataSelectorRegistry

    foreach widget [array names DataSelectorRegistry] {
        $widget delete 0 end
        foreach data [array names DataCache] {            
            #is this a valid type?
            if { [CompareDataCacheType $DataCache($data) \
                      $DataSelectorRegistry($widget)] == "true" } {
                $widget insert 0 $data
            }
        }
    } 
}

#
# Adds an entry to the global cache of data.
#
proc AddDataCacheEntry { tag entry_list } {
    global DataCache DataGlobals

    set DataCache($tag) $entry_list
    
    UpdateSelectors
    UpdateDataCacheWindow
}

#
# Erases a single entry from the global cache of data
#
proc EraseDataCacheEntry {tag} {
    global DataCache DataGlobals

    set filename [lindex $DataCache($tag) 1]

    set DataCache($tag) $DataGlobals(null_data_string)
    UpdateSelectors
    UpdateDataCacheWindow

}

# 
# Erases all the entries in the data cache
#
proc EraseDataCache {} {
    global DataCache DataGlobals DataHeaderWidgets

    foreach tag [array names DataCache] {
        EraseDataCacheEntry $tag
    }

    foreach item $DataGlobals(user_configurable_options) {
        $DataHeaderWidgets($item) delete 0 end
    }


    UpdateSelectors
    UpdateDataCacheWindow
}

# 
# Returns the string associated with the given tag for a data cache
# 
proc GetDataCacheEntry {tag} {
    global DataCache

    return $DataCache($tag)
}

#
# Returns a list of data cache tags whose entries match the given type
#
#proc GetDataCacheTagsByType {type} {
#    global DataCache
#
#    set ans ""
#
#    foreach tag [array names DataCache] {
#        if { [lindex $DataCache($tag) 1] == $type } {
#            lappend ans $tag
#        }
#    }
    
#    return $ans
#}

#
# Fills the data cache display window.
#
proc UpdateDataCacheWindow {} {
    global DataGlobals DataCache

    $DataGlobals(cache_display_window) configure -state normal
    $DataGlobals(cache_display_window) delete 0.0 end

    set i 0
    foreach element [array names DataCache] {

        if { [GetCacheEntryValue $DataCache($element) "data_type"] \
                 != "null_type" } { 
            $DataGlobals(cache_display_window) \
                insert 0.$i "$element   $DataCache($element) \n"
            set i [expr $i + 1]
        }
    }

    $DataGlobals(cache_display_window) configure -state disabled
}

#
# Add a data cache entry from the GUI input
# 
proc AddDataCacheEntryFromGUI {} {
    global DataLoadValues DataGlobals
    foreach val [split $DataGlobals(load_cache_entry_fields)] {
        if {$DataLoadValues($val) == ""} {
            return 
        }

        if {$val == "tag"} {
            set tag $DataLoadValues($val)
        } else {
            append arr $DataLoadValues($val)
            append arr " "
        }
    }

    AddDataCacheEntry $tag $arr
}


#
# Standard module method.  Creates the GUI
#
proc ConstructDataFrame {f} {
    global Options DataGlobals DataLoadValues DataLabels DataHeaderWidgets

    $f configure -background $Options(text_background_color)

    # Create some title text
    frame $f.title -background $Options(pleasant_background_color)
    label $f.title.text -textvariable DataGlobals(frame_title_text) \
        -background $Options(pleasant_background_color) \
        -font $Options(h1_font)
    pack $f.title.text -side left

    # Set up user configurable entries
    frame $f.configure -background $Options(text_background_color)
    set i 0
    foreach option $DataGlobals(user_configurable_options) {
        set optionlabel $option
        append optionlabel "Label"
        label $f.configure.$optionlabel -text $DataLabels($option) \
            -background $Options(text_background_color) \
            -borderwidth 0

        entry $f.configure.$option 
        $f.configure.$option insert insert $DataGlobals($option)

        set DataHeaderWidgets($option) $f.configure.$option

        # Change variable on leaving this widget
        set commandStr [subst -nocommands { set DataGlobals($option) [%W get] } ]
        bind $f.configure.$option <Key> +$commandStr
        bind $f.configure.$option <KeyPress> +$commandStr
        bind $f.configure.$option <KeyRelease> +$commandStr
        bind $f.configure.$option <Motion> +$commandStr
        bind $f.configure.$option <Enter> +$commandStr
        bind $f.configure.$option <Leave> +$commandStr
        bind $f.configure.$option <FocusOut> +$commandStr        
        bind $f.configure.$option <FocusIn> +$commandStr


        # Configure layout
        grid configure $f.configure.$optionlabel \
            -padx 3 -column 0 -row $i -sticky nw
        grid configure $f.configure.$option      \
            -padx 3 -column 1 -row $i -sticky nw

        set i [expr $i + 1]
    }

    # Set up a load data frame
    frame $f.add -background $Options(text_background_color)
    set i 0
    foreach option $DataGlobals(load_cache_entry_fields) {
        set optionlabel $option
        append optionlabel "Label"
        label $f.add.$optionlabel -text $DataLabels($option) \
            -background $Options(text_background_color) \
            -borderwidth 0

        entry $f.add.$option 
        $f.add.$option insert insert $DataLoadValues($option)

        # Change variable on leaving this widget
        set commandStr [subst -nocommands { set DataLoadValues($option) [%W get] } ]
        bind $f.add.$option <Key-space> { %W delete 0 end }
        bind $f.add.$option <Key> +$commandStr
        bind $f.add.$option <KeyPress> +$commandStr
        bind $f.add.$option <KeyRelease> +$commandStr
        bind $f.add.$option <FocusOut> +$commandStr
        bind $f.add.$option <FocusIn> +$commandStr
        bind $f.add.$option <Enter> +$commandStr
        bind $f.add.$option <Leave> +$commandStr
        bind $f.add.$option <Motion> +$commandStr

        # Configure layout
        grid configure $f.add.$optionlabel -padx 3 -column 0 -row $i -sticky nw
        grid configure $f.add.$option      -padx 3 -column 1 -row $i -sticky nw
        set i [expr $i + 1]
    }
    
    button $f.add.loadbutton -text "Add/Update" \
        -command { AddDataCacheEntryFromGUI }
    grid configure $f.add.loadbutton -padx 3 \
        -pady 10 -column 1 -row $i -sticky nw

    #Set up data cache member list
    frame $f.cachelist -background $Options(text_background_color)
    text $f.cachelist.textwindow -font $Options(mono_font) \
        -background $Options(pleasant_background_color) \
        -wrap "none" \
        -width 90 \
        -height 10 \
        -insertontime 0 \
        -setgrid true  \
        -yscrollcommand "$f.cachelist.scrollbar set" \
        -xscrollcommand "$f.cachelist.scrollbarH set" \
        -state disabled
    # keep a pointer to this widget for future reference
    set DataGlobals(cache_display_window) $f.cachelist.textwindow

    scrollbar $f.cachelist.scrollbar -orient "vert" \
        -command "$f.cachelist.textwindow yview"
    scrollbar $f.cachelist.scrollbarH -orient "horiz" \
        -command "$f.cachelist.textwindow xview"
    pack $f.cachelist.scrollbar  -side right -fill y
    pack $f.cachelist.scrollbarH  -side bottom -fill x
    pack $f.cachelist.textwindow -fill both -expand t

    # Set up controls to save/load cache lists
    frame $f.saveload
    button $f.saveload.save -text "Save session" -command SaveCacheList
    button $f.saveload.load -text "Load session" -command LoadCacheList
    button $f.saveload.clear -text "Clear session" -command EraseDataCache
    pack $f.saveload.load $f.saveload.save $f.saveload.clear -side left -padx 2

    # Create a little box for instructions
    frame $f.instructions
      text $f.instructions.text \
        -font $Options(mono_font) \
        -width 35 \
        -height 10 \
        -background $Options(help_box_color) \
        -wrap "word" \
        -insertontime 0 \
        -setgrid true  \
        -yscrollcommand "$f.instructions.scrollbar set"

    scrollbar $f.instructions.scrollbar -orient "vert" -command "$f.instructions.text yview"

    if {[file exists $DataGlobals(help_file)] == 1} {
        set fileId [open $DataGlobals(help_file) r]
        
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
        [subst {$f.instructions.text}] insert 0.0 $DataGlobals(help_text)
    }
    
    pack $f.instructions.scrollbar -side right -fill y
    pack $f.instructions.text -fill both -expand t

    $f.instructions.text configure -state disabled
    

    # Configure layout
    grid configure $f.title -row 0 -column 0 -columnspan 2 -sticky w -padx 10
    grid configure $f.configure -row 1 -column 0 -columnspan 1 -rowspan 1 -sticky nw -padx 10
    grid configure $f.add       -row 1 -column 1 -columnspan 1 -rowspan 2 -sticky nw -padx 10
    grid configure $f.instructions -row 2 -column 0 -columnspan 1 -sticky nw -padx 10
    grid configure $f.cachelist -row 3 -column 0 -columnspan 2 -sticky nw -padx 10
    grid configure $f.saveload  -row 4 -column 0 -columnspan 1 -sticky nw -padx 10 -pady 10

    grid rowconfigure $f 0 -pad 20 -weight 0
    grid rowconfigure $f 1 -pad 5 -weight 1
    grid rowconfigure $f 2 -pad 5 -weight 3

}

proc SaveCacheList {} {
    global DataGlobals DataCache

    set fn [tk_getSaveFile -initialdir $DataGlobals(cache_directory) \
               -defaultextension ".cl" \
                -filetypes [list "{File header lists} {.cl}"] ]
    if { $fn == "" } return
    set file_handle [open $fn w]

    #
    # write the header info
    #
    foreach item $DataGlobals(user_configurable_options) {
        puts $file_handle $DataGlobals($item)
    }

    foreach element [array names DataCache] {

        if { [GetCacheEntryValue $DataCache($element) "data_type"] \
                 != "null_type" } { 
            puts $file_handle "$element $DataCache($element)"
        }
    }

    close $file_handle
}

proc LoadCacheList {} {
    global DataGlobals DataCache DataHeaderWidgets

    set fn [tk_getOpenFile -initialdir $DataGlobals(cache_directory) \
                -defaultextension ".cl" \
                -filetypes [list "{File header lists} {.cl}"] ]
    if { $fn == "" } return
    set file_handle [open $fn r]

    #
    # Clear out old cache
    #
    EraseDataCache

    #
    # Get the header info
    #
    foreach item $DataGlobals(user_configurable_options) {
        gets $file_handle buf
        $DataHeaderWidgets($item) delete 0 end
        $DataHeaderWidgets($item) insert insert $buf
        set DataGlobals($item) $buf
    }

    while { ! [eof $file_handle] } {
        gets $file_handle buf

        set tag [ lindex [split $buf] 0 ]
        set entry [lrange [split $buf] 1 [llength [split $buf]]] 
	regsub -all \{ $entry "" entry
	regsub -all \} $entry "" entry
       AddDataCacheEntry $tag $entry
    }

    
  
    close $file_handle
    UpdateDataCacheWindow
}


proc ActivateDataModule {} {
    global DataGlobals

}

proc DeactivateDataModule {} {
    global DataGlobals

}

