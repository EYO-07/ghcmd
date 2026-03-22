// -- BEGIN 
// -- text marker highlight 
// {Notepad--T;Red:BUG,ISSUE,DEPRECATED,BUG/ISSUE} 
// {Notepad--T;Yellow:TESTING,NOT_TESTED,REVISION,PLACEHOLDER}
// {Notepad--T;Cyan:TODO,WORKING_>>>,<<<_WORKING} 
// {Notepad--T;Silver:SOLVED} 
// {Notepad--T;Silver:Inventory,Logic,Dialetic,Workflow} 
// {Notepad--T;magenta:imports,variables,functions} 
// {Notepad--H;1:silver;2:lightblue}

// -- search tokens 
// {Notepad--T;red:}
// {Notepad--S:}

// ghcmd.exe
// Usage:
//   ghcmd
//   ghcmd -h
//   ghcmd --help
//   ghcmd <command>:<hotkey> [ <command>:<hotkey> ... ]

// imports
#include "ghcmd_framework.h" // every include is shared and included on ghcmd_framework.h 

// imports | variables 
#define WM_RELOAD_HOTKEYS (WM_USER + 1) // REVISION
#define WM_EXIT_APP (WM_USER + 2) // REVISION 
HWND currentHwnd = nullptr; // current window handle, is the target to hotkey actions 
HWND consoleHwnd = nullptr; // console handle
HWND messageHwnd = nullptr; // Hidden window for messages
bool running = true; // Flag to control program execution REVISION
std::atomic<bool> keypress_running{true}; // for keypress loop 

// imports | variables | functions 
// forward declarations 
// LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
void show_usage(const char* prog);
bool hasKeypress();
void asyncKeyLoop();
/* Logic [ entry point  ]
M := main 
K := asyncKeyLoop, keypress thread
SKT := start keypress asyncKeyLoop thread
PRKH := parsing and registering keypress and hotkeys
-> M() || PRKH | SKT | & message loop | clean-up 
1. SKT is faster than hotkeys, necessary for mouse movement inputs 
2. message loop is the message loop for windows.h applications 
-> M() || ... | & message loop || TranslateMessage() | DispatchMessage() | % ( !ProcessHotkeys() ) || break 
-> K() || & (keypress_running==true) || ProcessKeypress() | Sleep(1) 
*/
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
    // AssignHotkey(L"EXIT:ctrl|alt|shift+delete"); 
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
    }
    // message loop 
    MSG msg = { 0 };
	std::string input;
    // 1. GetMessage with nullptr as window handle catches messages from all active windows, so the "global" hotkeys works
    while (keypress_running && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if ( ProcessHotkeys(msg)==false ) break; // Process hotkeys when a WM_HOTKEY message is received
    }
    // exit clean-up
    if (hasKeypress()) std::cout << "... killing keypress thread" << std::endl;
    keypress_running = false;
    if ( hasKeypress() ){ 
        keyThread.join(); // wait for async loop to finish
    }
    std::cout << "... unregistering the hotkeys" << std::endl;
    UnregisterHotkeys();
    std::cout << "... set all modified windows to normal" << std::endl;
    SetAllWindowNormal();
	running = false; // REVISION
    return 0;
}
// implementations
void show_usage(const char* prog) {
    std::cout
        << "Usage:\n"
        << "  " << prog << " <arg1> [<arg2> ...] \n"
        << "\nOptions:\n"
        << "  -h, --help    Show this help message\n\n";
    std::cout << "-- Argument Syntax" << std::endl;
    std::cout << "COMMAND:MOD+KEY COMMAND:MOD1|MOD2+KEY COMMAND:MOD1|MOD2|MOD3+KEY ... " << std::endl;
    std::cout << "-- List of Available Commands" << std::endl;
    std::cout << "WINDOW_UP move window" << "\n" ;
    std::cout << "WINDOW_DOWN move window" << "\n" ;
    std::cout << "WINDOW_LEFT move window" << "\n" ;
    std::cout << "WINDOW_RIGHT move window" << "\n" ;
    std::cout << "WINDOW_INCREASE" << "\n" ;
    std::cout << "WINDOW_DECREASE" << "\n" ;
    std::cout << "WINDOW_VERTICAL" << "\n" ;
    std::cout << "WINDOW_HORIZONTAL" << "\n" ;
    std::cout << "TOGGLE_TITLEBAR compact mode" << "\n" ;
    std::cout << "SELECT_WORKING_WINDOW used to bring_to_top and bring_to_last" << "\n" ;
    std::cout << "BRING_TO_TOP change z-order" << "\n" ;
    std::cout << "BRING_TO_LAST change z-order" << "\n" ;
    std::cout << "MOUSE_LBUTTON left click" << "\n" ;    
//    std::cout << "MOUSE_TOGGLE_LBUTTON toggle hold down left button" << "\n" ;
    std::cout << "MOUSE_MBUTTON click middle button" << "\n" ;
    std::cout << "MOUSE_RBUTTON right click" << "\n" ;
    std::cout << "MOUSE_SCROLLUP" << "\n" ;
    std::cout << "MOUSE_SCROLLDOWN" << "\n" ;
    std::cout << "MOUSE_TOGGLE_SPEED change movement speed" << "\n" ;
    std::cout << "MOUSE_UP move mouse" << "\n" ;
    std::cout << "MOUSE_DOWN move mouse" << "\n" ;
    std::cout << "MOUSE_LEFT move mouse" << "\n" ;
    std::cout << "MOUSE_RIGHT move mouse" << "\n" ;    
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