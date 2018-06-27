@echo OFF

set BAT=%0
set sCMD=%1

REM echo cmd %sCMD%
if "%sCMD%" NEQ "" (
  if "%sCMD%" NEQ "build" (
    call :Clean
  ) else (
    call :Build
  )
) else (
  call :Help
)
exit /b

:Help
echo =====%BAT% usage===============
echo  %BAT% build : build x86/x64 
echo  %BAT%  : clean build
echo ===================================
exit /b

:Build
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars32.bat"
MSBuild HWDetect.sln /t:Rebuild /p:Configuration=Release;Platform=x86

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"
MSBuild HWDetect.sln /t:Rebuild /p:Configuration=Release;Platform=x64

call "c:\Program Files (x86)\Inno Setup 5\compil32.exe" /cc installer\hwdetect.iss
exit /b

:Clean
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars32.bat"
MSBuild wlan.sln /t:clean /p:Configuration=Release;Platform=x86

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvars64.bat"
MSBuild wlan.sln /t:clean /p:Configuration=Release;Platform=x64

exit /b
