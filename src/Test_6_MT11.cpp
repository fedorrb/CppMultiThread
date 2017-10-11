#include <iostream>
#include <cstdlib>
#include <vector>
#include "mingw.mutex.h"
#include "mingw.thread.h"
using namespace std;

const int elementsCount = 10;

void push(vector<int> &arr, mutex& m_arr, mutex& m_out) {
    int num;
    for (int i = 0; i < elementsCount; i++) {
        m_arr.lock();
        num = rand();
        arr.push_back(num);
        m_arr.unlock();
        m_out.lock();
        cout << "Push " << num << "\n";
        m_out.unlock();
    }
}

void pop(vector<int> &arr, mutex& m_arr, mutex& m_out) {
    int i = 0, num;
    while (i < elementsCount) {
        m_arr.lock();
        if (arr.size() > 0) {
            num = arr.back();
            arr.pop_back();
            m_out.lock();
            cout << "Pop " << num << "\n";
            m_out.unlock();
            i++;
        }
        m_arr.unlock();
    }
}

int main() {
    mutex m_arr, m_out;
    vector<int> vec;
    thread push_thread(push, ref(vec), ref(m_arr), ref(m_out));
    thread pop_thread(pop, ref(vec), ref(m_arr), ref(m_out));
    if (push_thread.joinable()) push_thread.join();
    if (pop_thread.joinable()) pop_thread.join();
    return 0;
}
