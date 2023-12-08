#include <iostream>
#include <pthread.h>
#include <io.h>
#include <concrt.h>
using namespace std;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

void* producer(void*) {
    while (true) {
        pthread_mutex_lock(&lock);
        if (ready == 1) {
            pthread_mutex_unlock(&lock);
            continue;
        }
        ready = 1;
        cout << "Поставщик: отправлено событие" << endl;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
        Concurrency::wait(1000); // Задержка в 1 секунду
    }
    return nullptr;
}

void* consumer(void*) {
    while (true) {
        pthread_mutex_lock(&lock);
        while (ready == 0) {
            pthread_cond_wait(&cond1, &lock);
        }
        ready = 0;
        cout << "Потребитель: получено событие" << endl;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

int main() {
    pthread_t producerThread, consumerThread;

    // Запускаем потоки
    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);

    // Ожидаем завершения потоков (хотя в данном случае они выполняются в бесконечном цикле)
    pthread_join(producerThread, nullptr);
    pthread_join(consumerThread, nullptr);

    return 0;
}