# Microsoft Developer Studio Project File - Name="New_EZManage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=New_EZManage - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "New_EZManage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "New_EZManage.mak" CFG="New_EZManage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "New_EZManage - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "New_EZManage - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/New_EZManage", HLFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "New_EZManage - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "\mars" /I "\usb" /I "\fat16" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 zlib.lib unrar.lib ResCvt.lib Version.lib /nologo /subsystem:windows /machine:I386 /libpath:".\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Desc=Genheader
PreLink_Cmds=bin2h arm9_aac.bin	bin2h arm9_47c.bin	bin2h arm9_550.bin	bin2h arm9_aac.bin	bin2h arm7_erase_ez3.bin	bin2h arm7_readrom512_ez3.bin	bin2h arm7_readsaver_ez3.bin	bin2h arm7_verify_ez3.bin	bin2h arm7_writesaver_ez3.bin	bin2h arm9_thumb.bin	bin2h arm9_47c_thumb.bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "New_EZManage - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "\inc" /I "\mars" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlib.lib unrar.lib rescvt.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:".\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Desc=Genheader
PreLink_Cmds=bin2h arm9_aac.bin	bin2h arm9_47c.bin	bin2h arm9_550.bin	bin2h arm9_aac.bin	bin2h arm7_erase_ez3.bin	bin2h arm7_readrom512_ez3.bin	bin2h arm7_readsaver_ez3.bin	bin2h arm7_verify_ez3.bin	bin2h arm7_writesaver_ez3.bin	bin2h arm9_thumb.bin	bin2h arm9_47c_thumb.bin	bin2h arm9_readcard2.bin	bin2h arm7_thumb_erase_ez3.bin	bin2h arm7_thumb_verify_ez3.bin	bin2h arm7_thumb_writesaver_ez3.bin	bin2h arm7_thumb_readsaver_ez3.bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "New_EZManage - Win32 Release"
# Name "New_EZManage - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\usb\AmdEz3FlashBase.cpp
# End Source File
# Begin Source File

SOURCE=.\mars\blowfish.cpp
# End Source File
# Begin Source File

SOURCE=.\bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\chtfileinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DirListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DirTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DiskCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLanguageSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNandFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOSSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\fat16\ECC_Check.cpp
# End Source File
# Begin Source File

SOURCE=.\EfaCheat.cpp
# End Source File
# Begin Source File

SOURCE=.\usb\ezflashbase.cpp
# End Source File
# Begin Source File

SOURCE=.\EZListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\fileini.cpp
# End Source File
# Begin Source File

SOURCE=.\fat16\FileSys.cpp
# End Source File
# Begin Source File

SOURCE=.\usb\FujiEZ3Flash1g.cpp
# End Source File
# Begin Source File

SOURCE=.\gamenameinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GB2Big5.cpp
# End Source File
# Begin Source File

SOURCE=.\goldencheat.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Lz77.cpp
# End Source File
# Begin Source File

SOURCE=.\MyOleDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\New_EZManage.cpp
# End Source File
# Begin Source File

SOURCE=.\New_EZManage.rc
# End Source File
# Begin Source File

SOURCE=.\New_EZManageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PageDisk.cpp
# End Source File
# Begin Source File

SOURCE=.\PageFlash.cpp
# End Source File
# Begin Source File

SOURCE=.\RomEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\rommanager.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollLink.cpp
# End Source File
# Begin Source File

SOURCE=.\SortStringArray.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XCodeFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\usb\AmdEz3FlashBase.h
# End Source File
# Begin Source File

SOURCE=.\big5_10.h
# End Source File
# Begin Source File

SOURCE=.\mars\blowfish.h
# End Source File
# Begin Source File

SOURCE=.\bmp.h
# End Source File
# Begin Source File

SOURCE=.\chtfileinfo.h
# End Source File
# Begin Source File

SOURCE=.\DirListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DirTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DiskCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgLanguageSelect.h
# End Source File
# Begin Source File

SOURCE=.\DlgNandFormat.h
# End Source File
# Begin Source File

SOURCE=.\DlgOSSelect.h
# End Source File
# Begin Source File

SOURCE=.\fat16\ECC_Check.h
# End Source File
# Begin Source File

SOURCE=.\EfaCheat.h
# End Source File
# Begin Source File

SOURCE=.\usb\ezflashbase.h
# End Source File
# Begin Source File

SOURCE=.\EZListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\usb\ezusbsys.h
# End Source File
# Begin Source File

SOURCE=.\fat16\fathead.h
# End Source File
# Begin Source File

SOURCE=.\fileini.h
# End Source File
# Begin Source File

SOURCE=.\fat16\FileSys.h
# End Source File
# Begin Source File

SOURCE=.\usb\FujiEZ3Flash1g.h
# End Source File
# Begin Source File

SOURCE=.\gamenameinfo.h
# End Source File
# Begin Source File

SOURCE=.\GB2Big5.h
# End Source File
# Begin Source File

SOURCE=.\gb_10.h
# End Source File
# Begin Source File

SOURCE=.\goldencheat.h
# End Source File
# Begin Source File

SOURCE=.\instance.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\Lz77.h
# End Source File
# Begin Source File

SOURCE=.\MyOleDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\New_EZManage.h
# End Source File
# Begin Source File

SOURCE=.\New_EZManageDlg.h
# End Source File
# Begin Source File

SOURCE=.\out.h
# End Source File
# Begin Source File

SOURCE=.\PageDisk.h
# End Source File
# Begin Source File

SOURCE=.\PageFlash.h
# End Source File
# Begin Source File

SOURCE=.\usb\patch.h
# End Source File
# Begin Source File

SOURCE=.\ResCvt.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RomEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\rommanager.h
# End Source File
# Begin Source File

SOURCE=.\ScrollLink.h
# End Source File
# Begin Source File

SOURCE=.\SortStringArray.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XCodeFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arm9_47c.bin
# End Source File
# Begin Source File

SOURCE=.\arm9_550.bin
# End Source File
# Begin Source File

SOURCE=.\arm9_aac.bin
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\listctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\New_EZManage.ico
# End Source File
# Begin Source File

SOURCE=.\res\New_EZManage.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\langdata\china-simple.ini"
# End Source File
# Begin Source File

SOURCE=".\dada - final.ssk"
# End Source File
# Begin Source File

SOURCE=.\langdata\English.ini
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
