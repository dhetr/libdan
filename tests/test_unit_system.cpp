#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <unit_system.h>

using libdan::internal::UnitSystem;

TEST_CASE("UnitSystem stores its name", "[unit-system]") {
	const UnitSystem system("SI");

	CHECK(system.name() == "SI");
}

TEST_CASE("UnitSystem starts with no base dimensions", "[unit-system]") {
	const UnitSystem system("SI");

	CHECK(system.base_dimensions().empty());
}

TEST_CASE("UnitSystem registers base dimensions", "[unit-system]") {
	UnitSystem system("SI");

	const auto length = system.add_base_dimension("Length");
	const auto time = system.add_base_dimension("Time");
	const auto mass = system.add_base_dimension("Mass");

	REQUIRE(system.base_dimensions().size() == 3);

	CHECK(length == 0);
	CHECK(time == 1);
	CHECK(mass == 2);

	CHECK(system.base_dimensions()[length].name == "Length");
	CHECK(system.base_dimensions()[time].name == "Time");
	CHECK(system.base_dimensions()[mass].name == "Mass");
}

TEST_CASE("UnitSystem rejects an empty name", "[unit-system]") {
	CHECK_THROWS_AS(UnitSystem(""), std::invalid_argument);
}

TEST_CASE("UnitSystem rejects an empty base dimension name", "[unit-system]") {
	UnitSystem system("SI");

	CHECK_THROWS_AS(
		system.add_base_dimension(""),
		std::invalid_argument
	);

	CHECK(system.base_dimensions().empty());
}