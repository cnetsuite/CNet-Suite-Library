#pragma once

#ifndef CNET_TREE_NODE_H
#define CNET_TREE_NODE_H

#include "CNetIterator.h"
#include "CNetApi.h"

typedef struct sTreeNode TreeNode;

/**
 * Create a standalone tree node holding a value.
 *
 * @param value  Value to store by reference; caller retains ownership.
 * @return New TreeNode with no parent and no children, owned by the
 *         caller and released with treeNodeFree.
 */
CNET_API TreeNode* treeNodeCreate(void* value);
/**
 * Free a node and, recursively, all of its descendants. Stored values are
 * not freed.
 *
 * @param node  Node (subtree root) to free.
 */
CNET_API void treeNodeFree(TreeNode* node);

/**
 * Get the value stored in a node.
 *
 * @param node  Node to query.
 * @return The node's value.
 */
CNET_API void* treeNodeGetValue(const TreeNode* node);
/**
 * Replace the value stored in a node.
 *
 * @param node   Node to modify.
 * @param value  New value to store by reference; caller retains
 *               ownership.
 */
CNET_API void treeNodeSetValue(TreeNode* node, void* value);

/**
 * Get a node's parent.
 *
 * @param node  Node to query.
 * @return Parent node, or NULL if node is a root.
 */
CNET_API TreeNode* treeNodeGetParent(const TreeNode* node);
/**
 * Check whether a node has no parent.
 *
 * @param node  Node to query.
 * @return 1 if node is a root, 0 otherwise.
 */
CNET_API int treeNodeIsRoot(const TreeNode* node);
/**
 * Check whether a node has no children.
 *
 * @param node  Node to query.
 * @return 1 if node is a leaf, 0 otherwise.
 */
CNET_API int treeNodeIsLeaf(const TreeNode* node);
/**
 * Walk up through ancestors to find the root of node's tree.
 *
 * @param node  Node to query.
 * @return The root ancestor (node itself if it is already a root).
 */
CNET_API TreeNode* treeNodeGetRoot(const TreeNode* node);
/**
 * Get the number of ancestors above a node.
 *
 * @param node  Node to query.
 * @return Depth, where a root node has depth 0.
 */
CNET_API int treeNodeGetDepth(const TreeNode* node);

/**
 * Get the number of direct children of a node.
 *
 * @param node  Node to query.
 * @return Child count.
 */
CNET_API int treeNodeChildCount(const TreeNode* node);
/**
 * Get a direct child by position.
 *
 * @param node   Node to query.
 * @param index  Zero-based child index; must be in [0, childCount).
 * @return Child at that index, or NULL (with an error set) if index is
 *         out of range.
 */
CNET_API TreeNode* treeNodeGetChild(const TreeNode* node, int index);
/**
 * Find the position of a direct child.
 *
 * @param node   Node to search.
 * @param child  Child node to look for (matched by pointer identity).
 * @return Zero-based index of child, or -1 if it is not a direct child of
 *         node.
 */
CNET_API int treeNodeIndexOfChild(const TreeNode* node, const TreeNode* child);

/**
 * Append a node as the last child of a parent. If child already has a
 * parent, it is first detached from it.
 *
 * @param parent  Node to add the child to.
 * @param child   Node to attach; must not be NULL and must not be parent
 *                itself.
 * @return true on success, false (with an error set) if parent or child
 *         is NULL, or if parent equals child.
 */
CNET_API bool treeNodeAddChild(TreeNode* parent, TreeNode* child);
/**
 * Insert a node as a child of a parent at a given position, shifting
 * later children up by one. Unlike treeNodeAddChild, child must not
 * already have a parent.
 *
 * @param parent  Node to insert the child into.
 * @param index   Zero-based position to insert at; must be in
 *                [0, childCount].
 * @param child   Node to attach; must not be NULL, must not be parent
 *                itself, and must not already have a parent.
 * @return true on success, false (with an error set) if the arguments are
 *         invalid, child already has a parent, or index is out of range.
 */
CNET_API bool treeNodeInsertChild(TreeNode* parent, int index, TreeNode* child);
/**
 * Remove a specific direct child from its parent.
 *
 * @param parent  Node to remove the child from.
 * @param child   Child node to remove (matched by pointer identity).
 * @return true if removed, false (with an error set) if child is not a
 *         direct child of parent.
 */
CNET_API bool treeNodeRemoveChild(TreeNode* parent, TreeNode* child);
/**
 * Remove and return the direct child at a given position, shifting later
 * children down by one.
 *
 * @param parent  Node to remove the child from.
 * @param index   Zero-based child index; must be in [0, childCount).
 * @return The removed child (now parentless), or NULL (with an error set)
 *         if index is out of range.
 */
CNET_API TreeNode* treeNodeRemoveChildAt(TreeNode* parent, int index);
/**
 * Detach a node from its parent, if it has one. A no-op that succeeds if
 * node is already a root.
 *
 * @param node  Node to detach.
 * @return true on success (including when node was already a root).
 */
CNET_API bool treeNodeRemoveFromParent(TreeNode* node);

/**
 * Create an iterator over a node's direct children, in order. Supports
 * removal of the last-returned child via iteratorRemove.
 *
 * @param node  Node whose children to iterate.
 * @return New Iterator; caller should free it with iteratorFree when
 *         done.
 */
CNET_API Iterator* treeNodeChildIterator(TreeNode* node);
/**
 * Create an iterator that walks node and all of its descendants in
 * pre-order (a node before its children). Removal via iteratorRemove is
 * not supported.
 *
 * @param node  Subtree root to iterate.
 * @return New Iterator; caller should free it with iteratorFree when
 *         done.
 */
CNET_API Iterator* treeNodePreOrderIterator(TreeNode* node);

#endif
