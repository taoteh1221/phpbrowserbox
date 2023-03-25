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

#include "resource.h"
#include <stdexcept>
#include <regex>
#include <chrono>
#include <thread>
#include "funcs.h"

using namespace std;

HWND hwnd;    /* This is the handle for our window */
MSG messages; /* Here messages to the application are saved */


std::ofstream ofs;

int exitApp()
{
  // Close the window and cleanup GDI+
  DestroyWindow(hwnd);

  // Release the mutex handle
  //CloseHandle(hMutex);

  // stop log and remove file
  //ofs.close();
  //remove(szLogFileName);

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


  // initialize paths
  sprintf(szLogFileName, "%s%s", basePath, "tmp\\phpbrowserbox.pid");
  sprintf(szbbWebKitLog, "%s%s", basePath, "tmp\\phpbbwebkit.pid");
  sprintf(szbbWebKitRelaunch, "%s%s", basePath, "tmp\\phpbbwebkit2.pid");
  sprintf(szbbWebKit, "%s%s", basePath, "bin\\phpbbwebkit\\phpbbwebkit.exe");
  sprintf(szbbSplashApp, "%s%s", basePath, "bin\\support\\bin\\bbsplashscreen.exe");
  sprintf(szbbStartCmd, "%s%s", basePath, "bbshell.exe /c bbstart");

   hMutex = CreateMutex(NULL, TRUE, lockName);
   if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // Another instance of the application is already running
        CloseHandle(hMutex);

        //MessageBoxA(NULL, "Another instance of the application is already running.", "Error", MB_OK | MB_ICONERROR);

        //tell this instance of phpbbox running to show itself
        std::ofstream bbrelaunch(szbbWebKitRelaunch, std::ios_base::app | std::ios_base::out);
        bbrelaunch << "re-starting app";
        bbrelaunch.close();

        /* this window is too generic for any electronapp
        HWND bhwnd = FindWindow("Chrome_WidgetWin_1", NULL);
        if (bhwnd != NULL) {
           SetForegroundWindow(bhwnd);
        }
        */
        return 1;
    }

  // create application window
  if (!CreateApplicationWindow(hThisInstance, hPrevInstance, lpszArgument, nCmdShow, hwnd))
  {
    return 0;
  }

  // std::ofstream log(szLogFileName, std::ios_base::app | std::ios_base::out);

  //ofs.open(szLogFileName, std::ofstream::out | std::ofstream::app);
  //ofs << "starting app";


    //start splashscreen
    execCommand(szbbSplashApp,0,false);

    //start httpd service

   //start webkit
   if(!startWebkitEngine()) {
       return exitApp();
   }

   //start mysqld here


   //wait for webkit to complete
   waitForWebkitToExit();

  // start php/mysql server
  //execCommand2(szbbStartCmd, CREATE_NO_WINDOW, true);

   //std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  /*
  STARTUPINFO info = {0};
  PROCESS_INFORMATION processInfo;
  if (CreateProcess(NULL, szbbWebKit, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
  {
    int attempts = 0;
    while (true)
    {
      if (exist(szbbWebKitLog) || attempts>=5)
        break;

      std::this_thread::sleep_for(std::chrono::milliseconds(3000));
      attempts++;
    }

    DestroyWindow(hwnd);
  }
  */

  //ofs.close();
  //remove(szLogFileName);

  return exitApp();
}
