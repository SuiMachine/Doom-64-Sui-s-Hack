#include <Windows.h>
#include <psapi.h>
#include "CameraHack.h"
#include "OtherHacks.h"
#include "../externals/inireader/IniReader.h"

void StrToLower(char* chrArray, int Lenght)
{
	for (int i = 0; i < Lenght; i++)
	{
		chrArray[i] = ::tolower(chrArray[i]);
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		auto baseModule = GetModuleHandle(NULL);
		char baseModuleName[MAX_PATH];
		GetModuleFileName(baseModule, baseModuleName, sizeof(baseModuleName));
		StrToLower(baseModuleName, sizeof(baseModuleName));
		if (strstr(baseModuleName, "doom64_x64.exe") != 0)
		{
			MODULEINFO moduleInfo;
			GetModuleInformation(GetCurrentProcess(), baseModule, &moduleInfo, sizeof(moduleInfo));

			if (moduleInfo.SizeOfImage == 8491008)
			{
				CIniReader configReader("");

				if (configReader.ReadBoolean("MAIN", "EnableYAxisHack", false))
					CameraHack();

				if(configReader.ReadBoolean("MAIN", "FileDoesntSetCheatFlag", false))
					OtherHacks::DisableCheatFlagForFile();

				if (configReader.ReadBoolean("MAIN", "CorrectBaronOfHellMeleeDamage", false))
					OtherHacks::CorrectBaronOfHellMeleeDamage();
			}
			else
			{
				MessageBox(NULL, "Invalid exe size. Nothing was applied.", "Notification - Sui's Hack", MB_OK | MB_ICONEXCLAMATION);
			}

		}

	}
	else if (reason == DLL_PROCESS_DETACH) {
	}
	return TRUE;
}
