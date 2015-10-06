@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by WINPROM.HPJ. >"hlp\winprom.hm"
echo. >>"hlp\winprom.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\winprom.hm"
"c:\Program Files\Microsoft Visual Studio\Common\Tools\makehm" ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\winprom.hm"
echo. >>"hlp\winprom.hm"
echo // Prompts (IDP_*) >>"hlp\winprom.hm"
"c:\Program Files\Microsoft Visual Studio\Common\Tools\makehm" IDP_,HIDP_,0x30000 resource.h >>"hlp\winprom.hm"
echo. >>"hlp\winprom.hm"
echo // Resources (IDR_*) >>"hlp\winprom.hm"
"c:\Program Files\Microsoft Visual Studio\Common\Tools\makehm" IDR_,HIDR_,0x20000 resource.h >>"hlp\winprom.hm"
echo. >>"hlp\winprom.hm"
echo // Dialogs (IDD_*) >>"hlp\winprom.hm"
"c:\Program Files\Microsoft Visual Studio\Common\Tools\makehm" IDD_,HIDD_,0x20000 resource.h >>"hlp\winprom.hm"
echo. >>"hlp\winprom.hm"
echo // Frame Controls (IDW_*) >>"hlp\winprom.hm"
"c:\Program Files\Microsoft Visual Studio\Common\Tools\makehm" IDW_,HIDW_,0x50000 resource.h >>"hlp\winprom.hm"
REM -- Make help for Project WINPROM


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\winprom.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\winprom.hlp" goto :Error
if not exist "hlp\winprom.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\winprom.hlp" Debug
if exist Debug\nul copy "hlp\winprom.cnt" Debug
if exist Release\nul copy "hlp\winprom.hlp" Release
if exist Release\nul copy "hlp\winprom.cnt" Release
echo.
goto :done

:Error
echo hlp\winprom.hpj(1) : error: Problem encountered creating help file

:done
echo.
