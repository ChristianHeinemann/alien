#include <QGraphicsScene>

#include <Base/DebugMacros.h>

#include "EngineInterface/ChangeDescriptions.h"
#include "Gui/Settings.h"
#include "Gui/DataRepository.h"
#include "Gui/ViewportInterface.h"

#include "ItemManager.h"
#include "CellItem.h"
#include "ParticleItem.h"
#include "CellConnectionItem.h"
#include "CoordinateSystem.h"
#include "MarkerItem.h"

void ItemManager::init(QGraphicsScene * scene, ViewportInterface* viewport, SimulationParameters const& parameters)
{
	auto config = new ItemConfig();

	_scene = scene;
	_viewport = viewport;
	_parameters = parameters;
	SET_CHILD(_config, config);

	_config->init(parameters);
}

void ItemManager::activate(IntVector2D size)
{
    TRY;
    _scene->clear();
	_scene->setSceneRect(0, 0, CoordinateSystem::modelToScene(size.x), CoordinateSystem::modelToScene(size.y));
	_cellsByIds.clear();
	_particlesByIds.clear();
	_connectionsByIds.clear();
    CATCH;
}

void ItemManager::updateCells(DataRepository* dataController)
{
    TRY;
	auto const &data = dataController->getDataRef();

	map<uint64_t, CellItem*> newCellsByIds;
	if (data.clusters) {
		for (auto const &cluster : *data.clusters) {
			for (auto const &cell : *cluster.cells) {
				auto it = _cellsByIds.find(cell.id);
				CellItem* item;
				if (it != _cellsByIds.end()) {
					item = it->second;
					item->update(cell);
					newCellsByIds.insert_or_assign(cell.id, item);
					_cellsByIds.erase(it);
				}
				else {
					item = new CellItem(_config, cell);
					_scene->addItem(item);
					newCellsByIds[cell.id] = item;
				}
				if (dataController->isInSelection(cell.id)) {
					item->setFocusState(CellItem::FOCUS_CELL);
				}
				else if (dataController->isInExtendedSelection(cell.id)) {
					item->setFocusState(CellItem::FOCUS_CLUSTER);
				}
				else {
					item->setFocusState(CellItem::NO_FOCUS);
				}
			}
		}
	}
	for (auto const& cellById : _cellsByIds) {
		delete cellById.second;
	}
	_cellsByIds = newCellsByIds;
    CATCH;
}

void ItemManager::updateParticles(DataRepository* manipulator)
{
    TRY;
    auto const& data = manipulator->getDataRef();

	map<uint64_t, ParticleItem*> newParticlesByIds;
	if (data.particles) {
		for (auto const &particle : *data.particles) {
			auto it = _particlesByIds.find(particle.id);
			ParticleItem* item;
			if (it != _particlesByIds.end()) {
				item = it->second;
				item->update(particle);
				newParticlesByIds.insert_or_assign(particle.id, item);
				_particlesByIds.erase(it);
			}
			else {
				item = new ParticleItem(_config, particle);
				_scene->addItem(item);
				newParticlesByIds.insert_or_assign(particle.id, item);
			}
			if (manipulator->isInSelection(particle.id)) {
				item->setFocusState(ParticleItem::FOCUS);
			}
			else {
				item->setFocusState(ParticleItem::NO_FOCUS);
			}
		}
	}
	for (auto const& particleById : _particlesByIds) {
		delete particleById.second;
	}
	_particlesByIds = newParticlesByIds;
    CATCH;
}

void ItemManager::updateConnections(DataRepository* repository)
{
    TRY;
    auto const& data = repository->getDataRef();
	if (!data.clusters) {
		return;
	}

	map<set<uint64_t>, CellConnectionItem*> newConnectionsByIds;
	for (auto const &cluster : *data.clusters) {
		for (auto const &cell : *cluster.cells) {
			if (!cell.connections) {
				continue;
			}
            for (auto const& connection : *cell.connections) {
                auto connectingCellId = connection.cellId;
				if (!repository->isCellPresent(connectingCellId)) {
					continue;
				}
				auto &connectingCellD = repository->getCellDescRef(connectingCellId);
				set<uint64_t> connectionId;
				connectionId.insert(cell.id);
				connectionId.insert(connectingCellId);
				if (newConnectionsByIds.find(connectionId) != newConnectionsByIds.end()) {
					continue;
				}
				//update may lead to exception in rare cases (Qt bug?)
/*
				auto connectionIt = _connectionsByIds.find(connectionId);
				if (connectionIt != _connectionsByIds.end()) {
					CellConnectionItem* connection = connectionIt->second;
					connection->update(cell, connectingCellD);
					newConnectionsByIds[connectionId] = connection;
					_connectionsByIds.erase(connectionIt);
				}
				else {
*/
					CellConnectionItem* newConnection = new CellConnectionItem(_config, cell, connectingCellD);
					_scene->addItem(newConnection);
					newConnectionsByIds[connectionId] = newConnection;
/*
				}
*/
			}
		}
	}
	for (auto const& connectionById : _connectionsByIds) {
		delete connectionById.second;
	}
	_connectionsByIds = newConnectionsByIds;
    CATCH;
}

void ItemManager::update(DataRepository* repository)
{
    TRY;
    updateCells(repository);
	updateConnections(repository);
	updateParticles(repository);
    CATCH;
}

void ItemManager::setMarkerItem(QPointF const &upperLeft, QPointF const &lowerRight)
{
    TRY;
    if (_marker) {
		_marker->update(upperLeft, lowerRight);
	}
	else {
		_marker = new MarkerItem(upperLeft, lowerRight);
		_scene->addItem(_marker);
	}
	_scene->update();
    CATCH;
}

void ItemManager::setMarkerLowerRight(QPointF const & lowerRight)
{
    TRY;
    if (_marker) {
		_marker->setLowerRight(lowerRight);
		_scene->update();
	}
    CATCH;
}

void ItemManager::deleteMarker()
{
    TRY;
    if (_marker) {
		delete _marker;
		_marker = nullptr;
		_scene->update();
	}
    CATCH;
}

bool ItemManager::isMarkerActive() const
{
    TRY;
    return (bool)_marker;
    CATCH;
}

QList<QGraphicsItem*> ItemManager::getItemsWithinMarker() const
{
    TRY;
    return _marker->collidingItems();
    CATCH;
}

void ItemManager::toggleCellInfo(bool showInfo)
{
    TRY;
    _config->setShowCellInfo(showInfo);
    CATCH;
}
