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

class Level {
public:
    using Splay = SplayNode<DirectedEdge, std::pair<int, Edge>>;

    struct ReplacementEdgeResponse {
        std::vector<Edge> to_level_up;
        std::optional<Edge> found_edge;
    };

private:
    /// @brief Self-edge for each of the nodes
    Splay** representative_nodes;
    /// @brief Map from directed edges in the tree to pointers to them
    std::map<DirectedEdge, Splay*> used_edges;
    /// @brief Number of vertices
    int n;
    /// @brief Counter for the edge addition times
    int tag_ctr = 0;
    std::map<Edge, std::queue<int>> edge_tag_ids;

public:
    Level(int n);
    ~Level();
    /// @brief Insert a tag and also insert it into forest
    void insert_edge_with_tag(Edge e);
    /// @brief Check if the nodes are in the same tree
    bool connected(int a, int b);
    /// @brief Delete an edge from forest
    bool delete_edge(Edge e);
    /// @brief Delete the tag from it's endpoints
    bool delete_tag(Edge e);
    /// @brief Insert an edge from the forest, if it's ends are disconnected
    void insert_edge(Edge e);
    /// @brief Try to find a replecement edge for a just-removed edge
    ReplacementEdgeResponse get_replacement_edge(Edge e);

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&&) = default;

private:
    /// @brief Reroot the node's tree to the node
    void reroot(Splay* node);
    /// @brief Add an edge in the forest
    void link(Edge e);
    /// @brief Remove an edge from the forest
    void cut(Edge e);
    /// @brief Insert a tag
    void insert_tag(Edge e);
};

}  // namespace dnc
