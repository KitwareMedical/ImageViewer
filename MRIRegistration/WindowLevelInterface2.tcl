# a simple user interface that manipulates window level.
# places in the tcl top window.  Looks for object named viewer

#only use this interface when not doing regression tests
if {[info commands rtExMath] != "rtExMath"} {

# Take window level parameters from viewer
proc InitializeWindowLevelInterface {} {
   global sliceNumber
   global indentity
   # Get parameters from viewer
   set w [map1 GetColorWindow]
   set l [map1 GetColorLevel]
   set sliceNumber [map1 GetZSlice]
   set zMin [map1 GetWholeZMin]
   set zMax [map1 GetWholeZMax]
#   set zMin 0
#   set zMax 128

   frame .slice
   label .slice.label -text "Slice"
   scale .slice.scale  -from $zMin -to $zMax -orient horizontal \
     -command SetSlice -variable sliceNumber
#   button .slice.up -text "Up" -command SliceUp
#   button .slice.down -text "Down" -command SliceDown
   
   frame .wl
   frame .wl.f1
   label .wl.f1.windowLabel -text "Window"
   scale .wl.f1.window -from 1 -to [expr $w * 2]  -orient horizontal \
     -command SetWindow -variable window
   frame .wl.f2
   label .wl.f2.levelLabel -text "Level"
   scale .wl.f2.level -from [expr $l - $w] -to [expr $l + $w] \
     -orient horizontal -command SetLevel
   checkbutton .wl.video -text "Inverse Video" -command SetInverseVideo

    frame .wl.f3
    label .wl.f3.checkerLabel -text "Checkers"
    scale .wl.f3.ch -orient horizontal -from 1 -to 100 -command SetCheckerboard -variable checkerboard
   checkbutton .wl.swapCheck -text "Swap Checkers" -command SwapCheckers
   checkbutton .wl.swapRegister -text "Register On/Off" -command SwapRegister  -variable identity

   # resolutions less than 1.0
   if {$w < 10} {
      set res [expr 0.05 * $w]
      .wl.f1.window configure -resolution $res -from $res -to [expr 2.0 * $w]
      .wl.f2.level configure -resolution $res \
	-from [expr 0.0 + $l - $w] -to [expr 0.0 + $l + $w] 
   }

   .wl.f1.window set $w
   .wl.f2.level set $l

   .wl.f3.ch set 2

   frame .ex
   button .ex.exit -text "Exit" -command "exit"

   pack .slice .wl .ex -side top
   pack .slice.label .slice.scale -side left
   pack .wl.f1 .wl.f2 .wl.video -side top
   pack .wl.f1.windowLabel .wl.f1.window -side left
   pack .wl.f2.levelLabel .wl.f2.level -side left
   pack .wl.f3 -side top
   pack .wl.f3.checkerLabel .wl.f3.ch -side left
   pack .wl.swapCheck -side top
   pack .wl.swapRegister -side top
   pack .ex.exit -side left
}
   
proc SetCheckerboard { val } {
    global checkerboard
    checkers SetNumberOfDivisions $checkerboard $checkerboard 1
    renWin Render
}

proc SwapCheckers {} {

    set temp0 [checkers GetInput 0]
    set temp1 [checkers GetInput 1]

    checkers SetInput 0 $temp1
    checkers SetInput 1 $temp0
    checkers Modified

    renWin Render

}

proc SwapRegister {} {
  global identity
    if {$identity == 0} { 
        reslice SetResliceTransform identity
    } else {
        reslice SetResliceTransform transform
    }
    renWin Render
}

proc SetSlice { slice } {
   global sliceNumber

    mapCheckers SetZSlice $sliceNumber
    map1 SetZSlice $sliceNumber
    map2 SetZSlice $sliceNumber
    mapDiff SetZSlice $sliceNumber
    renWin Render
}

proc SetWindow window {
   global video
   if {$video} {
      map1 SetColorWindow [expr -$window]
      map2 SetColorWindow [expr -$window]
      mapCheckers SetColorWindow [expr -$window]
      mapDiff SetColorWindow [expr -$window]
   } else {
      map1 SetColorWindow $window
      map2 SetColorWindow $window
      mapCheckers SetColorWindow $window
      mapDiff SetColorWindow $window
   }
   renWin Render
}

proc SetLevel level {
    map1 SetColorLevel $level
    map2 SetColorLevel $level
    mapCheckers SetColorLevel $level
    mapDiff SetColorLevel $level
    renWin Render
}

proc SetInverseVideo {} {
   global video window
   if {$video} {
      map1 SetColorWindow [expr -$window]
      map2 SetColorWindow [expr -$window]
      mapCheckers SetColorWindow [expr -$window]
   } else {
      map1 SetColorWindow $window
      map2 SetColorWindow $window
      mapCheckers SetColorWindow $window
   }
   renWin Render
}


InitializeWindowLevelInterface

} else {
  renWin Render
}

