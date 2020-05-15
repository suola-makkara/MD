#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "atom.hpp"
#include "lattice.hpp"

#include <vector>
#include <list>

class Simulation
{
public:
	Simulation();

	void run(int time = 1000, double temp = 1.0);

	void addAtoms(const std::vector<Atom>& atoms);

	void setPeriodicity(unsigned int periodicity);

	void setVolume(const Vec3 volume);

	void setPotentialRange(double range);

	void setTimeStep(double step);

	void setRecordStep(int step);

	void setRecord(unsigned int state);

	void setRecordFileName(const std::string file);

	void addMeasureId(unsigned int id);

	enum Periodicity
	{
		NONE = 0x0, X = 0x1, Y = 0x2, Z = 0x4
	};

	enum RecordState
	{
		XYZ = 0x1, DATA = 0x2
	};

private:
	void init(double temp);

	void step();

	void checkBounds();

	void subdivide();

	void calculateForces();

	void updatePositions1();
	void updatePositions2();

	void collectData();

	void record();

	// Time
	int currentTime = 0;
	int simulationTime = 0;

	double timeStep = 0.005;

	// System
	std::vector<Atom> atoms;

	double potentialRange = 2.5;
	double potentialParameter1 = 0;
	double potentialParameter2 = 0;

	Vec3 volume = Vec3(1.0, 1.0, 1.0);
	Vec3 subVolume = Vec3(1.0, 1.0, 1.0);
	int subX = 0, subY = 0, subZ = 0;

	unsigned int periodicity = NONE;

	typedef std::vector<std::vector<std::vector<std::list<Atom*>>>> Subdivision;
	Subdivision subdivision;

	static const int subdivisionTable[14][3];

	// States
	double potentialEnergy = 0;
	double kineticEnergy = 0;

	// Recording
	unsigned int recordState = NONE;
	int recordStep = 10;
	std::string dataFile = "simulationOutput";
	
	struct MeasureData
	{
		unsigned int id;
		int count;
		Vec3 force;
		Vec3 velocity;
	};

	std::vector<MeasureData> measureData;
};

#endif // SIMULATION_HPP