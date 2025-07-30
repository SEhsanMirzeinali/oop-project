#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>

class ToolBar : public QWidget {
    Q_OBJECT
public:
    ToolBar(QWidget* parent = nullptr);
    QPushButton* getResistorButton() const;
    QPushButton* getVoltageButton() const;
    QPushButton* getCapacitorButton() const;
    QPushButton* getInductorButton() const;
    QPushButton* getWireButton() const;
    QPushButton* getGroundButton() const;

private:
    void setupUI();
    QPushButton* resistorBtn;
    QPushButton* voltageBtn;
    QPushButton* capacitorBtn;
    QPushButton* inductorBtn;
    QPushButton* wireBtn;
    QPushButton* groundBtn;
};

#endif // TOOLBAR_H//
// Created by Purple-Rose on 7/23/2025.
//

