﻿#pragma once
#include <QObject>

#include "Definitions.h"

class ActionHolder
	: public QObject
{
	Q_OBJECT

public:
	ActionHolder(QObject* parent = nullptr);
	virtual ~ActionHolder() = default;

	QAction* actionNewSimulation = nullptr;
    QAction* actionWebSimulation = nullptr;
    QAction* actionLoadSimulation = nullptr;
	QAction* actionSaveSimulation = nullptr;
	QAction* actionRunSimulation = nullptr;
	QAction* actionRunStepForward = nullptr;
	QAction* actionRunStepBackward = nullptr;
    QAction* actionSnapshot = nullptr;
	QAction* actionRestore = nullptr;
    QAction* actionAcceleration = nullptr;
    QAction* actionSimulationChanger = nullptr;
    QAction* actionExit = nullptr;

	QAction* actionComputationSettings = nullptr;
	QAction* actionEditSimParameters = nullptr;
	QAction* actionEditSymbols = nullptr;

	QActionGroup* actionGroupOpenGLItemView = nullptr;
    QAction* actionOpenGLView = nullptr;
	QAction* actionItemView = nullptr;

	QActionGroup* actionGroupNavigationAction = nullptr;
    QAction* actionNavigationMode = nullptr;
    QAction* actionActionMode = nullptr;

    QAction* actionMonitor = nullptr;
	QAction* actionZoomIn = nullptr;
	QAction* actionZoomOut = nullptr;
    QAction* actionDisplayLink = nullptr;
    QAction* actionGlowEffect = nullptr;
    QAction* actionMotionEffect = nullptr;
    QAction* actionShowCellInfo = nullptr;
	QAction* actionCenterSelection = nullptr;

	QAction* actionNewCell = nullptr;
	QAction* actionNewParticle = nullptr;
	QAction* actionCopyEntity = nullptr;
	QAction* actionPasteEntity = nullptr;
	QAction* actionDeleteEntity = nullptr;
	QAction* actionNewToken = nullptr;
	QAction* actionCopyToken = nullptr;
	QAction* actionPasteToken = nullptr;
	QAction* actionDeleteToken = nullptr;
    QAction* actionCopyToClipboard = nullptr;
    QAction* actionPasteFromClipboard = nullptr;

	QAction* actionNewRectangle = nullptr;
	QAction* actionNewHexagon = nullptr;
    QAction* actionNewParticles = nullptr;
    QAction* actionNewDisc = nullptr;
	QAction* actionLoadCol = nullptr;
	QAction* actionSaveCol = nullptr;
	QAction* actionCopyCol = nullptr;
	QAction* actionPasteCol = nullptr;
	QAction* actionDeleteCol = nullptr;
	QAction* actionDeleteSel = nullptr;
    QAction* actionColorizeSel = nullptr;
    QAction* actionGenerateBranchNumbers = nullptr;
    QAction* actionRandomizeCellFunctions = nullptr;
    QAction* actionRemoveFreeCellConnections = nullptr;
    QAction* actionRandomMultiplier = nullptr;
	QAction* actionGridMultiplier = nullptr;

    QAction* actionMostFrequentCluster = nullptr;
    QAction* actionWriteMonitorLog = nullptr;

	QAction* actionAbout = nullptr;
    QAction* actionGettingStarted = nullptr;
    QAction* actionDocumentation = nullptr;

	QAction* actionRestrictTPS = nullptr;
};
