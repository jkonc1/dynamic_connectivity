#include <utility>
#include "splay.h"

using Splay = SplayNode<int, std::pair<int,int>>;

struct Level{
    Splay** begin_nodes;
    Splay** end_nodes;
    Level(int n);
    void insert_edge(int a, int b);
    void delete_edge(int a, int b);
    bool connected(int a, int b);
    void reroot(Splay* node);
    void link(int a, int b);
    void cut(int a, int b);
};
