// -- BEGIN
// -- text marker highlight - tags example 
// {Notepad--T;red:ISSUE;yellow:DEPRECATED,REVISION,TESTING,PLACEHOLDER;silver:FIXED;cyan:TODO,>>>,<<<} 
// {Notepad--T;magenta:imports,variables,functions} 
// {Notepad--H;1:silver;2:lightblue} 
// -- search tokens 
// {Notepad--T;red:}
// {Notepad--S:}

// imports 
#include "ghcmd_framework.h"

// imports | variables 
std::map<std::wstring, std::wstring> command_map;        // Command to hotkey expression
std::map<std::wstring, UINT> keycode_map;                // Key string to Windows API keycode
int hotkeys_counter = 0;                                 // Hotkey counter, initialized to 0
std::map<int, std::wstring> hotkey_index_to_command;     // Hotkey ID to command
std::mutex hotkeyMutex;  // Define mutex
int mouse_step = 7; // mouse move speed 
std::unordered_map<HWND, WindowState> g_windowStates; 
std::unordered_map<HWND, bool> window_titlebar_map; // for toggling compact mode 
std::unordered_map<std::wstring, UINT> command_keypress_map; // REVISION
HWND working_window; // window used by BringTOLastZOrder and BringToTopZOrder
bool b_LBUTTON_toggle = false; // LBUTTON down toggle
bool is_LBUTTON_down = false;
bool b_RBUTTON_toggle = false; // RBUTTON down toggle 
bool is_RBUTTON_down = false;

// imports | variables | functions 

