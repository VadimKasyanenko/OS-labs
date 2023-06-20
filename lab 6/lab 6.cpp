#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <condition_variable>

int counter;
std::mutex critical_section;
std::condition_variable cv;
std::mutex event;
std::deque<double> result;

void Work(std::vector<_int16>& array, const int& size)
{
    std::cout << "\nThread work is started\n";
    int time;
    std::cout << "Enter time interval to sleep: ";
    std::cin >> time;

    for (int i = 0; i < size; i++)
    {
        if (array[i] > 0) {
            result.push_front(sqrt(array[i]));
        }
        else {
            result.push_back(0);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }

    std::cout << "Result array is: ";
    for (int i = 0; i < size; i++) {
        std::cout << result[i] << " ";
    }

    cv.notify_one();
    std::cout << "\nThread work is finished\n";
}

void Count(const int& size)
{
    std::cout << "\nThread Count is started\n";
    std::unique_lock<std::mutex> critical_section_locker(critical_section);
    counter = 0;
    for (int i = 0; i < size; i++) {
        if (result[i] == (int)result[i]) {
            counter++;
        }
    }

    critical_section_locker.unlock();
    std::cout << "\nThread Count is finished\n";
}

int main()
{
    int size;
    std::cout << "Enter size of array: ";
    std::cin >> size;
    std::vector<_int16> array(size);
    std::cout << "Enter " << size << " elements of array: ";
    for (int i = 0; i < size; i++) {
        std::cin >> array[i];
    }
    std::cout << "Size of array is: " << size << "\n";
    std::cout << "Elements of array is: ";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";

    std::unique_lock<std::mutex> critical_section_locker(critical_section);
    std::thread work(Work, std::ref(array), std::ref(size));
    std::thread countElement(Count, std::ref(size));

    std::unique_lock<std::mutex> lock_out(event);
    cv.wait(lock_out);

    std::cout << "Result array is: ";
    for (int i = 0; i < size; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << "\n";

    critical_section_locker.unlock();

    std::cout << "Number of elements: " << counter << "\n";
    
    lock_out.unlock();
    work.detach();
    countElement.detach();

    return 0;
}