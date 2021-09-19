#include "smart_mutex.h"

acyclic_graph smart_mutex::g;
std::mutex smart_mutex::g_mutex;

uint32_t smart_mutex::num_threads;
std::unordered_map<std::thread::id, uint32_t> smart_mutex::thread_id_to_vertex;

std::atomic<uint32_t> smart_mutex::num_mutexes{0};