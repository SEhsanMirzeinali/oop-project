#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);

    // Getter methods
    QPushButton* getResistorButton() const { return resistorBtn; }
    QPushButton* getVoltageButton() const { return voltageBtn; }
    QPushButton* getCapacitorButton() const { return capacitorBtn; }
    QPushButton* getInductorButton() const { return inductorBtn; }
    QPushButton* getWireButton() const { return wireBtn; }
    QPushButton* getGroundButton() const { return groundBtn; }
    QPushButton* getCurrentButton() const { return currentBtn; }

    signals:
    void newProject();
    void openProject();
    void saveProject();
    void undoAction();
    void redoAction();
    void spiceAnalysis();
    void probeAction();
    void newUnipolarAction();
    void openUnipolarAction();
    void openThevininAction();
    void openNortonAction();
    void newTNAction();




private:
    void setupUI();
    void createMenuBar();
    //void createQuickAccessToolbar();
    QPushButton* createToolButton(const QString& text, const QString& iconPath = "");
    void addToolAction(QToolBar* toolbar, const QString& text, const QString& iconPath = "");

    // Main components
    QMenuBar* menuBar;
    //QToolBar* quickAccessToolbar;
    QHBoxLayout* toolbarLayout;

    // Tool buttons
    QPushButton* resistorBtn;
    QPushButton* voltageBtn;
    QPushButton* capacitorBtn;
    QPushButton* inductorBtn;
    QPushButton* wireBtn;
    QPushButton* groundBtn;
    QPushButton* currentBtn;
};

#endif // TOOLBAR_H