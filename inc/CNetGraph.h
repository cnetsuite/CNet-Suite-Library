#pragma once

#ifndef CNET_GRAPH_H
#define CNET_GRAPH_H

#include "CNetIterator.h"
#include "CNetApi.h"

typedef struct sGraph Graph;
typedef struct sGraphNode GraphNode;
typedef struct sGraphEdge GraphEdge;

/**
 * Create an empty graph.
 *
 * @param directed  Nonzero to create a directed graph, 0 for an
 *                   undirected graph.
 * @return New empty Graph, owned by the caller and released with
 *         graphFree.
 */
CNET_API Graph* graphCreate(int directed);
/**
 * Free a graph and all of its nodes and edges. Stored node values are not
 * freed.
 *
 * @param graph  Graph to free.
 */
CNET_API void graphFree(Graph* graph);
/**
 * Check whether a graph is directed.
 *
 * @param graph  Graph to query.
 * @return 1 if directed, 0 if undirected.
 */
CNET_API int graphIsDirected(const Graph* graph);
/**
 * Get the number of nodes in a graph.
 *
 * @param graph  Graph to query.
 * @return Node count.
 */
CNET_API int graphNodeCount(const Graph* graph);
/**
 * Get the number of edges in a graph.
 *
 * @param graph  Graph to query.
 * @return Edge count.
 */
CNET_API int graphEdgeCount(const Graph* graph);

/**
 * Add a new, initially unconnected node holding a value.
 *
 * @param graph  Graph to add the node to.
 * @param value  Value to store by reference; caller retains ownership.
 * @return New GraphNode, owned by graph and released when graph is freed
 *         or the node is removed.
 */
CNET_API GraphNode* graphAddNode(Graph* graph, void* value);
/**
 * Remove a node and every edge incident to it.
 *
 * @param node  Node to remove.
 */
CNET_API void graphRemoveNode(GraphNode* node);

/**
 * Get the value stored in a node.
 *
 * @param node  Node to query.
 * @return The node's value.
 */
CNET_API void* graphNodeGetValue(const GraphNode* node);
/**
 * Replace the value stored in a node.
 *
 * @param node   Node to modify.
 * @param value  New value to store by reference; caller retains
 *               ownership.
 */
CNET_API void graphNodeSetValue(GraphNode* node, void* value);
/**
 * Get the number of edges leaving a node (its total edge count for an
 * undirected graph).
 *
 * @param node  Node to query.
 * @return Out-degree.
 */
CNET_API int graphNodeDegree(const GraphNode* node);
/**
 * Get the number of edges entering a node. For an undirected graph this
 * is the same as graphNodeDegree.
 *
 * @param node  Node to query.
 * @return In-degree.
 */
CNET_API int graphNodeInDegree(const GraphNode* node);

/**
 * Add a weighted edge between two nodes. In an undirected graph the edge
 * is traversable from either endpoint.
 *
 * @param graph   Graph the nodes belong to.
 * @param from    Source node; must not be NULL.
 * @param to      Target node; must not be NULL.
 * @param weight  Edge weight.
 * @return New GraphEdge, owned by graph and released when graph is freed
 *         or the edge is removed, or NULL (with an error set) if from or
 *         to is NULL.
 */
CNET_API GraphEdge* graphAddEdge(Graph* graph, GraphNode* from, GraphNode* to, double weight);
/**
 * Remove an edge from the graph and from both of its endpoints.
 *
 * @param edge  Edge to remove.
 */
CNET_API void graphRemoveEdge(GraphEdge* edge);
/**
 * Find the edge connecting two nodes, if any.
 *
 * @param from  Node to search from.
 * @param to    Node to look for a connection to.
 * @return The connecting edge, or NULL if from and to are not connected
 *         by an edge reachable from from.
 */
CNET_API GraphEdge* graphGetEdge(const GraphNode* from, const GraphNode* to);
/**
 * Check whether two nodes are connected by an edge.
 *
 * @param from  Node to search from.
 * @param to    Node to look for a connection to.
 * @return 1 if connected, 0 otherwise.
 */
CNET_API int graphHasEdge(const GraphNode* from, const GraphNode* to);

/**
 * Get the source endpoint of an edge.
 *
 * @param edge  Edge to query.
 * @return The edge's "from" node, as passed to graphAddEdge.
 */
CNET_API GraphNode* graphEdgeGetFrom(const GraphEdge* edge);
/**
 * Get the target endpoint of an edge.
 *
 * @param edge  Edge to query.
 * @return The edge's "to" node, as passed to graphAddEdge.
 */
CNET_API GraphNode* graphEdgeGetTo(const GraphEdge* edge);
/**
 * Get an edge's weight.
 *
 * @param edge  Edge to query.
 * @return The edge's weight.
 */
CNET_API double graphEdgeGetWeight(const GraphEdge* edge);
/**
 * Change an edge's weight.
 *
 * @param edge    Edge to modify.
 * @param weight  New weight.
 */
CNET_API void graphEdgeSetWeight(GraphEdge* edge, double weight);

/**
 * Create an iterator over all nodes in a graph. Supports removal (via
 * iteratorRemove) of the last-returned node, which also removes its
 * incident edges.
 *
 * @param graph  Graph to iterate.
 * @return New Iterator; caller should free it with iteratorFree when
 *         done.
 */
CNET_API Iterator* graphNodeIterator(Graph* graph);
/**
 * Create an iterator over all edges in a graph. Supports removal of the
 * last-returned edge via iteratorRemove.
 *
 * @param graph  Graph to iterate.
 * @return New Iterator; caller should free it with iteratorFree when
 *         done.
 */
CNET_API Iterator* graphEdgeIterator(Graph* graph);
/**
 * Create an iterator over the nodes directly reachable from a node via
 * its outgoing edges. Removal via iteratorRemove is not supported.
 *
 * @param node  Node whose neighbors to iterate.
 * @return New Iterator; caller should free it with iteratorFree when
 *         done.
 */
CNET_API Iterator* graphNeighborIterator(GraphNode* node);
/**
 * Create an iterator over the edges leaving a node. Removal via
 * iteratorRemove is not supported.
 *
 * @param node  Node whose incident edges to iterate.
 * @return New Iterator; caller should free it with iteratorFree when
 *         done.
 */
CNET_API Iterator* graphIncidentEdgeIterator(GraphNode* node);

/**
 * Create an iterator that performs a breadth-first traversal of the graph
 * starting at a given node. Removal via iteratorRemove is not supported.
 *
 * @param graph  Graph to traverse.
 * @param start  Node to start the traversal from; must not be NULL.
 * @return New Iterator, or NULL (with an error set) if start is NULL;
 *         caller should free it with iteratorFree when done to release
 *         its internal traversal state.
 */
CNET_API Iterator* graphBfsIterator(Graph* graph, GraphNode* start);
/**
 * Create an iterator that performs a depth-first traversal of the graph
 * starting at a given node. Removal via iteratorRemove is not supported.
 *
 * @param graph  Graph to traverse.
 * @param start  Node to start the traversal from; must not be NULL.
 * @return New Iterator, or NULL (with an error set) if start is NULL;
 *         caller should free it with iteratorFree when done to release
 *         its internal traversal state.
 */
CNET_API Iterator* graphDfsIterator(Graph* graph, GraphNode* start);

#endif
