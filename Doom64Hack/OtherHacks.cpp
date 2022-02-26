#include "OtherHacks.h"

void OtherHacks::DisableCheatFlagForFile()
{
	auto result = NopRegion(0x14019A1B1, 7);
	auto result2 = NopRegion(0x14019A1C5, 0x17);
}

