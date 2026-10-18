#pragma once

#include <unit.h>

namespace libdan::internal {
	class Quantity {
	public:
		Quantity(double value, Unit unit);

		[[nodiscard]] double value() const noexcept;
		[[nodiscard]] const Unit& unit() const noexcept;

		[[nodiscard]] Quantity to(const Unit& target) const;
	
	private:
		double value_;
		Unit unit_;
	};
}