#include <libdan/libdan.h>
#include <libdan/si.h>

#include <iostream>

int main() {
	const libdan::Quantity distance(5.0, libdan::si::metre);
	const libdan::Quantity duration(2.0, libdan::si::second);

	const auto speed = distance / duration;

	std::cout << distance.value() << ' ' << distance.unit().symbol() << '\n';

	std::cout << speed.value() << ' ' << speed.unit().symbol() << '\n';
}