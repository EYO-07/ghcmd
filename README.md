# ghcmd
Windows global hotkeys command-line utility. 

## Installation/Compilation 
1. go to https://winlibs.com/ and download the mingw gnu compiler suited for your computer. This [Windows 64](https://github.com/brechtsanders/winlibs_mingw/releases/download/15.2.0posix-13.0.0-msvcrt-r1/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64msvcrt-13.0.0-r1.zip) is a portable window version of gcc for window 64 machines.
2. extract somewhere like `C:\devtools\`
3. download this repository .zip and extract somewhere
4. open the file `session.bat` and modify the value of variable GCCDIR= to the gcc.exe location

```bash
@echo off

REM change the path C:\devtools\mingw64\bin on GCCDIR= to the path where you find gcc.exe 
set "GCCDIR=C:\devtools\mingw64\bin"

REM =================================================================================

REM Define temporary macros for this cmd.exe session
echo ... loading macros 
```

5. open cmd.exe or powershell on the folder where you extracted this repository
6. on cmd.exe or powershell execute the session.bat
7. now run the command `build` to compile
8. the ghcmd.exe will be located on `bin` folder

## Usage 
```bash

```
