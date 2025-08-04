// CustomPlotVisualizer.h
#pragma once

#include <QObject>
#include <QMainWindow>
#include "qcustomplot.h"
#include <vector>
#include <string>

class CustomPlotVisualizer : public QObject
{
    Q_OBJECT

public:
    explicit CustomPlotVisualizer(QObject *parent = nullptr);

    void plotBasicGraph(
        const std::vector<std::vector<std::vector<double>>>& multiData, // داده‌های چند نمودار
        const QString& title,
        const QString& xLabel,
        const QString& yLabel,
        const std::vector<QString>& legendNames // نام‌های راهنما برای هر نمودار
    );

    void plotBarChart(const std::vector<std::pair<QString, double>>& data,
                     const QString& title = "نمودار میله‌ای");

    void savePlot(QCustomPlot* plot, const QString& filename, int width = 800, int height = 600);
    void enableCursorInteraction(QCustomPlot* plot);
    void enableAdvancedCursor(QCustomPlot* plot);
    void updateCursorLines(
    QCustomPlot* plot,
    QCPItemTracer* cursor1,
    QCPItemTracer* cursor2,
    QCPItemLine* verticalLine1,
    QCPItemLine* horizontalLine1,
    QCPItemLine* verticalLine2,
    QCPItemLine* horizontalLine2
    );

private:
    QMainWindow* createPlotWindow(const QString& title = "نمودار", int width = 800, int height = 600);
    void setupCustomPlot(QCustomPlot* plot,
                        const QString& title,
                        const QString& xLabel,
                        const QString& yLabel,
                        bool showLegend);
    void setupAxes(QCustomPlot* plot, const QString& xLabel, const QString& yLabel);
    void setupLegend(QCustomPlot* plot, bool visible);
    void setupInteractions(QCustomPlot* plot);
};