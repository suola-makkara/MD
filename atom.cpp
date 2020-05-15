#include "atom.hpp"

Atom::Atom(const Vec3& position, const Vec3& velocity, bool fixed, unsigned int id)
	: position(position), velocity(velocity), fixed(fixed), id(id) { }