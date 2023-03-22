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
#include <ios>
#include <fstream>

#include "funcs.h"
#include "resource.h"
#include <stdexcept>
#include <regex>
#include <chrono>
#include <thread>

using namespace std;

HWND hwnd; /* This is the handle for our window */
MSG messages;            /* Here messages to the application are saved */

HANDLE hMutex;


TCHAR szLogFileName[MAX_PATH];
TCHAR szbbWebKit[MAX_PATH];
TCHAR szbbWebKitLog[MAX_PATH];
TCHAR szbbStartCmd[MAX_PATH];


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

  /*
  get basepath e.g. e:/phpbb/
  */
  char *basePath = NULL;
  getEXEPath(basePath, szFileName);

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


  // create application window
  if (!CreateApplicationWindow(hThisInstance, hPrevInstance, lpszArgument, nCmdShow, hwnd))
  {
    return 0;
  }

  // start log
  sprintf(szLogFileName, "%s%s", basePath, "tmp\\phpbrowserbox.pid");
  sprintf(szbbWebKitLog, "%s%s", basePath, "tmp\\phpbbwebkit.pid");
  sprintf(szbbWebKit, "%s%s", basePath, "bin\\phpbbwebkit\\phpbbwebkit.exe");
  sprintf(szbbStartCmd, "%s%s", basePath, "phpbb.exe /c bbstart");
  // std::ofstream log(szLogFileName, std::ios_base::app | std::ios_base::out);

  ofs.open(szLogFileName, std::ofstream::out | std::ofstream::app);

  ofs << "starting app";

  //if no pid then create a splash screen
  if(!exist(szbbWebKitLog)) {
   ShowWindow(hwnd, nCmdShow);
   LoadSplashImage(hwnd);
  }


  execCommand(szbbStartCmd,CREATE_NO_WINDOW,true);
  //execCommand(szbbStartCmd,CREATE_NO_WINDOW,true);

  STARTUPINFO info = {0};
  PROCESS_INFORMATION processInfo;

  if (CreateProcess(NULL, szbbWebKit, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
  {
      while (true) {
        if(exist(szbbWebKitLog)) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
      }
        //to remove splashscreen
        DestroyWindow(hwnd);
        //std::this_thread::sleep_for(std::chrono::milliseconds(4000));

        //WaitForSingleObject(processInfo.hProcess, INFINITE);
        //CloseHandle(processInfo.hProcess);
        //CloseHandle(processInfo.hThread);
  }

  return exitApp();
}
