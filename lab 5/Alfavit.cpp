#include <iostream>
#include <Windows.h>
#include <conio.h>

void main(int argc, char** argv) {
	if (argc == 1) {
		std::cout << "Error";
		return;
	}
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]);
	HANDLE hReadPipe = (HANDLE)atoi(argv[2]);
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");
	if (hEvent == nullptr) {
		std::cout << "Alfavit: Event wasn't read";
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		return;
	}
	int n;

	DWORD dwBytesRead1;
	if (!ReadFile(hReadPipe, &n, sizeof(int), &dwBytesRead1, nullptr)) {
		std::cout << "Alfavit: info wasn't read";
	}

	char* arr = new char[n];

	DWORD dwBytesRead2;
	if (!ReadFile(hReadPipe, arr, sizeof(char) * n, &dwBytesRead2, nullptr)) {
		std::cout << "Alfavit: info wasn't read";
	}
	
	int newSize = 0;
	char* newArr = new char[n];

	for (int i = 0; i < n; ++i) {
			const char ch = std::toupper(arr[i]);
			if ((ch >= 'A' && ch <= 'Z'))
			{
				newArr[newSize] = arr[i];
				newSize++;
			}
	}

	std::cout << "Recived array is: \n";
	for (int i = 0; i < n; ++i) {
		std::cout << arr[i] << " ";
	}

	std::cout <<"\nResult array is: \n";
	for (int i = 0; i < newSize; ++i) {
		std::cout << newArr[i] << " ";
	}

	DWORD dwBytesWritten1;
	if (!WriteFile(hWritePipe, &newSize, sizeof(int), &dwBytesWritten1, nullptr)) {
		std::cout << "Alfavit: arr wasn't written";
	}

	DWORD dwBytesWritten2;
	for (int i = 0; i < newSize; i++) {
		if (!WriteFile(hWritePipe, &newArr[i], sizeof(char), &dwBytesWritten2, nullptr)) {
			std::cout << "Alfavit: arr wasn't written";
		}
	}

	SetEvent(hEvent);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEvent);

	std::cout << "\nEnter 0 for exit\n";
	while (_getch() != '0') {}
}