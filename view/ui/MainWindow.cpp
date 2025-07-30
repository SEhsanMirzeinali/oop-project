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
    connect(toolbar->getWireButton(), &QPushButton::clicked,
            [this]() { view->startWiring(); });
}