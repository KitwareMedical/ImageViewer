#/*=========================================================================
#
#  Program:   Insight Segmentation & Registration Toolkit
#  Module:    Options.tcl
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

# Below are options only configurable from this file
set Options(console_is_resizable)       "false"
set Options(credits_author)             "cates@sci.utah.edu"
set Options(credits_date)               "July 2002"
set Options(data_window_size)           3
set Options(default_console_window_width)  50
set Options(default_console_window_height) 20
set Options(disclaimer_file)            "./Copyright.txt"
set Options(disclaimer_text)            "Please see \"Insight\Copyright.txt\""
set Options(fancy_font)                 "WST_Fancy"
set Options(h1_font)                    "WST_H1"
set Options(h2_font)                    "WST_H2"
set Options(h3_font)                    "WST_H3"
set Options(help_file)                  "./Help.txt"
set Options(help_text)                  "ERROR: Cannot find Help.txt"
set Options(itk_logo)                   "WST_ItkLogo"
set Options(menu_font)                  "WST_Menu"
set Options(mono_font)                  "WST_Mono"
set Options(menu_active_background_color) "lightyellow"
set Options(menu_bar_color)             "gray"
set Options(menu_select_color)          "red"
set Options(menu_text_color)            "black"
set Options(help_box_color)             "lightgreen"
set Options(message_box_font)           "WST_Message"
set Options(minimum_console_window_width)  30
set Options(minimum_console_window_height) 20
set Options(normal_font)                "WST_Normal"
set Options(pleasant_background_color)  "lightblue"
set Options(program_name)               "Segmentation Editor"
set Options(require_confirm_on_exit)    "false"
set Options(small_font)                 "WST_Small"
set Options(sci_logo)                   "WST_SCILogo"
set Options(text_background_color)      "lightyellow"
set Options(utah_logo)                  "WST_UtahLogo"
set Options(geek_logo)                  "WST_SomeGeek"
set Options(editor_help_image)          "WST_EditorHelp"
set Options(warning_color)              "red"
set Options(progress_bar_color)         "orange"
set Options(progress_bar_length)        300
set Options(progress_bar_height)        30
set Options(force_write_on_editor_exit) "false"
set Options(default_editor_magnification) 1.0
set Options(animate_logo)               "false"
set Options(configuration_file_name)    "WST.conf"

# Read local settings
if { [file exists $Options(configuration_file_name)] == 1  } {
    source $Options(configuration_file_name)
} else {
    source "./Configure.tcl"
    GenerateConfigurationFile
    source $Options(configuration_file_name)
}

# Below are options that derive from values listed above
set Options(credits_logos) "sci_logo itk_logo utah_logo"
set Options(farewell_message)  ""
set Options(console_title) "$Options(program_name) Console"


