#pragma once

#include <dimension.h>

#include <string>

namespace libdan::internal {
	class Unit {
	public:
		Unit(Dimension, double scale, std::string symbol);

		[[nodiscard]] const Dimension& dimension() const noexcept;
		[[nodiscard]] double scale() const noexcept;
		[[nodiscard]] const std::string& symbol() const noexcept;

	private:
		Dimension dimension_;
		double scale_;
		std::string symbol_;
	};
}