#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

#define MAX_THREADS 10

HANDLE thread_handles[MAX_THREADS];

DWORD WINAPI func(LPVOID lpParam);
void make_thread(int priority, int tnumber);
void terminate_thread(int thread_no);

int main() {
    int tnumber = 0;
    char command;
    bool quit = false;

    while(cin >> command && !quit){
        int priority;

        switch (command){
            case 'm':
                if (tnumber >= MAX_THREADS) {
                    cout << "Za duzo watkow!";
                    break;
                }
                cout << "Podaj priorytet watku: ";
                cin >> priority;

                make_thread(priority, tnumber);
                tnumber++;
                break;
            case 'c':
                int thread;

                cout << "Podaj numer watku: ";
                cin >> thread;

                terminate_thread(thread);
                break;
            case 'q':
                quit = true;
                break;
            default:
                cout << "Nie ma takiej komendy!\n";
        }
    }

    return 0;
}

DWORD WINAPI func(LPVOID lpParam) {
    int tnumber = (int)lpParam;
    double duration;
    unsigned long long result = 0;

    auto start = high_resolution_clock::now();

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 1389; i++) {
            result += i * i;
        }

        result += 27;

        for (int i = 0; i < 1389; i++) {
            result += -i * i;
        }
    }

    auto end = high_resolution_clock::now();
    auto difference = end - start;
    duration = chrono::duration<double, milli>(difference).count();

    cout << "Watek o numerze: " << tnumber << " czas: " << duration << "ms, wynik = " << result << endl;

    return 0;
}


void make_thread(int priority, int tnumber) {
    DWORD thread_id;
    HANDLE thread_handle;
    thread_handle = CreateThread(nullptr, 0, func, &tnumber, 0, &thread_id);
    if (thread_handle == nullptr) {
        cout <<"Blad podczas tworzenia watku.\n";
        exit(1);
    }

    if (!SetThreadPriority(thread_handle, priority)) {
        DWORD priori = GetThreadPriority(thread_handle);
        cout <<"Current priority: " << priori << endl;
        cout <<"Blad podczas zmiany priorytetu watku.\n";
        exit(1);
    }

    thread_handles[tnumber] = thread_handle;
}

void terminate_thread(int thread_no) {

    if (thread_handles[thread_no] == nullptr) {
        cout <<"Nie ma watku o takim numerze.\n";
        return;
    }

    if (!TerminateThread(thread_handles[thread_no], 0)) {
        cout <<"Blad podczas zakonczenia watku.\n";
        exit(1);
    }

    CloseHandle(thread_handles[thread_no]);
    thread_handles[thread_no] = nullptr;
}
