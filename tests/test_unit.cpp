#include <catch2/catch_test_macros.hpp>

#include <unit_system.h>
#include <unit.h>

using libdan::internal::Dimension;
using libdan::internal::Unit;
using libdan::internal::UnitSystem;

TEST_CASE("Unit stores its dimension, scale, and symbol", "[unit]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const Dimension length = Dimension::base(test_system, length_id);
	const Unit metre(length, 1.0, "m");

	CHECK(metre.dimension() == length);
	CHECK(metre.scale() == 1.0);
	CHECK(metre.symbol() == "m");
}

TEST_CASE("Unit accepts positive finite scales", "[unit]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const Dimension length = Dimension::base(test_system, length_id);

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
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const Dimension length = Dimension::base(test_system, length_id);

	SECTION("Zero") {
		CHECK_THROWS_AS(
			Unit(length, 0.0, "m"),
			std::invalid_argument
		);
	}

	SECTION("Negative") {
		CHECK_THROWS_AS(
			Unit(length, -1.0, "m"),
			std::invalid_argument
		);
	}

	SECTION("NaN") {
		CHECK_THROWS_AS(
			Unit(length, std::numeric_limits<double>::quiet_NaN(), "m"),
			std::invalid_argument
		);
	}

	SECTION("Positive infinity") {
		CHECK_THROWS_AS(
			Unit(length, std::numeric_limits<double>::infinity(), "m"),
			std::invalid_argument
		);
	}

	SECTION("Negative infinity") {
		CHECK_THROWS_AS(
			Unit(length, -std::numeric_limits<double>::infinity(), "m"),
			std::invalid_argument
		);
	}
}

TEST_CASE("Unit rejects an empty symbol", "[unit]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const Dimension length = Dimension::base(test_system, length_id);

	CHECK_THROWS_AS(
		Unit(length, 1.0, ""),
		std::invalid_argument
	);
}

TEST_CASE("Unit preserves its symbol", "[unit]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const Dimension time = Dimension::base(test_system, time_id);
	const Unit second(time, 1.0, "s");

	CHECK(second.symbol() == "s");
}