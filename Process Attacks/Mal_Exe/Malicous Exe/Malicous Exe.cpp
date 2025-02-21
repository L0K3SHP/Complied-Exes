#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <sddl.h>

// Function to get the username
std::string GetUsername() {
    char username[256];
    DWORD size = sizeof(username);
    if (GetUserNameA(username, &size)) {
        return std::string(username);
    }
    return "UnknownUser";
}

// Function to get process name by ID
std::string GetProcessName(DWORD processID) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return "UnknownProcess";

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    std::string processName = "UnknownProcess";

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ProcessID == processID) {
                //processName = pe32.szExeFile;
                char processNameBuffer[MAX_PATH];
                WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, processNameBuffer, MAX_PATH, NULL, NULL);
                processName = processNameBuffer;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return processName;
}

int main() {
    DWORD processID = GetCurrentProcessId();
    std::string processName = GetProcessName(processID);
    std::string username = GetUsername();

    // Display the results in a message box
    std::string message = "Injection Successful!\n\n"
        "Process Name: " + processName + "\n"
        "Process ID: " + std::to_string(processID) + "\n"
        "User: " + username;

    MessageBoxA(NULL, message.c_str(), "Injection Status", MB_OK | MB_ICONINFORMATION);

    return 0;
}
