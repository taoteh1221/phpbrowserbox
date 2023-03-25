inline BOOL useSplashScreen = true;

inline TCHAR szLogFileName[MAX_PATH];
inline TCHAR szbbWebKit[MAX_PATH];
inline TCHAR szbbWebKitLog[MAX_PATH];
inline TCHAR szbbStartCmd[MAX_PATH];
inline TCHAR szbbSplashApp[MAX_PATH];
inline TCHAR szbbWebKitRelaunch[MAX_PATH];
inline HANDLE hMutex;

void setBasePath(char *path);
void makeTempFolder();
void CreateFolder(const char *path);

BOOL execCommand(char* cmd, DWORD dwFlag,bool wait);
BOOL execCommand2(char *cmd, DWORD dwFlag, bool wait);
BOOL startWebkitEngine();
inline PROCESS_INFORMATION webkitProcessInfo;
void waitForWebkitToExit();




void LoadSplashImage(HWND hWnd);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void getEXEPath(char *&exePath, const char *szFileName);

std::string getFileContents(char *szFile);

BOOL CreateApplicationWindow(HINSTANCE hThisInstance,
                             HINSTANCE hPrevInstance,
                             LPSTR lpszArgument,
                             int nCmdShow, HWND &hwnd);

bool exist(char * name);
