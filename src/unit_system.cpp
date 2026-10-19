#include <dimension.h>
#include <unit_system.h>

#include <stdexcept>

namespace libdan::internal {
	UnitSystem::UnitSystem(std::string name) : name_(std::move(name)) {
		if (name_.empty())
			throw std::invalid_argument(
				"Unit system name cannot be empty"
			);
	}

	const std::string& UnitSystem::name() const noexcept {
		return name_;
	}

	const std::vector<BaseDimension>& UnitSystem::base_dimensions() const noexcept {
		return base_dimensions_;
	}

	BaseDimensionId UnitSystem::add_base_dimension(std::string name) {
		if (name.empty())
			throw std::invalid_argument("Base dimension name cannot be empty");

		const auto id = base_dimensions_.size();
		base_dimensions_.push_back({
			id,
			std::move(name)
		});

		return id;
	}

	UnitId UnitSystem::add_unit(Unit unit) {
		if (&unit.dimension().system() != this) {
			throw std::invalid_argument(
				"Unit dimension belongs to a different unit system"
			);
		}

		const UnitId id = units_.size();
		units_.push_back(std::move(unit));
		return id;
	}

	const Unit& UnitSystem::unit(UnitId id) const {
		if (id >= units_.size()) {
			throw std::out_of_range("Invalid unit ID");
		}

		return units_[id];
	}
}