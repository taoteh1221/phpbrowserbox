Set WshShell = CreateObject("WScript.Shell") 
WshShell.Run "..\phpbrowserbox.exe /shell", 1, True

Set WshShell = Nothing