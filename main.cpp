#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include "smart_mutex.h"

void test_simple() {
    smart_mutex sm;
    sm.lock();
    sm.unlock();
}

void test_simple_aa() {
    smart_mutex sm;
    sm.lock();
    sm.lock();
}

void test_simple_ab_ba() {
    smart_mutex sm1;
    smart_mutex sm2;
    std::vector<std::thread> threads;
    threads.emplace_back([](smart_mutex & sm1, smart_mutex & sm2) {
        sm1.lock();
        std::cerr << 1 << std::endl;
        // TODO: Replace with a barrier
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sm2.lock();
    }, std::ref(sm1), std::ref(sm2));
    threads.emplace_back([](smart_mutex & sm1, smart_mutex & sm2) {
        sm2.lock();
        std::cerr << 2 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sm1.lock();
    }, std::ref(sm1), std::ref(sm2));
    for (std::thread & thread : threads) {
        thread.join();
    }
}

void test_simple_ab_bc_ca() {
    smart_mutex sm1;
    smart_mutex sm2;
    smart_mutex sm3;
    std::vector<std::thread> threads;
    threads.emplace_back([](smart_mutex & sm1, smart_mutex & sm2, smart_mutex & sm3) {
        sm1.lock();
        std::cerr << 1 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sm2.lock();
    }, std::ref(sm1), std::ref(sm2), std::ref(sm3));
    threads.emplace_back([](smart_mutex & sm1, smart_mutex & sm2, smart_mutex & sm3) {
        sm2.lock();
        std::cerr << 2 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sm3.lock();
    }, std::ref(sm1), std::ref(sm2), std::ref(sm3));
    threads.emplace_back([](smart_mutex & sm1, smart_mutex & sm2, smart_mutex & sm3) {
        sm3.lock();
        std::cerr << 3 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sm1.lock();
    }, std::ref(sm1), std::ref(sm2), std::ref(sm3));
    for (std::thread & thread : threads) {
        thread.join();
    }
}

int main() {
    std::cout << "Tests that should run fine" << std::endl;
    test_simple();
    std::cout << "Done" << std::endl;
    std::cout << "Tests that should terminate" << std::endl;
    test_simple_aa();  // comment out to run the next test
    test_simple_ab_ba();
    test_simple_ab_bc_ca();
    std::cout << "Testing done" << std::endl;
    return 0;
}
