#pragma once
#include "PMemHelper.h"

class PMemHelperEx : public PMemHelper
{
public:
	PMemHelperEx();
	~PMemHelperEx();

	template<typename T, typename... Args>
	auto CallFunction(uint64_t fn_address, Args&&... args)
		->std::enable_if_t<!std::is_void<std::invoke_result_t<T, Args...>>::value,
		std::invoke_result_t<T, Args...>>
	{
		// Kernel NtShutdownSystem (we change this)
		uint64_t ntos_shutdown = SFGetModuleExport("ntoskrnl.exe", "NtShutdownSystem");

		// Process NtShutdownSystem (we call this)
		uint64_t nt_shutdown = (uint64_t)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtShutdownSystem");

		// Create payload
		BYTE jmp_bytes[14] = {
			0xff, 0x25, 0x00, 0x00, 0x00, 0x00,			// jmp [RIP+0x00000000]
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00		// RIP value
		};

		// Set jump address
		*reinterpret_cast<uint64_t*>(jmp_bytes + 6) = fn_address;

		// Save original bytes
		BYTE orig_bytes[sizeof(jmp_bytes)];
		ReadVirtual(GetKernelDirBase(), ntos_shutdown, orig_bytes, sizeof(orig_bytes));

		// Write payload
		WriteVirtual(GetKernelDirBase(), ntos_shutdown, jmp_bytes, sizeof(jmp_bytes));

		// Call function
		auto result = std::invoke(reinterpret_cast<T>(nt_shutdown), std::forward<Args>(args)...);
		
		// Restore original bytes
		WriteVirtual(GetKernelDirBase(), ntos_shutdown, orig_bytes, sizeof(orig_bytes));
		
		return result;
	}

	template<typename T, typename... Args>
	auto CallFunction(uint64_t fn_address, Args&&... args)
		-> std::enable_if_t<std::is_void<std::invoke_result_t<T, Args...>>::value,
		void>
	{
		// Kernel NtShutdownSystem (we change this)
		uint64_t ntos_shutdown = SFGetModuleExport("ntoskrnl.exe", "NtShutdownSystem");

		// Process NtShutdownSystem (we call this)
		uint64_t nt_shutdown = (uint64_t)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtShutdownSystem");

		// Create payload
		BYTE jmp_bytes[14] = {
			0xff, 0x25, 0x00, 0x00, 0x00, 0x00,			// jmp [RIP+0x00000000]
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00		// RIP value
		};

		// Set jump address
		*reinterpret_cast<uint64_t*>(jmp_bytes + 6) = fn_address;

		// Save original bytes
		BYTE orig_bytes[sizeof(jmp_bytes)];
		ReadVirtual(GetKernelDirBase(), ntos_shutdown, orig_bytes, sizeof(orig_bytes));

		// Write payload
		WriteVirtual(GetKernelDirBase(), ntos_shutdown, jmp_bytes, sizeof(jmp_bytes));
		
		// Call NtShutdownSystem
		std::invoke(reinterpret_cast<T>(nt_shutdown), std::forward<Args>(args)...);
		
		// Restore original bytes
		WriteVirtual(GetKernelDirBase(), ntos_shutdown, orig_bytes, sizeof(orig_bytes));
	}
};

