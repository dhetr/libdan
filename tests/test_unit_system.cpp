#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <libdan/types.h>
#include <libdan/unit_system.h>
#include <dimension.h>

using libdan::internal::Dimension;
using libdan::Unit;
using libdan::UnitSystem;
using namespace libdan::types;

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

TEST_CASE("UnitSystem registers and retrieves units", "[unit-system]") {
	UnitSystem system("test");

	const auto length_id = system.add_base_dimension("length");
	const auto length = Dimension::base(system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const UnitId metre_id = system.add_unit(metre);
	const UnitId kilometre_id = system.add_unit(kilometre);

	REQUIRE(metre_id == 0);
	REQUIRE(kilometre_id == 1);

	const Unit& stored_metre = system.unit(metre_id);
	const Unit& stored_kilometre = system.unit(kilometre_id);

	REQUIRE(stored_metre.scale() == 1.0);
	REQUIRE(stored_metre.dimension() == length);

	REQUIRE(stored_kilometre.scale() == 1000.0);
	REQUIRE(stored_kilometre.dimension() == length);
}

TEST_CASE("UnitSystem rejects units from another system", "[unit-system]") {
	UnitSystem system_a("A");
	UnitSystem system_b("B");

	const auto length_a = system_a.add_base_dimension("length");
	const auto length_b = system_b.add_base_dimension("length");

	const Unit unit_from_b(Dimension::base(system_b, length_b), 1.0, "S");

	REQUIRE_THROWS_AS(
		system_a.add_unit(unit_from_b),
		std::invalid_argument
	);
}

TEST_CASE("UnitSystem rejects invalid unit IDs", "[unit-system]") {
	UnitSystem system("test");

	REQUIRE_THROWS_AS(
		system.unit(UnitId{0}),
		std::out_of_range
	);
}


TEST_CASE("UnitSystem unit IDs remain valid as more units are added", "[unit-system]") {
	UnitSystem system("test");

	const auto length_id = system.add_base_dimension("length");
	const auto time_id = system.add_base_dimension("time");

	const Unit metre(Dimension::base(system, length_id), 1.0, "m");
	const Unit second(Dimension::base(system, time_id), 1.0, "s");

	const UnitId metre_id = system.add_unit(metre);
	const UnitId second_id = system.add_unit(second);

	REQUIRE(system.unit(metre_id).scale() == 1.0);
	REQUIRE(system.unit(second_id).scale() == 1.0);

	REQUIRE(system.unit(metre_id).dimension() ==
			Dimension::base(system, length_id));
	REQUIRE(system.unit(second_id).dimension() ==
			Dimension::base(system, time_id));
}
