#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>

void main(int argc, char** argv) {
	int n;
	std::cout << "Server: Enter array size: ";
	std::cin >> n;
	char* arr = new char[n];
	std::cout << "Enter array values: ";
	for (int i = 0; i < n; i++) {
		std::cin >> arr[i];
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe1;
	HANDLE hReadPipe1;
	HANDLE hWritePipe2;
	HANDLE hReadPipe2;
	HANDLE hEvent;
	SECURITY_ATTRIBUTES sa;

	hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
	if (hEvent == nullptr) {
		std::cout << "Server: Event wasn't created";
		return;
	}

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0)) {
		std::cout << "Server: Pipe creation failed";
		CloseHandle(hWritePipe1);
		CloseHandle(hReadPipe1);
		CloseHandle(hEvent);
		return;
	}

	if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
		std::cout << "Server: Pipe creation failed";
		CloseHandle(hWritePipe2);
		CloseHandle(hReadPipe2);
		CloseHandle(hEvent);
		return;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	wchar_t* wstr = new wchar_t[80];
	wsprintf(wstr, L"\"Alfavit.exe\" %d %d", (int)hWritePipe1, (int)hReadPipe2);

	if (!CreateProcess(nullptr, wstr, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, FALSE, FALSE, &si, &pi)) {
		std::cout << "Server: Client creation failed";
		CloseHandle(hWritePipe1);
		CloseHandle(hReadPipe2);
		CloseHandle(hEvent);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return;
	}

	DWORD dwBytesWritten1;
	if (!WriteFile(hWritePipe2, &n, sizeof(int), &dwBytesWritten1, nullptr)) {
		std::cout << "Server: info wasn't written";
		CloseHandle(hWritePipe1);
		CloseHandle(hReadPipe2);
		CloseHandle(hEvent);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return;
	}

	DWORD dwBytesWritten2;
	if (!WriteFile(hWritePipe2, arr, sizeof(char) * n, &dwBytesWritten2, nullptr)) {
		std::cout << "Server: info wasn't written";
		CloseHandle(hWritePipe1);
		CloseHandle(hReadPipe2);
		CloseHandle(hEvent);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return;
	}

	WaitForSingleObject(hEvent, INFINITE);

	int newSize;
	 
	DWORD dwBytesRead1;
	if (!ReadFile(hReadPipe1, &newSize, sizeof(int), &dwBytesRead1, nullptr)) {
		std::cout << "Server: arr wasn't read";
		CloseHandle(hWritePipe1);
		CloseHandle(hReadPipe2);
		CloseHandle(hEvent);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return;
	}

	char* newArr = new char[newSize];

	DWORD dwBytesRead2;
	for (int i = 0; i < newSize; i++) {
		if (!ReadFile(hReadPipe1,&newArr[i], sizeof(char), &dwBytesRead2, nullptr)) {
			std::cout << "Server: arr wasn't read";
			CloseHandle(hWritePipe1);
			CloseHandle(hReadPipe2);
			CloseHandle(hEvent);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return;
		}
	}

	std::cout << "\Starting array is: \n";
	for (int i = 0; i < n; ++i) {
		std::cout << arr[i] << " ";
	}

	std::cout << "\nResult array is: \n";
	for (int i = 0; i < newSize; ++i) {
		std::cout << newArr[i] << " ";
	}

	CloseHandle(hWritePipe1);
	CloseHandle(hReadPipe1);
	CloseHandle(hWritePipe2);
	CloseHandle(hReadPipe2);
	CloseHandle(hEvent);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}