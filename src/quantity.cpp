#include <libdan/quantity.h>
#include <dimension.h>
#include <stdexcept>

namespace libdan {
	Quantity::Quantity(double value, Unit unit) :
		value_(value),
		unit_(std::move(unit)) {}

	double Quantity::value() const noexcept {
		return value_;
	}

	const Unit& Quantity::unit() const noexcept {
		return unit_;
	}

	Quantity Quantity::to(const Unit& target) const {
		if (unit_.dimension() != target.dimension()) {
			throw std::invalid_argument(
				"Cannot convert between incompatible dimensions"
			);
		}

		const double converted_value =
			value_ * unit_.scale() / target.scale();
		
		return Quantity(converted_value, target);
	}

	Quantity operator+(const Quantity& a, const Quantity& b) {
		const Quantity converted_b = b.to(a.unit_);

		return Quantity(
			a.value_ + converted_b.value_,
			a.unit_
		);
	}

	Quantity operator-(const Quantity& a, const Quantity& b) {
		const Quantity converted_b = b.to(a.unit_);

		return Quantity(
			a.value_ - converted_b.value_,
			a.unit_
		);
	}

	Quantity operator*(const Quantity& a, const Quantity& b) {
		const Dimension dimension = a.unit_.dimension() * b.unit_.dimension();
		const double scale = a.unit_.scale() * b.unit_.scale();

		const std::string symbol = "(" + a.unit_.symbol() + "*" + b.unit_.symbol() + ")";

		const Unit result_unit(dimension, scale, symbol);
		return Quantity(a.value_ * b.value_, result_unit);
	}

	Quantity operator/(const Quantity& a, const Quantity& b) {
		if (b.value_ == 0.0)
			throw std::domain_error("Cannot divide a quantity by zero");

		const Dimension dimension = a.unit_.dimension() / b.unit_.dimension();
		const double scale = a.unit_.scale() / b.unit_.scale();

		const std::string symbol = "(" + a.unit_.symbol() + "/" + b.unit_.symbol() + ")";

		const Unit result_unit(dimension, scale, symbol);
		return Quantity(a.value_ / b.value_, result_unit);
	}
}