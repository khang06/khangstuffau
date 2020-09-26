#include "pch-il2cpp.h"

#include "QPatch.h"

QPatch::QPatch(void * ad, BYTE* n_bytes, size_t sz)
{
	addr = ad;
	size = sz;
	new_bytes = new BYTE[sz];
	orig_bytes = new BYTE[sz];
	memcpy(new_bytes, n_bytes, sz);
}

bool QPatch::patch()
{
	if (VirtualProtect((void*)addr, size, PAGE_EXECUTE_READWRITE, &oldproc) == 0)
	{
		std::cout << "VirtualProtect failed!" << std::endl;
		return false;
	}

	// save bytes and replace
	memcpy(orig_bytes, addr, size);
	memcpy(addr, new_bytes, size);

	if (VirtualProtect(addr, size, oldproc, &oldproc) == 0)
	{
		std::cout << "VirtualProtect failed!" << std::endl;
		return false;
	}

	return true;
}

bool QPatch::unpatch()
{
	if (VirtualProtect((void*)addr, size, PAGE_EXECUTE_READWRITE, &oldproc) == 0)
	{
		std::cout << "VirtualProtect failed!" << std::endl;
		return false;
	}

	// replace patched bytes with original
	memcpy(addr, orig_bytes, size);

	// return 
	if (VirtualProtect(addr, size, oldproc, &oldproc) == 0)
	{
		std::cout << "VirtualProtect failed!" << std::endl;
		return false;
	}

	return true;
}
