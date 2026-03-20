@REM @echo off
if "%~1"=="" goto usage
if "%~2"=="" goto usage
if "%~3"=="" goto usage
if "%~4"=="" goto usage
if not "%~5"=="" goto usage
goto main

:usage
echo Args: ^<WINDEPLOYQT^> ^<BUILD_EXE^> ^<DEPLOY_ROOT^> ^<DEPLOY_EXE^>
exit /b 1

:main
powershell -command "Add-Type -AssemblyName 'Microsoft.VisualBasic'; [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%~3', 'OnlyErrorDialogs', 'SendToRecycleBin')"
md "%~3"

xcopy /y "%~2" "%~4"

"%~1" --no-translations --no-system-d3d-compiler --no-opengl-sw -no-svg -no-network "%~4"

powershell -command "Add-Type -AssemblyName 'Microsoft.VisualBasic'; [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%~3\styles', 'OnlyErrorDialogs', 'SendToRecycleBin'); [Microsoft.VisualBasic.FileIO.FileSystem]::DeleteDirectory('%~3\imageformats', 'OnlyErrorDialogs', 'SendToRecycleBin')"
explorer.exe /select,"%~4"

exit 0