// -- Hotkey management
void InitializeKeycodeMap() {
    // -- arrow & navigation keys
    keycode_map[L"up"]       = VK_UP;
    keycode_map[L"down"]     = VK_DOWN;
    keycode_map[L"left"]     = VK_LEFT;
    keycode_map[L"right"]    = VK_RIGHT;
    keycode_map[L"home"]     = VK_HOME;
    keycode_map[L"end"]      = VK_END;
    keycode_map[L"pgup"]     = VK_PRIOR;
    keycode_map[L"pgdown"]   = VK_NEXT;
    keycode_map[L"pageup"]   = VK_PRIOR;
    keycode_map[L"pagedown"] = VK_NEXT;

    // -- editing keys
    keycode_map[L"space"]    = VK_SPACE;
    keycode_map[L"enter"]    = VK_RETURN;
    keycode_map[L"return"]   = VK_RETURN;
    keycode_map[L"tab"]      = VK_TAB;
    keycode_map[L"backspace"] = VK_BACK;
    keycode_map[L"ins"]      = VK_INSERT;
    keycode_map[L"insert"]   = VK_INSERT;
    keycode_map[L"del"]      = VK_DELETE;
    keycode_map[L"delete"]   = VK_DELETE;

    // -- system keys
    keycode_map[L"escape"]   = VK_ESCAPE;
    keycode_map[L"esc"]      = VK_ESCAPE;
    keycode_map[L"printscreen"] = VK_SNAPSHOT;
    keycode_map[L"prtsc"]    = VK_SNAPSHOT;
    keycode_map[L"scrolllock"] = VK_SCROLL;
    keycode_map[L"pause"]    = VK_PAUSE;
    keycode_map[L"capslock"]  = VK_CAPITAL;
    keycode_map[L"numlock"]  = VK_NUMLOCK;

    // -- function keys
    keycode_map[L"F1"]  = VK_F1;
    keycode_map[L"F2"]  = VK_F2;
    keycode_map[L"F3"]  = VK_F3;
    keycode_map[L"F4"]  = VK_F4;
    keycode_map[L"F5"]  = VK_F5;
    keycode_map[L"F6"]  = VK_F6;
    keycode_map[L"F7"]  = VK_F7;
    keycode_map[L"F8"]  = VK_F8;
    keycode_map[L"F9"]  = VK_F9;
    keycode_map[L"F10"] = VK_F10;
    keycode_map[L"F11"] = VK_F11;
    keycode_map[L"F12"] = VK_F12;
    keycode_map[L"F13"] = VK_F13;
    keycode_map[L"F14"] = VK_F14;
    keycode_map[L"F15"] = VK_F15;
    keycode_map[L"F16"] = VK_F16;
    keycode_map[L"F17"] = VK_F17;
    keycode_map[L"F18"] = VK_F18;
    keycode_map[L"F19"] = VK_F19;
    keycode_map[L"F20"] = VK_F20;
    keycode_map[L"F21"] = VK_F21;
    keycode_map[L"F22"] = VK_F22;
    keycode_map[L"F23"] = VK_F23;
    keycode_map[L"F24"] = VK_F24;

    // -- numpad keys
    keycode_map[L"num0"]     = VK_NUMPAD0;
    keycode_map[L"num1"]     = VK_NUMPAD1;
    keycode_map[L"num2"]     = VK_NUMPAD2;
    keycode_map[L"num3"]     = VK_NUMPAD3;
    keycode_map[L"num4"]     = VK_NUMPAD4;
    keycode_map[L"num5"]     = VK_NUMPAD5;
    keycode_map[L"num6"]     = VK_NUMPAD6;
    keycode_map[L"num7"]     = VK_NUMPAD7;
    keycode_map[L"num8"]     = VK_NUMPAD8;
    keycode_map[L"num9"]     = VK_NUMPAD9;
    keycode_map[L"num*"]     = VK_MULTIPLY;
    keycode_map[L"num+"]     = VK_ADD;
    keycode_map[L"num-"]     = VK_SUBTRACT;
    keycode_map[L"num."]     = VK_DECIMAL;
    keycode_map[L"num/"]     = VK_DIVIDE;
    keycode_map[L"numenter"] = VK_RETURN;   // distinct on some keyboards

    // -- OEM / punctuation keys
    keycode_map[L"backslash"] = VK_OEM_5;
    keycode_map[L"\\"]        = VK_OEM_5;
    keycode_map[L"/"]         = VK_OEM_2;
    keycode_map[L"["]         = VK_OEM_4;
    keycode_map[L"]"]         = VK_OEM_6;
    keycode_map[L","]         = VK_OEM_COMMA;
    keycode_map[L"."]         = VK_OEM_PERIOD;
    keycode_map[L";"]         = VK_OEM_1;
    keycode_map[L"'"]         = VK_OEM_7;
    keycode_map[L"-"]         = VK_OEM_MINUS;
    keycode_map[L"="]         = VK_OEM_PLUS;
    keycode_map[L"`"]         = VK_OEM_3;

    // -- modifiers
    keycode_map[L"win"]      = MOD_WIN;
    keycode_map[L"lwin"]     = VK_LWIN;     // left  Windows key (VK, not MOD)
    keycode_map[L"rwin"]     = VK_RWIN;     // right Windows key
    keycode_map[L"alt"]      = MOD_ALT;
    keycode_map[L"lalt"]     = VK_LMENU;    // left  Alt
    keycode_map[L"ralt"]     = VK_RMENU;    // right Alt
    keycode_map[L"ctrl"]     = MOD_CONTROL;
    keycode_map[L"lctrl"]    = VK_LCONTROL; // left  Ctrl
    keycode_map[L"rctrl"]    = VK_RCONTROL; // right Ctrl
    keycode_map[L"shift"]    = MOD_SHIFT;
    keycode_map[L"lshift"]   = VK_LSHIFT;   // left  Shift
    keycode_map[L"rshift"]   = VK_RSHIFT;   // right Shift
    keycode_map[L"apps"]     = VK_APPS;     // Menu / Application key
}
void AssignHotkey(std::wstring line) {
    size_t colonPos = line.find(L':');
    if (colonPos != std::wstring::npos) {
        std::wstring command = trim(line.substr(0, colonPos));
        std::wstring hotkeyExpr = trim(line.substr(colonPos + 1));
        command_map[command] = hotkeyExpr;  // Update command_map
    }
}
bool ExistsCommand(const std::wstring& command) {
    // Variável para armazenar o caminho completo do comando encontrado
    wchar_t buffer[MAX_PATH];

    // Tenta localizar o comando no PATH do sistema
    DWORD result = SearchPathW(
        NULL,          // Usa as pastas do PATH do sistema
        command.c_str(), // Nome do comando
        L".exe",       // Extensão padrão a ser considerada (executáveis)
        MAX_PATH,      // Tamanho do buffer
        buffer,        // Buffer para armazenar o caminho completo do comando
        NULL           // Parte restante do nome (opcional, não usada aqui)
    );

    // Se o resultado for diferente de 0, o comando foi encontrado
    return result != 0;
}

