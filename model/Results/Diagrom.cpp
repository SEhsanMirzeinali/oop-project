#include "Diagrom.h"
#include <QFileDialog>
#include <QFont>
#include <QToolTip>

CustomPlotVisualizer::CustomPlotVisualizer(QObject *parent) : QObject(parent) {}

void CustomPlotVisualizer::plotBasicGraph(
    const std::vector<std::vector<std::vector<double>>>& multiData,
    const QString& title,
    const QString& xLabel,
    const QString& yLabel,
    const std::vector<QString>& legendNames
) {
    QMainWindow* window = createPlotWindow(title);
    QCustomPlot* plot = new QCustomPlot();
    window->setCentralWidget(plot);

    setupCustomPlot(plot, title, xLabel, yLabel, true);

    // رسم نمودارها
    for (size_t i = 0; i < multiData.size(); ++i) {
        QVector<double> xData, yData;
        for (const auto& point : multiData[i]) {
            if (point.size() >= 2) {
                xData << point[0];
                yData << point[1];
            }
        }

        plot->addGraph();
        plot->graph(i)->setData(xData, yData);
        plot->graph(i)->setName(legendNames[i]);
        plot->graph(i)->setPen(QPen(QColor::fromHsv(i * 360 / multiData.size(), 255, 200), 2));
    }

    enableAdvancedCursor(plot);
    plot->rescaleAxes();
    window->show();
}

void CustomPlotVisualizer::enableAdvancedCursor(QCustomPlot* plot) {
    if (plot->graphCount() == 0) {
        qDebug() << "No graph available for cursor!";
        return;
    }

    QCPItemTracer *cursor1 = new QCPItemTracer(plot);
    QCPItemTracer *cursor2 = new QCPItemTracer(plot);
    QCPItemLine *vLine1 = new QCPItemLine(plot);
    QCPItemLine *hLine1 = new QCPItemLine(plot);
    QCPItemLine *vLine2 = new QCPItemLine(plot);
    QCPItemLine *hLine2 = new QCPItemLine(plot);

    enum CursorState { Hidden, MovingFirst, FixedFirst, MovingSecond, FixedBoth };
    auto state = new CursorState(Hidden);

    cursor1->setStyle(QCPItemTracer::tsCircle);
    cursor1->setPen(QPen(Qt::red, 2));
    cursor1->setBrush(Qt::red);
    cursor1->setSize(8);

    cursor2->setStyle(QCPItemTracer::tsSquare);
    cursor2->setPen(QPen(Qt::blue, 2));
    cursor2->setBrush(Qt::blue);
    cursor2->setSize(8);

    for (auto line : {vLine1, hLine1, vLine2, hLine2}) {
        line->setPen(QPen(Qt::darkGray, 1, Qt::DashLine));
        line->setVisible(false);
    }

    cursor1->setVisible(false);
    cursor2->setVisible(false);

    auto findNearestKeyOnGraph = [](QCPGraph* graph, double x) -> double {
        double minDist = std::numeric_limits<double>::max();
        double bestKey = x;
        if (!graph || graph->data()->isEmpty()) return x;

        const auto& data = *(graph->data());
        for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
            double dist = std::abs(it->key - x);
            if (dist < minDist) {
                minDist = dist;
                bestKey = it->key;
            }
        }
        return bestKey;
    };

    connect(plot, &QCustomPlot::mousePress, [=](QMouseEvent* event) {
        // تشخیص نمودار نزدیک‌تر به محل کلیک
        QCPAbstractPlottable* plottable = plot->plottableAt(event->pos(), false);
        QCPGraph* clickedGraph = qobject_cast<QCPGraph*>(plottable);
        if (!clickedGraph) return;

        double x = plot->xAxis->pixelToCoord(event->pos().x());
        double nearestX = findNearestKeyOnGraph(clickedGraph, x);

        switch (*state) {
            case Hidden:
                *state = MovingFirst;
                cursor1->setGraph(clickedGraph);
                cursor1->setVisible(true);
                vLine1->setVisible(true);
                hLine1->setVisible(true);
                cursor1->setGraphKey(nearestX);
                cursor1->updatePosition();
                break;

            case MovingFirst:
                *state = FixedFirst;
                cursor1->setGraphKey(nearestX);
                cursor1->updatePosition();
                break;

            case FixedFirst:
                *state = MovingSecond;
                cursor2->setGraph(clickedGraph);
                cursor2->setVisible(true);
                vLine2->setVisible(true);
                hLine2->setVisible(true);
                cursor2->setGraphKey(nearestX);
                cursor2->updatePosition();
                break;

            case MovingSecond:
                *state = FixedBoth;
                cursor2->setGraphKey(nearestX);
                cursor2->updatePosition();
                break;

            case FixedBoth:
                *state = Hidden;
                for (auto item : {cursor1, cursor2}) item->setVisible(false);
                for (auto line : {vLine1, hLine1, vLine2, hLine2}) line->setVisible(false);
                break;
        }

        plot->replot();
    });

    connect(plot, &QCustomPlot::mouseMove, [=](QMouseEvent* event) {
        double x = plot->xAxis->pixelToCoord(event->pos().x());

        if (*state == MovingFirst && cursor1->graph()) {
            double nearestX = findNearestKeyOnGraph(cursor1->graph(), x);
            cursor1->setGraphKey(nearestX);
            cursor1->updatePosition();
            updateCursorLines(plot, cursor1, cursor1, vLine1, hLine1, vLine1, hLine1);
        }
        else if (*state == MovingSecond && cursor2->graph()) {
            double nearestX = findNearestKeyOnGraph(cursor2->graph(), x);
            cursor2->setGraphKey(nearestX);
            cursor2->updatePosition();
            updateCursorLines(plot, cursor2, cursor2, vLine2, hLine2, vLine2, hLine2);
        }
        else if (*state == FixedBoth) {
            updateCursorLines(plot, cursor1, cursor2, vLine1, hLine1, vLine2, hLine2);
        }

        plot->replot();
    });
}


