#include "stdafx.h"
#include "CppUnitTest.h"
#include "GordianEngine/Containers/Public/TPrefixTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Gordian;

namespace UT_Goridan
{

TEST_CLASS(UT_TPrefixTree)
{

public:
		
	TEST_METHOD(Construct_Int)
	{
		TEXT("This is a test");
		TPrefixTree<int> IntTree;
		Assert::AreEqual(size_t(0), IntTree.Capacity());
		Assert::AreEqual(size_t(0), IntTree.Num());
	}

	TEST_METHOD(Construct_EmptyStruct)
	{
		struct EmptyStruct {};

		TPrefixTree<EmptyStruct> EmptyStructTree;
	}
};

}