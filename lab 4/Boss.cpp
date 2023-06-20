#include <iostream>
#include <Windows.h>
#include <string>

void main() {
	auto* Event_Employee_Ended = CreateEvent(nullptr, FALSE, FALSE, L"Employee_Ended");
	auto* Events_Boss = new HANDLE[4];
	Events_Boss[0] = CreateEvent(nullptr, TRUE, FALSE, L"Boss_0");
	Events_Boss[1] = CreateEvent(nullptr, TRUE, FALSE, L"Boss_1");
	Events_Boss[2] = CreateEvent(nullptr, TRUE, FALSE, L"Boss_2");
	Events_Boss[3] = CreateEvent(nullptr, TRUE, FALSE, L"Boss_3");

	int n;
	int n_e;
	std::cout << "Boss: Enter number of processes: ";
	std::cin >> n;
	std::cout << "Boss: Enter number of sended messages for employee: ";
	std::cin >> n_e;

	auto* si_e = new STARTUPINFO[n];
	auto* pi_e = new PROCESS_INFORMATION[n];
	auto* Semaphore = CreateSemaphore(nullptr, 3, 3, L"Semaphore");

	std::wstring str_e = L"\"Employee.exe\" ";
	str_e += std::to_wstring(n_e);
	auto* wstr_e = new wchar_t[str_e.length()];
	wcscpy_s(wstr_e, str_e.length() + 1, str_e.c_str());

	for (int i = 0; i < n; ++i) {
		ZeroMemory(&si_e[i], sizeof(STARTUPINFO));
		if (!CreateProcess(nullptr, wstr_e, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si_e[i], &pi_e[i])) {
			std::cout << "Boss: Employee opening Error";
			CloseHandle(Semaphore);
			CloseHandle(Event_Employee_Ended);
			CloseHandle(Events_Boss[0]);
			CloseHandle(Events_Boss[1]);
			CloseHandle(Events_Boss[2]);
			CloseHandle(Events_Boss[3]);
			return;
		}
	}
	bool mark = false;
	int len = 0;
	if (n % 3 == 0) {
		len = n_e * n / 3;
	}
	else {
		len = n_e * ((n / 3) + 1);
	}
	for (int i = 0; i < len; ++i) {
		int c;
		if (mark==false)
		{
			std::cout << "Boss: Enter '0', '1', '2', '3': ";
			std::cin >> c;
			switch (c)
			{
			case 0:
				SetEvent(Events_Boss[0]);
				mark = true;
				break;
			case 1:
				SetEvent(Events_Boss[1]);
				break;
			case 2:
				SetEvent(Events_Boss[2]);
				break;
			case 3:
				SetEvent(Events_Boss[3]);
				break;
			}
		}
		if (!((i + 1) % n_e)) {
			WaitForSingleObject(Event_Employee_Ended, INFINITE);
			std::cout << "Boss: Employee ended\n";
			mark = false;
		}
	}

	CloseHandle(Semaphore);
	CloseHandle(Event_Employee_Ended);
	CloseHandle(Events_Boss[0]);
	CloseHandle(Events_Boss[1]);
	CloseHandle(Events_Boss[2]);
	CloseHandle(Events_Boss[3]);
}