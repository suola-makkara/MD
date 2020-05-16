#ifndef ATOM_HPP
#define ATOM_HPP

#include "vector.hpp"

class Atom
{
public:
	Atom() = default;

	Atom(const Vec3& position, const Vec3& velocity = Vec3(), unsigned int constraints = NONE, unsigned int id = 0);

	enum Constraints
	{
		NONE, X = 0x1, Y = 0x2, Z = 0x4, ALL = X | Y | Z
	};

	Vec3 position, velocity, acceleration = Vec3();
	unsigned int constraints;
	unsigned int id;
};

#endif // ATOM_HPP