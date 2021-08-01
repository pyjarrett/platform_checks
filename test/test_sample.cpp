#include <catch.hpp>

TEST_CASE("__cplusplus sanity check")
{
	// Visual Studio might not correctly assign __cplusplus if the correct
	// settings aren't applied due to legacy behavior.
	REQUIRE(__cplusplus == 201703L);
}
