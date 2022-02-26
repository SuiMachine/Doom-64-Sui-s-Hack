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

	HookJmpTrampoline(0x140130900, handleInput2, 0xE4);
}
