#include <Windows.h>
#include "PMemHelperEx.h"

void main()
{
	PMemHelperEx *mem = new PMemHelperEx();
	uint8_t buffer[255];
	auto cr3 = mem->GetDirBase(GetCurrentProcessId()); // Put the PID of the process you want to read
	auto base = mem->GetProcessBase(GetCurrentProcessId()); // Put the PID of the process you want to read

	//Do all your reading here 
	mem->ReadVirtual(cr3, base, buffer, sizeof(buffer));
	return ;

}