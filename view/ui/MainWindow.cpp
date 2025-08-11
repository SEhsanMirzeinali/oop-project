#include "MainWindow.h"
#include <QVBoxLayout>
#include <QShowEvent>
#include "../grid/Grid.h"
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
    setupConnections();
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    view->setFocus();
}

void MainWindow::setupUI() {
    setWindowTitle("Circuit Simulator");
    setMinimumSize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    toolbar = new ToolBar(this);
    mainLayout->addWidget(toolbar);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);

    Grid* grid = new Grid();
    scene->addItem(grid);

    view = new ComponentView(scene, this);
    mainLayout->addWidget(view);
}

void MainWindow::setupConnections() {
    connect(toolbar->getResistorButton(), &QPushButton::clicked,
            [this]() { view->startPlacing("resistor"); });
    connect(toolbar->getVoltageButton(), &QPushButton::clicked,
            [this]() { view->startPlacing("voltage"); });
    connect(toolbar->getCapacitorButton(), &QPushButton::clicked,
            [this]() { view->startPlacing("capacitor"); });
    connect(toolbar->getInductorButton(), &QPushButton::clicked,
            [this]() { view->startPlacing("inductor"); });
    connect(toolbar->getGroundButton(), &QPushButton::clicked,
    [this](){view->startPlacing("ground");});
    connect(toolbar->getCurrentButton(), &QPushButton::clicked,
        [this]() { view->startPlacing("current"); });
    connect(toolbar->getWireButton(), &QPushButton::clicked,
            [this]() { view->startWiring(); });
    connect(toolbar, &ToolBar::spiceAnalysis, view, &ComponentView::handleAnalysis);
    connect(toolbar, &ToolBar::probeAction, view, &ComponentView::handleProbeAction);
    connect(toolbar, &ToolBar::newUnipolarAction, view, &ComponentView::createUnipolar);
    connect(toolbar, &ToolBar::newTNAction, view, &ComponentView::createTNSubCircuit);
    connect(toolbar, &ToolBar::openUnipolarAction, view, &ComponentView::loadUnipolarNetList);
    connect(toolbar, &ToolBar::newProject, view, &ComponentView::resetComponentView);
    connect(toolbar, &ToolBar::openThevininAction, view, &ComponentView::chooseThevenin);
    connect(toolbar, &ToolBar::openNortonAction, view, &ComponentView::chooseNorton);




}