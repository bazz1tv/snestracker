# Microsoft Developer Studio Project File - Name="jdkmidi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jdkmidi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jdkmidi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jdkmidi.mak" CFG="jdkmidi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jdkmidi - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jdkmidi - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jdkmidi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "jdkmidi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "jdkmidi - Win32 Release"
# Name "jdkmidi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\jdkmidi_advancedsequencer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_driver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_driverdump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_driverwin32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_edittrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_fileread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_filereadmultitrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_fileshow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_filewrite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_filewritemultitrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_keysig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_manager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_midi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_msg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_multitrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_process.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_queue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_sequencer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_showcontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_showcontrolhandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_smpte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_sysex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_tempo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_tick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\jdkmidi_track.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\jdkmidi\advancedsequencer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\driver.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\driverdump.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\driverwin32.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\edittrack.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\file.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\fileread.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\filereadmultitrack.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\fileshow.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\filewrite.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\filewritemultitrack.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\keysig.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\manager.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\midi.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\msg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\multitrack.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\parser.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\process.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\queue.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\sequencer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\showcontrol.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\showcontrolhandler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\smpte.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\song.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\sysex.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\tempo.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\tick.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\track.h
# End Source File
# Begin Source File

SOURCE=..\..\include\jdkmidi\world.h
# End Source File
# End Group
# End Target
# End Project