// تابع برای به‌روزرسانی خطوط نقطه‌چین
void CustomPlotVisualizer::updateCursorLines(
    QCustomPlot* plot,
    QCPItemTracer* cursor1,
    QCPItemTracer* cursor2,
    QCPItemLine* verticalLine1,
    QCPItemLine* horizontalLine1,
    QCPItemLine* verticalLine2,
    QCPItemLine* horizontalLine2
) {
    // خطوط برای کرسر اول (قرمز)
    verticalLine1->start->setCoords(cursor1->position->key(), plot->yAxis->range().lower);
    verticalLine1->end->setCoords(cursor1->position->key(), plot->yAxis->range().upper);
    horizontalLine1->start->setCoords(plot->xAxis->range().lower, cursor1->position->value());
    horizontalLine1->end->setCoords(plot->xAxis->range().upper, cursor1->position->value());

    // خطوط برای کرسر دوم (آبی)
    verticalLine2->start->setCoords(cursor2->position->key(), plot->yAxis->range().lower);
    verticalLine2->end->setCoords(cursor2->position->key(), plot->yAxis->range().upper);
    horizontalLine2->start->setCoords(plot->xAxis->range().lower, cursor2->position->value());
    horizontalLine2->end->setCoords(plot->xAxis->range().upper, cursor2->position->value());

    // نمایش اختلاف مقادیر در Tooltip
    double deltaX = cursor2->position->key() - cursor1->position->key();
    double deltaY = cursor2->position->value() - cursor1->position->value();
    QToolTip::showText(
        QCursor::pos(),
        QString("Cursor 1: X=%1, Y=%2\nCursor 2: X=%3, Y=%4\nΔX=%5, ΔY=%6")
            .arg(cursor1->position->key(), 0, 'f', 2)
            .arg(cursor1->position->value(), 0, 'f', 2)
            .arg(cursor2->position->key(), 0, 'f', 2)
            .arg(cursor2->position->value(), 0, 'f', 2)
            .arg(deltaX, 0, 'f', 2)
            .arg(deltaY, 0, 'f', 2),
        plot
    );
}

void CustomPlotVisualizer::plotBarChart(const std::vector<std::pair<QString, double>>& data,
                                      const QString& title)
{
    QMainWindow* window = createPlotWindow(title);
    QCustomPlot* plot = new QCustomPlot();
    window->setCentralWidget(plot);

    setupCustomPlot(plot, title, "دسته‌بندی", "مقدار", false);

    QCPBars* bars = new QCPBars(plot->xAxis, plot->yAxis);

    // روش جایگزین برای تنظیم ticks در نسخه‌های جدید
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);

    for (int i = 0; i < data.size(); ++i) {
        textTicker->addTick(i+1, data[i].first);
        bars->addData(i+1, data[i].second);
    }

    plot->xAxis->setTicker(textTicker);
    bars->setWidth(0.8);
    bars->setPen(QPen(Qt::black));
    bars->setBrush(QBrush(QColor(100, 149, 237)));

    plot->rescaleAxes();
    window->show();
}