/* Logic [ RegisterHotKeys ] 
R := RegisterHotkeys
-> R() || & (command,hotkeyexpr) || $ key | $ sys_keys | %% keypress, hotkey, invalid
*/

void RegisterHotkeys() { // REVISION
	std::lock_guard<std::mutex> lock(hotkeyMutex);  // Protect shared resources
    for (const auto& [command, hotkeyExpr] : command_map) {
        UINT sys_keys = 0;  // System keycode combination (modifiers)
        UINT key = 0;       // Main keycode
        // Split the hotkey expression into parts
        std::vector<std::wstring> parts;
        std::wstringstream wss(hotkeyExpr);
        std::wstring token;
        while (std::getline(wss, token, L'|')) {
            parts.push_back(token);
        }
        // The last part should contain the key (after the last '+')
        if (!parts.empty()) {
            std::wstring lastPart = parts.back();
            size_t plusPos = lastPart.find(L'+');
            if (plusPos != std::wstring::npos) {
                // Extract modifiers (all parts except the last, plus the part before '+' in the last)
                for (size_t i = 0; i < parts.size() - 1; ++i) {
                    if (keycode_map.count(parts[i])) {
                        sys_keys |= keycode_map[parts[i]];
                    }
                }
                // Add the modifier before '+' in the last part
                std::wstring modBeforePlus = lastPart.substr(0, plusPos);
                if (keycode_map.count(modBeforePlus)) {
                    sys_keys |= keycode_map[modBeforePlus];
                }
                // Extract the key after '+'
                std::wstring keyStr = lastPart.substr(plusPos + 1);
                if (keycode_map.count(keyStr)) {
                    key = keycode_map[keyStr];
                } else {
					key = (UINT) std::towupper(keyStr[0]);
				}
            }
            else {
                // No '+' found, assume single modifier format (e.g., "win+up")
                if (parts.size() == 1 && keycode_map.count(lastPart) == 0) {
                    size_t plusPos = hotkeyExpr.find(L'+');
                    if (plusPos != std::wstring::npos) {
                        std::wstring modStr = hotkeyExpr.substr(0, plusPos);
                        std::wstring keyStr = hotkeyExpr.substr(plusPos + 1);
                        if (keycode_map.count(modStr)) sys_keys |= keycode_map[modStr];
                        if (keycode_map.count(keyStr)) key = keycode_map[keyStr];
                    }
                }
            }
        }
        if (sys_keys==0) { 
            if ( hotkeyExpr.starts_with(L".") ) {
                std::wstring keyexpr = hotkeyExpr;
                std::erase(keyexpr, L'.');  // removes all '.' characters
                if ( keycode_map.contains(keyexpr) ) {
                    command_keypress_map[command] = keycode_map[keyexpr];
                    RegisterGlobalHotkey((UINT)0, keycode_map[keyexpr], L"NULL", keyexpr);
                    std::wcout << L"Registered Keypress : " << command << L" (" << keyexpr << L")" << std::endl;
                }
                continue;
            } else {
                if ( keycode_map.contains(hotkeyExpr) ) {
                    key = keycode_map[hotkeyExpr]; 
                }
            }
        }
        // Register the hotkey
        if (key != 0) {
            RegisterGlobalHotkey(sys_keys, key, command, hotkeyExpr);
        }
        else {
            std::wcerr << L"Invalid hotkey expression for " << command << L":" << hotkeyExpr << std::endl;
        }
    }
}
void UnregisterHotkeys() {
    std::lock_guard<std::mutex> lock(hotkeyMutex);
    for (const auto& [id, command] : hotkey_index_to_command) {
        if (UnregisterHotKey(nullptr, id)) {
            std::wcout << L"Unregistered ID " << id << L" (" << command << L")" << std::endl;
        } else {
            DWORD error = GetLastError();
            if (error != 1419) {  // Ignore "not registered" error
                std::wcerr << L"Failed to unregister ID " << id << L" (" << command << L") Error: " << error << std::endl;
            }
        }
    }
    hotkey_index_to_command.clear();
    hotkeys_counter = 0;
    std::wcout << L"All hotkeys unregistered." << std::endl;
}
void RegisterGlobalHotkey(UINT sys_keys, UINT key, std::wstring command, std::wstring hotkeyExpr) {
    int id = hotkeys_counter + 1;  // Next ID
    if (RegisterHotKey(nullptr, id, sys_keys, key)) {
        hotkeys_counter++;  // Only increment on success
        hotkey_index_to_command[id] = command;
        if (command != L"NULL")
            std::wcout << L"Registered " << command << L" (" << hotkeyExpr << L") as ID " << id << std::endl;
    } else {
        DWORD error = GetLastError();
        std::wcerr << L"Failed to register " << command << L" (" << hotkeyExpr << L") as ID " << id 
                   << L" Error: " << error << std::endl;
        if (error == 1409) {
            std::wcerr << L"Hotkey already registered by another app!" << std::endl;
        }
    }
}

