// simpleExternal.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>


const int pointerSize = 4; 


bool check(void* x) {
	if (x == nullptr) {
		std::cout << "The program has crashed\n" << GetLastError();
		exit(-1);
	}
	return 1;
}

template <typename R>
R getPointerChainValue(HANDLE procHandle, std::vector<uintptr_t> vec){
	uintptr_t currentAddr = vec[0];
	R readMem;
	for (int i = 0; i < vec.size() - 1; ++i) {
		if (i >= 1) {
			currentAddr += vec[i];
		}

		ReadProcessMemory(procHandle, (LPCVOID)currentAddr, &currentAddr, pointerSize, NULL);
		check((void*)currentAddr);
		//std::cout << std::hex << "0x" << currentAddr << '\n';
	}
	ReadProcessMemory(procHandle, (LPCVOID)currentAddr, &readMem, sizeof(int), NULL);
	return readMem;
}


int main()
{
	DWORD proc = 8172;
	
	HANDLE procHandle = OpenProcess(PROCESS_VM_READ| PROCESS_VM_WRITE, false, proc);
	
	check((void*)procHandle);

	int* ptr, readInt = 0;

	std::vector<uintptr_t> vec = { 0x008AFA08, 0, 0, 0 };

	readInt = getPointerChainValue<int>(procHandle, vec);

	std::cout << readInt << '\n';

	CloseHandle(procHandle);

    return 0;
}

