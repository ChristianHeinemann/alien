﻿#include <QAction>
#include <QActionGroup>

#include "Gui/Settings.h"

#include "ActionHolder.h"

ActionHolder::ActionHolder(QObject* parent) : QObject(parent)
{
	actionNewSimulation = new QAction("New", this);
	actionNewSimulation->setEnabled(true);
	actionNewSimulation->setShortcut(Qt::CTRL + Qt::Key_N);
    actionWebSimulation = new QAction("Web access", this);
    actionWebSimulation->setEnabled(true);
    actionWebSimulation->setCheckable(true);
    actionWebSimulation->setChecked(false);
    actionWebSimulation->setShortcut(Qt::CTRL + Qt::Key_W);
    actionLoadSimulation = new QAction("Load", this);
	actionLoadSimulation->setEnabled(true);
	actionLoadSimulation->setShortcut(Qt::CTRL + Qt::Key_L);
	actionSaveSimulation = new QAction("Save", this);
	actionSaveSimulation->setEnabled(true);
    actionSaveSimulation->setShortcut(Qt::CTRL + Qt::Key_S);
	actionRunSimulation = new QAction("Run", this);
	actionRunSimulation->setEnabled(true);
	actionRunSimulation->setCheckable(true);
    QIcon iconRunSimulation;
    iconRunSimulation.addFile(":/Icons/main/pause.png", QSize(), QIcon::Normal, QIcon::On);
    iconRunSimulation.addFile(":/Icons/main/run.png", QSize(), QIcon::Normal, QIcon::Off);
	actionRunSimulation->setIcon(iconRunSimulation);
	actionRunSimulation->setIconVisibleInMenu(false);
	actionRunStepForward = new QAction("Step forward", this);
	actionRunStepForward->setEnabled(true);
	QIcon iconStepForward;
	iconStepForward.addFile(":/Icons/main/step forward.png", QSize(), QIcon::Normal, QIcon::Off);
	actionRunStepForward->setIcon(iconStepForward);
	actionRunStepForward->setIconVisibleInMenu(false);
	actionRunStepBackward = new QAction("Step backward", this);
	actionRunStepBackward->setEnabled(false);
	QIcon iconStepBackward;
	iconStepBackward.addFile(":/Icons/main/step backward.png", QSize(), QIcon::Normal, QIcon::Off);
	actionRunStepBackward->setIcon(iconStepBackward);
	actionRunStepBackward->setIconVisibleInMenu(false);
	actionSnapshot = new QAction("Snapshot", this);
	actionSnapshot->setEnabled(true);
	QIcon iconSnapshot;
	iconSnapshot.addFile(":/Icons/main/snapshot.png", QSize(), QIcon::Normal, QIcon::Off);
	actionSnapshot->setIcon(iconSnapshot);
	actionSnapshot->setIconVisibleInMenu(false);
	actionRestore = new QAction("Restore", this);
	actionRestore->setEnabled(false);
	QIcon iconRestore;
	iconRestore.addFile(":/Icons/main/restore.png", QSize(), QIcon::Normal, QIcon::Off);
	actionRestore->setIcon(iconRestore);
	actionRestore->setIconVisibleInMenu(false);
    actionAcceleration = new QAction("Accelerate active clusters", this);
    actionAcceleration->setIconVisibleInMenu(false);
    actionAcceleration->setEnabled(true);
    actionAcceleration->setCheckable(true);
    actionAcceleration->setChecked(false);
    actionAcceleration->setToolTip("Accelerate computation of active clusters");
    QIcon iconAccelerate;
    iconAccelerate.addFile(":/Icons/main/accelerate on.png", QSize(), QIcon::Normal, QIcon::Off);
    iconAccelerate.addFile(":/Icons/main/accelerate off.png", QSize(), QIcon::Normal, QIcon::On);
    actionAcceleration->setIcon(iconAccelerate);
    actionAcceleration->setIconVisibleInMenu(false);
    actionRestrictTPS = new QAction("Restrict time steps per second", this);
    actionRestrictTPS->setCheckable(true);
    actionRestrictTPS->setChecked(false);
    actionRestrictTPS->setEnabled(true);
    actionExit = new QAction("Exit", this);
    actionExit->setEnabled(true);

    actionSimulationChanger = new QAction("Parameter changer", this);
    actionSimulationChanger->setIconVisibleInMenu(false);
    actionSimulationChanger->setEnabled(true);
    actionSimulationChanger->setCheckable(true);
    actionSimulationChanger->setChecked(false);
    actionSimulationChanger->setToolTip("Change simulation parameters automatically");

	actionComputationSettings = new QAction("General settings", this);
	actionComputationSettings->setEnabled(true);

	actionEditSimParameters = new QAction("Simulation parameters", this);
	actionEditSimParameters->setEnabled(true);

	actionEditSymbols = new QAction("Symbol map", this);
	actionEditSymbols->setEnabled(true);

	actionOpenGLView = new QAction("Pixel/Vector view", this);
    actionOpenGLView->setCheckable(true);
    actionOpenGLView->setChecked(true);
    actionOpenGLView->setEnabled(true);
    QIcon iconOpenGLView;
    iconOpenGLView.addFile(":/Icons/main/pixel view on.png", QSize(), QIcon::Normal, QIcon::On);
    iconOpenGLView.addFile(":/Icons/main/pixel view off.png", QSize(), QIcon::Normal, QIcon::Off);
    actionOpenGLView->setIcon(iconOpenGLView);
    actionOpenGLView->setIconVisibleInMenu(false);

	actionItemView = new QAction("Item-based view", this);
    actionItemView->setCheckable(true);
    actionItemView->setChecked(false);
    actionItemView->setEnabled(true);
    QIcon iconItemView;
    iconItemView.addFile(":/Icons/main/item view on.png", QSize(), QIcon::Normal, QIcon::On);
    iconItemView.addFile(":/Icons/main/item view off.png", QSize(), QIcon::Normal, QIcon::Off);
    actionItemView->setIcon(iconItemView);
    actionItemView->setIconVisibleInMenu(false);

    actionGroupOpenGLItemView = new QActionGroup(this);
    actionGroupOpenGLItemView->addAction(actionOpenGLView);
    actionGroupOpenGLItemView->addAction(actionItemView);

    actionNavigationMode = new QAction("Navigation mode", this);
    actionNavigationMode->setCheckable(true);
    actionNavigationMode->setChecked(true);
    actionNavigationMode->setEnabled(true);
    QIcon iconNavigator;
    iconNavigator.addFile(":/Icons/main/navigation on.png", QSize(), QIcon::Normal, QIcon::On);
    iconNavigator.addFile(":/Icons/main/navigation off.png", QSize(), QIcon::Normal, QIcon::Off);
    actionNavigationMode->setIcon(iconNavigator);
    actionNavigationMode->setIconVisibleInMenu(false);

    actionActionMode = new QAction("Action mode", this);
    actionActionMode->setCheckable(true);
    actionActionMode->setChecked(false);
    actionActionMode->setEnabled(true);
    QIcon iconActionMode;
    iconActionMode.addFile(":/Icons/main/action on.png", QSize(), QIcon::Normal, QIcon::On);
    iconActionMode.addFile(":/Icons/main/action off.png", QSize(), QIcon::Normal, QIcon::Off);
    actionActionMode->setIcon(iconActionMode);
    actionActionMode->setIconVisibleInMenu(false);

	actionGroupNavigationAction = new QActionGroup(this);
    actionGroupNavigationAction->addAction(actionActionMode);
    actionGroupNavigationAction->addAction(actionNavigationMode);

	actionMonitor = new QAction("Info bar", this);
	actionMonitor->setEnabled(true);
	QIcon iconMonitor;
	iconMonitor.addFile(":/Icons/main/monitor on.png", QSize(), QIcon::Normal, QIcon::On);
	iconMonitor.addFile(":/Icons/main/monitor off.png", QSize(), QIcon::Normal, QIcon::Off);
	actionMonitor->setIcon(iconMonitor);
	actionMonitor->setIconVisibleInMenu(false);
	actionMonitor->setCheckable(true);
	actionMonitor->setChecked(false);
	actionZoomIn = new QAction("Zoom in", this);
	actionZoomIn->setEnabled(true);
	QIcon iconZoomIn;
	iconZoomIn.addFile(":/Icons/main/zoom in.png", QSize(), QIcon::Normal, QIcon::Off);
	actionZoomIn->setIcon(iconZoomIn);
	actionZoomIn->setIconVisibleInMenu(false);

	actionZoomOut = new QAction("Zoom out", this);
	actionZoomOut->setEnabled(true);
	QIcon iconZoomOut;
	iconZoomOut.addFile(":/Icons/main/zoom out.png", QSize(), QIcon::Normal, QIcon::Off);
	actionZoomOut->setIcon(iconZoomOut);
	actionZoomOut->setIconVisibleInMenu(false);

    actionDisplayLink = new QAction("Display link", this);
    actionDisplayLink->setCheckable(true);
    actionDisplayLink->setChecked(true);
    actionDisplayLink->setEnabled(true);
    QIcon iconDisplayLink;
    iconDisplayLink.addFile(":/Icons/main/visual on.png", QSize(), QIcon::Normal, QIcon::On);
    iconDisplayLink.addFile(":/Icons/main/visual off.png", QSize(), QIcon::Normal, QIcon::Off);
    actionDisplayLink->setIcon(iconDisplayLink);
    actionDisplayLink->setIconVisibleInMenu(false);

    actionGlowEffect = new QAction("Glow", this);
    actionGlowEffect->setEnabled(true);
    actionGlowEffect->setCheckable(true);
    actionGlowEffect->setShortcut(Qt::ALT + Qt::Key_G);
    actionGlowEffect->setChecked(true);

    actionMotionEffect = new QAction("Motion blur", this);
    actionMotionEffect->setEnabled(true);
    actionMotionEffect->setCheckable(true);
    actionMotionEffect->setShortcut(Qt::ALT + Qt::Key_M);
    actionMotionEffect->setChecked(true);

	actionShowCellInfo = new QAction("Cell info", this);
	QIcon iconCellInfo;
	iconCellInfo.addFile("://Icons/editor/info_off.png", QSize(), QIcon::Normal, QIcon::Off);
	iconCellInfo.addFile("://Icons/editor/info_on.png", QSize(), QIcon::Normal, QIcon::On);
	actionShowCellInfo->setIcon(iconCellInfo);
	actionShowCellInfo->setIconVisibleInMenu(false);
	actionShowCellInfo->setEnabled(false);
	actionShowCellInfo->setCheckable(true);
	actionShowCellInfo->setChecked(true);
	actionShowCellInfo->setToolTip("cell info");

	actionCenterSelection = new QAction("Center selection", this);
	QIcon iconCenterSelection;
	iconCenterSelection.addFile("://Icons/editor/center_off.png", QSize(), QIcon::Normal, QIcon::Off);
	iconCenterSelection.addFile("://Icons/editor/center_on.png", QSize(), QIcon::Normal, QIcon::On);
	actionCenterSelection->setIcon(iconCenterSelection);
	actionCenterSelection->setIconVisibleInMenu(false);
	actionCenterSelection->setEnabled(false);
	actionCenterSelection->setCheckable(true);
	actionCenterSelection->setChecked(false);
	actionCenterSelection->setToolTip("center selection");

	actionNewCell = new QAction("New cell", this);
	QIcon iconNewCell;
	iconNewCell.addFile("://Icons/editor/add_cell.png", QSize(), QIcon::Normal, QIcon::Off);
	actionNewCell->setIcon(iconNewCell);
	actionNewCell->setIconVisibleInMenu(false);
	actionNewCell->setEnabled(true);
	actionNewCell->setToolTip("create cell");

	actionNewParticle = new QAction("New particle", this);;
	QIcon iconNewParticle;
	iconNewParticle.addFile("://Icons/editor/add_energy.png", QSize(), QIcon::Normal, QIcon::Off);
	actionNewParticle->setIcon(iconNewParticle);
	actionNewParticle->setIconVisibleInMenu(false);
	actionNewParticle->setEnabled(true);
	actionNewParticle->setToolTip("create particle");

	actionCopyEntity = new QAction("Copy entity", this);;
	actionCopyEntity->setEnabled(false);

	actionPasteEntity = new QAction("Paste entity", this);
	actionPasteEntity->setEnabled(false);

	actionDeleteEntity = new QAction("Delete entity", this);
	actionDeleteEntity->setEnabled(false);
	actionDeleteEntity->setToolTip("delete entity");

	actionNewToken = new QAction("New token", this);
	QIcon iconNewToken;
	iconNewToken.addFile("://Icons/editor/add_token.png", QSize(), QIcon::Normal, QIcon::Off);
	actionNewToken->setIcon(iconNewToken);
	actionNewToken->setIconVisibleInMenu(false);
	actionNewToken->setEnabled(false);
	actionNewToken->setToolTip("create token");

	actionCopyToken = new QAction("Copy token", this);
	actionCopyToken->setEnabled(false);

	actionPasteToken = new QAction("Paste token", this);
	actionPasteToken->setEnabled(false);

	actionDeleteToken = new QAction("Delete token", this);
	QIcon iconDelToken;
	iconDelToken.addFile("://Icons/editor/del_token.png", QSize(), QIcon::Normal, QIcon::Off);
	actionDeleteToken->setIcon(iconDelToken);
	actionDeleteToken->setIconVisibleInMenu(false);
	actionDeleteToken->setEnabled(false);
	actionDeleteToken->setToolTip("delete token");

    actionCopyToClipboard = new QAction("Copy memory to clipboard", this);
    actionCopyToClipboard->setEnabled(false);

    actionPasteFromClipboard = new QAction("Paste memory from clipboard", this);
    actionPasteFromClipboard->setEnabled(false);

	actionNewRectangle = new QAction("Add rectangle", this);
	actionNewRectangle->setEnabled(true);

	actionNewHexagon = new QAction("Add hexagon", this);
	actionNewHexagon->setEnabled(true);

	actionNewDisc = new QAction("Add disc", this);
    actionNewDisc->setEnabled(true);

	actionNewParticles = new QAction("Add particles", this);
	actionNewParticles->setEnabled(true);

	actionLoadCol = new QAction("Load", this);
	actionLoadCol->setEnabled(true);

	actionSaveCol = new QAction("Save", this);
	actionSaveCol->setEnabled(false);

	actionCopyCol = new QAction("Copy", this);
	actionCopyCol->setShortcut(Qt::CTRL + Qt::Key_C);
	actionCopyCol->setEnabled(false);

	actionPasteCol = new QAction("Paste", this);
	actionPasteCol->setShortcut(Qt::CTRL + Qt::Key_V);
	actionPasteCol->setEnabled(false);

	actionDeleteSel = new QAction("Delete selection", this);
	QIcon iconDeleteSel;
	iconDeleteSel.addFile("://Icons/editor/del_cell.png", QSize(), QIcon::Normal, QIcon::Off);
	actionDeleteSel->setIcon(iconDeleteSel);
	actionDeleteSel->setIconVisibleInMenu(false);
	actionDeleteSel->setEnabled(false);
	actionDeleteSel->setShortcut(Qt::Key_Delete);
	actionDeleteSel->setToolTip("delete selection");

	actionDeleteCol = new QAction("Delete extended selection", this);
	QIcon iconDeleteCol;
	iconDeleteCol.addFile("://Icons/editor/add_cluster.png", QSize(), QIcon::Normal, QIcon::Off);
	actionDeleteCol->setIcon(iconDeleteCol);
	actionDeleteCol->setIconVisibleInMenu(false);
	actionDeleteCol->setEnabled(false);
	actionDeleteCol->setToolTip("delete extended selection");

    actionColorizeSel = new QAction("Colorize", this);
    actionColorizeSel->setEnabled(false);

    actionGenerateBranchNumbers = new QAction("Generate branch numbers", this);
    actionGenerateBranchNumbers->setEnabled(false);

	actionRandomizeCellFunctions = new QAction("Randomize cell functions", this);
    actionRandomizeCellFunctions->setEnabled(false);

	actionRemoveFreeCellConnections = new QAction("Remove unused cell connections", this);
    actionRemoveFreeCellConnections->setEnabled(false);

	actionRandomMultiplier = new QAction("Random multiplier", this);
	actionRandomMultiplier->setEnabled(false);

	actionGridMultiplier = new QAction("Grid multiplier", this);
	actionGridMultiplier->setEnabled(false);

    actionRepetitiveActiveClusterAnalysis = new QAction("Repetitive pattern analysis", this);
    actionRepetitiveActiveClusterAnalysis->setEnabled(true);

	actionWriteMonitorLog = new QAction("Stream monitor data to file", this);
    actionWriteMonitorLog->setEnabled(true);
    actionWriteMonitorLog->setCheckable(true);
    actionWriteMonitorLog->setChecked(false);

	actionAbout = new QAction("About", this);
	actionAbout->setEnabled(true);
    
    actionGettingStarted = new QAction("Getting started", this);
    actionGettingStarted->setCheckable(true);
    actionGettingStarted->setChecked(true);
    actionGettingStarted->setEnabled(true);

	actionDocumentation = new QAction("Documentation", this);
	actionDocumentation->setEnabled(true);
	actionDocumentation->setCheckable(false);
}

