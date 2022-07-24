#include <Catch.hpp>
#include "GordianEngine/Containers/Public/TPrefixTree.h"
#include "GordianEngine/Debug/Public/Exceptions.h"

using PrefixTreeTypeList = std::tuple<float, int>;

TEMPLATE_LIST_TEST_CASE("Prefix Trees can be resized", "[template][containers][prefix_tree]", PrefixTreeTypeList)
{
	GIVEN("an empty Prefix Tree with some initial capacity")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TPrefixTree<TestType> PrefixTree(InitialReserveSize);

		REQUIRE(PrefixTree.MinCapacity() == InitialReserveSize);

		const size_t InitialSize = PrefixTree.Num();
		REQUIRE(InitialSize == 0);

		const size_t DesiredMinCapacity = PrefixTree.MinCapacity();
		const size_t InitialMaxCapacity = PrefixTree.MaxCapacity();
		REQUIRE(InitialMaxCapacity >= DesiredMinCapacity);

		WHEN("calling reserve with a larger size")
		{
			const size_t LargerReserveSize = 17;
			REQUIRE(LargerReserveSize > InitialReserveSize);

			PrefixTree.Reserve(LargerReserveSize);

			THEN("capacity increases, but size remains the same")
			{
				REQUIRE(PrefixTree.MinCapacity() == LargerReserveSize);
				REQUIRE(PrefixTree.Num() == InitialSize);
			}
		}

		WHEN("calling reserve with a smaller size")
		{
			const size_t SmallerReserveSize = 3;
			REQUIRE(SmallerReserveSize < InitialReserveSize);

			PrefixTree.Reserve(SmallerReserveSize);

			THEN("capacity and size remain the same")
			{
				REQUIRE(PrefixTree.MinCapacity() == InitialReserveSize);
				REQUIRE(PrefixTree.Num() == InitialSize);
			}
		}

		WHEN("insertions are made until the Prefix Tree is full")
		{
			std::string NextTestKey = "a";
			size_t TotalInsertionsMade = 0;

			AND_WHEN("no inserted key shares a starting sub-string (i.e. no required splits)")
			{
				while (TotalInsertionsMade <= 2 * InitialMaxCapacity)
				{
					if (!PrefixTree.InsertRigid(NextTestKey, TestType()))
					{
						break;
					}

					++TotalInsertionsMade;
					NextTestKey[0] += 1;
				}

				THEN("Total insertions should equal the max capacity")
				{
					REQUIRE(TotalInsertionsMade == InitialMaxCapacity);
				}
			}

			AND_WHEN("each inserted key has a divergent leading-sub-string (i.e. all require splits)")
			{
				while (TotalInsertionsMade <= 2 * InitialMaxCapacity)
				{
					const char LastChar = NextTestKey.back();
					NextTestKey.push_back(LastChar);

					if (!PrefixTree.InsertRigid(NextTestKey, TestType()))
					{
						break;
					}

					++TotalInsertionsMade;

					const size_t IndexOfLastCharacter = NextTestKey.length() - 1;
					NextTestKey[IndexOfLastCharacter] += 1;
				}

				THEN("Total insertions should equal the min capacity")
				{
					REQUIRE(TotalInsertionsMade == DesiredMinCapacity);
				}
			}
		}
	}
}

