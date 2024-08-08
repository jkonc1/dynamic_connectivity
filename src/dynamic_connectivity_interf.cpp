#include "dnc.h"
#include "dynamic_connectivity_impl.h"

namespace dnc
{

DynamicConnectivity::DynamicConnectivity(int n){
    dc = new DynamicConnectivityImpl(n);
}

DynamicConnectivity::~DynamicConnectivity(){
    delete dc;
}

void DynamicConnectivity::insert_edge(int u, int v){
    dc->insert_edge(u, v);
}

bool DynamicConnectivity::delete_edge(int u, int v){
    return dc->delete_edge(u, v);
}

bool DynamicConnectivity::is_connected(int u, int v){
    return dc->is_connected(u, v);
}

unsigned int DynamicConnectivity::connected_components(){
    return dc->connected_components();
}

} // namespace dnc