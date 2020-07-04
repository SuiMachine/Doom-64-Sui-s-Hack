#include "OtherHacks.h"

void OtherHacks::DisableCheatFlagForFile()
{
	auto result = NopRegion(0x000000014005DDB4, 7);
	auto result2 = NopRegion(0x000000014018F7DC, 7);
}

void OtherHacks::CorrectBaronOfHellMeleeDamage()
{
	/*	
	See https://twitter.com/SVKaiser/status/1278507698846183424 - unless Twitter with its great totaliterian lean removes it
	function located at 0x0000000140153F90 in Steam version decompiles to: (hitdice[rngElement] & 7) + 2 * (hitdice[rngElement] & 7) + 11)
	should be (hitdice[rngElement] & 7) * 11 + 11 
	*/
	//0000000140153FDD - 0xB - 44 8D 0C 4D 0B 00 00 00 44 03 C9 -> 6B C9 0B 83 C1 0B 4C 8B C9 90 90 
	byte overrideCode[] = { 0x6B, 0xC9, 0x0B, 0x83, 0xC1, 0x0B, 0x4C, 0x8B, 0xC9, 0x90, 0x90 };
	auto result = OverrideWithCode(0x0000000140153FDD, overrideCode, 0xB);

}
