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
  sprintf(szLogServerOutLogName, "%s%s", basePath, "tmp\\phpbbserverout.pid");
  sprintf(szbbWebKitLog, "%s%s", basePath, "tmp\\phpbbwebkit.pid");
  sprintf(szbbWebKitRelaunch, "%s%s", basePath, "tmp\\phpbbwebkit2.pid");
  sprintf(szbbWebKit, "%s%s", basePath, "bin\\nw.js\\nw.exe");
  sprintf(szbbSplashApp, "%s%s", basePath, "bin\\bbsplashscreen.exe");
  sprintf(szbbStartCmd, "%s%s", basePath, "bin\\bbserver.exe start");
  sprintf(szbbStopCmd, "%s%s", basePath, "bin\\bbserver stop");

   hMutex = CreateMutex(NULL, TRUE, lockName);
   if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // Another instance of the application is already running

        if(exist(szbbWebKitLog)) {
            //case 1 : a fully working bbwebkit already exist
            //tell this instance of phpbbox running to show itself
            std::ofstream bbrelaunch(szbbWebKitRelaunch, std::ios_base::app | std::ios_base::out);
            bbrelaunch << "restoring bbwebkit";
            bbrelaunch.close();

            CloseHandle(hMutex);
            return 1;
        } else if(exist(szLogServerOutLogName)) {
            //case 2: service is just closing
            //MessageBoxA(NULL, "Server is closing out.", "Error", MB_OK | MB_ICONERROR);
        } else {
            MessageBoxA(NULL, "Your last request could not be complete.", "Please Try Re-Launching App", MB_OK | MB_ICONERROR);

            CloseHandle(hMutex);
            return 1;
        }
    }

  // create application window
  if (!CreateApplicationWindow(hThisInstance, hPrevInstance, lpszArgument, nCmdShow, hwnd))
  {
    return 0;
  }

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

  //start splashscreen
  execCommand(szbbSplashApp,0,false);

  //start server with db and preload webkit
  execCommand(szbbStartCmd, CREATE_NO_WINDOW, true);

   //start webkit
  if(!startWebkitEngine()) {
       return exitApp();
  }

   //wait for webkit to complete
   waitForWebkitToExit();

   //create this when server is closing
   std::ofstream bbserverout(szLogServerOutLogName, std::ios_base::app | std::ios_base::out);
   bbserverout << "re-starting app";
   bbserverout.close();

   //stop php/mysql server
   execCommand(szbbStopCmd, CREATE_NO_WINDOW, true);

   //std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  ofs.close();
  remove(szLogFileName);
  remove(szbbWebKitRelaunch);
  remove(szLogServerOutLogName);

  return exitApp();
}
