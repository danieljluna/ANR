#include <Catch.hpp>
#include "GordianEngine/Containers/Public/TCircularBuffer.h"

using CircularBufferTypeList = std::tuple<float, int>;

TEMPLATE_LIST_TEST_CASE("Circular Buffer can be resized", "[template][containers]", CircularBufferTypeList)
{
	GIVEN("an empty circular buffer with some initial capacity")
	{
		const size_t InitialReserveSize = 5;
		Gordian::TCircularBuffer<TestType> CircularBuffer(InitialReserveSize);

		REQUIRE(CircularBuffer.Capacity() == InitialReserveSize);
		REQUIRE(CircularBuffer.Num() == 0);
	}
}

