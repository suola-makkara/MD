cmake_minimum_required (VERSION 3.8)

set (SRC
	"main.cpp"
	"atom.cpp"
	"simulation.cpp"
	"lattice.cpp"
	"xyzFileIO.cpp"
)

add_executable (MolecularDynamics ${SRC})
