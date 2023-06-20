#include <iostream>
#include <Windows.h>
#include <process.h>

struct Data {
	INT size;
	PINT array;
};

DWORD WINAPI Worker(LPVOID a) {
	Data* s = (Data*)(a);
	DWORD sum = 0;
	for (INT i = 0; i < s->size; ++i) {
		sum += s->array[i] * s->array[i];
		Sleep(200);
	}
	ExitThread(sum);
}

void main() {
	Data a;
	HANDLE hThread;
	DWORD result;
	DWORD StartTime;
	DWORD StopTime;
	std::cout << "Enter array size: ";
	std::cin >> a.size;
	a.array = new INT[a.size];
	srand(time(nullptr));
	for (INT i = 0; i < a.size; ++i) {
		a.array[i] = rand() % 10;
		std::cout << a.array[i] << ' ';
	}
	std::cout << "\nEnter StartTime and StopTime in milliseconds: ";
	std::cin >> StartTime >> StopTime;
	hThread = CreateThread(nullptr, 0, Worker, &a, CREATE_SUSPENDED, nullptr);
	if (hThread == nullptr) {
		std::cout << "\nThread wasn't created";
		return;
	}
	Sleep(StartTime);
	ResumeThread(hThread);
	std::cout << "\nThread started";
	std::cout << "\nThread stopped";
	Sleep(StopTime);
	SuspendThread(hThread);
	ResumeThread(hThread);
	std::cout << "\nThread started";
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, &result);
	std::cout << "\nsum of squares = " << result;
	result = 0;
	hThread = (HANDLE)_beginthreadex(nullptr, 0, (_beginthreadex_proc_type)Worker, &a, 0, nullptr);
	if (hThread == nullptr) {
		std::cout << "\nThread wasn't created";
		return;
	}
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, &result);
	std::cout << "\nsum of squares = " << result;
	CloseHandle(hThread);
}