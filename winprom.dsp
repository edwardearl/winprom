# Microsoft Developer Studio Project File - Name="winprom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=winprom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "winprom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "winprom.mak" CFG="winprom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winprom - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "winprom - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "winprom - Win32 Distribute" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=C:\Program Files\Microsoft Visual Studio\VC98\bin\cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winprom - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "administrator" /D "WIN32" /D "_WINDOWS" /D "io_support" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "winprom - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_AFXDLL" /D "administrator" /D "WIN32" /D "_WINDOWS" /D "io_support" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "winprom - Win32 Distribute"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "winprom___Win32_Distribute"
# PROP BASE Intermediate_Dir "winprom___Win32_Distribute"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Distribute"
# PROP Intermediate_Dir "Distribute"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "io_support" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "winprom - Win32 Release"
# Name "winprom - Win32 Debug"
# Name "winprom - Win32 Distribute"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnalyzeCtl_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Assoc_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgdTask_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BasinSadlListView.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseFormat_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Contour_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Crop_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CutoffOnly_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CycleListView.cpp
# End Source File
# Begin Source File

SOURCE=.\CycleSadl_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Data_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataAlign_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Database.cpp
# End Source File
# Begin Source File

SOURCE=.\DataFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DataImport_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataInfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListSummary_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataLocationFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DefineFlat_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Domain.cpp
# End Source File
# Begin Source File

SOURCE=.\Domain_anal.cpp
# End Source File
# Begin Source File

SOURCE=.\DomainView_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DomapInfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ElevColor_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ElevConflict_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ElevMapInfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ElevRect_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorAnal_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorParam_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureEdit_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureFmt_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureListSummary_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureListView.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureTypeFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureView_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Fields_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeHeight_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeHeightParam_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericListView.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphView.cpp
# End Source File
# Begin Source File

SOURCE=.\Grid.cpp
# End Source File
# Begin Source File

SOURCE=.\GridPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\GridPointFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridView_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupListView.cpp
# End Source File
# Begin Source File

SOURCE=.\IdentifyFeature_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Imparam_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImpOut_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ListSadl_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LocationFmt_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LPRtreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapView.cpp
# End Source File
# Begin Source File

SOURCE=.\NoiseSum_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParentListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ParentType_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PeakListView.cpp
# End Source File
# Begin Source File

SOURCE=.\PeakNbrListView.cpp
# End Source File
# Begin Source File

SOURCE=.\Point.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintMap_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileParam_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileView.cpp
# End Source File
# Begin Source File

SOURCE=.\PromCutoff_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PromDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PromFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PromRulerBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PromSadl_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PromSadlListView.cpp
# End Source File
# Begin Source File

SOURCE=.\Purge_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PurgeDomain_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PurgeNoise_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Relief_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RidgeView_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RQFinfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RulerBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RunoffList_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RunoffListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RunoffNbr_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RunoffNbrListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SaddleListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SaddleWalkInvView.cpp
# End Source File
# Begin Source File

SOURCE=.\Scale_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptCtl_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetLocation_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetProfile_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Shoreline_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StatOptions_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SWI_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Transform_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TransformTree_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TransPal_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeInfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreePaste_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeSelectFilter_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WalkListView.cpp
# End Source File
# Begin Source File

SOURCE=.\winprom.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\winprom.hpj
# End Source File
# Begin Source File

SOURCE=.\winprom.rc
# End Source File
# Begin Source File

SOURCE=.\WSinfo_dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XferTree_dlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnalyzeCtl_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Area_info.h
# End Source File
# Begin Source File

SOURCE=.\BrowseFormat_dlg.h
# End Source File
# Begin Source File

SOURCE=.\BrowserFrame.h
# End Source File
# Begin Source File

SOURCE=.\Contour_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Crop_dlg.h
# End Source File
# Begin Source File

SOURCE=.\CutoffOnly_dlg.h
# End Source File
# Begin Source File

SOURCE=.\CycleSadl_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Data_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DataAlign_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Database.h
# End Source File
# Begin Source File

SOURCE=.\DataFilter_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DataImport_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DataInfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DataListSummary_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DataListView.h
# End Source File
# Begin Source File

SOURCE=.\DataLocationFilter_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DefineFlat_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DomainView_dlg.h
# End Source File
# Begin Source File

SOURCE=.\DomapInfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\EditedList_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ElevColor_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ElevConflict_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ElevMapInfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ErrorAnal_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ErrorParam_dlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureEdit_dlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureFilter_dlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureFmt_dlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureListSummary_dlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureListView.h
# End Source File
# Begin Source File

SOURCE=.\FeatureTypeFilter_dlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureView_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Fields_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Format.h
# End Source File
# Begin Source File

SOURCE=.\FreeHeightParam_dlg.h
# End Source File
# Begin Source File

SOURCE=.\GenericListView.h
# End Source File
# Begin Source File

