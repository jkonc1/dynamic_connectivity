## Introduction

This library solves the problem of dynamic connectivity online.

## Dynamic Connectivity

In the problem of dynamic connectivity we have a graph and receive queries to add or remove an edge. We also receive queries asking us whether there is a path between a pair of vertices. In the online version of the problem, we receive the queries one by one and must answer them immediately. The goal is to answer the queries as effeciently as possible.

This library also supports duplicate edges.

## Interface

The library provides a single class - *dnc::DynamicConnectivity*.

The class provides the following methods:
- *DynamicConnectivity(int n)*
  - Constructor - creates a graph with $n$ vertices and no edges.
- *void insert_edge(int u, int v)*
  - Adds an edge between vertices $u$ and $v$.
  - The vertices are numbered from $0$ to $n-1$.
  - If the edge already exists, another copy of it is added.
- *bool delete_edge(int u, int v)*
  - Removes an edge between vertices $u$ and $v$.
  - If such edge does not exist, nothing happens.
  - Returns whether the edge was removed.
- *bool is_connected(int u, int v)*
  - Returns whether there is a path between vertices $u$ and $v$.
  - If $u = v$, the function returns true.
- *unsigned connected_components()*
  - Returns the number of connected components in the graph.