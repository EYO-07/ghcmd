@echo off

REM change the path C:\devtools\mingw64\bin on GCCDIR= to the path where you find gcc.exe 
set "GCCDIR=C:\devtools\mingw64\bin"

REM =================================================================================

REM Define temporary macros for this cmd.exe session
echo ... loading macros 
REM SYNTAX BASIC: doskey MACRONAME=MACRO
REM SYNTAX VARARG: doskey py=python $*
if not exist "%cd%\bin" mkdir "%cd%\bin"
doskey build=g++ -std=c++20 --verbose ghcmd.cpp ghcmd_framework.cpp -o bin\ghcmd.exe -static
echo use macro 'build' to build global hotkeys command-line utility.
echo Usage: build 
echo Result: bin\ghcmd.exe
echo ... macros loaded for this session.

REM =================================================================================

echo ... loading functional macros 
REM Ensure bin folder exists (relative to current directory)
REM Macro syntax: build SOURCE EXENAME
REM $1 = first argument (source file), $2 = second argument (executable name)
echo ... functional macros loaded for this session.

REM =================================================================================

REM path for this session 
echo ... changing PATH 
call :add_to_path "%cd%\bin"
call :add_to_path "%GCCDIR%"
echo ... PATH updated for this session.
echo ... Close this window to clear them.
goto :eof

REM =================================================================================
REM functions

:add_to_path
set "P=%~1"
REM Normalize: remove trailing backslash (optional but safer)
if "%P:~-1%"=="\" set "P=%P:~0,-1%"
REM echo %PATH% | find /I ";%P%;" >nul
echo ;%PATH%; | find /I ";%P%;" >nul
if errorlevel 1 (
    set "PATH=%PATH%;%P%"
    echo Added: %P%
) else (
    echo Already in PATH: %P%
)
exit /b


