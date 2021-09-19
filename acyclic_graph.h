#ifndef SMARTMUTEX_ACYCLIC_GRAPH_H
#define SMARTMUTEX_ACYCLIC_GRAPH_H

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <limits>

using vertex_t = uint64_t;
const vertex_t SENTINEL_VERTEX = std::numeric_limits<vertex_t>::max();

class acyclic_graph {
private:
    std::unordered_map<vertex_t, std::vector<vertex_t>> adj_list;

    bool has_cycles(const vertex_t parent, const vertex_t vertex, std::unordered_set<vertex_t> & visited) {
        if (visited.count(vertex)) {
            return true;
        }
        visited.insert(vertex);
        for (vertex_t child : adj_list[vertex]) {
            if (child == parent) {
                continue;
            }
            if (has_cycles(vertex, child, visited)) {
                return true;
            }
        }
        return false;
    }

    bool has_cycles() {
        std::unordered_set<vertex_t> visited;
        return has_cycles(SENTINEL_VERTEX, adj_list.begin()->first, visited);
    }

    void terminate_if_broken_invariant() {
        if (has_cycles()) {
            exit(1);
        }
    }

    void remove_directed_edge(uint64_t from, uint64_t to) {
        auto it = std::find(adj_list[from].begin(), adj_list[from].end(), to);
        adj_list[from].erase(it);
    }
public:
    void add_edge(uint64_t u, uint64_t v) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
        terminate_if_broken_invariant();
    }
    void remove_edge(uint64_t u, uint64_t v) {
        remove_directed_edge(u, v);
        remove_directed_edge(v, u);
    }
};

#endif //SMARTMUTEX_ACYCLIC_GRAPH_H
