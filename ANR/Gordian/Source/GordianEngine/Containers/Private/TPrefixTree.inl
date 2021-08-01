// Gordian by Daniel Luna (2019)

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/LogMacros.h"
#include "GordianEngine/Debug/Public/LogCategory.h"

#include <algorithm>

DECLARE_LOG_CATEGORY_STATIC(LogPrefixTree, Verbose, Verbose)

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
	ChildrenNodes.empty();
	CachedDescendantWords.empty();
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
			return RelatedChildNode.Find(InDivergingSubKey);
		}
	}

	return nullptr;
}

template<typename T>
bool Gordian::TPrefixTreeNode<T>::AddWord(const KeyType& InWordDivergingSubKey,
										  const T& InWordValue,
										  TPrefixTreeNode<T>*& InOutNextFreeNode)
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
		return RelatedChildNode.AddWord(NewWordDivergingSubKey, InWordValue, InOutNextFreeNode);
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

		if (RelatedChildNode.SplitNode(InWordDivergingSubKey, InOutNextFreeNode))
		{
			//...Then we can pass the call to the intermediate node
			// Note we have to regrab the node because it has been altered by the SpliteNode call
			TPrefixTreeNode<T>& NewIntermediateNode = *ChildrenNodes[ChildKey]->ParentNode;

			const KeyType& NewWordDivergingSubKey = InWordDivergingSubKey.substr(NewIntermediateNode.DivergingSubKey.length());
			return NewIntermediateNode.AddWord(NewWordDivergingSubKey, InWordValue, InOutNextFreeNode);
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
											TPrefixTreeNode<T>*& InOutNextFreeNode)
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
	
	check(ParentNode != nullptr);
	// Replace parent's child reference to be the new intermediate node
	ParentNode->ChildrenNodes[DivergingSubKey[0]] = &NewChildNode;

	const KeyType& ContestedSubString = DivergingSubKey.substr(MatchingSubString.length());

	NewChildNode.ParentNode = ParentNode;
	NewChildNode.DivergingSubKey = MatchingSubString;
	NewChildNode.CachedFullKey = ParentNode->CachedFullKey;
	NewChildNode.CachedFullKey.append(MatchingSubString);
	NewChildNode.CachedDescendantWords = CachedDescendantWords;

	// Update this node to be a child of new intermediate node
	NewChildNode.ChildrenNodes[ContestedSubString[0]] = this;
	ParentNode = &NewChildNode;
	DivergingSubKey = ContestedSubString;

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
}

template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree(size_t InReserveSize)
	: TPrefixTree()
{
	Reserve(InReserveSize);
}

template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree(const Gordian::TPrefixTree<T>& Other)
	: TPrefixTree()
{
	Reserve(Other._ReserveSize);
	for (size_t i = 0; i < Other._ReserveSize; ++i)
	{
		if (Other._ReservedNodeSpace[i] != nullptr)
		{
			_ReservedNodeSpace[i] = Other._ReservedNodeSpace[i];
		}
	}
}

template<typename T>
Gordian::TPrefixTree<T>::TPrefixTree(Gordian::TPrefixTree<T>&& Other)
{
	_ReservedNodeSpace = Other._ReservedNodeSpace;
	Other._ReservedNodeSpace = nullptr;

	_ReserveSize = Other._ReserveSize;
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
inline size_t Gordian::TPrefixTree<T>::Capacity() const
{
	return _ReserveSize;
}

template<typename T>
inline size_t Gordian::TPrefixTree<T>::Num() const
{
	return _Num;
}

template<typename T>
inline bool Gordian::TPrefixTree<T>::Reserve(size_t InReserveSize)
{
	if (InReserveSize <= _ReserveSize)
	{
		return true;
	}

	// todo: this code does not allow resizing
	ensure(_ReservedNodeSpace == nullptr);
	if (_ReservedNodeSpace == nullptr)
	{
		_ReservedNodeSpace = new TPrefixTreeNode<T>[InReserveSize];
		_ReserveSize = InReserveSize;
		_NextUnusedNode = nullptr;

		// Mark root as in use
		_ReservedNodeSpace[0].bIsActive = true;

		// Set up linked list of unused nodes
		size_t i = InReserveSize;
		do
		{
			--i;
			_ReservedNodeSpace[i].NextUnusedNode = _NextUnusedNode;
			_NextUnusedNode = &_ReservedNodeSpace[i];
			_ReservedNodeSpace[i].bIsActive = false;
		} while (i > 0);

		// Always use the root
		_ReservedNodeSpace[0].bIsActive = true;
		_NextUnusedNode = _ReservedNodeSpace[0].NextUnusedNode;

		return true;
	}

	return false;
}

template<typename T>
inline bool Gordian::TPrefixTree<T>::Insert(const KeyType& Key, const T& Value)
{
	check(_ReservedNodeSpace != nullptr && _ReservedNodeSpace[0].bIsActive);

	const bool bAddedWord = _ReservedNodeSpace[0].AddWord(Key, Value, _NextUnusedNode);
	if (bAddedWord)
	{
		++_Num;
	}

	return bAddedWord;
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

	if (Contains(Key))
	{
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
	}

	return nullptr;
}

template<typename T>
const T& Gordian::TPrefixTree<T>::At(const KeyType& Key) const
{
	const T* FindResult = Find(Key);
	check(FindResult != nullptr);
	return *T;
}


};	// namespace Gordian
