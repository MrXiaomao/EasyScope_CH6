# Microsoft Developer Studio Project File - Name="mcbc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mcbc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MCA4K.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MCA4K.mak" CFG="mcbc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mcbc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "mcbc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mcbc - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "mcbc - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mcbc - Win32 Release"
# Name "mcbc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\calInv.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplaySpm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAmp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEnergyKedu.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGuass.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMcsSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPaperV.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRoIDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRS232.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetC51.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetRoi.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStrip.cpp
# End Source File
# Begin Source File

SOURCE=.\FindPeak.cpp
# End Source File
# Begin Source File

SOURCE=.\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\GauseFit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mcbc.cpp
# End Source File
# Begin Source File

SOURCE=.\mcbc.rc
# End Source File
# Begin Source File

SOURCE=.\mcbcDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\mcbcView.cpp
# End Source File
# Begin Source File

SOURCE=.\pid.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\DialogTime.h
# End Source File
# Begin Source File

SOURCE=.\DisplaySpm.h
# End Source File
# Begin Source File

SOURCE=.\DlgAmp.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoSetup.h
# End Source File
# Begin Source File

SOURCE=.\DlgEnergyKedu.h
# End Source File
# Begin Source File

SOURCE=.\DlgGuass.h
# End Source File
# Begin Source File

SOURCE=.\DlgMcsSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgPaperV.h
# End Source File
# Begin Source File

SOURCE=.\DlgRoIDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgRS232.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetC51.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetRoi.h
# End Source File
# Begin Source File

SOURCE=.\DlgStrip.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mcbc.h
# End Source File
# Begin Source File

SOURCE=.\mcbcDoc.h
# End Source File
# Begin Source File

SOURCE=.\mcbcView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\mcbc.ico
# End Source File
# Begin Source File

SOURCE=.\res\mcbc.rc2
# End Source File
# Begin Source File

SOURCE=.\res\mcbcDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\aa.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\HTMLHELP.LIB
# End Source File
# End Target
# End Project