TEMPLATE_LIST_TEST_CASE("Prefix Trees allow persistent insertion and lookup", "[template][containers][prefix_tree]", PrefixTreeTypeList)
{
	GIVEN("an empty Prefix Tree with some initial capacity")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TPrefixTree<TestType> PrefixTree(InitialReserveSize);

		REQUIRE(PrefixTree.MinCapacity() == InitialReserveSize);

		const size_t InitialSize = PrefixTree.Num();
		REQUIRE(InitialSize == 0);

		const size_t InitialMinCapacity = PrefixTree.MinCapacity();
		const size_t InitialMaxCapacity = PrefixTree.MaxCapacity();
		REQUIRE(InitialMaxCapacity >= InitialMinCapacity);

		WHEN("inserting a new element")
		{
			const char* TestKey = "test";
			const TestType TestValue = TestType(60);
			REQUIRE(PrefixTree.Insert(TestKey, TestValue));

			THEN("size increases, but capacity remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 1);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity);
			}

			THEN("inserted element is persistent and fetchable")
			{
				REQUIRE(PrefixTree.Contains(TestKey));

				const TestType& FoundValue = PrefixTree.At(TestKey);
				REQUIRE(FoundValue == TestValue);

				const TestType* FoundValuePtr = PrefixTree.Find(TestKey);
				REQUIRE(FoundValuePtr != nullptr);
				REQUIRE(&FoundValue == FoundValuePtr);
			}
		}

		WHEN("looking up any key")
		{
			const char* AnyKey = "AnyKey";

			THEN("lookup will fail")
			{
				REQUIRE(!PrefixTree.Contains(AnyKey));
				REQUIRE(PrefixTree.Find(AnyKey) == nullptr);
				REQUIRE_THROWS_AS(PrefixTree.At(AnyKey), Gordian::AssertionFailure);
			}
		}
	}

	GIVEN("a Prefix Tree with some elements and excessive capacity")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TPrefixTree<TestType> PrefixTree(InitialReserveSize);

		const char* TestKey = "alli";
		const TestType TestValue = TestType(60);
		REQUIRE(PrefixTree.Insert("zzz", TestType(15)));
		REQUIRE(PrefixTree.Insert(TestKey, TestValue));

		const size_t InitialSize = PrefixTree.Num();
		REQUIRE(InitialSize == 2);

		const size_t InitialMinCapacity = PrefixTree.MinCapacity();
		const size_t InitialMaxCapacity = PrefixTree.MaxCapacity();
		REQUIRE(InitialMinCapacity > InitialSize);

		WHEN("making two insertions with a unique leading key")
		{
			REQUIRE(PrefixTree.Insert("basket", TestType(3)));
			REQUIRE(PrefixTree.Insert("carb", TestType(6)));

			// Small insertion - we will end up with a better 'worst case' capacity
			THEN("size and min cap increases, max cap remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 2);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity + 1);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity);
			}
		}

		WHEN("making two insertions with a key that extends an existing key")
		{
			REQUIRE(PrefixTree.Insert("alligator", TestType(9)));
			REQUIRE(PrefixTree.Insert("alligators", TestType(35)));

			// Small insertion - we will end up with a better 'worst case' capacity
			THEN("size and min cap increases, max cap remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 2);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity + 1);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity);
			}
		}

		WHEN("making one extension insertion and one unique insertion")
		{
			REQUIRE(PrefixTree.Insert("alligator", TestType(65)));
			REQUIRE(PrefixTree.Insert("carb", TestType(29)));

			// Small insertion - we will end up with a better 'worst case' capacity
			THEN("size and min cap increases, max cap remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 2);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity + 1);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity);
			}
		}

		WHEN("making an insertion that shares a leading substring with an existing key")
		{
			REQUIRE(PrefixTree.Insert("alia", TestType(300)));

			// Large insertion - we will end up with a worse 'best case' capacity
			THEN("size increases, max cap decreases, min cap remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 1);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity - 1);
			}
		}

		WHEN("making additional insertions")
		{
			REQUIRE(PrefixTree.Insert("alia", TestType(234)));
			REQUIRE(PrefixTree.Insert("vuvuzela", TestType(1)));

			THEN("persistence is still maintained")
			{
				REQUIRE(PrefixTree.Contains(TestKey));

				const TestType& FoundValue = PrefixTree.At(TestKey);
				REQUIRE(FoundValue == TestValue);

				const TestType* FoundValuePtr = PrefixTree.Find(TestKey);
				REQUIRE(FoundValuePtr != nullptr);
				REQUIRE(&FoundValue == FoundValuePtr);
			}
		}

		WHEN("looking up any key")
		{
			THEN("keys are only found if they were inserted")
			{
				REQUIRE(PrefixTree.Contains(TestKey));

				const char* NewTestKey = "tramp";

				REQUIRE(!PrefixTree.Contains(NewTestKey));
				REQUIRE(PrefixTree.Insert(NewTestKey, TestType(457)));
				REQUIRE(PrefixTree.Contains(NewTestKey));
			}
		}
	}
}