// -- Process hotkey events
/* Logic [ ProcessHotkeys ] 
P := ProcessHotkeys
Hi := loop through hotkeys indexes 
-> P() || % is msg.wParam a hotkey || %% identifiy the command || execute command 
-> P() || $currentHwnd | & i : Hi || % msg.wParam == i || $command | %% command || execute command 
-> P() || $currentHwnd | & i : Hi || % msg.wParam == i || $command | %% command | break 
1. The i will match only once with msg.wParam on this loop 
*/

bool ExecuteCommand(std::wstring command, HWND currentHwnd) {
    // return false to send exit command to ProcessHotkeys
    if (command == L"EXIT") {
        keypress_running = false;
        return false;
    }
    else if (command == L"WINDOW_UP") {
        MoveWindow(currentHwnd, 0, -10);  // Move up by 10 pixels
        return true; // TESTING
    }
    else if (command == L"WINDOW_DOWN") {
        MoveWindow(currentHwnd, 0, 10);   // Move down by 10 pixels
        return true; // TESTING
    }
    else if (command == L"WINDOW_LEFT") {
        MoveWindow(currentHwnd, -10, 0);  // Move left by 10 pixels 
        return true; // TESTING
    }
    else if (command == L"WINDOW_RIGHT") {
        MoveWindow(currentHwnd, 10, 0);   // Move right by 10 pixels
        return true; // TESTING
    }
    else if (command == L"WINDOW_INCREASE") {
        RatioResizeWindow(currentHwnd, 1.05);
        return true; // TESTING
    }
    else if (command == L"WINDOW_DECREASE") {
        RatioResizeWindow(currentHwnd, 0.95);
        return true; // TESTING
    }
    else if (command == L"WINDOW_VERTICAL") {
        VerticalResize(currentHwnd,9,12);
        return true; // TESTING
    }
    else if (command == L"WINDOW_HORIZONTAL") {
        HorizontalResize(currentHwnd); 
        return true; // TESTING
    }
    else if (command == L"MOUSE_UP") {
        MoveMouse(0,-mouse_step);
        return true; // TESTING
    }
    else if (command == L"MOUSE_DOWN") {
        MoveMouse(0,mouse_step);
        return true; // TESTING
    }
    else if (command == L"MOUSE_LEFT") {
        MoveMouse(-mouse_step,0);
        return true; // TESTING
    }
    else if (command == L"MOUSE_RIGHT") {
        MoveMouse(mouse_step,0);
        return true; // TESTING
    }
    else if (command == L"MOUSE_RBUTTON") {
        MouseClick_RBUTTON();
        return true; // TESTING
    }
    else if (command == L"MOUSE_RBUTTON_DOWN") {
        Mouse_RBUTTON_DOWN();
        return true;
    }
    else if (command == L"MOUSE_LBUTTON") {
        MouseClick_LBUTTON();
        return true; // TESTING
    }
    else if (command == L"MOUSE_LBUTTON_DOWN") {
        Mouse_LBUTTON_DOWN();
        return true;
    }
    else if (command == L"MOUSE_MBUTTON") {
        MouseClick_MBUTTON();
        return true; // TESTING
    }
    else if (command == L"TOGGLE_TITLEBAR") {
        if ( !window_titlebar_map.contains(currentHwnd) ) window_titlebar_map[currentHwnd] = true;
        window_titlebar_map[currentHwnd] = !window_titlebar_map[currentHwnd];
        if ( window_titlebar_map[currentHwnd] ) {
            SetWindowNormal(currentHwnd);
        } else {
            SetWindowBorderless(currentHwnd);
        }
        return true; // TESTING
    }
    else if (command == L"SELECT_WORKING_WINDOW") { 
        SelectWorkingWindow(currentHwnd);
        return true; // TESTING
    }
    else if (command == L"BRING_TO_TOP") { 
        BringToTopZOrder();
        return true; // TESTING
    }
    else if (command == L"BRING_TO_LAST") { 
        BringTOLastZOrder();
        return true; // TESTING
    }
    else if (command == L"MOUSE_SCROLLUP") { 
        Mouse_ScrollUp();
        return true; // TESTING
    }
    else if (command == L"MOUSE_SCROLLDOWN") { 
        Mouse_ScrollDown();
        return true; // TESTING
    }
    else if (command == L"MOUSE_TOGGLE_SPEED") { 
        MouseToggleSpeed();
        return true;
    }
    else if (command == L"MOUSE_TOGGLE_LBUTTON") { 
        Mouse_LBUTTON_TOGGLE(); // TESTING
        return true;
    }
    else {
        // std::cout << "Invalid Command" << std::endl;
        return true;
    }    
}
bool ReleaseCommand(std::wstring command, HWND currentHwnd) {
    if (command == L"MOUSE_LBUTTON_DOWN") {
        Mouse_LBUTTON_UP();
        return true;
    } 
    else if (command == L"MOUSE_RBUTTON_DOWN") {
        Mouse_RBUTTON_UP();
        return true;
    } 
    else {
        return true;
    }
}
bool ProcessHotkeys(MSG& msg) {
    if ( msg.message != WM_HOTKEY ) return true;     
    std::lock_guard<std::mutex> lock(hotkeyMutex);  // Protect shared resources
    HWND currentHwnd = GetForegroundWindow();
    if (currentHwnd == nullptr) return true;
    for (int i = 1; i <= hotkeys_counter; i++) {
        if (msg.wParam != i) continue; 
        std::wstring command = hotkey_index_to_command[i];
        return ExecuteCommand(command,currentHwnd);
    }
    return true;
}
bool ProcessKeypress() { // REVISION TESTING
    // return true to send continue to message loop , keypressed 
    std::lock_guard<std::mutex> lock(hotkeyMutex);  // Protect shared resources
    HWND currentHwnd = GetForegroundWindow();
    if (currentHwnd == nullptr) return false;
    bool b_keypress = false;
    for (const auto& [command, keycode] : command_keypress_map) { 
        if ( is_key_pressed(keycode) ) { 
            ExecuteCommand(command, currentHwnd);
            b_keypress = true;
        } else {
            ReleaseCommand(command, currentHwnd);
        }
    }
    return b_keypress;
}

