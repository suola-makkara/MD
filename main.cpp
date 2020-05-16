#include "vector.hpp"
#include "simulation.hpp"
#include "lattice.hpp"

#include <iostream>
#include <ctime>
#include <string>

std::vector<Atom> cutOff(const std::vector<Atom>& atoms, const Vec3& volume)
{
	std::vector<Atom> result;

	for (const Atom& atom : atoms)
	{
		bool inside = true;
		for (int i = 0; i < 3; i++)
		{
			if (atom.position[i] >= volume[i] || atom.position[i] < 0)
				inside = false;
		}
		if (inside) result.push_back(atom);
	}

	return result;
}

int main()
{
	srand(time(0));

	for (int a = 0; a <= 90; a++)
	{
		for (int i = 0; i < 3; i++)
		{
			Simulation sim;

			sim.setPeriodicity(Simulation::ALL);

			sim.setRecord(Simulation::DATA);
			//sim.setRecordStep(10);
			sim.setRecordFileName(std::string("angleData/angle") + std::to_string(a) + std::string("_") + std::to_string(i));

			//sim.setPotentialRange(2.5);
			//sim.setTimeStep(0.0025);

			double sp = 1.55;
			sim.setVolume(Vec3(20) * sp);

			Lattice baseFixed(Vec3(20, 20, 1) * sp, 20, 20, 1, Vec3(0, 0, 5) * sp);
			Lattice base(Vec3(20, 20, 1) * sp, 20, 20, 1, Vec3(0, 0, 6) * sp);
			baseFixed.createFCC(Vec3(), Atom::ALL);
			base.createFCC();

			Lattice topFixed(Vec3(30, 30, 1) * sp, 30, 30, 1, Vec3(-5, -5, 8.5) * sp);
			Lattice top(Vec3(30, 30, 1) * sp, 30, 30, 1, Vec3(-5, -5, 7.5) * sp);

			topFixed.createFCC(Vec3(2.0, 0, 0), Atom::Z | Atom::Y, 1u);
			top.createFCC(Vec3(2.0, 0, 0), Atom::NONE, 1u);

			topFixed.rotateXY(a * PI / 180);
			top.rotateXY(a * PI / 180);

			sim.addAtoms(baseFixed.getAtoms());
			sim.addAtoms(base.getAtoms());
			sim.addAtoms(cutOff(topFixed.getAtoms(), Vec3(19.5) * sp));
			sim.addAtoms(cutOff(top.getAtoms(), Vec3(19.5) * sp));

			sim.addMeasureId(1u);

			sim.run(2000, 0.25);
		}
	}

	return 0;
}