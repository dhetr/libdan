#include <quantity.h>
#include <stdexcept>

namespace libdan::internal {
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
}