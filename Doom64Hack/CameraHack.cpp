#include "CameraHack.h"


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



/*
This function also is used in input, but seems less so - might be something with acceleration or non-interpolated ticks?

signed __int64 __fastcall handleInput1(__int64 a1, __int64 a2, unsigned __int64 a3)
{
	long* v3; // rbx
	float v4; // xmm4_4
	int v5; // eax
	signed __int64 result; // rax
	__int64 v9; // rdx
	int v10; // edx
	__int64 v11; // rdx

	v3 = (long*)a1;
	if (!(*(BYTE*)(a1 + 548) & 4))
	{
		*(DWORD*)(*(LONG*)a1 + 0x68i64) += 2 * *(DWORD*)(a1 + 0x14);
		v4 = *(float*)(a1 + 0x270);
		if (v4 != 0.0)
		{
			float v6 = v4;
			float v7 = 0.0;

			v5 = normalizeInput(reinterpret_cast<float*>(*(unsigned __int64*)&v4 & *(unsigned __int64*)0x140419CC0));
			if (v6 < v7)
				*(DWORD*)(*v3 + 0x68) += v5;       // Turn left
			else
				*(DWORD*)(*v3 + 0x68) -= v5;       // Turn right
		}
	}
	result = *((unsigned int*)v3 + 3);
	if ((DWORD)result && *((BYTE*)v3 + 560))
	{
		v9 = 2 * (signed int)result;
		a3 = (unsigned __int64) * (unsigned int*)(*v3 + 0x68i64) >> 19;
		*(DWORD*)(*v3 + 0x88i64) += v9 * ((DWORD*)0x1405ED8A0)[a3 + 2048] >> 16;
		result = v9 * ((DWORD*)0x1405ED8A0)[a3] >> 16;
		*(DWORD*)(*v3 + 140i64) += result;
	}
	v10 = *((DWORD*)v3 + 4);
	if (v10 && *((BYTE*)v3 + 560))
	{
		v11 = 2 * v10;
		a3 = (unsigned __int64)(unsigned int)(*(DWORD*)(*v3 + 0x68i64) - 0x40000000) >> 19;
		*(DWORD*)(*v3 + 0x88i64) += v11 * ((DWORD*)0x1405ED8A0)[a3 + 2048] >> 16;
		result = v11 * ((DWORD*)0x1405ED8A0)[a3] >> 16;
		*(DWORD*)(*v3 + 0x8Ci64) += result;
	}
	if (*((DWORD*)v3 + 3) || *((DWORD*)v3 + 4))
	{
		result = (signed __int64)&*(intptr_t*)0x1403D8B30;
		if (*(void**)(*v3 + 0xA8) == &*(intptr_t*)0x1403D8B30)
			result = unknownFunction(*(DWORD*)v3, 2);
	}
	*((DWORD*)v3 + 7) &= 0xFFFFFEFF;
	*((DWORD*)v3 + 40) = 0;
	return result;
}
*/

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


void handleInput2()
{
	__int64 v0; // rax
	__int64 v1; // rsi
	__int64 v2; // rbx
	__int64 v3; // rdi

	if (!*(BYTE*)0x14073A3C4 && (unsigned __int8)unknownFunction2((__int64)0x140716FB0, 0))
	{
		if (*(DWORD*)0x14073A3D0 == *(DWORD*)0x140695E00)
		{
			v0 = unknownArrayFunction();
			v1 = v0;
			if (!*(DWORD*)(v0 + 8))
			{
				v2 = v0 + 592;
				v3 = *(ULONG*)v0;
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
	//unknownFunction = (unknownFunctionC)0x14015C130;

	//Used by call input functions?
	normalizeInput = (normalizeInputC)0x000000014018E7C0;


	//Functions used by input function 2
	unknownFunction2 = (unknownFunctionC2)0x140054AA0;
	unknownArrayFunction = (unknownArrayFunctionT)0x140130860;
	unknownArrayFlagsFunction = (unknownArrayFlagsFunctionT)0x1401774A0;
	unknownHandleInputFunction = (unknownHandleInputFunctionT)0x140177930;


	HookJmpTrampoline(0x140130900, handleInput2, 0xE4);
}
