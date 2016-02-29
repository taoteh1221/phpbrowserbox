#include <iostream>
#include <unistd.h>
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

FILE *fr;            /* declare the file pointer */

void Stealth()
{
 HWND Stealth;
 AllocConsole();
 Stealth = FindWindowA("ConsoleWindowClass", NULL);
 ShowWindow(Stealth,0);
}

int main(int argc, char** argv) {
	//std::cout << "Hello world!\n";
	
	STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo;

    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(STARTUPINFO);
    StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
    StartupInfo.wShowWindow = SW_HIDE;

    //to hide console window
    //Stealth();

    //system("title PHPBrowserBox");
    STARTUPINFO si = {};
    si.cb = sizeof si;

    PROCESS_INFORMATION pi = {};
    PROCESS_INFORMATION pi1 = {};
    PROCESS_INFORMATION pi2 = {};


    char the_path[256];
    char port[80];
    char cmd[250];
    char params[250];
    char cfile[250];

    char wcmd[250];
    char wparams[250];

    //grab app path
    getcwd(the_path, 255);

    //get server port
    sprintf(cfile, "%s\\support\\port.ini",the_path);
    fr = fopen (cfile, "rt");  /* open the file for reading */
    fgets(port, 80, fr);
    fclose(fr);  /* close the file prior to exiting the routine */


    //start mysqld at this point
    sprintf(cmd, "%s\\support\\mysql\\bin\\mysqld.exe",the_path);
    if ( !CreateProcess(cmd, 0, 0, FALSE, 0, 0, 0, 0, &StartupInfo,&pi1) )
    {
       printf("MySQL failed to start %s \n",cmd);
    } else {
       printf("MySQL launched\n");
    }


    //switch to the correct directory
    chdir("support\\inetpub");

    //initiate php server
    sprintf(cmd, "%s\\support\\php\\php.exe",the_path);
    sprintf(params, "php.exe -S localhost:%s ..\\php\\router.php -c php.ini",port);
    if ( !CreateProcess(cmd,params, 0, FALSE, 0, 0, 0, 0, &StartupInfo,&pi2))
    {
       printf("PHP failed to start \n");
    } else {
       printf("PHP launched on port %s\n",port);
    }


    sprintf(wcmd, "%s\\support\\webkit\\webkit.exe",the_path);
    sprintf(wparams, "webkit.exe http://localhost:%s",port);

    if(CreateProcess(wcmd, wparams, 0, FALSE, 0, 0, 0, 0, &si,&pi)) {
    printf("Launching webkit engine here \n");

    //wait for target to shutdown at this point
    WaitForSingleObject(pi.hProcess, INFINITE);
    } else {
    printf("Path %s",wcmd);
    printf("Unable to launch webkit engine here \n");
    }


    //close mysql and php handle
    TerminateProcess(pi1.hProcess,0);
    TerminateProcess(pi2.hProcess,0);

	//system("pause");

	return 0;
}
