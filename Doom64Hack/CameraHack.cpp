#include "CameraHack.h"

#include <stdio.h>
#include <wchar.h>

typedef signed __int64(__fastcall* normalizeInputC)(float turnAmount);
normalizeInputC normalizeInput;

typedef char(__fastcall* unknownFunctionC)(__int64 a1, int a2);
unknownFunctionC unknownFunction;

typedef __int64(__fastcall* unknownFunctionC2)(__int64 a1, int a2);
unknownFunctionC2 unknownFunction2;

typedef __int64(* unknownArrayFunctionT)();
unknownArrayFunctionT unknownArrayFunction;

typedef void(__fastcall* unknownArrayFlagsFunctionT)(__int64 a1, int a2);
unknownArrayFlagsFunctionT unknownArrayFlagsFunction;

typedef __int64(__fastcall* unknownHandleInputFunctionT)(__int64 a1, int a2);
unknownHandleInputFunctionT unknownHandleInputFunction;

typedef char(__fastcall* unknownHandleInputFunctionF1T)(__int64 a1, int a2);
unknownHandleInputFunctionF1T unknownHandleInputFunction2;



float convertValueForNormalization(float valueToConvert)
{
	auto castToDouble = (double)valueToConvert;
	auto doubleLoadedToXMM = _mm_load_sd(&castToDouble);
	auto bitWiseDWORD1 = _mm_loadu_pd((const double*)0x140419CC0);
	auto full128Word = _mm_loadl_pd(bitWiseDWORD1, (const double*)0x140419CC0);

	auto bitwiseNegate = _mm_and_pd(doubleLoadedToXMM, full128Word);
	auto whateverThatIs = _mm_cvtpd_ps(bitwiseNegate);
	return _mm_cvtss_f32(whateverThatIs);
}

char __fastcall handleInput1(__int64 a1, __int64 a2, unsigned __int64 a3)
{
	float v4; // xmm4_4
	float v6; // xmm4_4
	float v7; // xmm5_4
	signed __int64 v8; // rax
	__int64 v9; // rdx
	unsigned __int64 v10; // r8
	int v11; // edx
	__int64 v12; // rdx
	unsigned __int64 v13; // r8

	if (!(*(BYTE*)(a1 + 0x224) & 4))
	{
		*(DWORD*)(*(ULONG*)a1 + 0x68i64) += 2 * *(DWORD*)(a1 + 0x14);
		*(DWORD*)(*(ULONG*)a1 + 0x6Ci64) += 2 * *(DWORD*)(a1 + 0x18);


		float inputXAxis = *(float*)(a1 + 0x270);
		if (inputXAxis != 0.0)
		{
			int normalizedRotationX = normalizeInput(convertValueForNormalization(inputXAxis));
			if (inputXAxis < 0.0f)
				*(DWORD*)(*(ULONG*)a1 + 0x68i64) += normalizedRotationX;// Turn left
			else
				*(DWORD*)(*(ULONG*)a1 + 0x68i64) -= normalizedRotationX;// Turn right
		}

		float inputYAxis = *(float*)(a1 + 0x274);
		if (inputYAxis != 0.0)
		{
			int normalizedRotationY = normalizeInput(convertValueForNormalization(inputYAxis));

			auto currentOrientationY = *(int*)(*(ULONG*)a1 + 0x6Ci64);
			if (inputYAxis < 0.0f)
			{
				auto newYOrientation = currentOrientationY + normalizedRotationY;
				if (1104062110 < (currentOrientationY + normalizedRotationY))
					*(int*)(*(ULONG*)a1 + 0x6Ci64) = 1104062110;
				else
					*(int*)(*(ULONG*)a1 + 0x6Ci64) = newYOrientation;
			}
			else
			{
				auto newYOrientation = currentOrientationY - normalizedRotationY;
				if (-1071386491 > newYOrientation)
					*(int*)(*(ULONG*)a1 + 0x6Ci64) = -1071386491;
				else
					*(int*)(*(ULONG*)a1 + 0x6Ci64) = newYOrientation;
			}
		}

		/*
		wchar_t text[14] = { 0 };
		swprintf(text, 14, L"%d, %d\n", inputXAxis, inputYAxis);
		OutputDebugStringW(text);	*/	
	}
	v8 = *(DWORD*)(a1 + 12);

	int* someArray = (int*)(0x1405ED8A0);
	if ((DWORD)v8 && *(BYTE*)(a1 + 560))
	{
		v9 = 2 * (signed int)v8;
		v10 = (unsigned __int64)*(unsigned int*)(*(ULONG*)a1 + 0x68i64) >> 19;
		*(DWORD*)(*(ULONG*)a1 + 0x88i64) += v9 * someArray[v10 + 2048] >> 16;
		v8 = v9 * someArray[v10] >> 16;
		*(DWORD*)(*(ULONG*)a1 + 140i64) += v8;
	}
	v11 = *(DWORD*)(a1 + 16);
	if (v11 && *(BYTE*)(a1 + 560))
	{
		v12 = 2 * v11;
		v13 = (unsigned __int64)(unsigned int)(*(DWORD*)(*(ULONG*)a1 + 0x68i64) - 0x40000000) >> 19;
		*(DWORD*)(*(ULONG*)a1 + 0x88i64) += v12 * someArray[v13 + 2048] >> 16;
		v8 = v12 * someArray[v13] >> 16;
		*(DWORD*)(*(ULONG*)a1 + 0x8Ci64) += v8;
	}
	if (*(DWORD*)(a1 + 12) || *(DWORD*)(a1 + 16))
	{
		v8 = 0x1403D8B30;

		if (*(signed long long**)(*(ULONG*)a1 + 0xA8i64) == &v8)
		{
			auto result = unknownHandleInputFunction2(*(ULONG*)a1, 2);
			v8 &= ~255;
			v8 |= result;
		}
	}
	*(DWORD*)(a1 + 28) &= 0xFFFFFEFF;
	*(DWORD*)(a1 + 160) = 0;
	return v8;
}


