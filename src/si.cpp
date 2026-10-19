#include <libdan/si.h>
#include <dimension.h>
#include <libdan/unit_system.h>

namespace libdan::si {
	UnitSystem system{"SI"};

	const auto time_id = system.add_base_dimension("Time");
	const auto length_id = system.add_base_dimension("Length");
	const auto mass_id = system.add_base_dimension("Mass");
	const auto current_id = system.add_base_dimension("Electric current");
	const auto temperature_id =
		system.add_base_dimension("Thermodynamic temperature");
	const auto amount_id = system.add_base_dimension("Amount of substance");
	const auto luminous_intensity_id =
		system.add_base_dimension("Luminous intensity");

	const Dimension time = Dimension::base(system, time_id);
	const Dimension length = Dimension::base(system, length_id);
	const Dimension mass = Dimension::base(system, mass_id);
	const Dimension current = Dimension::base(system, current_id);
	const Dimension temperature = Dimension::base(system, temperature_id);
	const Dimension amount = Dimension::base(system, amount_id);
	const Dimension luminous_intensity =
		Dimension::base(system, luminous_intensity_id);

	const Unit second{time, 1.0, "s"};
	const Unit metre{length, 1.0, "m"};
	const Unit kilogram{mass, 1.0, "kg"};
	const Unit ampere{current, 1.0, "A"};
	const Unit kelvin{temperature, 1.0, "K"};
	const Unit mole{amount, 1.0, "mol"};
	const Unit candela{luminous_intensity, 1.0, "cd"};
}