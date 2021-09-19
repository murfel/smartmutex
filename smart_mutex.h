#ifndef SMARTMUTEX_SMART_MUTEX_H
#define SMARTMUTEX_SMART_MUTEX_H

#include <atomic>
#include <mutex>
#include <thread>

#include "acyclic_graph.h"

class smart_mutex {
private:
    std::mutex m;
    static acyclic_graph g;
    static std::mutex g_mutex;

    static uint32_t num_threads;
    static std::unordered_map<std::thread::id, uint32_t> thread_id_to_vertex;

    static std::atomic<uint32_t> num_mutexes;

    uint32_t mutex_id = num_mutexes++;

    static uint64_t get_thread_vertex() {
        std::thread::id this_id = std::this_thread::get_id();
        if (thread_id_to_vertex.find(this_id) == thread_id_to_vertex.end()) {
            thread_id_to_vertex[this_id] = num_threads++;
        }
        return thread_id_to_vertex[this_id];
    }

    [[nodiscard]] uint64_t get_mutex_vertex() const {
        return std::numeric_limits<uint32_t>::max() + mutex_id;
    }

    void add_edge() {
        g.add_edge(get_thread_vertex(), get_mutex_vertex());
    }

    void remove_edge() {
        g.remove_edge(get_thread_vertex(), get_mutex_vertex());
    }
public:
    smart_mutex() noexcept = default;
    ~smart_mutex() = default;

    smart_mutex(const smart_mutex&) = delete;
    smart_mutex& operator=(const smart_mutex&) = delete;

    bool try_lock() {
        std::lock_guard<std::mutex> guard(g_mutex);
        if (m.try_lock()) {
            add_edge();
            return true;
        }
        return false;
    }

    void lock() {
        {
            std::lock_guard<std::mutex> guard(g_mutex);
            add_edge();
        }
        m.lock();
    }

    void unlock() {
        // all unlockings are linearized
        std::lock_guard<std::mutex> guard(g_mutex);
        m.unlock();
        remove_edge();
    }
};

#endif //SMARTMUTEX_SMART_MUTEX_H
