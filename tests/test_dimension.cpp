#include <catch2/catch_test_macros.hpp>
#include <dimension.h>

using libdan::internal::Dimension;
using libdan::internal::DimensionTerm;

TEST_CASE("Dimension construction", "[dimension]") {
	Dimension dimensionless;
	Dimension length = Dimension::base(0);

	CHECK(dimensionless.is_dimensionless());
	CHECK_FALSE(length.is_dimensionless());

	REQUIRE(length.terms().size() == 1);
	CHECK(length.terms()[0] == (DimensionTerm{0, 1}));
}

TEST_CASE("Multiplying dimensions adds exponents", "[dimension]") {
	Dimension length = Dimension::base(0);
	Dimension time = Dimension::base(1);

	Dimension velocity = length / time;
	Dimension distance = velocity * time;

	CHECK(distance == length);
	CHECK(distance.terms().size() == 1);
	CHECK(distance.terms()[0] == (DimensionTerm{0, 1}));
}

TEST_CASE("Dividing a dimension by itself is dimensionless", "[dimension]") {
	Dimension length = Dimension::base(0);
	Dimension result = length / length;

	CHECK(result.is_dimensionless());
}

TEST_CASE("Dimension powers multiply exponents", "[dimension]") {
	Dimension length = Dimension::base(0);

	Dimension area = pow(length, 2);
	Dimension inverse_length = pow(length, -1);
	Dimension dimensionless = pow(length, 0);

	REQUIRE(area.terms().size() == 1);
	CHECK(area.terms()[0] == (DimensionTerm{0, 2}));

	REQUIRE(inverse_length.terms().size() == 1);
	CHECK(inverse_length.terms()[0] == (DimensionTerm{0, -1}));

	CHECK(dimensionless.is_dimensionless());
}

TEST_CASE("Dimension multiplication is canonical", "[dimension]") {
	Dimension length = Dimension::base(0);
	Dimension time = Dimension::base(1);

	Dimension result = (length * time) / length;

	CHECK(result == time);
	REQUIRE(result.terms().size() == 1);
	CHECK(result.terms()[0] == (DimensionTerm{1, 1}));
}