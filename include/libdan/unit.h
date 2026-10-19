#pragma once

#include <memory>
#include <string>

namespace libdan {
	namespace internal {
		class Dimension;
	}
	using internal::Dimension;

	class Unit {
	public:
		Unit(Dimension dimension, double scale, std::string symbol);

		[[nodiscard]] const Dimension& dimension() const noexcept;
		[[nodiscard]] double scale() const noexcept;
		[[nodiscard]] const std::string& symbol() const noexcept;

	private:
		std::shared_ptr<const Dimension> dimension_;
		double scale_;
		std::string symbol_;
	};
}