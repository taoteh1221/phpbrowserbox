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

#ifdef _WIN32
#include <direct.h>
// MSDN recommends against using getcwd & chdir names
#define cwd _getcwd
#define cd _chdir
#else
#include "unistd.h"
#define cwd getcwd
#define cd chdir
#endif

using namespace std;

void getEXEPath(char *&exePath,
                const char *szFileName)
{
  // Get the last position of '/'
  std::string aux(szFileName);

// get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
  int pos = aux.rfind('\\');
#else
  int pos = aux.rfind('/');
#endif

  // Get the path and the name
  std::string path = aux.substr(0, pos + 1);

  char *path_array = new char[path.length() + 1];
  strcpy(path_array, path.c_str());

  exePath = path_array;
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

  cd(basePath);
  cd("bin\\bbwebkit");

  STARTUPINFO info = {
      sizeof(info)};
  PROCESS_INFORMATION processInfo;

    if(!CreateProcess("bbwebkit.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)) {
      MessageBoxA(NULL, "Unable to launch application", "Startup Failure", MB_OK | MB_ICONERROR);
     }


  return 0;
}
