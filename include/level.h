#include <utility>
#include <optional>

#include "splay.h"

namespace dnc {

using Splay = SplayNode<int, std::pair<int, int>>;


struct Level {
    using Edge = std::pair<int, int>;

    struct ReplacementEdgeResponse{
        std::vector<Edge> to_level_up;
        std::optional<Edge> found_edge;
    };


    Splay** begin_nodes;
    Splay** end_nodes;
    std::set<Edge> used_edges;

    Level(int n);
    ~Level();
    void insert_edge(int a, int b);
    bool delete_used_edge(int a, int b);
    bool delete_unused_edge(int a, int b);
    bool connected(int a, int b);
    void reroot(Splay* node);
    void link(int a, int b);
    void cut(int a, int b);
    ReplacementEdgeResponse get_replacement_edge(int a, int b);
    Level (const Level&) = delete;
    Level& operator= (const Level&) = delete;
    Level(Level&&) = default;
};

}  // namespace dnc
