#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <libdan/unit_system.h>
#include <dimension.h>
#include <libdan/libdan.h>

using libdan::internal::Dimension;
using libdan::Quantity;
using libdan::Unit;
using libdan::UnitSystem;

TEST_CASE("Quantity converts compatible units", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");

	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const Quantity distance(5.0, kilometre);
	const auto converted = distance.to(metre);

	CHECK(converted.value() == Catch::Approx(5000.0));
	CHECK(converted.unit().symbol() == "m");
}

TEST_CASE("Quantity rejects incompatible dimensions", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");

	const Unit metre(Dimension::base(test_system, length_id), 1.0, "m");
	const Unit second(Dimension::base(test_system, time_id), 1.0, "s");

	const Quantity distance(5.0, metre);

	CHECK_THROWS_AS(distance.to(second), std::invalid_argument);
}


TEST_CASE("Quantity converts in both directions", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	CHECK(Quantity(2500.0, metre).to(kilometre).value()
		== Catch::Approx(2.5));

	CHECK(Quantity(2.5, kilometre).to(metre).value()
		== Catch::Approx(2500.0));
}


TEST_CASE("Quantity conversion preserves the original", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const Quantity original(5.0, kilometre);
	const auto converted = original.to(metre);

	CHECK(original.value() == Catch::Approx(5.0));
	CHECK(original.unit().symbol() == "km");
	CHECK(converted.value() == Catch::Approx(5000.0));
}

TEST_CASE("Quantity converts zero and negative values", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	CHECK(Quantity(0.0, kilometre).to(metre).value()
		== Catch::Approx(0.0));

	CHECK(Quantity(-2.0, kilometre).to(metre).value()
		== Catch::Approx(-2000.0));
}

TEST_CASE("Quantity supports conversion to the same unit", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const Unit metre(Dimension::base(test_system, length_id), 1.0, "m");
	const Quantity original(42.0, metre);

	const auto converted = original.to(metre);

	CHECK(converted.value() == Catch::Approx(42.0));
	CHECK(converted.unit().symbol() == "m");
}

TEST_CASE("Quantities can be added and subtracted", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const Quantity a(2.0, kilometre);
	const Quantity b(300.0, metre);

	const auto result1 = a + b;
	const auto result2 = a - b;

	CHECK(result1.value() == Catch::Approx(2.3));
	CHECK(result2.value() == Catch::Approx(1.7));
	CHECK(result1.unit().symbol() == "km");
	CHECK(result2.unit().symbol() == "km");
}

TEST_CASE("Quantity arithmetic does not modify its operands", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");

	const Quantity a(2.0, kilometre);
	const Quantity b(300.0, metre);

	const auto add = a + b;
	const auto sub = a - b;

	CHECK(add.value() == Catch::Approx(2.3));
	CHECK(sub.value() == Catch::Approx(1.7));

	CHECK(a.value() == Catch::Approx(2.0));
	CHECK(a.unit().symbol() == "km");
	CHECK(b.value() == Catch::Approx(300.0));
	CHECK(b.unit().symbol() == "m");
}

TEST_CASE("Multiplying quantities combines dimensions and scales", "[quantity]") {\
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);
	const auto time = Dimension::base(test_system, time_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");
	const Unit second(time, 1.0, "s");

	SECTION("Length times length") {
		const Quantity a(2.0, kilometre);
		const Quantity b(3.0, metre);

		const auto result = a * b;

		CHECK(result.value() == Catch::Approx(6.0));
		CHECK(result.unit().scale() == Catch::Approx(1000.0));
		CHECK(result.unit().dimension() == length * length);
		CHECK(result.unit().symbol() == "(km*m)");
	}

	SECTION("Length times time") {
		const Quantity distance(10.0, metre);
		const Quantity duration(2.0, second);

		const auto result = distance * duration;

		CHECK(result.value() == Catch::Approx(20.0));
		CHECK(result.unit().dimension() == length * time);
		CHECK(result.unit().scale() == Catch::Approx(1.0));
	}
}

TEST_CASE("Dividing quantities combines dimensions and scales", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);
	const auto time = Dimension::base(test_system, time_id);

	const Unit metre(length, 1.0, "m");
	const Unit kilometre(length, 1000.0, "km");
	const Unit second(time, 1.0, "s");

	SECTION("Length divided by time") {
		const Quantity distance(10.0, kilometre);
		const Quantity duration(2.0, second);

		const auto result = distance / duration;

		CHECK(result.value() == Catch::Approx(5.0));
		CHECK(result.unit().scale() == Catch::Approx(1000.0));
		CHECK(result.unit().dimension() == length / time);
		CHECK(result.unit().symbol() == "(km/s)");
	}

	SECTION("Length divided by length is dimensionless") {
		const Quantity a(6.0, kilometre);
		const Quantity b(3.0, metre);

		const auto result = a / b;

		CHECK(result.value() == Catch::Approx(2.0));
		CHECK(result.unit().scale() == Catch::Approx(1000.0));
		CHECK(result.unit().dimension().is_dimensionless());
	}
}

TEST_CASE("Dividing a quantity by zero throws", "[quantity]") {
	UnitSystem test_system("test");
	const auto length_id = test_system.add_base_dimension("length");
	const auto time_id = test_system.add_base_dimension("time");
	const auto length = Dimension::base(test_system, length_id);

	const Unit metre(length, 1.0, "m");

	const Quantity distance(5.0, metre);
	const Quantity zero(0.0, metre);

	CHECK_THROWS_AS(distance / zero, std::domain_error);
}