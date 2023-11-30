#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
bool eventTriggered = false;

void provider() 
{

    while (true) {
        // Имитация задержки в одну секунду
        this_thread::sleep_for(chrono::seconds(1));

        {
            lock_guard<mutex> lock(mtx);

            // Инициируем условное событие
            eventTriggered = true;
            cout << "Событие отправлено" << endl;
        }

        // Уведомляем поток-потребитель
        cv.notify_one();
    }
}

void consumer() 
{
    while (true) 
    {
        unique_lock<mutex> lock(mtx);

        // Ждем, пока условное событие произойдет
        cv.wait(lock, [] { return eventTriggered; });

        // Обрабатываем условное событие
        eventTriggered = false;
        cout << "Событие обработано" << endl << endl;
    }
}

int main() 
{
    setlocale(LC_ALL, "Russian");
    // Создаем и запускаем потоки
    thread providerThread(provider);
    thread consumerThread(consumer);

    providerThread.join();
    consumerThread.join();

    return 0;
}