// -- Skills - do things on active windows 
void MoveWindow(HWND hwnd, int dx, int dy) {
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        ::MoveWindow(hwnd, rect.left + dx, rect.top + dy, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    }
}
void RatioResizeWindow(HWND hwnd, double ratio) {
    if (!hwnd) return;
	if (!IsWindow(hwnd)) return;
	if (ratio <= 0) return;
	int minWidth = 100;  // Exemplo de valor mínimo
	int minHeight = 100;
    // Obtém as dimensões atuais da janela
    RECT rect;
    GetWindowRect(hwnd, &rect);
    // Calcula o novo tamanho (diminuído pela metade, mantendo a proporção)
    int width = (rect.right - rect.left) * ratio ;
    int height = (rect.bottom - rect.top) * ratio ;
	width = std::max(minWidth,width);
	height = std::max(minHeight, height);
    // Ajusta o tamanho da janela
    SetWindowPos(hwnd, NULL, rect.left, rect.top, width, height, SWP_NOZORDER | SWP_NOMOVE);
}
void VerticalResize(HWND hwnd) {
    if (!hwnd) return; // Verifica se a janela é válida
    // Obtém o retângulo da janela atual
    RECT rect;
    GetWindowRect(hwnd, &rect);
    // Calcula largura e altura atuais da janela
    int currentWidth = rect.right - rect.left;
    int currentHeight = rect.bottom - rect.top;
    // Verifica se a proporção já é 9:16
    if (currentWidth * 16 == currentHeight * 9) {
        return; // Proporção já está correta, nada a fazer
    }
    // Calcula a área atual (width * height)
    double area = static_cast<double>(currentWidth) * currentHeight;
    // Ajusta para a proporção 9:16 preservando a área
    int newWidth = static_cast<int>(std::sqrt(area * 9.0 / 16.0));
    int newHeight = static_cast<int>(std::sqrt(area * 16.0 / 9.0));

    // Garante que largura e altura estejam consistentes com a proporção
    if (newWidth * 16 > newHeight * 9) {
        newWidth = static_cast<int>(newHeight * 9.0 / 16.0);
    } else {
        newHeight = static_cast<int>(newWidth * 16.0 / 9.0);
    }
    // Redimensiona a janela com as novas dimensões
    SetWindowPos(hwnd, NULL, rect.left, rect.top, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE);
}
void VerticalResize(HWND hwnd, int width_asp, int height_asp) {
    if (!hwnd) return; // Verifica se a janela é válida
    // Obtém o retângulo da janela atual
    RECT rect;
    GetWindowRect(hwnd, &rect);
    // Calcula largura e altura atuais da janela
    int currentWidth = rect.right - rect.left;
    int currentHeight = rect.bottom - rect.top;
    // Verifica se a proporção já é 9:16
    if (currentWidth * height_asp == currentHeight * width_asp) {
        return; // Proporção já está correta, nada a fazer
    }
    // Calcula a área atual (width * height)
    double area = static_cast<double>(currentWidth) * currentHeight;
    // Ajusta para a proporção 9:16 preservando a área
    int newWidth = static_cast<int>(std::sqrt(area * width_asp / height_asp));
    int newHeight = static_cast<int>(std::sqrt(area * height_asp / width_asp));

    // Garante que largura e altura estejam consistentes com a proporção
    if (newWidth * height_asp > newHeight * width_asp) {
        newWidth = static_cast<int>(newHeight * width_asp / height_asp);
    } else {
        newHeight = static_cast<int>(newWidth * height_asp / width_asp);
    }
    // Redimensiona a janela com as novas dimensões
    SetWindowPos(hwnd, NULL, rect.left, rect.top, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE);
}
void HorizontalResize(HWND hwnd) {
    if (!hwnd) return; // Verifica se a janela é válida
    // Obtém o retângulo da janela atual
    RECT rect;
    GetWindowRect(hwnd, &rect);
    // Calcula largura e altura atuais da janela
    int currentWidth = rect.right - rect.left;
    int currentHeight = rect.bottom - rect.top;
    // Verifica se a proporção já é 16:9
    if (currentWidth * 9 == currentHeight * 16) {
        return; // Proporção já está correta, nada a fazer
    }
    // Calcula a área atual (width * height)
    double area = static_cast<double>(currentWidth) * currentHeight;
    // Ajusta para a proporção 16:9 preservando a área
    int newWidth = static_cast<int>(std::sqrt(area * 16.0 / 9.0));
    int newHeight = static_cast<int>(std::sqrt(area * 9.0 / 16.0));
    // Garante que largura e altura estejam consistentes com a proporção
    if (newWidth * 9 > newHeight * 16) {
        newWidth = static_cast<int>(newHeight * 16.0 / 9.0);
    } else {
        newHeight = static_cast<int>(newWidth * 9.0 / 16.0);
    }
    // Redimensiona a janela com as novas dimensões
    SetWindowPos(hwnd, NULL, rect.left, rect.top, newWidth, newHeight, SWP_NOZORDER | SWP_NOMOVE);
}
void ExecuteCMD(const std::wstring& command) {
    std::wstring cmdLine = L"cmd.exe /C " + command;  // /C runs the command and terminates

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };

    // Create the process
    BOOL success = CreateProcessW(
        nullptr,                    // No module name (use command line)
        &cmdLine[0],                // Command line (mutable)
        nullptr,                    // Process handle not inheritable
        nullptr,                    // Thread handle not inheritable
        FALSE,                      // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,           // No console window
        nullptr,                    // Use parent's environment block
        nullptr,                    // Use parent's starting directory
        &si,                        // Pointer to STARTUPINFO structure
        &pi                         // Pointer to PROCESS_INFORMATION structure
    );

    if (success) {
        // Close handles immediately to avoid blocking
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::wcerr << L"Failed to execute command: " << command << L" Error: " << GetLastError() << std::endl;
    }
}
void MoveMouse(int dx, int dy) {
    SendMouseInput(MOUSEEVENTF_MOVE, 0);
    // Note: To move relatively, we use MOUSEEVENTF_MOVE. 
    // dx and dy represent the movement in pixels.
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
}
void MouseClick_LBUTTON() {
    b_LBUTTON_toggle = false;
    is_LBUTTON_down = false;
    SendMouseInput(MOUSEEVENTF_LEFTDOWN);
    SendMouseInput(MOUSEEVENTF_LEFTUP);
}
void MouseClick_RBUTTON() {
    b_RBUTTON_toggle = false;
    is_RBUTTON_down = false;
    SendMouseInput(MOUSEEVENTF_RIGHTDOWN);
    SendMouseInput(MOUSEEVENTF_RIGHTUP);
}
void MouseClick_MBUTTON() {
    SendMouseInput(MOUSEEVENTF_MIDDLEDOWN);
    SendMouseInput(MOUSEEVENTF_MIDDLEUP);
}
void Mouse_ScrollUp() {
    // WHEEL_DELTA is usually 120
    SendMouseInput(MOUSEEVENTF_WHEEL, WHEEL_DELTA);
}
void Mouse_ScrollDown() {
    // Negative WHEEL_DELTA scrolls down
    SendMouseInput(MOUSEEVENTF_WHEEL, (DWORD)-WHEEL_DELTA);
}
void SetWindowBorderless(HWND hwnd) {
    if (!IsWindow(hwnd)) return;
    // If already stored, don't overwrite
    if (g_windowStates.contains(hwnd)) return;
    WindowState state{};
    state.style = GetWindowLong(hwnd, GWL_STYLE);
    state.exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    GetWindowRect(hwnd, &state.rect);
    state.placement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &state.placement);
    g_windowStates[hwnd] = state;
    // Remove borders, caption, thick frame, etc.
    LONG newStyle = state.style;
    newStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX |
                  WS_MAXIMIZEBOX | WS_SYSMENU);
    LONG newExStyle = state.exStyle;
    newExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(hwnd, GWL_STYLE, newStyle);
    SetWindowLong(hwnd, GWL_EXSTYLE, newExStyle);
}
void SetWindowNormal(HWND hwnd) {
    if (!IsWindow(hwnd)) return;
    auto it = g_windowStates.find(hwnd);
    if (it == g_windowStates.end()) return; // nothing to restore
    const WindowState& state = it->second;
    SetWindowLong(hwnd, GWL_STYLE, state.style);
    SetWindowLong(hwnd, GWL_EXSTYLE, state.exStyle);
    SetWindowPlacement(hwnd, &state.placement);
    SetWindowPos(hwnd, nullptr,
                 state.rect.left, state.rect.top,
                 state.rect.right - state.rect.left,
                 state.rect.bottom - state.rect.top,
                 SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
    g_windowStates.erase(it);
}
void SetAllWindowNormal() {
    std::vector<HWND> toRestore;
    toRestore.reserve(g_windowStates.size());
    for (const auto& pair : g_windowStates) toRestore.push_back(pair.first);
    for (HWND hwnd : toRestore) SetWindowNormal(hwnd);
}
void SelectWorkingWindow(HWND hwnd){
    if (!hwnd) return; 
    working_window = hwnd;
}
void BringToTopZOrder() { 
    if (!IsWindow(working_window)) return;
    SetForegroundWindow(working_window);
    SetWindowPos(
        working_window,
        HWND_TOP,          // move to top of Z-order
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE // | SWP_NOACTIVATE
    );
}
void BringTOLastZOrder() {
    if (!IsWindow(working_window)) return;
    SetWindowPos(
        working_window,
        HWND_BOTTOM,       // move to bottom of Z-order
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
    );
}
void MouseToggleSpeed() {
    if (mouse_step==7) { mouse_step = 25; } else { mouse_step = 7; }
}
void Mouse_LBUTTON_DOWN() { // ISSUE  
    if (!is_LBUTTON_down) {
        SendMouseInput(MOUSEEVENTF_LEFTDOWN);
        is_LBUTTON_down = true;
    }
}
void Mouse_LBUTTON_UP() { // ISSUE 
    if (is_LBUTTON_down) {
        SendMouseInput(MOUSEEVENTF_LEFTUP);
        is_LBUTTON_down = false;
    }
}
void Mouse_LBUTTON_TOGGLE() { // TESTING 
    b_LBUTTON_toggle = !b_LBUTTON_toggle;
    if (b_LBUTTON_toggle && !is_LBUTTON_down) {
        SendMouseInput(MOUSEEVENTF_LEFTDOWN);
        is_LBUTTON_down = true;
    } else {
        SendMouseInput(MOUSEEVENTF_LEFTUP);
        is_LBUTTON_down = false;
    }
}
void Mouse_RBUTTON_DOWN() { // ISSUE 
    if (!is_RBUTTON_down) {
        SendMouseInput(MOUSEEVENTF_RIGHTDOWN);
        is_RBUTTON_down = true;
    }
}
void Mouse_RBUTTON_UP() { // ISSUE  
    if (is_RBUTTON_down) {
        is_RBUTTON_down = false;
        SendMouseInput(MOUSEEVENTF_RIGHTUP);
    }
}
void Mouse_RBUTTON_TOGGLE() { // TESTING 
    b_RBUTTON_toggle = !b_RBUTTON_toggle;
    if (b_RBUTTON_toggle && !is_RBUTTON_down) {
        SendMouseInput(MOUSEEVENTF_RIGHTDOWN);
        is_RBUTTON_down = true;
    } else {
        SendMouseInput(MOUSEEVENTF_RIGHTUP);
        is_RBUTTON_down = false;
    }
}

// -- helpers
std::wstring trim(const std::wstring& str) {
    // Find the first non-space character
    size_t start = str.find_first_not_of(L" \t\n\r");
    if (start == std::wstring::npos) {
        return L""; // String is entirely whitespace
    }
    
    // Find the last non-space character
    size_t end = str.find_last_not_of(L" \t\n\r");
    
    // Return the trimmed string
    return str.substr(start, end - start + 1);
}
void SendMouseInput(DWORD flags, DWORD data) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = flags;
    input.mi.mouseData = data;
    SendInput(1, &input, sizeof(INPUT));
}
bool is_key_pressed(const int key_code) { 
    return (GetAsyncKeyState(key_code) & 0x8000) != 0; 
}













// -- END
