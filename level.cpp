#include "level.h"

Level::Level(int n) {
    begin_nodes = new Splay*[n];
    end_nodes = new Splay*[n];

    for (int i = 0; i < n; i++) {
        begin_nodes[i] = new Splay(i + 1);
        end_nodes[i] = new Splay(-i - 1);
        end_nodes[i]->parent = begin_nodes[i];
        begin_nodes[i]->right = end_nodes[i];
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
    Splay* remaining = parent->right;
    remaining->parent = 0;
    parent->right = 0;

    // put the b tree after the root node

    child->splay();

    begin_nodes[b]->parent = parent;
    parent->right = child;

    // put the rest of the euler tour at the end

    Splay* rightmost = parent->get_rightmost();
    rightmost->splay();
    rightmost->right = remaining;
    remaining->parent = rightmost;
}

void Level::insert_edge(int a, int b) {
    bool same_component = connected(a, b);
    if (same_component) {
        // if they're in the same component, we just add the edge to the tags
        begin_nodes[a]->add_tag({a, b});
        begin_nodes[b]->add_tag({b, a});
    } else {
        link(a, b);
    }
}

void Level::reroot(Splay* node) {
    node->splay();

    if (node->left == 0) return;  // already the root

    Splay* left_section = node->left;
    left_section->parent = 0;
    node->left = 0;

    Splay* rightmost = node->get_rightmost();

    rightmost->splay();
    rightmost->right = left_section;
    left_section->parent = rightmost;
}

void Level::delete_edge(int a, int b) {
    if (begin_nodes[a]->has_tag({a, b})) {
        // if the edge is stored as a tag, just pop the tag
        begin_nodes[a]->remove_tag({a, b});
        begin_nodes[b]->remove_tag({b, a});
    } else {
        cut(a, b);
    }
}

void Level::cut(int a, int b) {
    if (a > b) std::swap(a, b);

    Splay* parent = begin_nodes[a];
    Splay* child = begin_nodes[b];

    reroot(parent);
    child->splay();

    // cut off everything to the left of the beginning of the cut subtree

    Splay* left_part = child->left;
    left_part->parent = 0;
    child->left = 0;

    // cut off everything to the right of the end of the cut subtree

    Splay* right_end = end_nodes[b];
    right_end->splay();

    Splay* right_leftover = right_end->right;
    right_leftover->parent = 0;
    right_end->right = 0;

    // put the right leftover at the end of the left part left_part->splay();
    left_part->right = right_leftover;
    right_leftover->parent = left_part;
}

// for debugging

int main() {
    Level l(5);
    l.insert_edge(0, 1);
    l.insert_edge(1, 2);
    l.insert_edge(2, 3);
    l.insert_edge(3, 4);
    std::cout << l.connected(0, 4) << std::endl;
    l.delete_edge(1, 2);
    std::cout << l.connected(0, 4) << std::endl;
    l.insert_edge(0, 3);
    std::cout << l.connected(0, 4) << std::endl;
}