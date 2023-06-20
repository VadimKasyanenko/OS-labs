
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

mutex mtx;
condition_variable cv;
queue<int> clients;
bool isBarberSleeping = true;

void barber()
{
    while (true)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !clients.empty(); });

        int client = clients.front();
        clients.pop();

        isBarberSleeping = false;
        lock.unlock();
        cv.notify_all();

        cout << "Barber is cutting hair of client " << client << endl;
        this_thread::sleep_for(chrono::seconds(5));
        cout << "Barber finished cutting hair of client " << client << endl;

        isBarberSleeping = true;
        if(clients.size() == 0)
        cout << "Barber sleeping" << endl;
    }
}

void customer(int id)
{
    unique_lock<mutex> lock(mtx);
    if (clients.size() >= 5)
    {
        cout << "Customer " << id << " left because the barber shop is full." << endl;
        return;
    }

    clients.push(id);
    cout << "Customer " << id << " entered the barber shop." << endl;

    if (isBarberSleeping)
    {
        isBarberSleeping = false;
        lock.unlock();
        cv.notify_all();
    }
}

int main()
{
    thread barberThread(barber);
    thread customersThread[10];

    for (int i = 0; i < 10; i++)
    {
        customersThread[i] = thread(customer, i + 1);
        this_thread::sleep_for(chrono::seconds(1));
    }

    for (int i = 0; i < 10; i++)
    {
        customersThread[i].join();
    }

    barberThread.join();

    return 0;
}