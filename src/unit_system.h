#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace libdan::internal {
	using BaseDimensionId = std::size_t;

	struct BaseDimension {
		BaseDimensionId id;
		std::string name;
	};

	class UnitSystem {
	public:
		explicit UnitSystem(std::string name);

		[[nodiscard]] const std::string& name() const noexcept;
		[[nodiscard]] const std::vector<BaseDimension>& base_dimensions() const noexcept;

		BaseDimensionId add_base_dimension(std::string name);
	private:
		std::string name_;
		std::vector<BaseDimension> base_dimensions_;
	};
}