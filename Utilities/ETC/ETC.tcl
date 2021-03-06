
namespace eval ETC {

  variable Jobs

  proc Reset {} {
    variable Jobs
    array unset Jobs
  }

  proc SetProject { p } {
    variable Jobs
    set Jobs(Project) $p
  }

  proc SetWorkingDirectory { p } {
    variable Jobs
    set Jobs(WorkingDirectory) $p
  }

  proc FindData { pattern } {
    variable Jobs
    return [WalkDirectory $Jobs(WorkingDirectory) $pattern]
  }

  proc RootTask { task } {
    variable Jobs
    set Jobs($Jobs(Project),RootTask) $task
  }

  proc Schedule { args } {
    variable Jobs
    set Project $Jobs(Project)
    if { ![info exists Jobs(Counter)] } {
      set Jobs(Counter) 0
    }
    array set A [lrange $args 0 end-1]
    foreach {key value} [list -command "" -depends ""] {
      if { ![info exist A($key)] } { set A($key) $value }
    }
    if { ![info exist A(-name)] } {
      error "::ETC::Schedule requires a -name argument"
    }
    set Jobs($Project,$A(-name),Name) $A(-name)
    set Jobs($Project,$A(-name),Command) [lindex $args end]
    set Jobs($Project,$A(-name),Depends) $A(-depends)
    set Jobs($Project,$A(-name),Id) Job[incr Jobs(Counter)]
  }
  
  proc ExpandDepends { Name } {
    variable Jobs
    set Project $Jobs(Project)
    set Names [array names Jobs $Project*Name]
    set matches ""
    foreach pattern $Jobs($Project,$Name,Depends) {
      foreach n $Names {
        if { [string match $pattern $Jobs($n)] } {
          set matchname $Jobs($n)
          lappend matches $Jobs($Project,$matchname,Id)
        }
      }
    }
    return $matches
  }

  proc Generate { fn } {
    variable Jobs
    set Project $Jobs(Project)
    set WorkingDirectory $Jobs(WorkingDirectory)

    set fid [open $fn "w"]
    puts $fid "\# Generated by ECT!"
    
    puts $fid "\nall: $Jobs($Project,RootTask)"

    set Root $Jobs($Project,RootTask)
    puts $fid "\n$Root: $Jobs($Project,$Root,Id)"

    foreach key [array names Jobs $Project*Name] {
      puts $fid "\n"
      set name $Jobs($key)
      set id $Jobs($Project,$name,Id)
      set done "$WorkingDirectory/ETC/$id.completed"
      puts $fid "\# $name"
      puts $fid "$Jobs($Project,$name,Id): [ExpandDepends $name] "
      puts $fid "\tcd $WorkingDirectory && loopy -r 10 -d $done \'loopy -d $done -w \"$Jobs($Project,$name,Command)\"\'"
    }
    close $fid
  }


  proc WalkDirectory { Queue pattern } {
    set Output ""
    while { [llength $Queue] != 0 } \
      {
        set Filename [lindex $Queue 0]
        set Queue [lrange $Queue 1 end]
        
        if { [file isdirectory $Filename] } \
          {
            set Status [catch { set Queue [concat $Queue [glob -nocomplain -- [file join $Filename $pattern]]] } Result]
            continue;
          }
        lappend Output $Filename
      }
    return $Output
  }


}


