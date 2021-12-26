#include "./ProcessModule.h"
#include <TlHelp32.h>

ProcessModule::operator uintptr_t() const
{
	return address;
}

ProcessModule::ProcessModule(const char* name) : name(name), h(nullptr), size(0), address(0)
{
}

ProcessStatus ProcessModule::open(const ProcessMemory& memory) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, memory.pid);
	if (snapshot == INVALID_HANDLE_VALUE)
        return ProcessStatus::InvalidSnapshot;

	MODULEENTRY32 entry{};
	entry.dwSize = sizeof(entry);

	do {
		if (!lstrcmpi(entry.szModule, name)) {
			CloseHandle(snapshot);
			address = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
			size = entry.modBaseSize;
			h = entry.hModule;
			return ProcessStatus::Ok;
		}
	} while (Module32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return ProcessStatus::NotFound;
}
