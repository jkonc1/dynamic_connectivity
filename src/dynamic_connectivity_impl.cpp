#include "dynamic_connectivity_impl.h"

namespace dnc {

DynamicConnectivityImpl::DynamicConnectivityImpl(int _n) {
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

void DynamicConnectivityImpl::insert_edge(int a, int b) {
    if (a == b) return;
    connected_components_count += is_connected(a, b);

    Edge edge(a, b);

    edges.insert(edge);
    levels[0].insert_edge_with_tag(edge);

    connected_components_count -= is_connected(a, b);
}

bool DynamicConnectivityImpl::is_connected(int a, int b) {
    return levels[0].connected(a, b);
}

bool DynamicConnectivityImpl::delete_edge(int a, int b) {
    Edge edge(a, b);
    auto ptr = edges.find(edge);
    if (ptr == edges.end()) {
        // the edge doesn't exist
        return false;
    }
    edges.erase(ptr);

    // first try to delete the edge as unused
    int edge_deletion_level = -1;

    for (int level = levels_count - 1; level >= 0; level--) {
        if (levels[level].delete_tag(edge)) {
            edge_deletion_level = level;
            break;
        }
    }

    bool used = false;

    for (int level = edge_deletion_level; level >= 0; level--) {
        used |= levels[level].delete_edge(edge);
    }

    if (!used) {
        // the edge was not used, we can just return
        return true;
    }

    for (int level = edge_deletion_level; level >= 0; level--) {
        auto replacement_edge = levels[level].get_replacement_edge(edge);
        for (auto& edge : replacement_edge.to_level_up) {
            levels[level + 1].insert_edge_with_tag(edge);
        }

        if (replacement_edge.found_edge.has_value()) {
            for (int i = 0; i <= level; i++) {
                levels[i].insert_edge(replacement_edge.found_edge.value());
            }
            // levels[level].insert_edge(replacement_edge.found_edge.value());

            return true;
        }
    }

    connected_components_count++;
    return true;
}

unsigned int DynamicConnectivityImpl::connected_components() {
    return connected_components_count;
}

}  // namespace dnc
