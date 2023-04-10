#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <process.h>
#include <stdexcept>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <regex>

#include "vcredist.h"

using namespace std;


// registry check
BOOL DoesVCRedistNeedUpdate()
{
  BOOL requireUpdate = true;

  CHAR message[MAX_PATH];
  CHAR requiredVal[MAX_PATH] = "14.34.31938";
  CHAR currentVal[MAX_PATH];

  DWORD dataSize = MAXWORD;

  // Computer\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\X64
  // Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Installer\Dependencies\Microsoft.VS.VC_RuntimeAdditionalVSU_amd64,v14
  // norm - v14.34.31938.00
  LONG result = RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Classes\\Installer\\Dependencies\\Microsoft.VS.VC_RuntimeAdditionalVSU_amd64,v14", "Version", RRF_RT_REG_SZ, nullptr, &currentVal, &dataSize);
  if (result != ERROR_SUCCESS)
  {
    requireUpdate = true;
    strcpy(message, "No VCRedist Found");
  }
  else
  {
    // compare version
    std::string str_inp1(requiredVal);
    std::string str_inp2(currentVal);

    int res = str_inp1.compare(str_inp2);

    if (res == 0)
    {
      strcpy(message, "Exact match Found");
      requireUpdate = false;
    }
    else if (res < 0)
    {
      strcpy(message, "More up to date than required");
      requireUpdate = false;
    }
    else
    {
      strcpy(message, "Not up to date at all");
      requireUpdate = true;
    }
  }
  //cout << "VC Redistributable test : " << message << "\n";
  return requireUpdate;
}


void UpdateVCRedist(TCHAR path[])
{
  STARTUPINFO info = {
      sizeof(info)};
  PROCESS_INFORMATION processInfo;

  // char cmdArgs[] = "VC_redist.x64.exe /Q /norestart";
  char cmdArgs[] = "VC_redist.x64.exe /passive /norestart";

  //MessageBoxA(NULL, szVcRedistrMessage, szVcRedistrTitle, MB_OK);

  if (CreateProcess(path, cmdArgs, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
  {
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

     //MessageBoxA(NULL, path, "Update Complete", MB_OK | MB_ICONERROR);
  }
  else
  {
   // MessageBoxA(NULL, path, "Update Failed", MB_OK | MB_ICONERROR);
  }
}
