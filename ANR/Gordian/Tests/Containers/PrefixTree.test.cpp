#include <Catch.hpp>
#include "GordianEngine/Containers/Public/TPrefixTree.h"

SCENARIO("Prefix Trees can be sized and resized", "[PrefixTree][Containers]")
{
	GIVEN("a Prefix Tree with some items")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TPrefixTree<int> IntTree(InitialReserveSize);

		REQUIRE(IntTree.Capacity() == InitialReserveSize);
		REQUIRE(IntTree.Num() == 0);

		WHEN("more capacity is reserved")
		{
			const size_t LargerReserveSize = 17;
			REQUIRE(LargerReserveSize > InitialReserveSize);

			IntTree.Reserve(LargerReserveSize);

			THEN("capacity increases")
			{
				REQUIRE(IntTree.Capacity() == LargerReserveSize);
			}
			THEN("size remains unchanged")
			{
				REQUIRE(IntTree.Num() == 0);
			}
		}
	}
}