SOURCE=.\GridPointFilter_dlg.h
# End Source File
# Begin Source File

SOURCE=.\GridView_dlg.h
# End Source File
# Begin Source File

SOURCE=.\GroupListView.h
# End Source File
# Begin Source File

SOURCE=.\Imparam_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ImpOut_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ListFilter_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ListFrame.h
# End Source File
# Begin Source File

SOURCE=.\MapInfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\MapView.h
# End Source File
# Begin Source File

SOURCE=.\NoiseSum_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ParentListView.h
# End Source File
# Begin Source File

SOURCE=.\ParentType_dlg.h
# End Source File
# Begin Source File

SOURCE=.\PeakListView.h
# End Source File
# Begin Source File

SOURCE=.\PrintMap_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ProfileParam_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ProfileView.h
# End Source File
# Begin Source File

SOURCE=.\PromCutoff_dlg.h
# End Source File
# Begin Source File

SOURCE=.\PromDoc.h
# End Source File
# Begin Source File

SOURCE=.\PromFrame.h
# End Source File
# Begin Source File

SOURCE=.\PromSadlListView.h
# End Source File
# Begin Source File

SOURCE=.\PurgeDomain_dlg.h
# End Source File
# Begin Source File

SOURCE=.\PurgeNoise_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Relief_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RidgeView_dlg.h
# End Source File
# Begin Source File

SOURCE=.\RQFinfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\RunoffList_dlg.h
# End Source File
# Begin Source File

SOURCE=.\RunoffListView.h
# End Source File
# Begin Source File

SOURCE=.\RunoffNbr_dlg.h
# End Source File
# Begin Source File

SOURCE=.\RunoffNbrListView.h
# End Source File
# Begin Source File

SOURCE=.\SaddleListView.h
# End Source File
# Begin Source File

SOURCE=.\SaddleWalkInvView.h
# End Source File
# Begin Source File

SOURCE=.\Scale_dlg.h
# End Source File
# Begin Source File

SOURCE=.\ScriptCtl_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Shoreline_dlg.h
# End Source File
# Begin Source File

SOURCE=.\StatOptions_dlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SWI_dlg.h
# End Source File
# Begin Source File

SOURCE=.\Transform_dlg.h
# End Source File
# Begin Source File

SOURCE=.\TransformTree_dlg.h
# End Source File
# Begin Source File

SOURCE=.\TransPal_dlg.h
# End Source File
# Begin Source File

SOURCE=.\TreeFrame.h
# End Source File
# Begin Source File

SOURCE=.\TreeInfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\TreePaste_dlg.h
# End Source File
# Begin Source File

SOURCE=.\WalkListView.h
# End Source File
# Begin Source File

SOURCE=.\winprom.h
# End Source File
# Begin Source File

SOURCE=.\WSinfo_dlg.h
# End Source File
# Begin Source File

SOURCE=.\XferTree_dlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\basin.ico
# End Source File
# Begin Source File

SOURCE=.\res\Basin_saddle.ico
# End Source File
# Begin Source File

SOURCE=.\res\basinsadl_list.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\browse.ico
# End Source File
# Begin Source File

SOURCE=.\res\browser.bmp
# End Source File
# Begin Source File

SOURCE=.\res\csv.bmp
# End Source File
# Begin Source File

SOURCE=.\res\csv.ico
# End Source File
# Begin Source File

SOURCE=.\res\cycle.ico
# End Source File
# Begin Source File

SOURCE=.\res\DataList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dlpr.ico
# End Source File
# Begin Source File

SOURCE=.\res\edge.ico
# End Source File
# Begin Source File

SOURCE=.\res\Elev.ico
# End Source File
# Begin Source File

SOURCE=.\res\FeatureList.bmp
# End Source File
# Begin Source File

SOURCE=.\res\file_tb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graph_drain.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graphtype.bmp
# End Source File
# Begin Source File

SOURCE=.\res\html.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_data.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_mai.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\LPRtree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mixed.ico
# End Source File
# Begin Source File

SOURCE=.\res\mixed_drain.ico
# End Source File
# Begin Source File

SOURCE=.\res\noedit.ico
# End Source File
# Begin Source File

SOURCE=.\res\noedit_edge.ico
# End Source File
# Begin Source File

SOURCE=.\res\offmap.ico
# End Source File
# Begin Source File

SOURCE=.\res\Peak.ico
# End Source File
# Begin Source File

SOURCE=.\res\Prom.ico
# End Source File
# Begin Source File

SOURCE=.\res\PromIslandTree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\promsadl_list.ico
# End Source File
# Begin Source File

SOURCE=.\res\runoff.ico
# End Source File
# Begin Source File

SOURCE=.\res\runoff1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Saddle.ico
# End Source File
# Begin Source File

SOURCE=.\res\sbro.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\text.bmp
# End Source File
# Begin Source File

SOURCE=.\res\text.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\winprom.rc2
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\winprom.cnt
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\winprom.reg
# End Source File
# End Target
# End Project
