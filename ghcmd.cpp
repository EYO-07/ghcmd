// -- BEGIN 

// myprogram.cpp
// Usage:
//   myprogram
//   myprogram -h
//   myprogram --help
//   myprogram <command> <hotkey>
//   myprogram <command> <hotkey> <command> <hotkey> 

// -- text marker highlight 
// {Notepad--T;Red:BUG,ISSUE,DEPRECATED,BUG/ISSUE} 
// {Notepad--T;Yellow:TESTING,NOT_TESTED,REVISION,PLACEHOLDER}
// {Notepad--T;Cyan:TODO,WORKING_>>>,<<<_WORKING} 
// {Notepad--T;Silver:SOLVED} 
// {Notepad--T;Silver:Inventory,Logic,Dialetic,Workflow} 
// {Notepad--H;1:silver;2:lightblue}

#include "ghcmd_framework.h"

// Global variable for the current window handle
HWND currentHwnd = nullptr;
HWND consoleHwnd = nullptr;
HWND messageHwnd = nullptr;  // Hidden window for messages
bool running = true; // Flag to control program execution REVISION
std::atomic<bool> keypress_running{true}; // for keypress loop 
// Custom messages
#define WM_RELOAD_HOTKEYS (WM_USER + 1)
#define WM_EXIT_APP (WM_USER + 2)

// forward declarations 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void show_usage(const char* prog);
bool hasKeypress();
void asyncKeyLoop();

// entry point 
int main(int argc, char* argv[]) {
    consoleHwnd = GetConsoleWindow(); 
    std::cout << "[ Global Hotkeys ]" << std::endl;
    // usage messages 
    if (argc < 2) {
        show_usage(argv[0]);
        return 0;
    }
    if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0) {
        show_usage(argv[0]);
        return 0;
    }    
    InitializeKeycodeMap(); 
    // parsing hotkeys
    AssignHotkey(L"EXIT:ctrl|shift+delete");
    for(int i=1;i<argc;i++) {
        std::string temp = argv[i];
        std::wstring arg(temp.begin(), temp.end());
        AssignHotkey(arg);
    }
    // registering the hotkeys 
    std::cout << "... registering hotkeys" << std::endl;
    RegisterHotkeys();
    std::thread keyThread;  // empty thread, not started
    if ( hasKeypress() ) { 
        keyThread = std::thread(asyncKeyLoop); // Start async polling thread
//        hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
//        if (!hHook) {
//            std::cerr << "Failed to install hook\n";
//            return 1;
//        }
    }
    // message loop 
    MSG msg = { 0 };
	std::string input;
    while (keypress_running && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        // Process hotkeys when a WM_HOTKEY message is received
        if ( ProcessHotkeys(msg)==false ) break;
    }
    // exit clean-up
    if (hasKeypress()) std::cout << "... killing keypress thread" << std::endl;
    keypress_running = false;
    if ( hasKeypress() ){ 
        keyThread.join(); // wait for async loop to finish
//        UnhookWindowsHookEx(hHook);
    }
    std::cout << "... unregistering the hotkeys" << std::endl;
    UnregisterHotkeys();
    std::cout << "... set all modified windows to normal" << std::endl;
    SetAllWindowNormal();
	running = false;
//    BlockInput(FALSE);  // release input when not pressed
    return 0;
}
// -- 
void show_usage(const char* prog) {
    std::cout
        << "Usage:\n"
        << "  " << prog << " <arg1> [<arg2> ...] \n"
        << "\nOptions:\n"
        << "  -h, --help    Show this help message\n\n";
    std::cout << "-- Argument Syntax" << std::endl;
    std::cout << "COMMAND:MOD+KEY COMMAND:MOD1|MOD2+KEY COMMAND:MOD1|MOD2|MOD3+KEY ... " << std::endl;
    std::cout << "-- List of Available Commands" << std::endl;
    std::cout << "WINDOW_UP" << "\n" ;
    std::cout << "WINDOW_DOWN" << "\n" ;
    std::cout << "WINDOW_LEFT" << "\n" ;
    std::cout << "WINDOW_RIGHT" << "\n" ;
    std::cout << "WINDOW_INCREASE" << "\n" ;
    std::cout << "WINDOW_DECREASE" << "\n" ;
    std::cout << "WINDOW_VERTICAL" << "\n" ;
    std::cout << "WINDOW_HORIZONTAL" << "\n" ;
    std::cout << "TOGGLE_TITLEBAR" << "\n" ;
    std::cout << "SELECT_WORKING_WINDOW used to bring_to_top and bring_to_last" << "\n" ;
    std::cout << "BRING_TO_TOP" << "\n" ;
    std::cout << "BRING_TO_LAST" << "\n" ;
    std::cout << "EXIT:ctrl+delete" << " --> ends the global hotkeys program\n" ;
}
void asyncKeyLoop() {
    while (keypress_running) {
        ProcessKeypress();
        Sleep(1); // avoid busy loop
    }
}
bool hasKeypress() {
    return !command_keypress_map.empty();
}







// -- END 