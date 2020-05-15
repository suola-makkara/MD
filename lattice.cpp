#include "lattice.hpp"

Lattice::Lattice(const Vec3& volume, int x, int y, int z, const Vec3& position)
	: volume(volume), position(position), constants(volume / Vec3(x, y, z)), x(x), y(y), z(z) { }

void Lattice::createSC(bool fixed)
{
	atoms.reserve((size_t)x * y * z);
	for (int xx = 0; xx < x; xx++)
		for (int yy = 0; yy < y; yy++)
			for (int zz = 0; zz < z; zz++)
				atoms.push_back(Atom(Vec3(xx, yy, zz) * constants + position, Vec3(), fixed));
}

void Lattice::createFCC(bool fixed, const Vec3& initialVelocity, unsigned int id)
{
	atoms.reserve((size_t)4 * x * y * z);
	for (int xx = 0; xx < x; xx++)
		for (int yy = 0; yy < y; yy++)
			for (int zz = 0; zz < z; zz++)
			{
				atoms.push_back(Atom(Vec3(xx, yy, zz) * constants + position, initialVelocity, fixed, id));
				atoms.push_back(Atom(Vec3(xx + 0.5, yy + 0.5, zz) * constants + position, initialVelocity, fixed, id));
				atoms.push_back(Atom(Vec3(xx, yy + 0.5, zz + 0.5) * constants + position, initialVelocity, fixed, id));
				atoms.push_back(Atom(Vec3(xx + 0.5, yy, zz + 0.5) * constants + position, initialVelocity, fixed, id));
			}
}

void Lattice::rotateXY(double angle)
{
	double c = cos(angle), s = sin(angle);
	Vec3 center = volume / 2 + position;

	for (Atom& atom : atoms)
	{
		Vec3 r = atom.position - center;

		double x = r.x * c - r.y * s;
		double y = r.x * s + r.y * c;

		r.x = x;
		r.y = y;

		atom.position = r + center;
	}
}

Vec3 Lattice::getConstants()
{
	return constants;
}

const std::vector<Atom>& Lattice::getAtoms()
{
	return atoms;
}