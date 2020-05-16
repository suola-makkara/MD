#ifndef LATTICE_HPP
#define LATTICE_HPP

#include "vector.hpp"
#include "atom.hpp"

#include <vector>

class Lattice
{
public:
	Lattice(const Vec3& volume, int x = 1, int y = 1, int z = 1, const Vec3& position = Vec3());

	void createSC(const Vec3& initialVelocity = Vec3(), unsigned int constraints = Atom::NONE, unsigned int id = 0);
	void createFCC(const Vec3& initialVelocity = Vec3(), unsigned int constraints = Atom::NONE, unsigned int id = 0);

	void rotateXY(double angle);

	Vec3 getConstants();

	const std::vector<Atom>& getAtoms();

private:
	Vec3 volume;
	Vec3 position;
	Vec3 constants;
	int x, y, z;

	std::vector<Atom> atoms;
};

#endif // LATTICE_HPP