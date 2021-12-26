#include "./ProcessMemory.h"
#include <TlHelp32.h>

ProcessMemory::ProcessMemory(const char* name) : name(name) {}

ProcessStatus ProcessMemory::write(uintptr_t address, size_t size, const void* value) const
{
	size_t bytes_read = 0;
	if (WriteProcessMemory(handle, reinterpret_cast<void*>(address), value, size, &bytes_read))
        return ProcessStatus::Ok;
    return ProcessStatus::Unknown;
}

ProcessStatus ProcessMemory::read(uintptr_t address, size_t size, void* value) const
{
	size_t bytes_read = 0; 
	if (ReadProcessMemory(handle, reinterpret_cast<void*>(address), value, size, &bytes_read))
        return ProcessStatus::Ok;
    return ProcessStatus::Unknown;
}

ProcessMemory::~ProcessMemory() {
	close();
}

ProcessStatus ProcessMemory::close() {
	if (!handle || !CloseHandle(handle)) return ProcessStatus::HandleClose;

	pid = NULL;
	handle = nullptr;
	return ProcessStatus::Ok;
}

ProcessStatus ProcessMemory::open() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(entry);

	do {
		if (!lstrcmpi(entry.szExeFile, name)) {
			CloseHandle(snapshot);
			pid = entry.th32ProcessID;
			handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			return ProcessStatus::Ok;
		}
	} while (Process32Next(snapshot, &entry));
	
	CloseHandle(snapshot);
	return ProcessStatus::NotFound;
}