QMainWindow* CustomPlotVisualizer::createPlotWindow(const QString& title, int width, int height)
{
    QMainWindow* window = new QMainWindow();
    window->setWindowTitle(title);
    window->resize(width, height);
    return window;
}

void CustomPlotVisualizer::setupCustomPlot(QCustomPlot* plot,
                                         const QString& title,
                                         const QString& xLabel,
                                         const QString& yLabel,
                                         bool showLegend)
{
    // عنوان
    plot->plotLayout()->insertRow(0);
    QCPTextElement* titleElement = new QCPTextElement(plot);
    titleElement->setText(title);
    titleElement->setFont(QFont("B Nazanin", 12, QFont::Bold));
    plot->plotLayout()->addElement(0, 0, titleElement);

    // محورها
    setupAxes(plot, xLabel, yLabel);

    // راهنما
    setupLegend(plot, showLegend);

    // تعاملات
    setupInteractions(plot);
}

void CustomPlotVisualizer::setupAxes(QCustomPlot* plot, const QString& xLabel, const QString& yLabel)
{
    plot->xAxis->setLabel(xLabel);
    plot->yAxis->setLabel(yLabel);
    plot->xAxis->setLabelFont(QFont("B Nazanin", 10));
    plot->yAxis->setLabelFont(QFont("B Nazanin", 10));
    plot->xAxis->setTickLabelFont(QFont("B Nazanin", 8));
    plot->yAxis->setTickLabelFont(QFont("B Nazanin", 8));
    plot->xAxis->grid()->setVisible(true);
    plot->yAxis->grid()->setVisible(true);
}

void CustomPlotVisualizer::setupLegend(QCustomPlot* plot, bool visible)
{
    plot->legend->setVisible(visible);
    plot->legend->setFont(QFont("B Nazanin", 9));
    plot->legend->setBrush(QBrush(QColor(255, 255, 255, 200)));
    plot->legend->setBorderPen(QPen(QColor(0, 0, 0, 150)));
}

void CustomPlotVisualizer::setupInteractions(QCustomPlot* plot)
{
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // امکان ذخیره با دابل کلیک
    connect(plot, &QCustomPlot::mouseDoubleClick, [this, plot]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "ذخیره نمودار", "", "*.png");
        if (!fileName.isEmpty()) {
            savePlot(plot, fileName);
        }
    });
}

void CustomPlotVisualizer::savePlot(QCustomPlot* plot, const QString& filename, int width, int height)
{
    plot->savePng(filename, width, height);
}

void CustomPlotVisualizer::enableCursorInteraction(QCustomPlot* plot) {
    // ایجاد QLabel برای نمایش مختصات
    QLabel* coordLabel = new QLabel(plot);
    coordLabel->setStyleSheet("QLabel { background-color: white; padding: 2px; }");
    coordLabel->setVisible(false);

    // خطوط عمودی و افقی Cursor
    QCPItemLine* verticalLine = new QCPItemLine(plot);
    QCPItemLine* horizontalLine = new QCPItemLine(plot);

    verticalLine->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    horizontalLine->setPen(QPen(Qt::gray, 1, Qt::DashLine));

    // اتصال سیگنال‌های ماوس
    connect(plot, &QCustomPlot::mouseMove, [=](QMouseEvent* event) {
        double x = plot->xAxis->pixelToCoord(event->pos().x());
        double y = plot->yAxis->pixelToCoord(event->pos().y());

        // به‌روزرسانی خطوط Cursor
        verticalLine->start->setCoords(x, plot->yAxis->range().lower);
        verticalLine->end->setCoords(x, plot->yAxis->range().upper);
        horizontalLine->start->setCoords(plot->xAxis->range().lower, y);
        horizontalLine->end->setCoords(plot->xAxis->range().upper, y);

        // نمایش مختصات
        coordLabel->setText(QString("X: %1, Y: %2").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2));
        coordLabel->move(event->pos() + QPoint(10, 10));
        coordLabel->setVisible(true);
        plot->replot();
    });

    // connect(plot, &QCustomPlot::mouseExit, [=]() {
    //     coordLabel->setVisible(false);
    //     plot->replot();
    // });
}