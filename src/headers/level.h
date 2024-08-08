#include <map>
#include <optional>
#include <utility>

#include "splay.h"

namespace dnc {

using DirectedEdge = std::pair<int, int>;

struct Edge {
    int u, v;
    bool operator<(const Edge& other) const {
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
    Edge(int _u, int _v) : u(_u), v(_v) {
        if (u > v) std::swap(u, v);
    }
    DirectedEdge to_directed() const { return {u, v}; }
    DirectedEdge to_directed_reversed() const { return {v, u}; }
};

struct Level {
    using Splay = SplayNode<DirectedEdge, std::pair<int, Edge>>;

    struct ReplacementEdgeResponse {
        std::vector<Edge> to_level_up;
        std::optional<Edge> found_edge;
    };

    Splay** representative_nodes;
    std::map<DirectedEdge, Splay*> used_edges;
    std::map<Splay*, Splay*> twin;
    int n;
    int tag_ctr = 0;
    std::map<Edge, std::queue<int>> edge_tag_ids;

    Level(int n);
    ~Level();
    void insert_edge_with_tag(Edge e);
    bool delete_edge(Edge e);
    bool delete_tag(Edge e);
    bool connected(int a, int b);
    void reroot(Splay* node);
    void link(Edge e);
    void cut(Edge e);
    void insert_tag(Edge e);
    void insert_edge(Edge e);
    ReplacementEdgeResponse get_replacement_edge(Edge e);
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&&) = default;
};

}  // namespace dnc