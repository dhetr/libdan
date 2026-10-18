#pragma once

#include <cstdint>
#include <vector>

namespace libdan::internal {
	using BaseDimensionId = std::uint32_t;
	using Exponent = std::int32_t;

	struct DimensionTerm {
		BaseDimensionId base;
		Exponent exponent;

		constexpr bool operator==(const DimensionTerm&) const = default;
	};

	class Dimension {
	public:
		constexpr Dimension() = default;

		static Dimension base(BaseDimensionId id);

		[[nodiscard]] bool is_dimensionless() const noexcept;
		[[nodiscard]] const std::vector<DimensionTerm>& terms() const noexcept;
		
		bool operator==(const Dimension&) const = default;

		friend Dimension operator*(const Dimension&, const Dimension&);
		friend Dimension operator/(const Dimension&, const Dimension&);
		friend Dimension pow(const Dimension&, Exponent);
		
	private:
		std::vector<DimensionTerm> terms_;

		explicit Dimension(std::vector<DimensionTerm> terms);

		static Exponent checked_add(Exponent a, Exponent b);
		static Exponent checked_multiply(Exponent a, Exponent b);

		static Dimension combine(
			const Dimension& a,
			const Dimension& b,
			Exponent sign
		);
	};
}