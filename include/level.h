#include <utility>
#include <optional>
#include <map>

#include "splay.h"

namespace dnc {

using DirectedEdge = std::pair<int, int>;

struct Edge {
    int u, v;
    bool operator<(const Edge& other) const {
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
    Edge(int u, int v) : u(u), v(v) {
        if (u > v) std::swap(u, v);
    }
    DirectedEdge to_directed() const {
        return {u, v};
    }
    DirectedEdge to_directed_reversed() const {
        return {v, u};
    }
};

struct Level {
    using Splay = SplayNode<DirectedEdge, Edge>;

    struct ReplacementEdgeResponse{
        std::vector<Edge> to_level_up;
        std::optional<Edge> found_edge;
    };


    Splay** representative_nodes;
    std::map<DirectedEdge, Splay*> used_edges;
    std::map<Splay*, Splay*> twin;
    std::multiset<Edge> owned_edges;
    int n;


    Level(int n);
    ~Level();
    void insert_edge(Edge e);
    bool delete_used_edge(Edge e);
    bool delete_owned_edge(Edge e);
    bool connected(int a, int b);
    void reroot(Splay* node);
    void link(Edge e);
    void cut(Edge e);
    void insert_edge_tags(Edge e); 
    void insert_non_owned_edge(Edge e);
    ReplacementEdgeResponse get_replacement_edge(Edge e);
    Level (const Level&) = delete;
    Level& operator= (const Level&) = delete;
    Level(Level&&) = default;
};

}  // namespace dnc
