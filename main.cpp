#include "vector.hpp"
#include "simulation.hpp"
#include "lattice.hpp"

#include <iostream>
#include <ctime>

int main()
{
	srand(time(0));

	Simulation sim;

	sim.setPeriodicity(Simulation::X | Simulation::Y | Simulation::Z);// | Simulation::Z);

	sim.setRecord(Simulation::DATA | Simulation::XYZ);
	sim.setRecordStep(10);
	sim.setRecordFileName("test");

	//sim.setPotentialRange(2.5);
	//sim.setTimeStep(0.0025);

	double sp = 1.55;
	sim.setVolume(Vec3(20 * sp, 20 * sp, 20 * sp));

	Lattice baseFixed(Vec3(20 * sp, 20 * sp, 1 * sp), 20, 20, 1, Vec3(0, 0, 5 * sp));
	baseFixed.createFCC(true);
	Lattice base(Vec3(20 * sp, 20 * sp, 1 * sp), 20, 20, 1, Vec3(0, 0, 6 * sp));
	base.createFCC();

	Lattice topFixed(Vec3(6 * sp, 6 * sp, 1 * sp), 6, 6, 1, Vec3(7 * sp, 7 * sp, 13.5 * sp));
	Lattice top(Vec3(6 * sp, 6 * sp, 5 * sp), 6, 6, 5, Vec3(7 * sp, 7 * sp, 8.5 * sp));
	topFixed.createFCC(true, Vec3(0.5,0,0));
	//topFixed.rotateXY(0);
	top.createFCC(false, Vec3(0.5, 0, 0), 1);
	//top.rotateXY(0);
	

	sim.addAtoms(baseFixed.getAtoms());
	sim.addAtoms(base.getAtoms());
	sim.addAtoms(topFixed.getAtoms());
	sim.addAtoms(top.getAtoms());

	//Lattice test(Vec3(10 * sp, 10 * sp, 4 * sp), 10, 10, 4, Vec3(5 * sp,5 * sp,5 * sp));
	//test.createFCC(false, Vec3(), 1u);

	sim.addMeasureId(1u);

	//sim.addAtoms(baseFixed.getAtoms());
	//sim.addAtoms(base.getAtoms());
	//sim.addAtoms(top.getAtoms());

	sim.run(20000, 0.25);

	return 0;
}