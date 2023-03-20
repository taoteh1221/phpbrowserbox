#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>

#include <windows.h>

#include <stdio.h>

#include <iostream>

#include <string>

#include <process.h>

#include <cstdio>

#include "resource.h"

#include <ios>

#include <fstream>

#include "funcs.h"

#include <stdexcept>
#include <regex>

using namespace std;

HWND hwnd; /* This is the handle for our window */
MSG messages;            /* Here messages to the application are saved */

HANDLE hMutex;
TCHAR szLogFileName[MAX_PATH];
std::ofstream ofs;

int exitApp() {
  // Close the window and cleanup GDI+
  DestroyWindow(hwnd);

  // Release the mutex handle
  CloseHandle(hMutex);

  // stop log and remove file
  ofs.close();
  remove(szLogFileName);

  return 0;
}

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{

  // get full EXEpath
  TCHAR szFileName[MAX_PATH];
  GetModuleFileName(NULL, szFileName, MAX_PATH);

  cout << "Welcome to PHPBrowserbox v6.0" ;

  /*
  get basepath e.g. e:/phpbb/
  */
  char *basePath = NULL;
  getEXEPath(basePath, szFileName);

  setBasePath(basePath);
  makeTempFolder();

  //get commandline Arguments
  std::string cmdLine(GetCommandLine());
  std::string exeName (szFileName);

  //check whether to start shell
  cmdLine.replace(cmdLine.find(exeName),exeName.length(),"");

  if (cmdLine.find("/doctor") != string::npos) {
    //Execute bbdoctor
    ExecScriptInResource(IDR_DOCTOR_SCRIPT, "bbdoctor");
    return 0;
  }

  if (cmdLine.find("/shell") != string::npos) {
    //Execute Shell script start
    ExecScriptInResource(IDR_SHELL_SCRIPT, "bbshell");
    return 0;
  }


  // Create a mutex with a unique name
  char lockName[MAX_PATH];
  std::string spath(basePath);
  std::replace(spath.begin(), spath.end(), '\\', '/');
  sprintf(lockName, "%s", spath.c_str());

  hMutex = CreateMutex(NULL, TRUE, lockName);
  if (GetLastError() == ERROR_ALREADY_EXISTS)
  {
    // Another instance of the application is already running
    MessageBoxA(NULL, "Another instance is already running.", "Error", MB_OK | MB_ICONERROR);
    CloseHandle(hMutex);
    return 0;
  }

  // check config
  if (!preprocessAppConfig(basePath))
  {
    return 0;
  }

  // create application window
  if (!CreateApplicationWindow(hThisInstance, hPrevInstance, lpszArgument, nCmdShow, hwnd))
  {
    return 0;
  }

  // start log
  sprintf(szLogFileName, "%s%s", basePath, "tmp/phpbrowserbox.pid");
  // std::ofstream log(szLogFileName, std::ios_base::app | std::ios_base::out);

  ofs.open(szLogFileName, std::ofstream::out | std::ofstream::app);

  ofs << "starting app";

  // registry check for vcruntime140
  if (DoesVCRedistNeedUpdate())
  {
    TCHAR szVcRedistPath[MAX_PATH];
    sprintf(szVcRedistPath, "%sbin\\vc_redist\\VC_redist.x64.exe", basePath);
    UpdateVCRedist(szVcRedistPath);
  }

  // do this first so that tmp/ports.txt can be initialized
  firewallConfig(basePath);

  // dynamic port scanning
  freePortScanning();

  // update the lamp configs on the fly
  rebuildConfig();

  if(!startupTest(basePath)) {
    exitApp();
  }

  //Sleep(5 * 1000);

  //start apache, mysql and webkit here
  startInternalServer(hwnd);

  exitApp();

  return 0;
}
