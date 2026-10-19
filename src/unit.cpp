#include <stdexcept>
#include <dimension.h>
#include <libdan/unit.h>

#include <cmath>

namespace libdan {
	Unit::Unit(Dimension dimension, double scale, std::string symbol)
        : dimension_(
              std::make_shared<const Dimension>(std::move(dimension))),
          scale_(scale),
          symbol_(std::move(symbol))
	{
		if (!std::isfinite(scale_) || scale_ <= 0.0) {
			throw std::invalid_argument("Unit scale must be finite and positive");
		}

		if (symbol_.empty()) {
			throw std::invalid_argument("Unit symbol cannot be empty");
		}
	}

	const Dimension& Unit::dimension() const noexcept {
		return *dimension_;
	}

	double Unit::scale() const noexcept {
		return scale_;
	}

	const std::string& Unit::symbol() const noexcept {
		return symbol_;
	}
}