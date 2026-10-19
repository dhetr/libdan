#pragma once

#include <libdan/unit.h>
#include <libdan/types.h>

#include <string>
#include <vector>

namespace libdan {
	struct BaseDimension {
		types::BaseDimensionId id;
		std::string name;
	};

	class UnitSystem {
	public:
		explicit UnitSystem(std::string name);

		UnitSystem(const UnitSystem&) = delete;
		UnitSystem& operator=(const UnitSystem&) = delete;
		UnitSystem(UnitSystem&&) = delete;
		UnitSystem& operator=(UnitSystem&&) = delete;

		[[nodiscard]] const std::string& name() const noexcept;
		[[nodiscard]] const std::vector<BaseDimension>& base_dimensions() const noexcept;
		[[nodiscard]] const Unit& unit(types::UnitId id) const;

		types::BaseDimensionId add_base_dimension(std::string name);
		types::UnitId add_unit(Unit unit);
	private:
		std::string name_;
		std::vector<BaseDimension> base_dimensions_;
		std::vector<Unit> units_;
	};
}