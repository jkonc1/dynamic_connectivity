#include <set>
#include <utility>
#include "level.h"

namespace dnc {

class DynamicConnectivityImpl{
private:
    int levels_count;
    int n;
    unsigned int connected_components_count;
    std::vector<Level> levels;
    std::multiset<Edge> edges;
public:
    DynamicConnectivityImpl(int n);
    void insert_edge(int u, int v);
    bool delete_edge(int u, int v);
    bool is_connected(int u, int v);
    unsigned int connected_components();
};

}
