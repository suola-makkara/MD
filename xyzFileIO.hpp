#ifndef XYZ_FILE_IO_HPP
#define XYZ_FILE_IO_HPP

#include "atom.hpp"

#include <vector>
#include <string>

class XYZ_FileIO
{
public:
	static void write(const std::string& file, const std::vector<Atom>& atoms);

	static void append(const std::string& file, const std::vector<Atom>& atoms);
};

#endif // XYZ_FILE_IO_HPP