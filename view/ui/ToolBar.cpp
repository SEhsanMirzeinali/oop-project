#include "ToolBar.h"
#include <QHBoxLayout>
#include <QIcon>

ToolBar::ToolBar(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ToolBar::setupUI() {
    QHBoxLayout* toolbarLayout = new QHBoxLayout(this);

    resistorBtn = new QPushButton(this);
    voltageBtn = new QPushButton(this);
    capacitorBtn = new QPushButton(this);
    inductorBtn = new QPushButton(this);
    wireBtn = new QPushButton(this);
    groundBtn= new QPushButton(this);

    resistorBtn->setIcon(QIcon("../images/Resistor.png"));
    resistorBtn->setIconSize(QSize(32, 32));
    resistorBtn->setFixedSize(40, 40);

    voltageBtn->setIcon(QIcon("../images/VoltageSource.png"));
    voltageBtn->setIconSize(QSize(32, 32));
    voltageBtn->setFixedSize(40, 40);

    capacitorBtn->setIcon(QIcon("../images/Capacitor.png"));
    capacitorBtn->setIconSize(QSize(32, 32));
    capacitorBtn->setFixedSize(40, 40);

    inductorBtn->setIcon(QIcon("../images/Inductor.png"));
    inductorBtn->setIconSize(QSize(32, 32));
    inductorBtn->setFixedSize(40, 40);

    wireBtn->setIcon(QIcon("../images/Wire.png"));
    wireBtn->setIconSize(QSize(32, 32));
    wireBtn->setFixedSize(40, 40);

    groundBtn->setIcon(QIcon("../images/GND.png"));
    groundBtn->setIconSize(QSize(32, 32));
    groundBtn->setFixedSize(40, 40);

    resistorBtn->setStyleSheet("QPushButton { border: none; }");
    voltageBtn->setStyleSheet("QPushButton { border: none; }");
    capacitorBtn->setStyleSheet("QPushButton { border: none; }");
    inductorBtn->setStyleSheet("QPushButton { border: none; }");
    wireBtn->setStyleSheet("QPushButton { border: none; }");
    groundBtn->setStyleSheet("QPushButton { border: none; }");

    toolbarLayout->addWidget(resistorBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(voltageBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(wireBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(capacitorBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(inductorBtn);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(groundBtn);
    toolbarLayout->addStretch();
}

QPushButton* ToolBar::getResistorButton() const { return resistorBtn; }
QPushButton* ToolBar::getVoltageButton() const { return voltageBtn; }
QPushButton* ToolBar::getCapacitorButton() const { return capacitorBtn; }
QPushButton* ToolBar::getInductorButton() const { return inductorBtn; }
QPushButton* ToolBar::getWireButton() const { return wireBtn; }
QPushButton* ToolBar::getGroundButton() const {return groundBtn;}