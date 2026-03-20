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
Below is an example to control any window position using the key combination `win+alt+arrow_key`, you can also increase it's size with `win+alt+pgup` and decrease `win+alt+pgdown` and change it's orientation with `win+alt+end` and `win+alt+home`
in cmd.exe
```bash
ghcmd WINDOW_UP:win|alt+up WINDOW_DOWN:win|alt+down WINDOW_LEFT:win|alt+left WINDOW_RIGHT:win|alt+right WINDOW_INCREASE:win|alt+pgup WINDOW_DECREASE:win|alt+pgdown WINDOW_VERTICAL:win|alt+end WINDOW_HORIZONTAL:win|alt+home
```
in powershell:
```powershell
.\ghcmd.exe WINDOW_UP:win|alt+up WINDOW_DOWN:win|alt+down WINDOW_LEFT:win|alt+left WINDOW_RIGHT:win|alt+right WINDOW_INCREASE:win|alt+pgup WINDOW_DECREASE:win|alt+pgdown WINDOW_VERTICAL:win|alt+end WINDOW_HORIZONTAL:win|alt+home
```
### Available commands:
1. WINDOW_UP, WINDOW_DOWN, WINDOW_LEFT, WINDOW_RIGHT
2. WINDOW_INCREASE, WINDOW_DECREASE
3. WINDOW_VERTICAL, WINDOW_HORIZONTAL
4. MOUSE_UP, MOUSE_DOWN, MOUSE_LEFT, MOUSE_RIGHT, MOUSE_RBUTTON, MOUSE_LBUTTON, MOUSE_MBUTTON
5. MOUSE_SCROLLUP, MOUSE_SCROLLDOWN, MOUSE_TOGGLE_SPEED
6. TOGGLE_TITLEBAR
7. SELECT_WORKING_WINDOW, BRING_TO_TOP, BRINT_TO_LAST

It's under development, if find any issues please report.
