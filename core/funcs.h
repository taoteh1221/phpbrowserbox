void setBasePath(char *path);
void makeTempFolder();

void CreateFolder(const char *path);
void LoadFileInResource(int name, int type, DWORD &size, const char *&data);

void ExecScriptInResource(int scriptResourceID,const char *szName);
void ExecScriptInResource(int scriptResourceID, const char *szName, DWORD dwFlag);

void LoadSplashImage(HWND hWnd);
void UpdateVCRedist(TCHAR path[]);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void getEXEPath(char *&exePath, const char *szFileName);

BOOL preprocessAppConfig(char *basePath);

void firewallConfig(char *basePath);

void freePortScanning();

void rebuildConfig();

BOOL startInternalServer(HWND hwnd);

BOOL startupTest(char *basePath);

std::string getFileContents(char *szFile);

void msgbox(char *message, char *title);
void msgbox(std::string message, char *title);
void msgbox(int message, char *title);

BOOL DoesVCRedistNeedUpdate();
BOOL CreateApplicationWindow(HINSTANCE hThisInstance,
                             HINSTANCE hPrevInstance,
                             LPSTR lpszArgument,
                             int nCmdShow, HWND &hwnd);
