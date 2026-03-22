@REM @echo off
if "%~1"=="" goto usage
if "%~2"=="" goto usage
if "%~3"=="" goto usage
if not "%~4"=="" goto usage

set "WINDEPLOYQT=%~1"
set "BUILD_EXE=%~2"
set "DEPLOY_EXE=%~3"
set "DEPLOY_ROOT=%~dp3"
set "DEPLOY_ROOT=%DEPLOY_ROOT:~0,-1%"

for /f "usebackq delims=" %%i in (`powershell -c "[Environment]::GetFolderPath('Desktop')"`) do set "DESKTOP_PATH=%%i"

goto main



:usage
echo Args: ^<WINDEPLOYQT^> ^<BUILD_EXE^> ^<DEPLOY_EXE^>
exit /b 1



:main
if exist "%DEPLOY_ROOT%" (
    powershell -c "Add-Type -AssemblyName 'Microsoft.VisualBasic'; [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%DEPLOY_ROOT%', 'OnlyErrorDialogs', 'SendToRecycleBin')"
)
md "%DEPLOY_ROOT%"

echo F | xcopy /y "%BUILD_EXE%" "%DEPLOY_EXE%"

"%WINDEPLOYQT%" --no-translations --no-system-d3d-compiler --no-opengl-sw -no-svg -no-network "%DEPLOY_EXE%"

if exist "%DEPLOY_ROOT%\styles" (
    powershell -c "Add-Type -AssemblyName 'Microsoft.VisualBasic'; [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%DEPLOY_ROOT%\styles', 'OnlyErrorDialogs', 'SendToRecycleBin')"
)
if exist "%DEPLOY_ROOT%\imageformats" (
    powershell -c "Add-Type -AssemblyName 'Microsoft.VisualBasic'; [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%DEPLOY_ROOT%\imageformats', 'OnlyErrorDialogs', 'SendToRecycleBin')"
)

set "ARCHIVE=%DESKTOP_PATH%\%~nx3.7z"
if exist "%ARCHIVE%" (
    powershell -c "Add-Type -AssemblyName 'Microsoft.VisualBasic'; [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%ARCHIVE%', 'OnlyErrorDialogs', 'SendToRecycleBin')"
)

bz c -fmt:7z -l:9 -y "%ARCHIVE%" "%DEPLOY_ROOT%"



exit 0