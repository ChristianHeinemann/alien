#pragma once

#include <QWidget>

#include "Gui/Definitions.h"
#include "Gui/DataManipulator.h"

class DataEditorController
	: public QObject
{
	Q_OBJECT
public:
	DataEditorController(QWidget *parent = nullptr);
	virtual ~DataEditorController() = default;

	void init(IntVector2D const& upperLeftPosition, DataManipulator* manipulator, SimulationContext* context);

	DataEditorContext* getContext() const;
	SimulationParameters* getSimulationParameters() const;

	void notificationFromCellEditWidget();
	void notificationFromClusterEditWidget();
	void notificationFromParticleEditWidget();
	void notificationFromMetadataEditWidget();
	void notificationFromCellComputerEditWidget();

private:
	Q_SLOT void onShow(bool visible);
	Q_SLOT void notificationFromManipulator(set<DataManipulator::Receiver> const& targets);

	DataEditorModel* _model = nullptr;
	DataEditorView* _view = nullptr;
	DataManipulator* _manipulator = nullptr;
	DataEditorContext* _context = nullptr;
	SimulationParameters* _parameters = nullptr;
};