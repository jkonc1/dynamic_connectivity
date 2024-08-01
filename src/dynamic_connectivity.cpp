#include "dynamic_connectivity.h"

namespace dnc {

DynamicConnectivity::DynamicConnectivity(int _n) {
    n = _n;

    levels_count = 0;

    connected_components_count = n;

    // there should be log2(n) + 1 levels
    while ((1 << levels_count) < n) levels_count++;

    levels_count++;

    levels.reserve(levels_count);

    for (int i = 0; i < levels_count; i++) {
        levels.emplace_back(n);
    }
}

void DynamicConnectivity::insert_edge(int a, int b) {
    if (a == b) return;
    connected_components_count += is_connected(a, b);

    Edge edge(a, b);

    edges.insert(edge);
    levels[0].insert_edge(edge);

    connected_components_count -= is_connected(a, b);
}

bool DynamicConnectivity::is_connected(int a, int b) {
    return levels[0].connected(a, b);
}

bool DynamicConnectivity::delete_edge(int a, int b) {
        Edge edge(a, b);
    auto ptr = edges.find(edge);
    if (ptr == edges.end()) {
        // the edge doesn't exist
        return false;
    }
    edges.erase(ptr);

    // first try to delete the edge as unused

    for (int level = levels_count - 1; level >= 0; level--) {
        levels[level].delete_owned_edge(edge);
    }

    int edge_deletion_level = -1;
    for (int level = levels_count-1; level >= 0; level--) {
        if(levels[level].delete_used_edge(edge)){
            edge_deletion_level = level;
            break;
        }
    }

    if(edge_deletion_level == -1){
        // the edge was not used, we can just return
        return true;
    }

    for (int level = edge_deletion_level; level >= 0; level--) {
        auto replacement_edge = levels[level].get_replacement_edge(edge);
        for (auto& edge : replacement_edge.to_level_up) {
            levels[level + 1].insert_edge(edge);
        }

        if (replacement_edge.found_edge.has_value()) {
            for (int i = 0; i <= level; i++) {
                levels[i].insert_non_owned_edge(replacement_edge.found_edge.value());
            }
            // levels[level].insert_edge(replacement_edge.found_edge.value());

            return true;
        }
    }

    connected_components_count++;
    return true;
}

unsigned int DynamicConnectivity::connected_components() {
    return connected_components_count;
}

}  // namespace dnc
