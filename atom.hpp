#ifndef ATOM_HPP
#define ATOM_HPP

#include "vector.hpp"

class Atom
{
public:
	Atom() = default;

	Atom(const Vec3& position, const Vec3& velocity = Vec3(), bool fixed = false, unsigned int id = 0);

	Vec3 position, velocity, acceleration = Vec3();
	bool fixed;
	unsigned int id;
};

#endif // ATOM_HPP