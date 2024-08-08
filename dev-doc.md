
# Algorithm

This library is an implementation of the algorithm for online dynamic connectivity described in 
Holm, J.; De Lichtenberg, K.; Thorup, M. (2001). "Poly-logarithmic deterministic fully-dynamic algorithms for connectivity, minimum spanning tree, 2-edge, and biconnectivity". Journal of the ACM. 48 (4): 723. doi:10.1145/502090.502095. S2CID 7273552

## Simplified explanation of the algorithm

Let's denote the number of vertices of the graph $n$.
Also let's say $l=\left \lceil{n}\right \rceil$.

In this algorithm, we assign each edge an integer level from $0$ to $l$. When inserted, each edge starts at level $0$. Under certain conditions, the level of an edge may increase, but never decrease.

We keep a set of $l$ forests $T_0$ through $T_l$. The forest $T_i$ is a spanning forest of all the edges of level at least $i$. This implies that $T_0$ is a spanning forest of the whole graph.

During the algorithm, we will be keeping two invariants:
1. $T_l \subseteq T_{l-1} \dots T_1 \subseteq T_0$ -- meaning if an edge is used in one forest, it must also be used at all the lower levels.
2. In $T_i$, every connected component has at most $n/2^i$ vertices.

## Euler tour tree

An Euler tour tree is a balanced binary tree that contains an euler tour of a tree. 

Specifically in this algorithm, we will split each edge of the tree into two directed edges. Then we take a walk of this tree, using each directed edge exactly once and store it in a balanced binary tree, such that the inorder travelsal of the tree is same as the walk.

For implementation reasons explained later, we also give each vertex one self loop. That means every Euler tour tree contains at least one vertex.

For the balanced binary tree, I decided to use splay trees. They are reasonably simple to implement, have good amortized complexity (which is enough since the whole algorithm has amortized complexity), their complexity is not randomized and they support simple splitting and merging.

## Forests

Since a forest is a set of trees, we can represent it as a set of Euler tour trees. Let's see how we'd go about adding and removing edges in a forest.

But before that, let's notice one thing. The euler tour of a tree starts in the root of the tree. This means that the root of the tree is the same as the source of the first edge in the walk. This means, that Euler tour trees can be simply rerooted - simply find any directed edge starting in the desired root, split the tree before it and paste the split part at the end. We can find an edge starting in any vertex by taking the beforementioned self-edge.

Now let's talk about inserting an edge between $a$ and $b$. This can be achieved by rerooting both trees to these vertices and making $b$ a child of $a$. We already know how to reroot. Since subtrees are contiguous segments in an Euler tour,  making $b$ a child of $a$ simply means adding edges $a \rightarrow b$ and $b \rightarrow a$ to the ends of $b$'s walk and concatenating the two walks together. 

Removing edge is the exact opposite - we just need to split the walk by both the directed versions of the deleted edge, delete these directed edges and merge the leftmost and rightmost part.

## Tags

For the rest of the algorithm to work, we need to give Euler tour trees one more ability - to hold tags. A tag is a piece of data attached to a node. Given a tree, we can fetch the oldest tag (the one that was added first) it contains. We can also remove a given tag and add a tag to a given vertex.

This can be achieved by the standard way of keeping subtree aggregate data in a tree - by just updating it every time one of it's children changes. To be able to fetch the oldest tag, we add the addition timestamp to each tag and hold the smallest one for each subtree. The fetched tag is then in the root of the Euler tour tree.

Adding and removing tags are trivial - we just splay the node with tag and update it directly.

Now let's talk about what we actually need the tags for. For each edge, we will keep a tag at each of it's endpoints. If this edge is on level $i$, these tags will also be in the forest level $i$.

## The whole thing

Now let's walk through the whole algorithm.

At the beginning, we create all the $l$ forests, each containing no edges. Then we need to process 3 types of updates:
1. Insert an edge
   - Insert this edge as tags at it's endpoints at level $0$.
   - If it's endpoints are not connected in $F_0$, add this edge to the forest.
2. Remove an edge
    - Remove the edge's tags.
    - Go through all the forests and check if it's used in any of them. If it is, remove it.
    - Now we need to find out, whether there is any edge that is currently not used in the forests but would reconnect the two components we just disconnected.
    - If there are multiple, we need to find the highest level one because of the first invariant.
    - We go through the levels from the highest.
      - On each level, look at the two trees that just got disconnected. Take the smaller one (one with less vertices).
      - Fetch a tag from this tree. There are 3 options:
        1. The edge connects two vertices within the tree
          - In this case we level up this edge.
          - Move this and the other tag of the edge a level higher
          - Add this edge to a forest level higher if it's used in this forest.
          - This is the reason we need the tags to be fetched in the correct order - if that didn't happen we could level up an unused edge before used one and used it in the new forest level. But this breaks the property that the forests are spanning and because of that is impossible.
          - Notice this keeps the second invariant - the smaller tree is at most half of the current level's limit size.
        2. The edge connects the two trees that just got disconnected
          - We've found our replacement edge, we add it to all the forests at lower and equal level
        3. The edge connects to some other tree
          - This would mean the edge connected to the other tree even before the deletion. But this breaks the property that the forests are spanning and because of that is impossible. 
3. Check if 2 edges are in the same components
   - Because of the first invariant, we just need to check whether they are connected in $F_0$. That can be done by splaying the self-edge of the first node, walking up the tree from the second and checking if we end up in the first. At the end we need to also splay to ensure correct complexity.
   - This operation does two splays, so it's amortized time complexity is $O(\log n)$.

The amortized time complexity of the first two operations is $O(\log^2 n)$. That's because each edge will get leveled up at most $\log n$ times, each levelup having time complexity $O(\log n)$. All other parts of these operations have time complexity $O(\log n)$ amortized.

# Implementation

The algorithm is implemented using several composed classes.

The first one is a templated `SplayNode` class. The splay tree is held together with pointers and each node contains a multiset of tags, the oldest tag in it's subtree and the size of the subtree. The class also has methods for merging and splitting the trees easily.

The splay trees form the spanning forests, which are stored in the `Level` class. This class has methods for rerooting merging and splitting trees in the forest.

All the levels are stored in the `DynamicConnectivityImpl` class, which performs the major operations.

And finally, it's all PIMPLed to the `DynamicConnectivity` class to hide unnecessary information from public headers.