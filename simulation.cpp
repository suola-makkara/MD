#include "simulation.hpp"
#include "xyzFileIO.hpp"

#include <fstream>

Simulation::Simulation() { }

void Simulation::run(int time, double temp)
{
	currentTime = 0;
	simulationTime = time;

	init(temp);
	record(); // Record initial state

	while (currentTime < simulationTime)
	{
		currentTime++;
		step();
	}
}

void Simulation::addAtoms(const std::vector<Atom>& atoms)
{
	this->atoms.insert(this->atoms.end(), atoms.begin(), atoms.end());
}

void Simulation::setPeriodicity(unsigned int periodicity)
{
	this->periodicity = periodicity;
}

void Simulation::setVolume(const Vec3 volume)
{
	this->volume = volume;
}

void Simulation::setPotentialRange(double range)
{
	potentialRange = range;
}

void Simulation::setTimeStep(double step)
{
	timeStep = step;
}

void Simulation::setRecordStep(int step)
{
	recordStep = step;
}

void Simulation::setRecord(unsigned int state)
{
	recordState = state;
}

void Simulation::setRecordFileName(const std::string file)
{
	this->dataFile = file;
}

void Simulation::addMeasureId(unsigned int id)
{
	measureData.push_back({ id, 0, Vec3() });
}

void Simulation::init(double temp)
{
	// Calculate potential parameters
	potentialParameter1 = -4 * (pow(potentialRange, -12) - pow(potentialRange, -6));
	potentialParameter2 = 48 * (pow(potentialRange, -13) - 0.5 * pow(potentialRange, -7));

	// Set initial velocities
	double vAvg = std::sqrt(3. * temp * (1. - 1. / atoms.size()));
	Vec3 vTot;
	int dynamicCount = 0;
	for (Atom& atom : atoms)
	{
		if (atom.fixed) continue;

		Vec3 vel = Vec3::rand3D() * vAvg;
		atom.velocity += vel;
		vTot += vel;
		dynamicCount++;
	}

	// Adjust so that center of mass is stable
	vTot /= (double)dynamicCount;
	for (Atom& atom : atoms)
	{
		if (atom.fixed) continue;
		atom.velocity -= vTot;
	}

	// Calculate initial subdivision and forces
	subdivide();

	calculateForces();
}

void Simulation::step()
{
	std::cout << "Time: " << currentTime << '\n';
	
	updatePositions1(); // Updates position and half velocities

	subdivide();

	calculateForces();

	updatePositions2(); // Updates rest of velocities

	collectData();

	record();
}

void Simulation::checkBounds()
{
	for (Atom& atom : atoms)
	{
		for (int i = 0; i < 3; i++)
		{
			if (periodicity & (0x1 << i))
			{
				if (atom.position[i] >= volume[i])
					atom.position[i] -= volume[i];
				if (atom.position[i] < 0)
					atom.position[i] += volume[i];
			}
		}
	}
}

void Simulation::subdivide()
{
	checkBounds();

	subdivision.clear();

	subX = (int)(volume.x / potentialRange);
	subY = (int)(volume.y / potentialRange);
	subZ = (int)(volume.z / potentialRange);

	// Initialize subdivision volume
	for (int x = 0; x < subX; x++)
	{
		subdivision.push_back(std::vector<std::vector<std::list<Atom*>>>());
		for (int y = 0; y < subY; y++)
		{
			subdivision[x].push_back(std::vector<std::list<Atom*>>());
			for (int z = 0; z < subZ; z++)
			{
				subdivision[x][y].push_back(std::list<Atom*>());
			}
		}
	}

	subVolume = volume / Vec3(subX, subY, subZ);

	for (Atom& atom : atoms)
	{	
		Vec3 subPosition = atom.position / subVolume;
	
		subdivision[(int)subPosition.x][(int)subPosition.y][(int)subPosition.z].push_back(&atom);
	}
}

void Simulation::calculateForces()
{
	potentialEnergy = 0;

	for (Atom& atom : atoms) atom.acceleration = Vec3();

	for (int x = 0; x < subX; x++)
		for (int y = 0; y < subY; y++)
			for (int z = 0; z < subZ; z++)
			{
				std::list<Atom*>& cell = subdivision[x][y][z];
				int a1 = 0;
				for (Atom* atom : cell)
				{
					a1++;
					for (int i = 0; i < 14; i++)
					{
						int nx = x + subdivisionTable[i][0],
							ny = y + subdivisionTable[i][1],
							nz = z + subdivisionTable[i][2];

						nx = nx >= subX ? 0 : nx < 0 ? subX - 1 : nx;
						ny = ny >= subY ? 0 : ny < 0 ? subY - 1 : ny;
						nz = nz >= subZ ? 0 : nz < 0 ? subZ - 1 : nz;

						std::list<Atom*>& ncell = subdivision[nx][ny][nz];

						int a2 = 0;
						for (Atom* natom : ncell)
						{
							a2++;
							if (i == 0 && a2 <= a1) continue;

							const Vec3 r = atom->position.to(natom->position);

							for (int i = 0; i < 3; i++)
							{
								if ((periodicity >> i) & 0x1)
								{
									if (r[i] > 0.5 * volume[i])
										r[i] -= volume[i];
									if (r[i] < -0.5 * volume[i])
										r[i] += volume[i];
								}
							}

							const double r2 = r.norm2();
							const double r1 = sqrt(r2);
							if (r1 > potentialRange) continue;

							const double ir2 = 1.0 / r2;
							const double ir6 = ir2 * ir2 * ir2;

							const Vec3 force = (48 * ir2 * ir6 * (ir6 - 0.5) - potentialParameter2 / r1) * r;

							atom->acceleration -= force;
							natom->acceleration += force;

							potentialEnergy += 4 * ir6 * (ir6 - 1) + potentialParameter1 + potentialParameter2 * (r1 - potentialRange);
						}
					}
				}
			}

	potentialEnergy /= atoms.size();
}

