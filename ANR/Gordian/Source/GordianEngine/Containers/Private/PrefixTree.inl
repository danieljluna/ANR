// Gordian by Daniel Luna (2019)

#include "GordianEngine/Debug/Public/Asserts.h"
#include "GordianEngine/Debug/Public/LogMacros.h"

DECLARE_LOG_CATEGORY_STATIC(LogPrefixTree, Log, Verbose)

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
	if (TestDivergingSubKey.length() <= DivergingSubKey.length())
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

		WordValue.Set(InWordValue);
		return true;
	}

	if (InOutNextFreeNode == nullptr)
	{
		GE_LOG(LogPrefixTree, Warning, "Could not add to prefix tree, not enough free nodes!");
		return false;
	}

	const std::map<CharType, TPrefixTreeNode<T>*>::const_iterator ExistingChildNode = ChildrenNodes.find(InWordDivergingSubKey[0]);
	if (ExistingChildNode != ChildrenNodes.cend())
	{
		check(ExistingChildNode->second != nullptr);
		TPrefixTreeNode<T>& RelatedChildNode = *ExistingChildNode->second;

		// If the existing child node has a matching diverging key...
		if (RelatedChildNode.HasMatchingDivergingKey(InWordDivergingSubKey))
		{
			//...Simply pass the call along!
			const KeyType& NewWordDivergingSubKey = InWordDivergingSubKey.substr(RelatedChildNode.DivergingSubKey.length());
			return RelatedChildNode.AddWord(NewWordDivergingSubKey, InWordValue, InOutNextFreeNode);
		}
		else
		{
			//...Split the child node so both sub-trees can exist...
			if (RelatedChildNode.SplitNode(InWordDivergingSubKey, InOutNextFreeNode))
			{
				//...Then we can pass the call to the intermediate node
				TPrefixTreeNode<T>& NewIntermediateNode = *RelatedChildNode.ParentNode;

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

	TPrefixTreeNode<T>& NewChildNode = GetNextFreeNode(InOutNextFreeNode);

	NewChildNode.ParentNode = this;
	NewChildNode.DivergingSubKey = InWordDivergingSubKey;
	NewChildNode.CachedFullKey = CachedFullKey.append(InWordDivergingSubKey);
	NewChildNode.WordValue.Set(InWordValue);

	ChildrenNodes[InWordDivergingSubKey[0]] = &NewChildNode;
	CachedDescendantWords.insert(NewChildNode.CachedFullKey);

	return true;
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
	NewChildNode.CachedFullKey = ParentNode->CachedFullKey.append(MatchingSubString);
	NewChildNode.CachedDescendantWords = CachedDescendantWords;
	NewChildNode.ChildrenNodes[ContestedSubString[0]] = &NewChildNode;

	// Update this node to be a child of new intermediate node
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
		for (size_t i = InReserveSize - 1; i > 0; --i)
		{
			_ReservedNodeSpace[i].NextUnusedNode = _NextUnusedNode;
			_NextUnusedNode = &_ReservedNodeSpace[i];
			_ReservedNodeSpace[i].bIsActive = false;
		}

		return true;
	}

	return false;
}

template<typename T>
inline bool Gordian::TPrefixTree<T>::Insert(const KeyType& Key, const T& Value)
{
	check(_ReservedNodeSpace != nullptr);

	return _ReservedNodeSpace[0].AddWord(Key, Value, _NextUnusedNode);
}


};	// namespace Gordian
