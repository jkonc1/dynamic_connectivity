#include <set>
#include <utility>
#include "level.h"

namespace dnc {

class DynamicConnectivity{
private:
    int levels_count;
    int n;
    unsigned int connected_components_count;
    std::vector<Level> levels;
    std::multiset<Edge> edges;
public:
    DynamicConnectivity(int n);
    void insert_edge(int a, int b);
    bool delete_edge(int a, int b);
    bool is_connected(int a, int b);
    unsigned int connected_components();
};

}