void Simulation::updatePositions1()
{
	for (Atom& atom : atoms)
	{
		if (!atom.fixed) atom.velocity += 0.5 * timeStep * atom.acceleration;
		atom.position += timeStep * atom.velocity;
	}
}

void Simulation::updatePositions2()
{
	double v2 = 0;
	for (Atom& atom : atoms)
	{
		if (!atom.fixed) atom.velocity += 0.5 * timeStep * atom.acceleration;
		v2 += atom.velocity.norm2();
	}
	
	kineticEnergy = 0.5 * v2 / atoms.size();
}

void Simulation::collectData()
{
	for (int i = 0; i < measureData.size(); i++)
	{
		MeasureData& data = measureData[i];

		int& count = data.count;
		Vec3& force = data.force;
		Vec3& velocity = data.velocity;
		
		count = 0;
		force = Vec3();
		velocity = Vec3();

		for (Atom& atom : atoms)
		{
			if (atom.id != data.id) continue;

			count++;
			force += atom.acceleration;
			velocity += atom.velocity;
		}

		velocity /= count;
	}
}

void Simulation::record()
{
	if ((currentTime % recordStep) == 0)
	{
		if (recordState & XYZ)
		{
			std::string file = dataFile + std::string(".xyz");
			if (currentTime == 0) XYZ_FileIO::write(file, atoms);
			else XYZ_FileIO::append(file, atoms);
		}

		if (recordState & DATA)
		{
			std::ofstream dataStream;

			std::string file = dataFile + std::string(".txt");
			if (currentTime == 0) dataStream.open(file);
			else dataStream.open(file, std::ofstream::app);

			double totalEnergy = potentialEnergy + kineticEnergy;
			dataStream << currentTime * timeStep << " " << potentialEnergy << " " << kineticEnergy << " " << totalEnergy;

			for (MeasureData& data : measureData)
			{
				dataStream << " " << data.id << " " << data.count << " " << data.force << " " << data.velocity;
			}

			dataStream << "\n";

			dataStream.close();
		}
	}
}

const int Simulation::subdivisionTable[14][3] = {
		{ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
		{ 1, 1, 0 }, { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 1 },
		{ -1, 1, 1 }, { -1, 1, 0 }, { -1, 1, -1 }, { 1, 1, -1 },
		{ 0, 1, -1 }, { -1, 0, 1 } };


//
////void Simulation::run(int time)
////{
////	/*simulationTime = time;
////	for (currentTime = 0; currentTime < simulationTime; currentTime++)
////	{
////		std::cout << currentTime << '\n';
////		step();
////	}*/
////}
//
//void Simulation::step()
//{
//	/*potentialEnergy = 0.;
//	kineticEnergy = 0.;
//
//	updatePositions1();
//
//	checkBounds();
//
//	calculateForces();
//
//	updatePositions2();
//
//	record();*/
//}
//
//void Simulation::calculateForces()
//{
//	/*for (Atom& atom : lattice.atoms) atom.acceleration = Vec3();
//
//	for (int i = 0; i < lattice.atoms.size() - 1; i++)
//		for (int j = i + 1; j < lattice.atoms.size(); j++)
//		{
//			Atom& ai = lattice.atoms[i];
//			Atom& aj = lattice.atoms[j];
//
//			const Vec3 rij = ai.position.to(aj.position);
//
//			for (int i = 0; i < 3; i++)
//			{
//				if ((periodicity >> i) & 0x1)
//				{
//					if (rij[i] > 0.5 * lattice.volume[i])
//						rij[i] -= lattice.volume[i];
//					if (rij[i] < -0.5 * lattice.volume[i])
//						rij[i] += lattice.volume[i];
//				}
//			}
//
//			const double r2 = rij.norm2();
//			const double ir2 = 1. / r2;
//			const double ir6 = ir2 * ir2 * ir2;
//
//			const Vec3 force = 48 * ir2 * ir6 * (ir6 - 0.5) * rij;
//
//			ai.acceleration -= force;
//			aj.acceleration += force;
//
//			potentialEnergy += 4 * ir6 * (ir6 - 1);
//		}
//
//	potentialEnergy /= lattice.atoms.size();*/
//}
//
//void Simulation::checkBounds()
//{
//	/*for (Atom& atom : lattice.atoms)
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			if ((periodicity >> i) & 0x1)
//			{
//				if (atom.position[i] - lattice.position[i] > lattice.volume[i])
//					atom.position[i] -= lattice.volume[i];
//				if (atom.position[i] - lattice.position[i] < 0)
//					atom.position[i] += lattice.volume[i];
//			}
//		}
//	}*/
//}
//
//void Simulation::record()
//{
//	/*if ((currentTime % recordStep) == 0)
//	{
//		std::ofstream dataStream;
//
//		if (currentTime == 0)
//		{
//			dataStream.open("data.txt");
//			XYZ_FileIO::write(trajectoryFile, lattice.atoms);
//		}
//		else
//		{
//			dataStream.open("data.txt", std::ofstream::app);
//			XYZ_FileIO::append(trajectoryFile, lattice.atoms);
//		}
//
//		// Other data
//		double totalEnergy = potentialEnergy + kineticEnergy;
//		
//		dataStream << (currentTime + 1.) * timeStep << " " << potentialEnergy << " " << kineticEnergy << " " << totalEnergy << '\n';
//
//		dataStream.close();
//	}*/
//}