inline BOOL useSplashScreen = true;

void setBasePath(char *path);
void makeTempFolder();
void CreateFolder(const char *path);

void LoadSplashImage(HWND hWnd);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void getEXEPath(char *&exePath, const char *szFileName);

std::string getFileContents(char *szFile);

BOOL CreateApplicationWindow(HINSTANCE hThisInstance,
                             HINSTANCE hPrevInstance,
                             LPSTR lpszArgument,
                             int nCmdShow, HWND &hwnd);
