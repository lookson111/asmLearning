#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace std::literals;

SCENARIO("model") {
	GIVEN("empty  model") {
		WHEN("maps not added") {
			CHECK(true);
		}
	}
}