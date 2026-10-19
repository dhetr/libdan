#pragma once

#include <libdan/types.h>
#include <libdan/unit_system.h>

#include <vector>

namespace libdan::internal {	
	struct DimensionTerm {		
		types::BaseDimensionId base;
		types::Exponent exponent;

		constexpr bool operator==(const DimensionTerm&) const = default;
	};

	class Dimension {
	public:
		explicit Dimension(const UnitSystem& system);

		static Dimension base(const UnitSystem& system, types::BaseDimensionId id);

		[[nodiscard]] const UnitSystem& system() const noexcept;
		[[nodiscard]] bool is_dimensionless() const noexcept;
		[[nodiscard]] const std::vector<DimensionTerm>& terms() const noexcept;
		
		bool operator==(const Dimension&) const;

		friend Dimension operator*(const Dimension&, const Dimension&);
		friend Dimension operator/(const Dimension&, const Dimension&);
		friend Dimension pow(const Dimension&, types::Exponent);
		
	private:
		const UnitSystem* system_;
		std::vector<DimensionTerm> terms_;

		explicit Dimension(const UnitSystem& system, std::vector<DimensionTerm> terms);

		static types::Exponent checked_add(types::Exponent a, types::Exponent b);
		static types::Exponent checked_multiply(types::Exponent a, types::Exponent b);

		static Dimension combine(
			const Dimension& a,
			const Dimension& b,
			types::Exponent sign
		);
	};
}