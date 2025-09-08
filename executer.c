#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: executor.exe <PID> <Address>\n");
        printf("Example: executor.exe 1234 0x000001A1B2C3D4E5\n");
        return 1;
    }

    DWORD pid = atoi(argv[1]);
    LPVOID address;
    sscanf_s(argv[2], "%p", &address);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("Failed to open process! Error: %d\n", GetLastError());
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,(LPTHREAD_START_ROUTINE)address,NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Failed to create remote thread! Error: %d\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    printf("Remote thread created successfully!\n");
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
}

