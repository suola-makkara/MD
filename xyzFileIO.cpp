#include "xyzFileIO.hpp"

#include <fstream>

void XYZ_FileIO::XYZ_FileIO::write(const std::string& file, const std::vector<Atom>& atoms)
{
	std::ofstream stream(file);

	stream << atoms.size() << "\n\n";

	for (const Atom& atom : atoms) stream << "C " << atom.position << "\n";

	stream.close();
}

void XYZ_FileIO::XYZ_FileIO::append(const std::string& file, const std::vector<Atom>& atoms)
{
	std::ofstream stream(file, std::ofstream::app);

	stream << atoms.size() << "\n\n";

	for (const Atom& atom : atoms) stream << "C " << atom.position << "\n";

	stream.close();
}