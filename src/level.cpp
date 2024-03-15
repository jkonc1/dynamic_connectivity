#include "level.h"

namespace dnc {

Level::Level(int n) {
    begin_nodes = new Splay*[n];
    end_nodes = new Splay*[n];

    for (int i = 0; i < n; i++) {
        begin_nodes[i] = new Splay(i + 1);
        end_nodes[i] = new Splay(-i - 1);
        end_nodes[i]->parent = begin_nodes[i];
        begin_nodes[i]->right = end_nodes[i];
        begin_nodes[i]->pull();
    }
}

bool Level::connected(int a, int b) {
    begin_nodes[a]->splay();
    Splay* rightmost_a = begin_nodes[a]->get_rightmost();
    Splay* rightmost_b = begin_nodes[b]->get_rightmost();
    return rightmost_a == rightmost_b;
}

void Level::link(int a, int b) {
    if (a > b) std::swap(a, b);

    Splay* parent = begin_nodes[a];
    Splay* child = begin_nodes[b];

    // reroot the trees to a and b
    reroot(parent);
    reroot(child);

    // split off the first vertex of a
    parent->splay();
    Splay* remaining = parent->cut_right();

    // put the b tree after the root node

    child->attach_left(parent);

    // put the rest of the euler tour at the end

    Splay* rightmost = parent->get_rightmost();
    rightmost->attach_right(remaining);
}

void Level::insert_edge(int a, int b) {
    if(a > b) std::swap(a,b);
    bool same_component = connected(a, b);
    if (same_component) {
        // if they're in the same component, we just add the edge to the tags
        begin_nodes[a]->add_tag({a, b});
        begin_nodes[b]->add_tag({b, a});
    } else {
        link(a, b);
        used_edges.insert({a, b});
    }
}

void Level::reroot(Splay* node) {
    node->splay();

    if (node->left == 0) return;  // already the root

    Splay* left_section = node->cut_left();

    Splay* rightmost = node->get_rightmost();

    rightmost->attach_right(left_section);
}

bool Level::delete_used_edge(int a, int b) {
    if(a > b) std::swap(a,b);

    auto edge_ptr = used_edges.find({a, b});
    if(edge_ptr != used_edges.end()){
        cut(a, b);
        used_edges.erase(edge_ptr);
        return true;
    }
    return false;
}

//3 5 -5 -4 4 -3

//4 5 -5 -4
  
bool Level::delete_unused_edge(int a, int b) {
    if (begin_nodes[a]->has_tag({a, b})) {
        // if the edge is stored as a tag, just pop the tag
        begin_nodes[a]->remove_tag({a, b});
        begin_nodes[b]->remove_tag({b, a});
        return true;
    }
    return false;
}

void Level::cut(int a, int b) {
    if (a > b) std::swap(a, b);

    Splay* parent = begin_nodes[a];
    Splay* child = begin_nodes[b];

    reroot(parent);

    // cut off everything to the left of the beginning of the cut subtree

    Splay* left_part = child->cut_left();

    // cut off everything to the right of the end of the cut subtree

    Splay* right_end = end_nodes[b];

    Splay* right_leftover = right_end->cut_right();


    // put the right leftover at the end of the left part
    Splay* rightmost = left_part->get_rightmost();
    rightmost->attach_right(right_leftover);
}

Level::ReplacementEdgeResponse Level::get_replacement_edge(int a, int b) {
    std::vector<Edge> to_level_up;

    Splay* root_a = begin_nodes[a];
    root_a->splay();
    Splay* root_b = begin_nodes[b];
    root_b->splay();

    // make the a tree smaller

    if(root_a->size > root_b->size) {
        std::swap(root_a, root_b);
        std::swap(a, b);
    }

    // start looking for edges in the smaller (a) tree

    while(1){
        auto tag = root_a->get_tag();

        if(!tag.has_value()){
            // tried all edges, none found

            return {to_level_up, std::nullopt};
        }


        auto edge = tag->value;
        auto node = tag->node;

        auto [from, to] = edge;

        // pop the tag
        node->remove_tag(edge);

        auto other = begin_nodes[to];
        other->remove_tag({to, from});

        // if the edges connect vertices in the same tree, level up the edge
        if(connected(from, to)){
            to_level_up.push_back(edge);
        } else {
            // if the edge connects vertices in different trees, it's the replacement edge
            return {to_level_up, edge};
        }
    }
}

Level::~Level() {
    for(int i = 0; i < 5; i++) {
        delete begin_nodes[i];
        delete end_nodes[i];
    }
    delete[] begin_nodes;
    delete[] end_nodes;
}

}
