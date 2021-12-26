#pragma once
#include <Windows.h>

enum class ProcessStatus
{
    Ok = 0,
    NotFound = 1,
    InvalidSnapshot = 2,
    HandleClose = 3,
    Unknown = 4
};

class ProcessMemory {
public:
	HANDLE handle = nullptr;
	DWORD pid = 0;
	const char* name = nullptr;

	ProcessMemory(const char* name);
	~ProcessMemory();

    ProcessStatus write(uintptr_t address, size_t size, const void* value) const;
    ProcessStatus read(uintptr_t address, size_t size, void* value) const;

	template<class var>
    ProcessStatus write(uintptr_t address, const var& value) {
        return write(address, sizeof(var), (void*)&value);
	}

	template<class var>
    ProcessStatus read(uintptr_t address, var& value) {
		return read(address, sizeof(var), &value);
	}

    ProcessStatus open();
    ProcessStatus close();
};
