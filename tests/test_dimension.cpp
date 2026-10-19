#include <catch2/catch_test_macros.hpp>
#include <libdan/unit_system.h>
#include <dimension.h>

using libdan::UnitSystem;
using libdan::internal::Dimension;
using libdan::internal::DimensionTerm;

TEST_CASE("Dimension construction", "[dimension]") {
	UnitSystem system("test");
	const auto length_id = system.add_base_dimension("length");

	Dimension dimensionless(system);
	Dimension length = Dimension::base(system, length_id);

	CHECK(dimensionless.is_dimensionless());
	CHECK_FALSE(length.is_dimensionless());

	REQUIRE(length.terms().size() == 1);
	CHECK(length.terms()[0] == (DimensionTerm{length_id, 1}));
}

TEST_CASE("Multiplying dimensions adds exponents", "[dimension]") {
	UnitSystem system("test");
	const auto length_id = system.add_base_dimension("length");
	const auto time_id = system.add_base_dimension("time");

	Dimension length = Dimension::base(system, length_id);
	Dimension time = Dimension::base(system, time_id);

	Dimension velocity = length / time;
	Dimension distance = velocity * time;

	CHECK(distance == length);
	REQUIRE(distance.terms().size() == 1);
	CHECK(distance.terms()[0] == (DimensionTerm{length_id, 1}));
}


TEST_CASE("Dividing a dimension by itself is dimensionless", "[dimension]") {
	UnitSystem system("test");
	const auto length_id = system.add_base_dimension("length");

	Dimension length = Dimension::base(system, length_id);
	Dimension result = length / length;

	CHECK(result.is_dimensionless());
}


TEST_CASE("Dimension powers multiply exponents", "[dimension]") {
	UnitSystem system("test");
	const auto length_id = system.add_base_dimension("length");

	Dimension length = Dimension::base(system, length_id);

	Dimension area = pow(length, 2);
	Dimension inverse_length = pow(length, -1);
	Dimension dimensionless = pow(length, 0);

	REQUIRE(area.terms().size() == 1);
	CHECK(area.terms()[0] == (DimensionTerm{length_id, 2}));

	REQUIRE(inverse_length.terms().size() == 1);
	CHECK(inverse_length.terms()[0] == (DimensionTerm{length_id, -1}));

	CHECK(dimensionless.is_dimensionless());
}

TEST_CASE("Dimension multiplication is canonical", "[dimension]") {
	UnitSystem system("test");
	const auto length_id = system.add_base_dimension("length");
	const auto time_id = system.add_base_dimension("time");

	Dimension length = Dimension::base(system, length_id);
	Dimension time = Dimension::base(system, time_id);

	Dimension result = (length * time) / length;

	CHECK(result == time);
	REQUIRE(result.terms().size() == 1);
	CHECK(result.terms()[0] == (DimensionTerm{time_id, 1}));
}

TEST_CASE("Dimensions from different systems cannot be combined", "[dimension]") {
	UnitSystem first_system("first");
	UnitSystem second_system("second");

	const auto first_length_id = first_system.add_base_dimension("length");
	const auto second_length_id = second_system.add_base_dimension("length");

	Dimension first_length = Dimension::base(first_system, first_length_id);
	Dimension second_length = Dimension::base(second_system, second_length_id);

	CHECK_FALSE(first_length == second_length);

	CHECK_THROWS_AS(first_length * second_length, std::invalid_argument);
	CHECK_THROWS_AS(first_length / second_length, std::invalid_argument);
}