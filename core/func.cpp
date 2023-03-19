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

#include <cstring>

#include "RSJparser.tcc"

#include <stdexcept>

using namespace std;

// global vars

char *basePath = NULL;

TCHAR phpbbPath[MAX_PATH];

TCHAR phpbbname[MAX_PATH];
TCHAR phpbbversion[MAX_PATH];

TCHAR szStartupLog[MAX_PATH];

TCHAR szConfigFile[MAX_PATH];
TCHAR szWebkitConfigFile[MAX_PATH];

TCHAR szApacheExe[MAX_PATH];
TCHAR szPhpExe[MAX_PATH];
TCHAR szMysqlExe[MAX_PATH];
TCHAR szWebkitExe[MAX_PATH];
TCHAR szMysqlDataDir[MAX_PATH];

TCHAR szMysqlService[MAX_PATH];

TCHAR szPHP8DLL[MAX_PATH];
TCHAR szPHP7DLL[MAX_PATH];
TCHAR szPHP5DLL[MAX_PATH];

TCHAR szApacheConfSrc[MAX_PATH];
TCHAR szApacheConfDest[MAX_PATH];

TCHAR szPhpBaseConfSrc[MAX_PATH];
TCHAR szPhpConfSrc[MAX_PATH];
TCHAR szPhpConfDest1[MAX_PATH];
TCHAR szPhpConfDest2[MAX_PATH];

TCHAR szMysqlConfSrc[MAX_PATH];
TCHAR szMysqlConfDest[MAX_PATH];

TCHAR szPortScanFile[MAX_PATH];

int apachePort = 0;
int mysqlPort = 0;
BOOL useSplashScreen = FALSE;

string apachePortStr;
string mySqlPortStr;
string phpSAPILoader;

void setBasePath(char *path) {
   basePath = path;
}

BOOL closeWindowByTitle(char * title) {

    auto hwnd = FindWindow(nullptr, title);

	if (hwnd != nullptr) {
		PostMessage(hwnd, WM_CLOSE, 0, 0);
        //MessageBoxA(NULL, "success", title, MB_OK | MB_ICONERROR);
        return true;
	} else {
        //MessageBoxA(NULL, "failed", title, MB_OK | MB_ICONERROR);
        return false;
	}
}

uint32_t fnv1a_hash(const char* data, size_t size) {
    uint32_t hash = 2166136261u; // FNV offset basis
    for (size_t i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= 16777619u; // FNV prime
    }
    return hash;
}

std::string hashString(std::string original, std::string prefix) {
    std::string result = prefix;
    uint32_t hash = fnv1a_hash(original.c_str(), original.size());
    string substr = std::to_string(hash);
    result.append(substr);
    return result;
}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void msgbox(char *message, char *title)
{
  MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR);
}

void msgbox(std::string message, char *title)
{
  MessageBoxA(NULL, message.c_str(), title, MB_OK | MB_ICONERROR);
}

void msgbox(int message, char *title)
{
  string msg = std::to_string(message);
  MessageBoxA(NULL, msg.c_str(), title, MB_OK | MB_ICONERROR);
}

void replace_all(
    std::string &s,
    std::string const &toReplace,
    std::string const &replaceWith)
{
  std::string buf;
  std::size_t pos = 0;
  std::size_t prevPos;

  // Reserves rough estimate of final size of string.
  buf.reserve(s.size());

  while (true)
  {
    prevPos = pos;
    pos = s.find(toReplace, pos);
    if (pos == std::string::npos)
      break;
    buf.append(s, prevPos, pos - prevPos);
    buf += replaceWith;
    pos += toReplace.size();
  }

  buf.append(s, prevPos, s.size() - prevPos);
  s.swap(buf);
}

void LoadFileInResource(int name, int type, DWORD &size,
                        const char *&data)
{
  HMODULE handle = ::GetModuleHandle(NULL);
  HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name),
                            MAKEINTRESOURCE(type));
  HGLOBAL rcData = ::LoadResource(handle, rc);
  size = ::SizeofResource(handle, rc);
  data = static_cast<
      const char *>(::LockResource(rcData));
}

