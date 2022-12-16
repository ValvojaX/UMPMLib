#include "PhysicalMemory.h"

int isAscii(int c)
{
	return((c >= 'A' && c <= 'z') || (c >= '0' && c <= '9') || c == 0x20 || c == '@' || c == '_' || c == '?');
}

int isPrintable(uint32_t uint32)
{
	if ((isAscii((uint32 >> 24) & 0xFF)) && (isAscii((uint32 >> 16) & 0xFF)) && (isAscii((uint32 >> 8) & 0xFF)) &&
		(isAscii((uint32) & 0xFF)))
		return true;
	else
		return false;
}
