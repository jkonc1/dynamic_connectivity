#include "level.h"

namespace dnc {

Level::Level(int _n) {
    n = _n;
    representative_nodes = new Splay*[n];

    for (int i = 0; i < n; i++) {
        representative_nodes[i] = new Splay({i, i});
    }
}

bool Level::connected(int a, int b) {
    Splay* rightmost_a = representative_nodes[a]->get_rightmost();
    Splay* rightmost_b = representative_nodes[b]->get_rightmost();
    return rightmost_a == rightmost_b;
}

void Level::link(Edge e) {
    Splay* edge_node = new Splay(e.to_directed());
    Splay* twin_node = new Splay(e.to_directed_reversed());
    used_edges[e.to_directed()] = edge_node;
    used_edges[e.to_directed_reversed()] = twin_node;

    Splay* u_representative = representative_nodes[e.u];
    reroot(u_representative);

    Splay* v_representative = representative_nodes[e.v];
    reroot(v_representative);

    Splay* u_remainder = u_representative->cut_right();

    // order: representative node of u, edge u->v, v subtree, edge v->u, rest of
    // the u tree
    u_representative->merge_right(edge_node);
    u_representative->merge_right(v_representative);
    u_representative->merge_right(twin_node);
    u_representative->merge_right(u_remainder);

    // TODO make merge a method of level instead?
}

void Level::insert_edge(Edge e) {
    insert_non_owned_edge(e);
    insert_edge_tags(e);
}

void Level::insert_non_owned_edge(Edge e) {
    bool same_component = connected(e.u, e.v);
    if (!same_component) {
        link(e);
    }
}

void Level::insert_edge_tags(Edge e) {
    representative_nodes[e.u]->add_tag(e);
    representative_nodes[e.v]->add_tag(e);
}

void Level::reroot(Splay* node) {
    node->splay();

    if (node->left == 0) return;  // already the root

    Splay* left_section = node->cut_left();

    node->merge_right(left_section);
}

bool Level::delete_used_edge(Edge e) {
    auto edge_ptr = used_edges.find(e.to_directed());
    if (edge_ptr != used_edges.end()) {
        cut(e);
        return true;
    }
    return false;
}

void Level::delete_owned_edge(Edge e) {
    representative_nodes[e.u]->pop_tag(e);
    representative_nodes[e.v]->pop_tag(e);
}

void Level::cut(Edge e) {
    Splay* edge_node = used_edges[e.to_directed()];
    Splay* twin_node = used_edges[e.to_directed_reversed()];

    reroot(edge_node);

    // cut off edge
    Splay* b_subtree = edge_node->cut_right();

    // and twin edge
    Splay* a_subtree = twin_node->cut_right();
    twin_node->cut_left();

    delete edge_node;
    delete twin_node;
    used_edges.erase(e.to_directed());
    used_edges.erase(e.to_directed_reversed());
}

Level::ReplacementEdgeResponse Level::get_replacement_edge(Edge e) {
    std::vector<Edge> to_level_up;
    std::optional<Edge> replacement_edge = std::nullopt;

    Splay* root_u = representative_nodes[e.u];
    root_u->splay();
    Splay* root_v = representative_nodes[e.v];
    root_v->splay();

    // make the u tree smaller

    if (root_u->size > root_v->size) {
        std::swap(root_u, root_v);
    }

    // start looking for edges in the smaller (u) tree

    while (1) {
        auto tag = root_u->get_tag();

        if (!tag.has_value()) {
            // tried all edges, none found

            break;
        }

        auto edge = tag->value;
        auto node = tag->node;

        if (connected(edge.u, edge.v)) {
            // if the edges connect vertices in the same tree, level up the edge
            delete_owned_edge(edge);

            to_level_up.push_back(edge);
        } else {
            // if the edge connects vertices in different trees, it's the
            // replacement edge
            replacement_edge = edge;
            break;
        }
    }

    return {to_level_up, replacement_edge};
}

Level::~Level() {
    for (int i = 0; i < n; i++) {
        delete representative_nodes[i];
    }
    delete[] representative_nodes;
    for (auto [edge, node] : used_edges) {
        delete node;
    }
}

}  // namespace dnc
