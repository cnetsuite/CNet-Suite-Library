#pragma once

#ifndef CNET_XML_H
#define CNET_XML_H

#include "CNetGc.h"
#include "CNetApi.h"

typedef struct sXmlNode XmlNode;

/**
 * Create an empty element node. The tag name is copied, so the caller
 * retains ownership of `tagName`.
 *
 * @param gc       Arena the node is allocated from; borrowed.
 * @param tagName  Element tag name; must not be NULL.
 * @return New element XmlNode with no children or attributes, owned by
 *         the arena; NULL if `tagName` is NULL.
 */
CNET_API XmlNode* xmlCreateElement(Gc* gc, const wchar_t* tagName);

/**
 * Create a text node. The text is copied, so the caller retains
 * ownership of `text`.
 *
 * @param gc    Arena the node is allocated from; borrowed.
 * @param text  Text content; must not be NULL.
 * @return New text XmlNode, owned by the arena; NULL if `text` is NULL.
 */
CNET_API XmlNode* xmlCreateText(Gc* gc, const wchar_t* text);

/**
 * Test whether a node is a text node (as opposed to an element).
 *
 * @param node  Node to inspect; borrowed.
 * @return Nonzero if `node` is a text node, zero if it is an element.
 */
CNET_API int xmlIsText(const XmlNode* node);

/**
 * Get an element's tag name.
 *
 * @param node  Node to inspect; borrowed.
 * @return The tag name, borrowed from `node`; NULL if `node` is a text
 *         node.
 */
CNET_API const wchar_t* xmlGetTagName(const XmlNode* node);

/**
 * Get the raw text of a text node.
 *
 * @param node  Node to read; borrowed; must be a text node.
 * @return The text, borrowed from `node`; NULL (with an error set) if
 *         `node` is not a text node.
 */
CNET_API const wchar_t* xmlGetText(const XmlNode* node);

/**
 * Get the concatenated text of a node: the node's own text if it is a
 * text node, or the text of all descendant text nodes (depth-first) if
 * it is an element.
 *
 * @param node  Node to read; borrowed.
 * @return Text content; for an element this is newly allocated from
 *         `node`'s arena, for a text node it is borrowed from `node`.
 */
CNET_API const wchar_t* xmlGetTextContent(const XmlNode* node);

/**
 * Set (or overwrite) an attribute on an element. Name and value are
 * copied, so the caller retains ownership of both.
 *
 * @param element  Element to modify; borrowed; must not be a text node.
 * @param name     Attribute name.
 * @param value    Attribute value.
 * @return true on success; false if `element` is a text node.
 */
CNET_API bool xmlSetAttribute(XmlNode* element, const wchar_t* name, const wchar_t* value);

/**
 * Look up an attribute value by name.
 *
 * @param element  Element to inspect; borrowed; must not be a text node.
 * @param name     Attribute name to look up.
 * @return The attribute value, borrowed from `element`; NULL if
 *         `element` is a text node or has no attribute with that name.
 */
CNET_API const wchar_t* xmlGetAttribute(const XmlNode* element, const wchar_t* name);

/**
 * Test whether an element has an attribute with the given name.
 *
 * @param element  Element to inspect; borrowed.
 * @param name     Attribute name to look up.
 * @return Nonzero if the attribute exists, zero otherwise.
 */
CNET_API int xmlHasAttribute(const XmlNode* element, const wchar_t* name);

/**
 * Get the number of attributes on an element.
 *
 * @param element  Element to inspect; borrowed; must not be a text node.
 * @return Attribute count; 0 (with an error set) if `element` is a text
 *         node.
 */
CNET_API int xmlAttributeCount(const XmlNode* element);

/**
 * Get the name of the attribute at a position, in insertion order.
 *
 * @param element  Element to read; borrowed; must not be a text node.
 * @param index    Zero-based position.
 * @return The attribute name, borrowed from `element`; NULL if
 *         `element` is a text node or `index` is out of range.
 */
CNET_API const wchar_t* xmlAttributeNameAt(const XmlNode* element, int index);

/**
 * Get the value of the attribute at a position, in insertion order.
 *
 * @param element  Element to read; borrowed; must not be a text node.
 * @param index    Zero-based position.
 * @return The attribute value, borrowed from `element`; NULL if
 *         `element` is a text node or `index` is out of range.
 */
CNET_API const wchar_t* xmlAttributeValueAt(const XmlNode* element, int index);

/**
 * Append a child node to an element, after any existing children.
 *
 * @param parent  Element to modify; borrowed; must not be a text node.
 * @param child   Node to append; ownership stays with its arena,
 *                `parent` only stores a reference.
 * @return true on success; false if `parent` is a text node.
 */
CNET_API bool xmlAppendChild(XmlNode* parent, XmlNode* child);

/**
 * Get the number of children of a node.
 *
 * @param node  Node to inspect; borrowed.
 * @return Child count; 0 for text nodes.
 */
CNET_API int xmlChildCount(const XmlNode* node);

/**
 * Get the child at a position, in document order.
 *
 * @param node   Node to read; borrowed.
 * @param index  Zero-based position.
 * @return The child, borrowed from `node`; NULL if `index` is out of
 *         range.
 */
CNET_API XmlNode* xmlChildAt(const XmlNode* node, int index);

/**
 * Find the first element with the given tag name, searching `node`
 * itself and then its descendants depth-first.
 *
 * @param node     Subtree to search; borrowed.
 * @param tagName  Tag name to match.
 * @return The matching element, borrowed from `node`'s tree; NULL if no
 *         element with that tag name is found.
 */
CNET_API XmlNode* xmlFindElement(const XmlNode* node, const wchar_t* tagName);

/**
 * Parse XML text into a document tree. The XML declaration, DOCTYPE, and
 * comments are skipped rather than represented in the tree. Numeric
 * character references ("&#NNNN;"/"&#xHHHH;") emit their UTF-16 code
 * unit(s) directly - a reference to a supplementary-plane codepoint
 * produces a surrogate pair.
 *
 * @param gc    Arena the resulting tree is allocated from; borrowed.
 * @param text  NUL-terminated XML text to parse; caller retains
 *              ownership.
 * @return Root element of the parsed document, owned by `gc`; NULL on
 *         parse failure (see getLastError()/getLastErrorMessage()).
 */
CNET_API XmlNode* xmlParse(Gc* gc, const wchar_t* text);

/**
 * Serialize a node and its descendants back to XML text.
 *
 * @param gc    Arena the returned string is allocated from; borrowed.
 * @param node  Root node to serialize; borrowed.
 * @return NUL-terminated XML text, owned by `gc`.
 */
CNET_API wchar_t* xmlStringify(Gc* gc, const XmlNode* node);

#endif
