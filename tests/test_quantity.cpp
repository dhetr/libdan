#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <quantity.h>

using libdan::internal::Dimension;
using libdan::internal::Unit;
using libdan::internal::Quantity;

TEST_CASE("Quantity converts compatible units", "[quantity]") {
	const auto length = Dimension::base(0);
	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const Quantity distance(5.0, kilometre);
	const auto converted = distance.to(metre);

	CHECK(converted.value() == Catch::Approx(5000.0));
	CHECK(converted.unit().symbol() == "m");
}

TEST_CASE("Quantity rejects incompatible dimensions", "[quantity]") {
	const Unit metre(Dimension::base(0), 1.0, "m");
	const Unit second(Dimension::base(1), 1.0, "s");

	const Quantity distance(5.0, metre);

	CHECK_THROWS_AS(distance.to(second), std::invalid_argument);
}

TEST_CASE("Quantity converts in both directions", "[quantity]") {
	const auto length = Dimension::base(0);
	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	CHECK(Quantity(2500.0, metre).to(kilometre).value()
		== Catch::Approx(2.5));

	CHECK(Quantity(2.5, kilometre).to(metre).value()
		== Catch::Approx(2500.0));
}

TEST_CASE("Quantity conversion preserves the original", "[quantity]") {
	const auto length = Dimension::base(0);
	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const Quantity original(5.0, kilometre);
	const auto converted = original.to(metre);

	CHECK(original.value() == Catch::Approx(5.0));
	CHECK(original.unit().symbol() == "km");
	CHECK(converted.value() == Catch::Approx(5000.0));
}

TEST_CASE("Quantity converts zero and negative values", "[quantity]") {
	const auto length = Dimension::base(0);
	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	CHECK(Quantity(0.0, kilometre).to(metre).value()
		== Catch::Approx(0.0));

	CHECK(Quantity(-2.0, kilometre).to(metre).value()
		== Catch::Approx(-2000.0));
}

TEST_CASE("Quantity supports conversion to the same unit", "[quantity]") {
	const Unit metre(Dimension::base(0), 1.0, "m");
	const Quantity original(42.0, metre);

	const auto converted = original.to(metre);

	CHECK(converted.value() == Catch::Approx(42.0));
	CHECK(converted.unit().symbol() == "m");
}