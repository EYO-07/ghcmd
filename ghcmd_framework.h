// -- BEGIN

// -- text marker highlight 
// {Notepad--T;Red:BUG,ISSUE,DEPRECATED,BUG/ISSUE} 
// {Notepad--T;Yellow:TESTING,NOT_TESTED,REVISION}
// {Notepad--T;Cyan:TODO,WORKING_>>>,<<<_WORKING} 
// {Notepad--T;Silver:SOLVED} 
// {Notepad--H;1:silver;2:lightblue}

#ifndef ghcmd_framework_H
#define ghcmd_framework_H

#include <windows.h>        // For Windows API functions (MoveWindow, RegisterHotKey, etc.)
#include <string>           // For std::wstring
#include <map>              // For std::map
#include <fstream>          // For file operations (hotkeys.sav)
#include <regex>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <cwctype> 
#include <thread> 
#include <mutex>
#include <vector>
#include <cstring>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <atomic>

// Global Variables Declaration
extern std::map<std::wstring, std::wstring> command_map;        // Maps command (e.g., "UP") to hotkey expression (e.g., "win|alt+up")
extern std::map<std::wstring, UINT> keycode_map;                // Maps key strings (e.g., "up") to Windows API virtual keycodes (e.g., VK_UP)
extern int hotkeys_counter;                                     // Counter for registered hotkeys, initialized to 0
extern std::map<int, std::wstring> hotkey_index_to_command;     // Maps hotkey ID (counter) to command string
extern HWND consoleHwnd;
extern std::mutex hotkeyMutex;  // Mutex for thread safety
struct WindowState {
    LONG style;
    LONG exStyle;
    RECT rect;
    WINDOWPLACEMENT placement;
};
extern std::unordered_map<HWND, WindowState> g_windowStates;
extern std::atomic<bool> keypress_running;
extern std::unordered_map<std::wstring, UINT> command_keypress_map;

// Function Declarations
void MoveWindow(HWND hwnd, int dx, int dy);
void InitializeKeycodeMap();
void RegisterHotkeys();
void UnregisterHotkeys();
bool ProcessHotkeys(MSG& msg);
void RatioResizeWindow(HWND hwnd, double ratio); //
void VerticalResize(HWND hwnd); // 
void HorizontalResize(HWND hwnd); //
bool ExistsCommand(const std::wstring& command);
void ExecuteCMD(const std::wstring& command); 
std::wstring trim(const std::wstring& str);
void AssignHotkey(std::wstring line);
void MoveMouse(int dx, int dy);
void MouseClick_LBUTTON();
void MouseClick_RBUTTON();
void MouseClick_MBUTTON();
void Mouse_ScrollUp();
void Mouse_ScrollDown();
void SendMouseInput(DWORD flags, DWORD data = 0);
void SetActiveWindowBorderless(HWND hwnd);
void SetActiveWindowNormal(HWND hwnd);
void SetAllWindowNormal();
void VerticalResize(HWND hwnd, int width_asp, int height_asp); // height_p width_p are used for aspect ratio 
void SelectWorkingWindow(HWND hwnd);
void BringToTopZOrder();
void BringTOLastZOrder();
bool is_key_pressed(const int key_code); 
bool ExecuteCommand(std::wstring command, HWND currentHwnd);
bool ProcessKeypress();
void RegisterGlobalHotkey(UINT sys_keys, UINT key, std::wstring command, std::wstring hotkeyExpr);
void MouseToggleSpeed();

#endif // ghcmd_framework_H

