#ifndef LATTICE_HPP
#define LATTICE_HPP

#include "vector.hpp"
#include "atom.hpp"

#include <vector>

class Lattice
{
public:
	Lattice(const Vec3& volume, int x = 1, int y = 1, int z = 1, const Vec3& position = Vec3());

	void createSC(bool fixed = false);
	void createFCC(bool fixed = false, const Vec3& initialVelocity = Vec3(), unsigned int id = 0);

	void rotateXY(double angle);

	Vec3 getConstants();

	const std::vector<Atom>& getAtoms();

private:
	Vec3 volume;
	Vec3 position;
	Vec3 constants;
	int x, y, z;

	std::vector<Atom> atoms;

	//static Lattice cubicSC(double a, int x, int y, int z);
	//static Lattice cubicFCC(double a, int x, int y, int z);
	//static std::vector<Atom> cubicBCC(double a, int x, int y, int z);
	

	//std::vector<Atom> atoms;
	//Vec3 volume;
	//Vec3 position;
};

#endif // LATTICE_HPP