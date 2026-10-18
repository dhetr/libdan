#include <dimension.h>

#include <stdexcept>

namespace libdan::internal {
	Dimension::Dimension(std::vector<DimensionTerm> terms) : terms_(std::move(terms)) {}

	Dimension Dimension::base(BaseDimensionId id) {
		return Dimension({DimensionTerm{id, 1}});
	}

	bool Dimension::is_dimensionless() const noexcept {
		return terms_.empty();
	}

	const std::vector<DimensionTerm>& Dimension::terms() const noexcept {
		return terms_;
	}

	Dimension operator*(const Dimension& a, const Dimension& b) {
		return Dimension::combine(a, b, 1);
	}

	Dimension operator/(const Dimension& a, const Dimension& b) {
		return Dimension::combine(a, b, -1);
	}

	Dimension pow(const Dimension& d, Exponent exponent) {
		std::vector<DimensionTerm> result;
		result.reserve(d.terms_.size());

		for (const auto& term : d.terms_) {
			const auto new_exponent = Dimension::checked_multiply(term.exponent, exponent);

			if (new_exponent != 0) {
				result.push_back({term.base, new_exponent});
			}
		}

		return Dimension(std::move(result));
	}

	Dimension Dimension::combine(const Dimension& a, const Dimension& b, Exponent sign) {
		std::vector<DimensionTerm> result;
		result.reserve(a.terms_.size() + b.terms_.size());

		auto i = a.terms_.begin();
		auto j = b.terms_.begin();

		while (i != a.terms_.end() || j != b.terms_.end()) {
			if (j == b.terms_.end() ||
				(i != a.terms_.end() && i->base < j->base)) {
				result.push_back(*i++);
			} else if (i == a.terms_.end() || j->base < i->base) {
				const auto exponent = sign == 1
					? j->exponent
					: checked_multiply(j->exponent, -1);

				result.push_back({j->base, exponent});
				++j;
			} else {
				const auto exponent = checked_add(
					i->exponent,
					sign == 1
						? j->exponent
						: checked_multiply(j->exponent, -1)
				);

				if (exponent != 0) {
					result.push_back({i->base, exponent});
				}

				++i;
				++j;
			}
		}

		return Dimension(std::move(result));
	}

	Exponent Dimension::checked_add(Exponent a, Exponent b) {
		const auto result = static_cast<std::int64_t>(a) + b;

		if (result < std::numeric_limits<Exponent>::min() ||
			result > std::numeric_limits<Exponent>::max()) {
			throw std::overflow_error("Dimension exponent overflow");
		}

		return static_cast<Exponent>(result);
	}

	Exponent Dimension::checked_multiply(Exponent a, Exponent b) {
		const auto result = static_cast<std::int64_t>(a) * b;

		if (result < std::numeric_limits<Exponent>::min() ||
			result > std::numeric_limits<Exponent>::max()) {
			throw std::overflow_error("Dimension exponent overflow");
		}

		return static_cast<Exponent>(result);
	}
}