#include "dynamic_connectivity.h"

namespace dnc{

DynamicConnectivity::DynamicConnectivity(int _n){
    n=_n;

    levels_count = 0;

    connected_components_count = n;

    // there should be log2(n) + 1 levels
    while((1<<levels_count) < n) levels_count++;

    levels_count++;

    levels.reserve(levels_count);

    for(int i=0;i<levels_count;i++){
        levels.emplace_back(n);
    }
}

void DynamicConnectivity::insert_edge(int a, int b){
    connected_components_count += is_connected(a,b);

    edges.insert({a,b});
    levels[0].insert_edge(a,b);

    connected_components_count -= is_connected(a,b);
}

bool DynamicConnectivity::is_connected(int a, int b){
    return levels[0].connected(a,b);
}

bool DynamicConnectivity::delete_edge(int a, int b){

    auto ptr = edges.find({a,b});
    if(ptr == edges.end()){
        // the edge doesn't exist
        return false;
    }
    edges.erase(ptr);

    // first try to delete the edge as unused
    for(auto& level: levels){
        if(level.delete_unused_edge(a,b)){
            return true;
        }
    }

    int edge_deletion_level;

    for(int i=0;i<levels_count;i++){
        if(levels[i].delete_used_edge(a,b)){
            edge_deletion_level = i;
        }
    }

    for(int level = edge_deletion_level; level>=0; level--){
        auto replacement_edge = levels[level].get_replacement_edge(a,b);
        for(auto& edge: replacement_edge.to_level_up){
            levels[level+1].insert_edge(edge.first, edge.second);
        }

        if(replacement_edge.found_edge.has_value()){
            auto [from,to] = replacement_edge.found_edge.value();
            for(int i=0; i<=level; i++){
                levels[i].insert_edge(from,to);
            }

            return true;
        }
    }

    connected_components_count++;
    return true;
}

unsigned int DynamicConnectivity::connected_components(){
    return connected_components_count;
}

}  // namespace dnc
