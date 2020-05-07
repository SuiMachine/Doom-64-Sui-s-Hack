#pragma once
#include <Windows.h>

static void UnprotectModule(HMODULE moduleHandle)
{
	//This function was provided by Orfeasz
	PIMAGE_DOS_HEADER s_Header = (PIMAGE_DOS_HEADER)moduleHandle;
	PIMAGE_NT_HEADERS s_NTHeader = (PIMAGE_NT_HEADERS)((intptr_t)moduleHandle + s_Header->e_lfanew);

	SIZE_T s_ImageSize = s_NTHeader->OptionalHeader.SizeOfImage;

	DWORD s_OldProtect;
	VirtualProtect((LPVOID)moduleHandle, s_ImageSize, PAGE_EXECUTE_READWRITE, &s_OldProtect);
}

static bool HookJmpTrampoline(intptr_t targetToHook, void* ourFunction, int overrideLenght)
{
	if (overrideLenght < 9)
		return false;

	DWORD curProtectionFlag;
	VirtualProtect((void*)targetToHook, overrideLenght, PAGE_EXECUTE_READWRITE, &curProtectionFlag);
	memset((void*)targetToHook, 0x90, overrideLenght);

	*(BYTE*)targetToHook = 0x48;
	*(BYTE*)(targetToHook + 1L) = 0xB8;
	*(intptr_t*)((intptr_t)targetToHook + 2L) = (intptr_t)ourFunction;
	*(BYTE*)(targetToHook + 10L) = 0xFF;
	*(BYTE*)(targetToHook + 11L) = 0XE0;

	DWORD temp;
	VirtualProtect((void*)targetToHook, overrideLenght, curProtectionFlag, &temp);
	return true;
}

static bool NopRegion(intptr_t address, int lenght)
{
	DWORD curProtectionFlag;
	VirtualProtect((void*)address, lenght, PAGE_EXECUTE_READWRITE, &curProtectionFlag);
	memset((void*)address, 0x90, lenght);

	DWORD temp;
	VirtualProtect((void*)address, lenght, curProtectionFlag, &temp);
	return true;
}