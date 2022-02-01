// Gordian by Daniel Luna (2019)

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/Logging.h"

#include <algorithm>

DECLARE_LOG_CATEGORY_STATIC(LogPrefixTree, Log, Log)

namespace Gordian
{

//--------------------------------------------------------------------------
// PrefixTreeNode
//--------------------------------------------------------------------------

template<typename T>
Gordian::TPrefixTreeNode<T>::TPrefixTreeNode()
{
	Initialize(false);
}

template<typename T>
void Gordian::TPrefixTreeNode<T>::Initialize(bool bInIsActive)
{
	bIsActive = bInIsActive;
	WordValue.Clear();
	DivergingSubKey = "";
	CachedFullKey = "";
	bIsActive ? ParentNode : NextUnusedNode = nullptr;
	ChildrenNodes.clear();
	CachedDescendantWords.clear();
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::IsFullWord() const
{
	return WordValue.IsSet();
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::HasMatchingKey(const KeyType& TestKey) const
{
	if (TestKey.length() <= CachedFullKey.length())
	{
		return false;
	}

	for (size_t i = 0; i < CachedFullKey.length(); ++i)
	{
		if (TestKey[i] != CachedFullKey[i])
		{
			return false;
		}
	}
	
	return true;
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::HasMatchingDivergingKey(const KeyType& TestDivergingSubKey) const
{
	if (TestDivergingSubKey.length() < DivergingSubKey.length())
	{
		return false;
	}

	for (size_t i = 0; i < DivergingSubKey.length(); ++i)
	{
		if (TestDivergingSubKey[i] != DivergingSubKey[i])
		{
			return false;
		}
	}

	return true;
}

template<typename T>
const Gordian::TPrefixTreeNode<T>* Gordian::TPrefixTreeNode<T>::Find(const KeyType& InDivergingSubKey) const
{
	if (InDivergingSubKey.length() == 0)
	{
		return this;
	}

	const std::map<CharType, TPrefixTreeNode<T>*>::const_iterator ExistingChildNode = ChildrenNodes.find(InDivergingSubKey[0]);
	if (ExistingChildNode != ChildrenNodes.cend())
	{
		check(ExistingChildNode->second != nullptr);
		TPrefixTreeNode<T>& RelatedChildNode = *ExistingChildNode->second;

		if (RelatedChildNode.HasMatchingDivergingKey(InDivergingSubKey))
		{
			const KeyType& NewDivergingSubKey = InDivergingSubKey.substr(RelatedChildNode.DivergingSubKey.length());
			return RelatedChildNode.Find(NewDivergingSubKey);
		}
	}

	return nullptr;
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::AddWord(const KeyType& InWordDivergingSubKey,
										  const T& InWordValue,
										  TPrefixTreeNode<T>*& InOutNextFreeNode,
										  size_t& OutNodesAdded)
{
	if (InWordDivergingSubKey.length() == 0)
	{
		if (IsFullWord())
		{
			GE_LOG(LogPrefixTree, Display, "Tried to add a node that already exists (%s)", CachedFullKey.c_str());
			return false;
		}

		GE_LOG(LogPrefixTree, Verbose, "...Marking existing node (%s) as full word.", CachedFullKey.c_str());
		WordValue.Set(InWordValue);
		return true;
	}

	if (InOutNextFreeNode == nullptr)
	{
		GE_LOG(LogPrefixTree, Warning, "Could not add to prefix tree, not enough free nodes!");
		return false;
	}

	if (GE_IS_LOG_ACTIVE(LogPrefixTree, Verbose))
	{
		std::string LogFormat = "Children:";
		if (ChildrenNodes.size() == 0)
		{
			LogFormat.append("None");
		}
		else
		{
			for (auto const& Pair : ChildrenNodes)
			{
				LogFormat.append("(");
				LogFormat.push_back(Pair.first);
				LogFormat.append(",");
				LogFormat.append(Pair.second->DivergingSubKey);
				LogFormat.append(") ");
			}
		}

		GE_LOG(LogPrefixTree, Verbose, "%s", LogFormat.c_str());
	}


	const std::map<CharType, TPrefixTreeNode<T>*>::const_iterator ExistingChildNode = ChildrenNodes.find(InWordDivergingSubKey[0]);
	if (ExistingChildNode == ChildrenNodes.cend())
	{
		// No child exists with a matching next character
		TPrefixTreeNode<T>& NewChildNode = GetNextFreeNode(InOutNextFreeNode);
		++OutNodesAdded;

		NewChildNode.ParentNode = this;
		NewChildNode.DivergingSubKey = InWordDivergingSubKey;
		NewChildNode.CachedFullKey = CachedFullKey;
		NewChildNode.CachedFullKey.append(InWordDivergingSubKey);
		NewChildNode.WordValue.Set(InWordValue);

		ChildrenNodes.emplace(NewChildNode.DivergingSubKey[0], &NewChildNode);
		CachedDescendantWords.insert(NewChildNode.CachedFullKey);

		GE_LOG(LogPrefixTree,
			   Verbose,
			   "...Tree ends without matching child node, adding leaf node. [DivergingSubKey - %s, CachedFullKey - %s]",
			   NewChildNode.DivergingSubKey.c_str(), 
			   NewChildNode.CachedFullKey.c_str());

		return true;
	}
	
	// If we get here, there is an existing competing node
	check(ExistingChildNode->second != nullptr);
	TPrefixTreeNode<T>& RelatedChildNode = *ExistingChildNode->second;

	// If the existing child node has a matching diverging key...
	if (RelatedChildNode.HasMatchingDivergingKey(InWordDivergingSubKey))
	{
		//...Simply pass the call along!
		GE_LOG(LogPrefixTree, 
			   Verbose, 
			   "...Passing call to child node (%s) with matching diverging key (%s)...", 
			   RelatedChildNode.DivergingSubKey.c_str(),
			   InWordDivergingSubKey.c_str());

		const KeyType& NewWordDivergingSubKey = InWordDivergingSubKey.substr(RelatedChildNode.DivergingSubKey.length());
		return RelatedChildNode.AddWord(NewWordDivergingSubKey, InWordValue, InOutNextFreeNode, OutNodesAdded);
	}
	else
	{
		//...Split the child node so both sub-trees can exist...
		GE_LOG(LogPrefixTree,
			   Verbose,
			   "...Splitting node so the existing key (%s) can exist side by side with the new competing key (%s)...",
			   RelatedChildNode.DivergingSubKey.c_str(),
			   InWordDivergingSubKey.c_str());

		const CharType ChildKey = InWordDivergingSubKey[0];

		if (RelatedChildNode.SplitNode(InWordDivergingSubKey, InOutNextFreeNode, OutNodesAdded))
		{
			//...Then we can pass the call to the intermediate node
			// Note we have to regrab the node because it has been altered by the SpliteNode call
			TPrefixTreeNode<T>& NewIntermediateNode = *ChildrenNodes[ChildKey];

			const KeyType& NewWordDivergingSubKey = InWordDivergingSubKey.substr(NewIntermediateNode.DivergingSubKey.length());
			return NewIntermediateNode.AddWord(NewWordDivergingSubKey, InWordValue, InOutNextFreeNode, OutNodesAdded);
		}
		else
		{
			GE_LOG(LogPrefixTree,
					Warning,
					"Failed to split child (%s) with diverging sub-key (%s)!",
					RelatedChildNode.CachedFullKey.c_str(),
					InWordDivergingSubKey.c_str());

			return false;
		}
	}
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::SplitNode(const KeyType& SplitSubKey,
											TPrefixTreeNode<T>*& InOutNextFreeNode,
											size_t& OutNodesAdded)
{
	check(DivergingSubKey.length() > 0);
	if (SplitSubKey.length() == 0)
	{
		GE_LOG(LogPrefixTree, Warning, "Tried to split a node with an empty SplitKey (Full Key: %s)", CachedFullKey.c_str());
		return false;
	}

	if (HasMatchingDivergingKey(SplitSubKey))
	{
		GE_LOG(LogPrefixTree, Warning, "Tried to split a node (%s) with an compatible SplitKey (%s)", CachedFullKey.c_str(), SplitSubKey.c_str());
		return false;
	}

	if (InOutNextFreeNode == nullptr)
	{
		GE_LOG(LogPrefixTree, Warning, "Could not split prefix tree, not enough free nodes!");
		return false;
	}

	KeyType MatchingSubString = "";
	const size_t SmallerDivergingSubKeyLength = std::min(DivergingSubKey.length(), SplitSubKey.length());
	for (size_t i = 0; i < SmallerDivergingSubKeyLength; ++i)
	{
		if (DivergingSubKey[i] != SplitSubKey[i])
		{
			break;
		}

		MatchingSubString.push_back(SplitSubKey[i]);
	}

	if (MatchingSubString.length() == 0)
	{
		GE_LOG(LogPrefixTree, Warning, "Tried to split a node (%s) with a non-competing SplitKey (%s)", DivergingSubKey.c_str(), SplitSubKey.c_str());
		return false;
	}
	
	// Create new node at split
	TPrefixTreeNode<T>& NewChildNode = GetNextFreeNode(InOutNextFreeNode);
	++OutNodesAdded;
	
	check(ParentNode != nullptr);
	// Replace parent's child reference to be the new intermediate node
	ParentNode->ChildrenNodes[DivergingSubKey[0]] = &NewChildNode;
	NewChildNode.ParentNode = ParentNode;

	NewChildNode.DivergingSubKey = MatchingSubString;
	NewChildNode.CachedFullKey = ParentNode->CachedFullKey;
	NewChildNode.CachedFullKey.append(NewChildNode.DivergingSubKey);

	NewChildNode.CachedDescendantWords = CachedDescendantWords;

	const KeyType& ContestedSubString = DivergingSubKey.substr(MatchingSubString.length());

	// Update this node to be a child of new intermediate node
	NewChildNode.ChildrenNodes[ContestedSubString[0]] = this;
	ParentNode = &NewChildNode;

	DivergingSubKey = ContestedSubString;

	return true;
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::DeepCopy(TPrefixTreeNode<T>& InDestination, TPrefixTreeNode<T>*& OutNextFreeNode) const
{
	if (!bIsActive)
	{
		GE_LOG(LogPrefixTree, Error, "Tried to copy a node that is currently not active");
		return false;
	}

	if (InDestination.ChildrenNodes.size() > 0)
	{
		GE_LOG(LogPrefixTree, Error, "Tried to copy a node into another node with existing children");
		return false;
	}

	InDestination.bIsActive = bIsActive;
	InDestination.CachedDescendantWords = CachedDescendantWords;
	InDestination.CachedFullKey = CachedFullKey;
	InDestination.DivergingSubKey = DivergingSubKey;
	InDestination.WordValue = WordValue;

	for (const std::pair<CharType, TPrefixTreeNode<T>*>& KeyValuePair : ChildrenNodes)
	{
		const CharType& ChildKey = KeyValuePair.first;
		check(KeyValuePair.second != nullptr);
		const TPrefixTreeNode<T>& ChildNode = *KeyValuePair.second;
		Gordian::TPrefixTreeNode<T>& NewChildAllocation = GetNextFreeNode(OutNextFreeNode);

		NewChildAllocation.ParentNode = &InDestination;
		
		ChildNode.DeepCopy(NewChildAllocation, OutNextFreeNode);

		InDestination.ChildrenNodes.emplace(ChildKey, &NewChildAllocation);
	}

	return true;
}

template<typename T>
TPrefixTreeNode<T>& Gordian::TPrefixTreeNode<T>::GetNextFreeNode(TPrefixTreeNode<T>*& InOutNextFreeNode) const
{
	check(InOutNextFreeNode != nullptr);
	TPrefixTreeNode<T>* NewChildNode = InOutNextFreeNode;
	InOutNextFreeNode = InOutNextFreeNode->NextUnusedNode;

	// Safety initialization
	NewChildNode->Initialize(true);

	return *NewChildNode;
}



//--------------------------------------------------------------------------
// Prefix Tree
//--------------------------------------------------------------------------
template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree()
{
	_NextUnusedNode = nullptr;
	_ReservedNodeSpace = nullptr;
	_ReserveSize = 0;
	_NumWords = 0;
	_NumNodes = 0;
}

template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree(size_t InReserveSize)
	: TPrefixTree()
{
	Reserve(InReserveSize);
}

template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree(const Gordian::TPrefixTree<T>& Other)
	: TPrefixTree(Other._ReserveSize)
{
	_ReservedNodeSpace[0].DeepCopy(Other._ReservedNodeSpace[0], _NextUnusedNode);
	_NumWords = Other._NumWords;
	_NumNodes = Other._NumNodes;
}

template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree(Gordian::TPrefixTree<T>&& Other)
{
	_ReservedNodeSpace = Other._ReservedNodeSpace;
	Other._ReservedNodeSpace = nullptr;

	_NextUnusedNode = Other._NextUnusedNode;
	Other._NextUnusedNode = nullptr;

	_ReserveSize = Other._ReserveSize;
	Other._ReserveSize = 0;
	_NumWords = Other._NumWords;
	Other._NumWords = 0;
	_NumNodes = Other._NumNodes;
	Other._NumNodes = 0;
}

template<typename T>
Gordian::TPrefixTree<T>::~TPrefixTree()
{
	if (_ReservedNodeSpace != nullptr)
	{
		delete[] _ReservedNodeSpace;
		_ReservedNodeSpace = nullptr;
	}
}

template<typename T>
inline size_t Gordian::TPrefixTree<T>::MinCapacity() const
{
	// The first word always requires exactly two nodes.
	// Worst case, each additional word requires a new split to be made.
	// When a split is made, one node becomes 3 (preexisting word -> split + preexisting suffix + new word suffix)
	// i.e. ReserveSize = 2 * WordCount 

	if (_NumNodes <= 2)
	{
		// Root costs 1, first word is never a split - so total 2 nodes
		return _ReserveSize / 2;
	}
	else
	{
		check(_ReserveSize >= _NumNodes);

		const size_t UnusedNodeCount = _ReserveSize - _NumNodes;
		const size_t MinimumCapacityOfUnusedNodes = UnusedNodeCount / 2;
		const size_t ReturnValue = _NumWords + MinimumCapacityOfUnusedNodes;
		return ReturnValue;
	}
}

template<typename T>
inline size_t Gordian::TPrefixTree<T>::MaxCapacity() const
{
	// The first word always requires exactly two nodes.
	// Best case, each additional word requires no splits - i.e. they all uniquely branch from the root
	// When no split is made, only one node is added
	// i.e. ReserveSize = WordCount + 1

	if (_NumNodes <= 1)
	{
		return _ReserveSize - 1;
	}
	else
	{
		check(_ReserveSize >= _NumNodes);

		const size_t UnusedNodeCount = _ReserveSize - _NumNodes;
		const size_t MaximumCapacityOfUnusedNodes = UnusedNodeCount;
		const size_t ReturnValue = _NumWords + MaximumCapacityOfUnusedNodes;
		return ReturnValue;
	}
}

template<typename T>
inline size_t Gordian::TPrefixTree<T>::Num() const
{
	return _NumWords;
}

template<typename T>
bool Gordian::TPrefixTree<T>::Reserve(size_t SizeInWords)
{
	check(_ReserveSize >= 0);

	const size_t SizeInNodes = GetMaxNodesRequired(SizeInWords, AllocateRootNode);
	if (_ReserveSize >= SizeInNodes)
	{
		return true;
	}

	return ResizeTree(SizeInNodes);
}

template<typename T>
inline bool Gordian::TPrefixTree<T>::Insert(const KeyType& Key, const T& Value)
{
	return Insert_Internal(Key, Value, true);
}

template<typename T>
bool Gordian::TPrefixTree<T>::InsertRigid(const KeyType& Key,
										  const T& Value)
{
	return Insert_Internal(Key, Value, false);
}

template<typename T>
bool Gordian::TPrefixTree<T>::Contains(const KeyType& Key) const
{
	if (_ReservedNodeSpace == nullptr || !_ReservedNodeSpace[0].bIsActive)
	{
		GE_LOG(LogPrefixTree, Warning, "Tried to check if an uninitialized prefix tree contained key (%s)", Key.c_str());
		return false;
	}

	auto NodeMatchingKey = _ReservedNodeSpace[0].CachedDescendantWords.find(Key);

	return NodeMatchingKey != _ReservedNodeSpace[0].CachedDescendantWords.cend();
}

template<typename T>
const T* Gordian::TPrefixTree<T>::Find(const KeyType& Key) const
{
	check(_ReservedNodeSpace != nullptr && _ReservedNodeSpace[0].bIsActive);

	if (!Contains(Key))
	{
		return nullptr;
	}

	const TPrefixTreeNode<T>* _FoundNode = _ReservedNodeSpace[0].Find(Key);
	if (_FoundNode != nullptr)
	{
		check(_FoundNode->bIsActive);
		if (_FoundNode->GetKey() == Key && _FoundNode->IsFullWord())
		{
			return &_FoundNode->WordValue.Get();
		}
	}
	else
	{
		GE_LOG(LogPrefixTree,
			   Error,
			   "Root node belives it has a descendant of key '%s' but the matching node could not be found!",
			   Key.c_str());
	}

	return false;
}

template<typename T>
const T& Gordian::TPrefixTree<T>::At(const KeyType& Key) const
{
	const T* FindResult = Find(Key);
	check(FindResult != nullptr);
	return *FindResult;
}

template<typename T>
bool Gordian::TPrefixTree<T>::Insert_Internal(const KeyType& Key,
											  const T& Value,
											  bool bAllowResize)
{
	check(_ReservedNodeSpace != nullptr);
	check(_ReservedNodeSpace[0].bIsActive);

	bool bAddedWord = _ReservedNodeSpace[0].AddWord(Key, Value, _NextUnusedNode, _NumNodes);
	if (bAllowResize && !bAddedWord && EnsureUnusedNodeSpace(2))
	{
		// We just needed more node space, try again
		bAddedWord = _ReservedNodeSpace[0].AddWord(Key, Value, _NextUnusedNode, _NumNodes);
	}

	if (bAddedWord)
	{
		++_NumWords;
	}

	return bAddedWord;
}

template<typename T>
Gordian::TPrefixTreeNode<T>* Gordian::TPrefixTree<T>::CreateAndFormatTreeAllocation(size_t InDesiredSize)
{
	check(InDesiredSize > 0);

	TPrefixTreeNode<T>* NewAllocation = new TPrefixTreeNode<T>[InDesiredSize];
	check(NewAllocation != nullptr);

	// Set up reserve space as a linked list of unused nodes 
	TPrefixTreeNode<T>* LastUnusedNode = nullptr;
	size_t i = InDesiredSize;
	while (i > 0)
	{
		--i;
		NewAllocation[i].NextUnusedNode = LastUnusedNode;
		LastUnusedNode = &NewAllocation[i];
		NewAllocation[i].bIsActive = false;
	}

	// Mark root as in use
	NewAllocation[0].Initialize(true);

	return NewAllocation;
}

template<typename T>
size_t Gordian::TPrefixTree<T>::GetMaxNodesRequired(size_t WordCount, EAccountForRootParam AccountForRoot)
{
	// Each extra node needs at most 2 additional nodes created, ignoring the first node
	// If we're accounting for the root, then the math is simple
	size_t MaxNodesRequired = 2 * WordCount;

	if (AccountForRoot == IgnoreRootNode)
	{
		// If we are not, just remove the one node needed by the root
		--MaxNodesRequired;
	}

	return MaxNodesRequired;
}

template<typename T>
bool Gordian::TPrefixTree<T>::EnlargeTree(size_t DesiredIncrement)
{
	if (DesiredIncrement < 0)
	{
		return false;
	}

	return ResizeTree(_ReserveSize + DesiredIncrement);
}

template<typename T>
bool Gordian::TPrefixTree<T>::ResizeTree(size_t InDesiredSize)
{
	check(InDesiredSize > 0);

	checkMsgf(_NumNodes <= _ReserveSize, "PrefixTree should never have more items in it than it has reserved space for");
	checkMsgf(_NumNodes >= 0, "PrefixTree should always have a non-negative item count");
	checkMsgf(_ReserveSize >= 0, "PrefixTree should always have a non-negative reserve size");

	if (_ReserveSize >= InDesiredSize)
	{
		return true;
	}

	check(InDesiredSize >= 1);

	TPrefixTreeNode<T>* NewAllocation = CreateAndFormatTreeAllocation(InDesiredSize);
	check(NewAllocation != nullptr);

	// Mark root as in use
	NewAllocation[0].bIsActive = true;
	_NextUnusedNode = NewAllocation[0].NextUnusedNode;
	_NumNodes = 1;

	if (_NumWords > 0)
	{
		_ReservedNodeSpace[0].DeepCopy(NewAllocation[0], _NextUnusedNode);
	}

	// Commit new allocation
	delete[] _ReservedNodeSpace;
	_ReservedNodeSpace = NewAllocation;
	_ReserveSize = InDesiredSize;

	return true;
}

template<typename T>
bool Gordian::TPrefixTree<T>::HasAllocatedTree() const
{
	return _ReservedNodeSpace != nullptr && _ReservedNodeSpace[0].bIsActive;
}

template<typename T>
bool Gordian::TPrefixTree<T>::EnsureUnusedNodeSpace(size_t DesiredUnusedSpace)
{
	const size_t MissingNodeSpace = _NumNodes + DesiredUnusedSpace - _ReserveSize;
	if (MissingNodeSpace > 0)
	{
		return EnlargeTree(MissingNodeSpace);
	}

	return false;
}


};	// namespace Gordian
