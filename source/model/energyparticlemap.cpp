#include "energyparticlemap.h"

#include "model/topology.h"
#include "model/entities/energyparticle.h"


EnergyParticleMap::EnergyParticleMap()
{
}

EnergyParticleMap::~EnergyParticleMap()
{
	deleteGrid();
}


void EnergyParticleMap::init(Topology* topo)
{
	_topo = topo;
	deleteGrid();
	IntVector2D size = _topo->getSize();
	_gridSize = size.x;
	_energyGrid = new EnergyParticle**[size.x];
	for (int x = 0; x < size.x; ++x) {
		_energyGrid[x] = new EnergyParticle*[size.y];
	}
	clear();
}

void EnergyParticleMap::clear()
{
	IntVector2D size = _topo->getSize();
	for (int x = 0; x < size.x; ++x)
		for (int y = 0; y < size.y; ++y)
			_energyGrid[x][y] = nullptr;
}

void EnergyParticleMap::removeParticleIfPresent(QVector3D pos, EnergyParticle * energy)
{
	IntVector2D intPos = _topo->correctPositionWithIntPrecision(pos);
	if (_energyGrid[intPos.x][intPos.y] == energy)
		_energyGrid[intPos.x][intPos.y] = nullptr;
}

void EnergyParticleMap::setParticle(QVector3D pos, EnergyParticle * energy)
{
	IntVector2D intPos = _topo->correctPositionWithIntPrecision(pos);
	_energyGrid[intPos.x][intPos.y] = energy;
}

EnergyParticle * EnergyParticleMap::getParticle(QVector3D pos) const
{
	IntVector2D intPos = _topo->correctPositionWithIntPrecision(pos);
	return _energyGrid[intPos.x][intPos.y];
}

void EnergyParticleMap::serializePrimitives(QDataStream & stream) const
{
	//determine number of energy particle entries
	quint32 numEntries = 0;
	IntVector2D size = _topo->getSize();
	for (int x = 0; x < size.x; ++x)
		for (int y = 0; y < size.y; ++y)
			if (_energyGrid[x][y])
				numEntries++;
	stream << numEntries;

	//write energy particle entries
	for (qint32 x = 0; x < size.x; ++x)
		for (qint32 y = 0; y < size.y; ++y) {
			EnergyParticle* e = _energyGrid[x][y];
			if (e) {
				stream << x << y << e->getId();
			}

		}
}

void EnergyParticleMap::deserializePrimitives (QDataStream & stream, QMap<quint64, EnergyParticle*> const & oldIdEnergyMap)
{
	quint32 numEntries = 0;
	qint32 x = 0;
	qint32 y = 0;
	quint64 oldId = 0;
	stream >> numEntries;
	for (quint32 i = 0; i < numEntries; ++i) {
		stream >> x >> y >> oldId;
		EnergyParticle* particle = oldIdEnergyMap[oldId];
		_energyGrid[x][y] = particle;
	}
}

void EnergyParticleMap::deleteGrid()
{
	if (_energyGrid) {
		for (int x = 0; x < _gridSize; ++x) {
			delete[] _energyGrid[x];
		}
		delete[] _energyGrid;
	}
}