# Microsoft Developer Studio Project File - Name="mandi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=mandi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mandi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mandi.mak" CFG="mandi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mandi - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mandi - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mandi - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "mandi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /I "c:\vpat\joints2" /I "c:\\" /I "c:\xercesc\\" /I "c:\xercesc\dom" /I "c:\inventor\build\inventor\libsoxt\include" /I "c:\inventor\install\include\\" /I "c:\inventor\install\include\inventor" /I "c:\qt\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "SOQT_DLL" /D "COIN_NOT_DLL" /D "DQT_DLL" /D "QT_DLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 inventor.lib xerces-c_2.lib qutil.lib qtmain.lib qt-mt230nc.lib soqt.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"c:\inventor\install\lib" /libpath:"C:\xerces-c2_4_0-windows_nt-msvc_60\lib" /libpath:"c:\coin3d\lib" /libpath:"C:\Qt\lib" /libpath:"C:\inventor\install\lib"

!ENDIF 

# Begin Target

# Name "mandi - Win32 Release"
# Name "mandi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbezier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbiaxialjoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbody.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcolor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcurve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcylinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpdirectionallight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpdof.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpedge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpgraphicobj.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpinventorview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpjoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpjointmotion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpjointsimulator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vplight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpmatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpmodifier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpmovement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpplanejoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppoint2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppoint3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppointlight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppolyaxialjoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpscene.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpsphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpspotlight.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpuniaxialjoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpvector3d.cpp
# End Source File
# Begin Source File

SOURCE=..\joints2\vpvertex3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpxml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbezier.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbiaxialjoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbody.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcamera.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcolor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcone.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcurve.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpcylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpdirectionallight.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpdof.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpedge.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpgraphicobj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpinventorview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpjoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpjointmotion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpjointsimulator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vplight.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpmatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpmodifier.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpmovement.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpplanejoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppoint2d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppoint3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppointlight.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vppolyaxialjoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpscene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpsphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpspotlight.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpuniaxialjoint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpvector3d.h
# End Source File
# Begin Source File

SOURCE=..\joints2\vpvertex3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\vpat\joints2\vpxml.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
