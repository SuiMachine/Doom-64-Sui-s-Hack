#include "OtherHacks.h"

void OtherHacks::DisableCheatFlagForFile()
{
	auto result = NopRegion(0x000000014005DDB4, 7);
	auto result2 = NopRegion(0x000000014018F7DC, 7);
}
