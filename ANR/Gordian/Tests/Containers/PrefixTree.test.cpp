#include <Catch.hpp>
#include "GordianEngine/Containers/Public/TPrefixTree.h"

using PrefixTreeTypeList = std::tuple<float, int*>;

TEMPLATE_LIST_TEST_CASE("Prefix Trees can be resized", "[template]", PrefixTreeTypeList)
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

TEMPLATE_LIST_TEST_CASE("Prefix Trees allow persistent insertion", "[template]", PrefixTreeTypeList)
{
	GIVEN("an empty Prefix Tree with some initial capacity")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TPrefixTree<TestType> PrefixTree(InitialReserveSize);

		const size_t InitialSize = PrefixTree.Num();
		REQUIRE(InitialSize == 0);

		const size_t InitialMinCapacity = PrefixTree.MinCapacity();
		const size_t InitialMaxCapacity = PrefixTree.MaxCapacity();
		REQUIRE(InitialMinCapacity > InitialSize);

		WHEN("inserting one element")
		{
			REQUIRE(PrefixTree.Insert("test", TestType()));

			THEN("size increases, but capacity remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 1);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity);
			}
		}
	}

	GIVEN("a Prefix Tree with one element and excessive capacity")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TPrefixTree<TestType> PrefixTree(InitialReserveSize);

		PrefixTree.Insert("alli", TestType());
		const size_t InitialSize = PrefixTree.Num();
		REQUIRE(InitialSize == 1);

		const size_t InitialMinCapacity = PrefixTree.MinCapacity();
		const size_t InitialMaxCapacity = PrefixTree.MaxCapacity();
		REQUIRE(InitialMinCapacity > InitialSize);

		WHEN("making two insertions with a unique leading key")
		{
			REQUIRE(PrefixTree.Insert("basket", TestType()));
			REQUIRE(PrefixTree.Insert("carb", TestType()));

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
			REQUIRE(PrefixTree.Insert("alligator", TestType()));
			REQUIRE(PrefixTree.Insert("alligators", TestType()));

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
			REQUIRE(PrefixTree.Insert("alligator", TestType()));
			REQUIRE(PrefixTree.Insert("carb", TestType()));

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
			REQUIRE(PrefixTree.Insert("alia", TestType()));

			// Large insertion - we will end up with a worse 'best case' capacity
			THEN("size increases, max cap decreases, min cap remains the same")
			{
				REQUIRE(PrefixTree.Num() == InitialSize + 1);
				REQUIRE(PrefixTree.MinCapacity() == InitialMinCapacity);
				REQUIRE(PrefixTree.MaxCapacity() == InitialMaxCapacity - 1);
			}
		}
	}
}

