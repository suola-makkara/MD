#include "atom.hpp"

Atom::Atom(const Vec3& position, const Vec3& velocity, unsigned int constraints, unsigned int id)
	: position(position), velocity(velocity), constraints(constraints), id(id) { }