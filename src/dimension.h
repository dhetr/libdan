#pragma once

#include <unit_system.h>

#include <cstdint>
#include <vector>

namespace libdan::internal {
	using Exponent = std::int32_t;

	struct DimensionTerm {
		BaseDimensionId base;
		Exponent exponent;

		constexpr bool operator==(const DimensionTerm&) const = default;
	};

	class Dimension {
	public:
		explicit Dimension(const UnitSystem& system);

		static Dimension base(const UnitSystem& system, BaseDimensionId id);

		[[nodiscard]] const UnitSystem& system() const noexcept;
		[[nodiscard]] bool is_dimensionless() const noexcept;
		[[nodiscard]] const std::vector<DimensionTerm>& terms() const noexcept;
		
		bool operator==(const Dimension&) const;

		friend Dimension operator*(const Dimension&, const Dimension&);
		friend Dimension operator/(const Dimension&, const Dimension&);
		friend Dimension pow(const Dimension&, Exponent);
		
	private:
		const UnitSystem* system_;
		std::vector<DimensionTerm> terms_;

		explicit Dimension(const UnitSystem& system, std::vector<DimensionTerm> terms);

		static Exponent checked_add(Exponent a, Exponent b);
		static Exponent checked_multiply(Exponent a, Exponent b);

		static Dimension combine(
			const Dimension& a,
			const Dimension& b,
			Exponent sign
		);
	};
}