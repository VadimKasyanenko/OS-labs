#include <iostream>
#include <Windows.h>
#include <random>
CRITICAL_SECTION cs;
HANDLE WorkEvent, CountEvent;

struct Array {
	int size;
	int new_size;
	_int16* arr;
	float* res_arr;
	int int_count;
};

DWORD WINAPI work(LPVOID Arr) {
	std::cout << "\nwork started\n";
	auto massiv = (Array*)(Arr);
	std::cout << "\nEnter time interval (milliseconds): ";
	int Interval;
	std::cin >> Interval;
	massiv->new_size = 0;
	for (int i = 0; i < massiv->size; ++i) {
		if (massiv->arr[i]>0) {
			massiv->res_arr[massiv->new_size] = sqrt(massiv->arr[i]);
			++massiv->new_size;
			Sleep(Interval);
		}
	}
	SetEvent(WorkEvent);
	std::cout << "\nThread Work is finished\n";
	return 0;
}

DWORD WINAPI count(LPVOID Arr) {
	std::cout << "\ncount started\n";
	EnterCriticalSection(&cs);
	
	auto massiv = (Array*)Arr;
	massiv->int_count = 0;
	for (int i = 0; i < massiv->new_size; ++i) {
		if (massiv->res_arr[i] == (int)massiv->res_arr[i]) {
			++massiv->int_count;
		}
	}
	SetEvent(CountEvent);
	LeaveCriticalSection(&cs);
	std::cout << "\nThread Count is finished\n";
	return 0;
}

void main() {
	Array massiv;
	std::cout << "Enter array size: ";
	std::cin >> massiv.size;
	massiv.arr = new _int16[massiv.size];
	massiv.res_arr = new float[massiv.size];
	std::cout << "Enter elements: ";
	for (int i = 0; i < massiv.size; ++i) {
		std::cin >> massiv.arr[i];
		massiv.res_arr[i] = 0;
	}
	std::cout << "Array size: " << massiv.size << "\nArray: ";
	for (int i = 0; i < massiv.size; ++i) {
		std::cout << massiv.arr[i] << " ";
	}

	InitializeCriticalSection(&cs);
	
	WorkEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	CountEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	EnterCriticalSection(&cs);
	HANDLE hThreadWork;
	DWORD IDThreadWork;
	DWORD dwCountWork;
	hThreadWork = CreateThread(NULL, 0, work, &massiv, NULL, &IDThreadWork);

	HANDLE hThreadCount;
	DWORD IDThreadCount;
	DWORD dwCountCount;
	hThreadCount = CreateThread(NULL, 0, count, &massiv, NULL, &IDThreadCount);


	WaitForSingleObject(WorkEvent, INFINITE);

	std::cout << "Elements of array: ";
	for (int i = 0; i < massiv.size; ++i) {
		std::cout << massiv.res_arr[i] << " ";
	}
	LeaveCriticalSection(&cs);
	
	
	WaitForSingleObject(CountEvent, INFINITE);
	std::cout << std::endl << "Result of Count: " << massiv.int_count << std::endl;

	DeleteCriticalSection(&cs);
	WaitForSingleObject(hThreadWork, INFINITE);
	WaitForSingleObject(hThreadCount, INFINITE);
	CloseHandle(hThreadWork);
	CloseHandle(hThreadCount);
	
}