void handleInput2()
{

	if (!*(BYTE*)0x14073A3C4 && (unsigned __int8)unknownFunction2((__int64)0x140716FB0, 0))
	{
		if (*(DWORD*)0x14073A3D0 == *(DWORD*)0x140695E00)
		{
			__int64 v0 = unknownArrayFunction();
			__int64 v1 = v0;
			if (!*(DWORD*)(v0 + 8))
			{
				__int64 v2 = v0 + 592;
				__int64 v3 = *(ULONG*)v0;
				if (*(ULONG*)v0)
				{
					unknownArrayFlagsFunction(v0 + 592, 0i64);
					unknownHandleInputFunction(v2, 0);
					if (!(*(BYTE*)(v1 + 548) & 4))
					{
						float inputXAxis = *(float*)(v2 + 0x20);
						if (inputXAxis != 0.0)
						{
							int normalizedRotationX = normalizeInput(convertValueForNormalization(inputXAxis));


							if (inputXAxis < 0.0f)
								*(DWORD*)(v3 + 0x68) += normalizedRotationX;    // Turn Left
							else
								*(DWORD*)(v3 + 0x68) -= normalizedRotationX;    // Turn right
						}

						float inputYAxis = *(float*)(v2 + 0x24);
						if (inputYAxis != 0.0)
						{
							int normalizedRotationY = normalizeInput(convertValueForNormalization(inputYAxis));

							//unlike X rotation, Y rotation needs to be clamped, so have to read the value and compare it against limits
							auto currentOrientationY = *(int*)(v3 + 0x6C);
							if (inputYAxis < 0.0f)
							{
								auto newYOrientation = currentOrientationY + normalizedRotationY;
								if (1104062110 < (currentOrientationY + normalizedRotationY))
									*(int*)(v3 + 0x6C) = 1104062110;
								else
									*(int*)(v3 + 0x6C) = newYOrientation;
							}
							else
							{
								auto newYOrientation = currentOrientationY - normalizedRotationY;
								if(-1071386491 > newYOrientation)
									*(int*)(v3 + 0x6C) = -1071386491;
								else
									*(int*)(v3 + 0x6C) = newYOrientation;
							}
						}
					}
					*(DWORD*)(v2 + 108) = 1;
					*(ULONG*)(v2 + 32) = 0i64;
					*(ULONG*)(v2 + 40) = 0i64;
					*(ULONG*)(v2 + 48) = 0i64;
				}
			}
		}
		else
		{
			*(DWORD*)0x14073A3D0 = *(DWORD*)0x140695E00;
		}
	}
}


CameraHack::CameraHack()
{
	//Used by call input functions?
	normalizeInput = (normalizeInputC)0x000000014018E7C0;


	//Functions used by input function 2
	unknownFunction2 = (unknownFunctionC2)0x140054AA0;
	unknownArrayFunction = (unknownArrayFunctionT)0x140130860;
	unknownArrayFlagsFunction = (unknownArrayFlagsFunctionT)0x1401774A0;
	unknownHandleInputFunction = (unknownHandleInputFunctionT)0x140177930;
	unknownHandleInputFunction2 = (unknownHandleInputFunctionF1T)0x14015C130;


	HookJmpTrampoline(0x140176E20, handleInput1, 0xE4);
	HookJmpTrampoline(0x140130900, handleInput2, 0xE4);
}