void CreateFolder(const char *path)
{
  if (!CreateDirectory(path, NULL))
  {
    return;
  }
}

void makeTempFolder() {
  // create temp folder
  TCHAR szTempFolder[MAX_PATH];
  sprintf(szTempFolder, "%s%s", basePath, "tmp");
  CreateFolder(szTempFolder);
}

void LoadSplashImage(HWND hWnd)
{
  RECT rect;
  HDC hdc = GetDC(hWnd);

  // TCHAR splashImage[ ] = "assets\\splashscreen.bmp";

  HBRUSH brush = CreatePatternBrush((HBITMAP)LoadImage(NULL, _T("assets\\splashscreen.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
  GetWindowRect(hWnd, &rect);

  rect.left = 0;
  rect.top = 0;
  // rect.right = 540;
  // rect.bottom = 356;

  std::cout << "Left: " << rect.left << "\n";
  std::cout << "Right: " << rect.right << "\n";
  std::cout << "Top: " << rect.top << "\n";
  std::cout << "Bottom: " << rect.bottom << "\n";

  FillRect(hdc, &rect, brush);
  DeleteObject(brush);
  ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) /* handle the messages */
  {
  case WM_DESTROY:
    PostQuitMessage(0); /* send a WM_QUIT to the message queue */
    break;
  case WM_KILLFOCUS:
  /*
  case EN_KILLFOCUS:
    msgbox("lost focus",NULL);
    break;
  */
  default:
    /* for messages that we don't deal with */
    return DefWindowProc(hwnd, message, wParam, lParam);
  }

  return 0;
}

/*  This function is called by the Windows function DispatchMessage()  */

WNDCLASSEX wincl; /* Data structure for the windowclass */

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

BOOL CreateApplicationWindow(HINSTANCE hThisInstance,
                             HINSTANCE hPrevInstance,
                             LPSTR lpszArgument,
                             int nCmdShow, HWND &hwnd)
{

  /* The Window structure */
  wincl.hInstance = hThisInstance;
  wincl.lpszClassName = szClassName;
  wincl.lpfnWndProc = WindowProcedure; /* This function is called by windows */
  wincl.style = CS_DBLCLKS;            /* Catch double-clicks */
  wincl.cbSize = sizeof(WNDCLASSEX);

  /* Use default icon and mouse-pointer */
  wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wincl.lpszMenuName = NULL; /* No menu */
  wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
  wincl.cbWndExtra = 0;      /* structure or the window instance */
  /* Use Windows's default colour as the background of the window */
  wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

  /* Register the window class, and if it fails quit the program */
  if (!RegisterClassEx(&wincl))
    return FALSE;

  int width = 640;
  int height = 456;

  /* The class is registered, let's create the program*/
  hwnd = CreateWindowEx(
      0,                                            /* Extended possibilites for variation */
      szClassName,                                  /* Classname */
      NULL,                                         /* Title Text */
      WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, /* default window */
      (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
      (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
      width,         /* The programs width */
      height,        /* and height in pixels */
      HWND_DESKTOP,  /* The window is a child-window to desktop */
      NULL,          /* No menu */
      hThisInstance, /* Program Instance handler */
      NULL           /* No Window Creation data */
  );

  /* Make the window visible on the screen */
  if (useSplashScreen)
  {
    ShowWindow(hwnd, nCmdShow);

    // splash image
    LoadSplashImage(hwnd);
  }

  return TRUE;
}

// registry check
BOOL DoesVCRedistNeedUpdate()
{
  BOOL requireUpdate = true;

  CHAR message[MAX_PATH];
  CHAR requiredVal[MAX_PATH] = "v14.34.31938.00";
  CHAR currentVal[MAX_PATH];

  DWORD dataSize = MAXWORD;

  LONG result = RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\X64", "Version", RRF_RT_REG_SZ, nullptr, &currentVal, &dataSize);
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

  // MessageBoxA(NULL, message, currentVal, MB_OK | MB_ICONERROR);

  return requireUpdate;
}

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

void UpdateVCRedist(TCHAR path[])
{
  // MessageBoxA(NULL, path, "Update Start", MB_OK | MB_ICONERROR);

  STARTUPINFO info = {
      sizeof(info)};
  PROCESS_INFORMATION processInfo;

  char cmdArgs[] = "VC_redist.x64.exe /Q /norestart";
  //char cmdArgs[] = "VC_redist.x64.exe /passive /norestart";

  if (CreateProcess(path, cmdArgs, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
  {
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    // MessageBoxA(NULL, path, "Update Complete", MB_OK | MB_ICONERROR);
  }
  else
  {
    // MessageBoxA(NULL, path, "Update Failed", MB_OK | MB_ICONERROR);
  }
}


BOOL execCommand(char* cmd, DWORD dwFlag) {
  DWORD size = 0;
  STARTUPINFO info = {
      sizeof(info)};
  PROCESS_INFORMATION processInfo;

  if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, dwFlag, NULL, NULL, &info, &processInfo))
  {
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return true;
  }
  return false;
}



void ExecScriptInResource(int scriptResourceID,
                          const char *szName, DWORD dwFlag)
{
  DWORD size = 0;
  const char *data = NULL;
  LoadFileInResource(scriptResourceID, TEXTFILE, size, data);

  // create the script file
  std::ofstream outfile(szName);
  outfile << data << std::endl;
  outfile.close();

  TCHAR cmdLine[MAX_PATH];
  sprintf(cmdLine, "%s\"%s\"", "cmd.exe /c ", szName);

  execCommand(cmdLine, dwFlag);
}

void ExecScriptInResource(int scriptResourceID,const char *szName) {
    TCHAR szLaunchFile[MAX_PATH];
    sprintf(szLaunchFile, "%s%s%s%s", basePath, "tmp\\",szName,".cmd");

    DWORD dwFlag = 0;
    dwFlag = CREATE_NO_WINDOW;
    ExecScriptInResource(scriptResourceID, szLaunchFile, dwFlag);
}

inline bool exist(const std::string &name)
{
  return GetFileAttributes(name.c_str()) != INVALID_FILE_ATTRIBUTES;
}

BOOL IsMissingFile(char *szFileName)
{
  if (exist(szFileName))
  {
    return false;
  }
  MessageBoxA(NULL, szFileName, "Fatal Error: Unable to locate file", MB_OK | MB_ICONERROR);
  return true;
}

auto getResFileContents(int resourceID)
{
  const char *data;
  DWORD size = 0;
  LoadFileInResource(resourceID, TEXTFILE, size, data);
  return data;
}

std::string getFileContents(char *szFile)
{
  std::string str;
  std::getline(std::ifstream(szFile), str, '\0');

  return str;
}

void saveFileContents(char *szFile, std::string str)
{
  std::ofstream cfile;
  cfile.open(szFile, std::ofstream::out | ios::trunc);
  cfile << str;
  cfile.close();
}

void enablePHPEXT(std::string &data, std::string ext)
{
  string strSearch = ";extension=";
  string strReplace = "extension=";
  strSearch.append(ext);
  strReplace.append(ext);

  replace_all(data, strSearch, strReplace);
}

BOOL IsPHPConfigValid()
{
  if (exist(szPhpConfSrc))
    return true; // if php.ini.phpbb exists

  // if php.ini-production is missing, terminate
  if (!exist(szPhpBaseConfSrc))
    return false;

  // build the php.ini.phpbb from php.ini-production
  string data = getFileContents(szPhpBaseConfSrc);

  // replace tokens
  replace_all(data, "short_open_tag = Off", "short_open_tag = On");
  replace_all(data, "max_execution_time = 30", "max_execution_time = 120");
  replace_all(data, "max_input_time = 60", "max_input_time = 120");
  replace_all(data, ";error_log = syslog", "error_log = \"%phpbrowserbox%/logs/php_error.log\"");
  replace_all(data, ";extension_dir = \"ext\"", "extension_dir = \"%phpbrowserbox%/bin/php/ext/\" ");

  enablePHPEXT(data, "curl");
  enablePHPEXT(data, "ffi");
  enablePHPEXT(data, "ftp");
  enablePHPEXT(data, "fileinfo");
  enablePHPEXT(data, "gmp");
  enablePHPEXT(data, "gd");
  enablePHPEXT(data, "gettext");
  enablePHPEXT(data, "intl");
  enablePHPEXT(data, "imap");
  enablePHPEXT(data, "mbstring");
  enablePHPEXT(data, "exif");
  enablePHPEXT(data, "mysqli");
  enablePHPEXT(data, "openssl");
  enablePHPEXT(data, "pdo_mysql");
  enablePHPEXT(data, "pdo_pgsql");
  enablePHPEXT(data, "pdo_sqlite");
  enablePHPEXT(data, "pgsql");
  enablePHPEXT(data, "soap");
  enablePHPEXT(data, "sockets");
  enablePHPEXT(data, "sodium");
  enablePHPEXT(data, "sqlite3");
  enablePHPEXT(data, "tidy");
  enablePHPEXT(data, "xsl");
  enablePHPEXT(data, "zip");

  replace_all(data, ";upload_tmp_dir =", "upload_tmp_dir = \"%phpbrowserbox%/tmp\"");
  replace_all(data, "upload_max_filesize = 2M", "upload_max_filesize = 64M");

  replace_all(data, "mysqli.default_port = 3306", "mysqli.default_port = %mysql_port%");
  replace_all(data, ";session.save_path = \"/tmp\"", "session.save_path = \"%phpbrowserbox%/tmp\"");

  data.append("openssl.cafile=\"%phpbrowserbox%/bin/cacert.pem\"\n");
  data.append("curl.cainfo=\"%phpbrowserbox%/bin/cacert.pem\"");

  saveFileContents(szPhpConfSrc, data);

  return true;
}

BOOL preprocessAppConfig(char *basePath)
{
  // remove trail from basePath e.g. e:\phpbb\ to e:\phpbb
  std::string curPath = basePath;
  curPath.pop_back();
  replace_all(curPath, "\\", "/");

  sprintf(phpbbPath, "%s", curPath.c_str());

  sprintf(szConfigFile, "%s%s", basePath, "assets\\app.json");
  sprintf(szWebkitConfigFile, "%s%s", basePath, "tmp\\webkit.json");

  sprintf(szPortScanFile, "%s%s", basePath, "tmp\\ports.txt");
  sprintf(szStartupLog, "%s%s", basePath, "tmp\\startup.txt");


  sprintf(szApacheExe, "%s%s", basePath, "bin\\apache\\bin\\httpd.exe");
  sprintf(szPhpExe, "%s%s", basePath, "bin\\php\\php.exe");
  sprintf(szWebkitExe, "%s%s", basePath, "bin\\phpbbwebkit\\phpbbwebkit.exe");

  // known php DLLs
  sprintf(szPHP8DLL, "%s%s", basePath, "bin\\php\\php8apache2_4.dll");
  sprintf(szPHP7DLL, "%s%s", basePath, "bin\\php\\php7apache2_4.dll");
  sprintf(szPHP5DLL, "%s%s", basePath, "bin\\php\\php5apache2_4.dll");

  if (exist(szPHP8DLL))
  {
    phpSAPILoader = "LoadModule php_module \"%phpbrowserbox%/bin/php/php8apache2_4.dll\"";
  }
  else if (exist(szPHP7DLL))
  {
    phpSAPILoader = "LoadModule php7_module \"%phpbrowserbox%/bin/php/php7apache2_4.dll\"";
  }
  else if (exist(szPHP5DLL))
  {
    phpSAPILoader = "LoadModule php5_module \"%phpbrowserbox%/bin/php/php5apache2_4.dll\"";
  }
  else
  {
    // default to 8
    phpSAPILoader = "LoadModule php_module \"%phpbrowserbox%/bin/php/php8apache2_4.dll\"";
  }

  sprintf(szMysqlExe, "%s%s", basePath, "bin\\mysql\\bin\\mysqld.exe");

  string mySqlHash = hashString(szMysqlExe,"MySQLPHPBb");
  sprintf(szMysqlService,"%s",mySqlHash.c_str());


  // source config
  sprintf(szApacheConfSrc, "%s%s", basePath, "bin\\apache\\conf\\httpd.conf.phpbb");

  sprintf(szPhpBaseConfSrc, "%s%s", basePath, "bin\\php\\php.ini-production");
  sprintf(szPhpConfSrc, "%s%s", basePath, "bin\\php\\php.ini.phpbb");

  sprintf(szMysqlConfSrc, "%s%s", basePath, "bin\\mysql\\my.ini.phpbb");

  // destination config
  sprintf(szApacheConfDest, "%s%s", basePath, "bin\\apache\\conf\\httpd.conf");

  sprintf(szPhpConfDest1, "%s%s", basePath, "bin\\php\\php.ini");
  sprintf(szPhpConfDest2, "%s%s", basePath, "bin\\apache\\bin\\php.ini");

  sprintf(szMysqlConfDest, "%s%s", basePath, "bin\\mysql\\my.ini");

  // prepare php config if it is not
  if (!IsPHPConfigValid())
  {
    return false;
  }

  // check for file existence start
  if (IsMissingFile(szConfigFile))
    return false;
  if (IsMissingFile(szApacheExe))
    return false;
  if (IsMissingFile(szPhpExe))
    return false;
  if (IsMissingFile(szMysqlExe))
    return false;

  if (IsMissingFile(szWebkitExe))
    return false;


  if (IsMissingFile(szApacheConfSrc))
    return false;

  if (IsMissingFile(szPhpConfSrc))
    return false;

  if (IsMissingFile(szMysqlConfSrc))
    return false;
  // check for file existence stop

  // get ports
  std::string strConfig = getFileContents(szConfigFile);

  RSJresource cfg(strConfig);

  // retrieve ports
  apachePort = cfg["ports"]["apache"].as<int>();
  mysqlPort = cfg["ports"]["mysql"].as<int>();
  useSplashScreen = cfg["splashscreen"].as<bool>();

  sprintf(phpbbname, "%s", cfg["name"].as<string>().c_str());
  sprintf(phpbbversion, "%s", cfg["version"].as<string>().c_str());

  apachePortStr = std::to_string(apachePort);
  mySqlPortStr = std::to_string(mysqlPort);

  return true;
}

int freePortScanningMode(std::string data, int port)
{
  std::string line;

  while (true)
  {
    line = "0.0.0.0:";
    line.append(std::to_string(port));

    if (data.find(line) == std::string::npos)
    {
      break;
    }
    port++;
  }
  return port;
}

void freePortScanning()
{
  std::string data = getFileContents(szPortScanFile);

  //msgbox(data.length(),szPortScanFile);

  if (apachePort == 0)
  {
    apachePort = freePortScanningMode(data, 80);
    apachePortStr = std::to_string(apachePort);
  }
  if (mysqlPort == 0)
  {
    mysqlPort = freePortScanningMode(data, 3306);
    mySqlPortStr = std::to_string(mysqlPort);
  }
}

void replace_config_tokens(
    std::string &str) {

  replace_all(str, "%phpmoduleloader%", phpSAPILoader);
  replace_all(str, "%apache_port%", apachePortStr);
  replace_all(str, "%mysql_port%", mySqlPortStr);
  replace_all(str, "%phpbrowserbox%", phpbbPath);
  replace_all(str, "%phpbbname%", phpbbname);
  replace_all(str, "%phpbbversion%", phpbbversion);
  replace_all(str, "http://localhost:80/", "http://localhost/");
}


std::string transformConfigData(char *szFile)
{
  string str = getFileContents(szFile);

  replace_config_tokens(str);

  return str;
}

void rebuildConfig()
{
  // prepare config and transform them right away
  string strApacheConfData = transformConfigData(szApacheConfSrc);
  string strMysqlConfData = transformConfigData(szMysqlConfSrc);
  string strPhpConfData = transformConfigData(szPhpConfSrc);

  // load config from json resource and save it
  std::string szWebkitConfigData = getResFileContents(IDR_CONFIG_JSON);
  replace_config_tokens(szWebkitConfigData);

  // save files
  saveFileContents(szApacheConfDest, strApacheConfData);
  saveFileContents(szMysqlConfDest, strMysqlConfData);
  saveFileContents(szPhpConfDest1, strPhpConfData);
  saveFileContents(szPhpConfDest2, strPhpConfData);

  saveFileContents(szWebkitConfigFile, szWebkitConfigData);
}

void firewallConfig(char *basePath)
{
  // Execute Prelaunch script start
  TCHAR szPrelaunchFile[MAX_PATH];
  sprintf(szPrelaunchFile, "%s%s", basePath, "tmp\\firewall.cmd");

  DWORD dwFlag = 0;
  dwFlag = CREATE_NO_WINDOW;
  ExecScriptInResource(IDR_PRE_LAUNCH_SCRIPT, szPrelaunchFile, dwFlag);
}


BOOL startupTest(char *basePath) {
  TCHAR szlaunchFile[MAX_PATH];
  sprintf(szlaunchFile, "%s%s", basePath, "tmp\\startupcheck.cmd");

  DWORD dwFlag = 0;
  dwFlag = CREATE_NO_WINDOW;
  ExecScriptInResource(IDR_STARTUP_SCRIPT, szlaunchFile, dwFlag);

  //check file
  string szStartupLogContent = getFileContents(szStartupLog);

  if (szStartupLogContent.find("Loaded Modules") == std::string::npos)
  {
       msgbox("Please troubleshoot Apache Configuration",NULL);
       return false;
  }

  return true;
}


BOOL startInternalServer(HWND hwnd) {

  STARTUPINFO infoWebkit = {0};
  STARTUPINFO infoApache = {0};

  PROCESS_INFORMATION processInfoWebkit;
  PROCESS_INFORMATION processInfoApache;

  //TCHAR initializeMySQLService[MAX_PATH];
  TCHAR installMySQLService[MAX_PATH];
  TCHAR removeMySQLService[MAX_PATH];

  TCHAR startMySQLService[MAX_PATH];
  TCHAR stopMySQLService[MAX_PATH];


  //sprintf(initializeMySQLService,"%s%s%s",szMysqlExe," --initialize-insecure --user=mysql --console",szMysqlService);
  sprintf(installMySQLService,"%s%s%s",szMysqlExe," --install-manual ",szMysqlService);
  sprintf(removeMySQLService,"%s%s%s",szMysqlExe," --remove ",szMysqlService);

  sprintf(startMySQLService,"net start %s",szMysqlService);
  sprintf(stopMySQLService,"net stop %s",szMysqlService);

  DWORD dwFlag = 0;
  dwFlag = CREATE_NO_WINDOW;

  if (!CreateProcess(NULL, szApacheExe, NULL, NULL, TRUE, dwFlag, NULL, NULL, &infoApache, &processInfoApache)) {
    msgbox("Unable to start Apache Server","Startup Error");
    return false;
  }

  //do from script
  ExecScriptInResource(IDR_INITDB_SCRIPT, "bbinit");


  if (!execCommand(installMySQLService, dwFlag)) {
    msgbox("Unable to Install MySQL Service","Startup Error");
    return false;
  }

  if (!execCommand(startMySQLService, dwFlag)) {
    msgbox("Unable to start MySQL Service","Startup Error");
    return false;
  }

  if (CreateProcess(NULL, szWebkitExe, NULL, NULL, TRUE, 0, NULL, NULL, &infoWebkit, &processInfoWebkit))
  {
    Sleep(2 * 1000);
    ShowWindow (hwnd, SW_HIDE);

    WaitForSingleObject(processInfoWebkit.hProcess, INFINITE);

    CloseHandle(processInfoWebkit.hProcess);
    CloseHandle(processInfoWebkit.hThread);

    TerminateProcess(processInfoApache.hProcess,0);

    //stop mysql
    if (!execCommand(stopMySQLService, dwFlag)) {
       msgbox("Unable to stop MySQL Service","Exit Error");
       return false;
    }

    if (!execCommand(removeMySQLService, dwFlag)) {
        msgbox("Unable to remove MySQL Service","Exit Error");
       return false;
    }


  }

  return true;
}


