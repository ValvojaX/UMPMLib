#pragma once
#include "PMemHelper.h"

class PMemHelperEx : public PMemHelper
{
public:
	PMemHelperEx();
	~PMemHelperEx();

	// System calls
	template <class T, class ... Args>
	auto syscall(uint64_t fn_address, Args ... args) {
		// Kernel NtShutdownSystem (we change this)
		uint64_t ntos_shutdown = SFGetModuleExport("ntoskrnl.exe", "NtShutdownSystem");

		// Process NtShutdownSystem (we call this)
		uint64_t nt_shutdown = (uint64_t)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtShutdownSystem");

		// Create payload
		BYTE jmp_bytes[14] = {
			0xff, 0x25, 0x00, 0x00, 0x00, 0x00,					// absolute JMP
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00		// JMP address
		};

		// Set jump address
		*reinterpret_cast<uint64_t*>(jmp_bytes + 6) = fn_address;

		// Save original bytes
		BYTE orig_bytes[sizeof(jmp_bytes)];
		ReadVirtual(GetKernelDirBase(), ntos_shutdown, orig_bytes, sizeof(orig_bytes));

		// Write payload
		WriteVirtual(GetKernelDirBase(), ntos_shutdown, jmp_bytes, sizeof(jmp_bytes));

		// Call NtShutdownSystem
		auto result = reinterpret_cast<T>(nt_shutdown)(args ...);

		// Restore original bytes
		WriteVirtual(GetKernelDirBase(), ntos_shutdown, orig_bytes, sizeof(orig_bytes));

		return result;
	}
};

