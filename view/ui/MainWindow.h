#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ComponentView.h"
#include "ToolBar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* event) override;

private:
    void setupUI();
    void setupConnections();
    QGraphicsScene* scene;
    ComponentView* view;
    ToolBar* toolbar;
};

#endif // MAINWINDOW_H//
// Created by Purple-Rose on 7/23/2025.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H



class MainWindow {

};



#endif //MAINWINDOW_H
