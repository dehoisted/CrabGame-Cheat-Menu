#pragma once
#include "./ProcessMemory.h"

class ProcessModule {
public:
	const char* name;
	HMODULE h;
	size_t size;
	uintptr_t address;
    explicit ProcessModule(const char* name);
    ProcessStatus open(const ProcessMemory& memory);
    operator uintptr_t() const;
};
