#include "PMemHelperEx.h"

PMemHelperEx::PMemHelperEx() {
	// Offsets for Windows 10 21H2
	EPNameOffset = 0x5a8;		// EPROCESS->ImageFileName
	EPPidOffset = 0x440;		// EPROCESS->UniqueProcessId
	EPDirBaseOffset = 0x28;		// EPROCESS->Pcb.DirectoryTableBase
	EPBaseOffset = 0x520;		// EPROCESS->SectionBaseAddress
	EPLinkOffset = 0x448;		// EPROCESS->ActiveProcessLinks
	
	// Map physical memory into ramImage from 0 to end of last page (mMemInfo[mInfoCount - 1].End)
	
}

PMemHelperEx::~PMemHelperEx() {
	// Unmap physical memory from ramImage

}