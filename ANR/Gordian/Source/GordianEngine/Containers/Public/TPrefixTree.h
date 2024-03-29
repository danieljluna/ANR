// Gordian by Daniel Luna (2019)

#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>

#include "GordianEngine/Containers/Public/TOptional.h"

namespace Gordian
{

template<typename T>
class TPrefixTree;

// Describes a node in a prefix tree
template<typename MappedT>
class TPrefixTreeNode
{
public:

	friend class TPrefixTree<MappedT>;

	//----------------------------------------------------------
	// Eventually these should be part of the template
	using KeyType	= std::string;
	using CharType	= char;
	//----------------------------------------------------------

	TPrefixTreeNode();

	// Accessors

	// Returns true if this node's key is a full word that was entered into the tree
	bool IsFullWord() const;

	// Returns the key for this node
	const KeyType& GetKey() const { return CachedFullKey; }


private:

	// Re-initializes the node
	void Initialize(bool bInIsActive);

	// Returns the next free node, updating the in-out pointer
	TPrefixTreeNode<MappedT>& GetNextFreeNode(TPrefixTreeNode<MappedT>*& InOutNextFreeNode) const;

	// Returns true if the passed in key could be a child key of this node
	bool HasMatchingKey(const KeyType& TestKey) const;

	// Returns true if the passed in diverging sub-key starts with this node's 
	//	DivergingSubKey.
	bool HasMatchingDivergingKey(const KeyType& TestDivergingSubKey) const;

	// Recursive search to find the first node with a DivergingSubKey >= InDivergingSubKey
	const TPrefixTreeNode<MappedT>* Find(const KeyType& InDivergingSubKey) const;

	// Adds a child word with the given key using the passed data struct.
	// Returns whether or not the child was successfully added.
	bool AddWord(const KeyType& InWordDivergingSubKey,
				 const MappedT& InWordValue,
				 TPrefixTreeNode<MappedT>*& InOutNextFreeNode,
				 size_t& OutNodesAdded);

	// Splits the node that overlaps the passed in key so both nodes can exist.
	// SplitSubKey should be the desired DivergingSubKey that this node competes with.
	bool SplitNode(const KeyType& SplitSubKey,
				   TPrefixTreeNode<MappedT>*& InOutNextFreeNode,
				   size_t& OutNodesAdded);

	// Copies all data in this node to the passed node, ignoring pointer specific values
	bool DeepCopy(TPrefixTreeNode<MappedT>& InDestination, TPrefixTreeNode<MappedT>*& OutNextFreeNode) const;

	// Map of all child nodes
	std::map<CharType, TPrefixTreeNode<MappedT>*> ChildrenNodes;

	// Cached set of all words for descendant nodes
	std::set<KeyType> CachedDescendantWords;

	// Portion of this node's key that diverges from parent
	KeyType DivergingSubKey;

	// Cache of this node's full key
	KeyType CachedFullKey;

	// If this node completes a word, then this is the value of that word key
	TOptional<MappedT> WordValue;

	// Flags if this is currently in use.
	// If it is, ParentNode is valid. Otherwise NextUnusedNode is valid.
	bool bIsActive;

	union
	{
		// Next Unused Node in allocation
		TPrefixTreeNode<MappedT>* NextUnusedNode;

		// Parent Node
		TPrefixTreeNode<MappedT>* ParentNode;
	};

};	// class TPrefixTreeNode


// A map from strings to some arbitrary type where a list of possible keys from a given prefix can be found.
template<typename MappedT>
class TPrefixTree
{
public:

	//----------------------------------------------------------
	// Eventually these should be part of the template
	using KeyType	= std::string;
	using CharType	= char;
	//----------------------------------------------------------
	

	TPrefixTree();
	TPrefixTree(size_t ReserveSize);
	TPrefixTree(const TPrefixTree& Other);
	TPrefixTree(TPrefixTree&& Other);

	~TPrefixTree();

	// Minimum number of words this tree can hold
	size_t MinCapacity() const;

	// Maximum number of words this tree can hold
	size_t MaxCapacity() const;

	// Number of complete words in this tree
	size_t Num() const;

	// Ensures there is enough space for ReserveSize words.
	bool Reserve(size_t SizeInWords);

	// Inserts a new pairing into the tree.
	// Returns true if the insert was successful.
	bool Insert(const KeyType& Key, 
				const MappedT& Value);

	// Inserts a new pairing into the tree.
	// Fails if there is not enough room to add the value
	// Returns true if the insert was successful.
	bool InsertRigid(const KeyType& Key,
					const MappedT& Value);

	// Returns true if the Prefix Tree contains a key-value pair at the specified key
	bool Contains(const KeyType& Key) const;

	// Returns the mapped value for the given key.
	// Returns nullptr if there was no mapped value at the specified key.
	const MappedT* Find(const KeyType& Key) const;

	// Returns the mapped value for the given key.
	// Asserts if there was no mapped value at the specified key.
	const MappedT& At(const KeyType& Key) const;

protected:

	bool Insert_Internal(const KeyType& Key,
						const MappedT& Value,
						bool bAllowResize);

private:

	enum EAccountForRootParam
	{
		AllocateRootNode = 0,
		IgnoreRootNode = 1
	};

	static TPrefixTreeNode<MappedT>* CreateAndFormatTreeAllocation(size_t DesiredSize);

	// Returns the max number of nodes required to store WordCount words
	static size_t GetMaxNodesRequired(size_t WordCount, EAccountForRootParam AccountForRoot);

	bool EnlargeTree(size_t DesiredIncrement);
	bool ResizeTree(size_t DesiredSize);

	// Returns true if the node space was increased
	bool EnsureUnusedNodeSpace(size_t DesiredUnusedSpace);

	bool HasAllocatedTree() const;

	// The next free node in the allocation
	TPrefixTreeNode<MappedT>* _NextUnusedNode;

	// Allocated Array of all nodes
	TPrefixTreeNode<MappedT>* _ReservedNodeSpace;

	size_t _ReserveSize;
	size_t _NumWords;
	size_t _NumNodes;


};	// class TPrefixTree


};	// namespace Gordian

#include "../Private/TPrefixTree.inl"
