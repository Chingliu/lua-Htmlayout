# Microsoft Developer Studio Project File - Name="libmfc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libmfc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libmfc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libmfc.mak" CFG="libmfc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libmfc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libmfc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libmfc - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../" /I "../../../lua" /I "../../../htmlayout" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\libmfc.lib"

!ELSEIF  "$(CFG)" == "libmfc - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libmfc___Win32_Debug"
# PROP BASE Intermediate_Dir "libmfc___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../" /I "../../../lua" /I "../../../htmlayout" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\libmfc_d.lib"

!ENDIF 

# Begin Target

# Name "libmfc - Win32 Release"
# Name "libmfc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\HtmlayoutWDlg.cpp

!IF  "$(CFG)" == "libmfc - Win32 Release"

!ELSEIF  "$(CFG)" == "libmfc - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LuaGraphics.cpp

!IF  "$(CFG)" == "libmfc - Win32 Release"

!ELSEIF  "$(CFG)" == "libmfc - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LuaHtmlayoutElment.cpp

!IF  "$(CFG)" == "libmfc - Win32 Release"

!ELSEIF  "$(CFG)" == "libmfc - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\mfc.cpp

!IF  "$(CFG)" == "libmfc - Win32 Release"

!ELSEIF  "$(CFG)" == "libmfc - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\stdafx.cpp

!IF  "$(CFG)" == "libmfc - Win32 Release"

!ELSEIF  "$(CFG)" == "libmfc - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\behavior_lua.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ExtendWidget.h
# End Source File
# Begin Source File

SOURCE=..\..\..\exwidget.h
# End Source File
# Begin Source File

SOURCE=..\..\..\HtmlayoutWDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\htmlayoutwrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\LuaGraphics.h
# End Source File
# Begin Source File

SOURCE=..\..\..\LuaHtmlayoutElment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\mfc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\targetver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\util.h
# End Source File
# End Group
# End Target
# End Project
