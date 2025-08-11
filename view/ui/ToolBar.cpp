#include "ToolBar.h"
#include <QAction>
#include <QIcon>
#include <QStyle>
#include <QApplication>

ToolBar::ToolBar(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    createMenuBar();
    //createQuickAccessToolbar();
    setupUI();

    mainLayout->addWidget(menuBar);
    //mainLayout->addWidget(quickAccessToolbar);

    QWidget* mainToolbar = new QWidget();
    mainToolbar->setLayout(toolbarLayout);
    mainLayout->addWidget(mainToolbar);
}

void ToolBar::setupUI()
{
    toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(10);

    // Create buttons with text and icons
    resistorBtn = createToolButton("Resistor", "../images/Resistor.png");
    voltageBtn = createToolButton("Voltage", "../images/VoltageSource.png");
    currentBtn = createToolButton("Current", "../images/CurrentSource.png");
    wireBtn = createToolButton("Wire", "../images/Wire.png");
    capacitorBtn = createToolButton("Capacitor", "../images/Capacitor.png");
    inductorBtn = createToolButton("Inductor", "../images/Inductor.png");
    groundBtn = createToolButton("Ground", "../images/GND.png");

    // Add buttons to layout
    toolbarLayout->addWidget(resistorBtn);
    toolbarLayout->addWidget(voltageBtn);
    toolbarLayout->addWidget(currentBtn);
    toolbarLayout->addWidget(wireBtn);
    toolbarLayout->addWidget(capacitorBtn);
    toolbarLayout->addWidget(inductorBtn);
    toolbarLayout->addWidget(groundBtn);
}

void ToolBar::createMenuBar()
{
    menuBar = new QMenuBar(this);

    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&New Unipolar", this, &ToolBar::newUnipolarAction, QKeySequence("Ctrl+U"));
    fileMenu->addAction("&New Thevenin&Norton", this, &ToolBar::newTNAction, QKeySequence("Ctrl+T"));
    fileMenu->addAction("&New Project", this, &ToolBar::newProject, QKeySequence::New);
    fileMenu->addAction("&Open Unipolar", this, &ToolBar::openUnipolarAction, QKeySequence::Open);
    fileMenu->addAction("&Open Thevinin", this, &ToolBar::openThevininAction, QKeySequence("Ctrl+Shift+T"));
    fileMenu->addAction("&Open Norton", this, &ToolBar::openNortonAction, QKeySequence("Ctrl+Shift+N"));

    fileMenu->addAction("&Save", this, &ToolBar::saveProject, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", qApp, &QApplication::quit, QKeySequence::Quit);

    // Edit menu
    QMenu* editMenu = menuBar->addMenu("&Edit");
    editMenu->addAction("&Spice Analysis", this, &ToolBar::spiceAnalysis, QKeySequence("Ctrl+A"));
    editMenu->addAction("&Probe On/Off", this, &ToolBar::probeAction, QKeySequence("P"));

    // editMenu->addAction("&Undo", this, &ToolBar::spiceAnalysis, QKeySequence::Undo);
    // editMenu->addAction("&Redo", this, &ToolBar::redoAction, QKeySequence::Redo);
}

//void ToolBar::createQuickAccessToolbar()
//{
    // quickAccessToolbar = new QToolBar("Quick Access", this);
    // quickAccessToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // quickAccessToolbar->setIconSize(QSize(24, 24));
    //
    // addToolAction(quickAccessToolbar, "New", ":/icons/new.png");
    // addToolAction(quickAccessToolbar, "Open", ":/icons/open.png");
    // addToolAction(quickAccessToolbar, "Save", ":/icons/save.png");
    // quickAccessToolbar->addSeparator();
    // addToolAction(quickAccessToolbar, "Undo", ":/icons/undo.png");
    // addToolAction(quickAccessToolbar, "Redo", ":/icons/redo.png");
//}

QPushButton* ToolBar::createToolButton(const QString& text, const QString& iconPath)
{
    QPushButton* btn = new QPushButton(this);
    btn->setText(text);

    if (!iconPath.isEmpty()) {
        btn->setIcon(QIcon(iconPath));
    }

    btn->setIconSize(QSize(24, 24));
    btn->setFixedHeight(40);
    btn->setMinimumWidth(80);

    btn->setStyleSheet(R"(
        QPushButton {
            border: 1px solid #c0c0c0;
            border-radius: 4px;
            padding: 5px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #f6f7fa, stop:1 #dadbde);
            font-size: 11px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #ebf4fc, stop:1 #d0e3f8);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #d0e3f8, stop:1 #ebf4fc);
        }
    )");

    return btn;
}

void ToolBar::addToolAction(QToolBar* toolbar, const QString& text, const QString& iconPath)
{
    QAction* action = new QAction(text, this);
    if (!iconPath.isEmpty()) {
        action->setIcon(QIcon(iconPath));
    }
    toolbar->addAction(action);
}