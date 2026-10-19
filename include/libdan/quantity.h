#pragma once

#include <libdan/unit.h>

namespace libdan {
	class Quantity {
	public:
		Quantity(double value, Unit unit);

		[[nodiscard]] double value() const noexcept;
		[[nodiscard]] const Unit& unit() const noexcept;

		[[nodiscard]] Quantity to(const Unit& target) const;

		friend Quantity operator+(const Quantity&, const Quantity&);
		friend Quantity operator-(const Quantity&, const Quantity&);

		friend Quantity operator*(const Quantity&, const Quantity&);
		friend Quantity operator/(const Quantity&, const Quantity&);
	
	private:
		double value_;
		Unit unit_;
	};
}