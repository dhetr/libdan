#include <catch2/catch_test_macros.hpp>

#include <unit.h>

using libdan::internal::Dimension;
using libdan::internal::Unit;

TEST_CASE("Unit stores its dimension, scale, and symbol", "[unit]") {
	const Dimension length = Dimension::base(0);
	const Unit metre(length, 1.0, "m");

	CHECK(metre.dimension() == length);
	CHECK(metre.scale() == 1.0);
	CHECK(metre.symbol() == "m");
}

TEST_CASE("Unit accepts positive finite scales", "[unit]") {
	const Dimension length = Dimension::base(0);

	SECTION("Fractional scale") {
		const Unit centimetre(length, 0.01, "cm");
		CHECK(centimetre.scale() == 0.01);
	}

	SECTION("Large scale") {
		const Unit kilometre(length, 1000.0, "km");
		CHECK(kilometre.scale() == 1000.0);
	}
}

TEST_CASE("Unit rejects invalid scales", "[unit]") {
	const Dimension length = Dimension::base(0);

	SECTION("Zero")
	{
		CHECK_THROWS_AS(Unit(length, 0.0, "m"), std::invalid_argument);
	}

	SECTION("Negative")
	{
		CHECK_THROWS_AS(Unit(length, -1.0, "m"), std::invalid_argument);
	}

	SECTION("NaN")
	{
		CHECK_THROWS_AS(
			Unit(length, std::numeric_limits<double>::quiet_NaN(), "m"),
			std::invalid_argument
		);
	}

	SECTION("Positive infinity")
	{
		CHECK_THROWS_AS(
			Unit(length, std::numeric_limits<double>::infinity(), "m"),
			std::invalid_argument
		);
	}

	SECTION("Negative infinity")
	{
		CHECK_THROWS_AS(
			Unit(length, -std::numeric_limits<double>::infinity(), "m"),
			std::invalid_argument
		);
	}
}

TEST_CASE("Unit rejects an empty symbol", "[unit]") {
	const Dimension length = Dimension::base(0);

	CHECK_THROWS_AS(Unit(length, 1.0, ""), std::invalid_argument);
}

TEST_CASE("Unit preserves its symbol", "[unit]") {
	const Dimension time = Dimension::base(1);
	const Unit second(time, 1.0, "s");

	CHECK(second.symbol() == "s");
}