#include "Diagrom.h"
#include <QFileDialog>
#include <QFont>
#include <QToolTip>

CustomPlotVisualizer::CustomPlotVisualizer(QObject *parent) : QObject(parent) {}

struct GraphData {
    QVector<double> x;
    QVector<double> y;
    QString name;
};

std::vector<GraphData> internalGraphs;  // ذخیره داخلی همه نمودارها

void CustomPlotVisualizer::plotBasicGraphWithMathOps(
    const std::vector<std::vector<std::vector<double>>>& multiData,
    const QString& title,
    const QString& xLabel,
    const QString& yLabel,
    const std::vector<QString>& legendNames,
    bool logX // ← پارامتر جدید برای کنترل محور X
) {
    internalGraphs.clear();

    QMainWindow* window = createPlotWindow(title);
    QCustomPlot* plot = new QCustomPlot();
    window->setCentralWidget(plot);

    setupCustomPlot(plot, title, xLabel, yLabel, true);

    // اگر کاربر خواست محور X لگاریتمی باشد
    if (logX) {
        plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        plot->xAxis->setNumberFormat("eb");  // scientific notation
        plot->xAxis->setNumberPrecision(0);
    }

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
        plot->graph(i)->setName(i < legendNames.size() ? legendNames[i] : QString("Graph %1").arg(i + 1));
        plot->graph(i)->setPen(QPen(QColor::fromHsv(i * 360 / multiData.size(), 255, 200), 2));

        internalGraphs.push_back({xData, yData, plot->graph(i)->name()});
    }

    enableAdvancedCursor(plot);
    plot->rescaleAxes();
    window->show();


    plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(plot, &QWidget::customContextMenuRequested, [=](const QPoint& pos) {
        QMenu menu;
        QAction* changeColorAction = menu.addAction("تغییر رنگ نمودار");
        QAction* addGraphsAction = menu.addAction("جمع دو نمودار");
        QAction* subGraphsAction = menu.addAction("تفریق دو نمودار");
        QAction* mulGraphsAction = menu.addAction("ضرب دو نمودار");

        QAction* changeTitleAction = menu.addAction("تغییر عنوان نمودار");
        QAction* changeXLabelAction = menu.addAction("تغییر برچسب محور X");
        QAction* changeYLabelAction = menu.addAction("تغییر برچسب محور Y");

        QAction* selected = menu.exec(plot->mapToGlobal(pos));
        if (!selected) return;

        if (selected == changeColorAction) {
            QStringList graphNames;
            for (const auto& g : internalGraphs)
                graphNames << g.name;

            bool ok;
            QString selectedGraph = QInputDialog::getItem(plot, "انتخاب نمودار", "نمودار:", graphNames, 0, false, &ok);
            if (!ok) return;

            int graphIndex = -1;
            for (int i = 0; i < plot->graphCount(); ++i) {
                if (plot->graph(i)->name() == selectedGraph) {
                    graphIndex = i;
                    break;
                }
            }
            if (graphIndex == -1) return;

            QStringList colorNames = {"Red", "Green", "Blue", "Black", "Cyan", "Magenta", "Yellow"};
            QString selectedColorName = QInputDialog::getItem(plot, "انتخاب رنگ", "رنگ جدید:", colorNames, 0, false, &ok);
            if (!ok || selectedColorName.isEmpty()) return;

            QColor newColor(selectedColorName);
            QColor currentColor = plot->graph(graphIndex)->pen().color();
            if (newColor != currentColor) {
                plot->graph(graphIndex)->setPen(QPen(newColor, 2));
                plot->replot();
            }
        }

        else if (selected == changeTitleAction) {
            bool ok;
            QString currentTitle = plot->plotLayout()->rowCount() > 0 && plot->plotLayout()->elementAt(0)
                                   ? plot->plotLayout()->elementAt(0)->objectName()
                                   : "";

            QString newTitle = QInputDialog::getText(plot, "تغییر عنوان", "عنوان جدید:", QLineEdit::Normal, currentTitle, &ok);
                        if (ok && !newTitle.isEmpty()) {
                plot->plotLayout()->insertRow(0);
                QCPTextElement* titleElement = new QCPTextElement(plot, newTitle, QFont("Sans", 12, QFont::Bold));
                plot->plotLayout()->addElement(0, 0, titleElement);
                plot->replot();
            }
        }

        else if (selected == changeXLabelAction) {
            bool ok;
            QString newX = QInputDialog::getText(plot, "تغییر برچسب محور X", "متن جدید:", QLineEdit::Normal, plot->xAxis->label(), &ok);
            if (ok && !newX.isEmpty()) {
                plot->xAxis->setLabel(newX);
                plot->replot();
            }
        }

        else if (selected == changeYLabelAction) {
            bool ok;
            QString newY = QInputDialog::getText(plot, "تغییر برچسب محور Y", "متن جدید:", QLineEdit::Normal, plot->yAxis->label(), &ok);
            if (ok && !newY.isEmpty()) {
                plot->yAxis->setLabel(newY);
                plot->replot();
            }
        }

        else {
            QStringList graphNames;
            for (const auto& g : internalGraphs)
                graphNames << g.name;

            bool ok1, ok2;
            QString g1Name = QInputDialog::getItem(plot, "انتخاب نمودار اول", "نمودار:", graphNames, 0, false, &ok1);
            QString g2Name = QInputDialog::getItem(plot, "انتخاب نمودار دوم", "نمودار:", graphNames, 0, false, &ok2);
            if (!ok1 || !ok2 || g1Name == g2Name) return;

            auto it1 = std::find_if(internalGraphs.begin(), internalGraphs.end(), [&](const GraphData& g){ return g.name == g1Name; });
            auto it2 = std::find_if(internalGraphs.begin(), internalGraphs.end(), [&](const GraphData& g){ return g.name == g2Name; });
            if (it1 == internalGraphs.end() || it2 == internalGraphs.end()) return;

            QVector<double> x, y;
            int n = std::min(it1->x.size(), it2->x.size());
            for (int i = 0; i < n; ++i) {
                x.push_back(it1->x[i]);
                double val = 0;
                if (selected == addGraphsAction) val = it1->y[i] + it2->y[i];
                else if (selected == subGraphsAction) val = it1->y[i] - it2->y[i];
                else if (selected == mulGraphsAction) val = it1->y[i] * it2->y[i];
                y.push_back(val);
            }

            int index = plot->graphCount();
            plot->addGraph();
            plot->graph(index)->setData(x, y);
            QString newName = QString("(%1 %2 %3)")
                              .arg(it1->name)
                              .arg(selected == addGraphsAction ? "+" : selected == subGraphsAction ? "−" : "×")
                              .arg(it2->name);
            plot->graph(index)->setName(newName);
            plot->graph(index)->setPen(QPen(Qt::darkMagenta, 2));
            plot->replot();

            internalGraphs.push_back({x, y, newName});
        }
        // امکان تغییر label نمودار با کلیک راست روی نمودار
        plot->setInteractions(QCP::iSelectPlottables); // اطمینان از فعال بودن انتخاب نمودار

    connect(plot, &QCustomPlot::mousePress, plot, [=](QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        // بررسی اینکه روی چه چیزی کلیک شده
        bool foundGraph = false;
        for (int i = 0; i < plot->graphCount(); ++i) {
            QCPGraph* graph = plot->graph(i);
            if (!graph) continue;

            double distance = graph->selectTest(event->pos(), false); // بررسی نزدیکی به graph
            if (distance < 5) { // اگه خیلی نزدیک بود (5 پیکسل)
                foundGraph = true;

                // نمایش پنجره تغییر نام
                bool ok;
                QString currentLabel = graph->name();
                QString newLabel = QInputDialog::getText(plot, "تغییر برچسب نمودار", "نام جدید:", QLineEdit::Normal, currentLabel, &ok);
                if (ok && !newLabel.isEmpty()) {
                    graph->setName(newLabel);
                    plot->legend->setVisible(true);
                    plot->replot();

                    // بروزرسانی internalGraphs همزمان
                    for (GraphData& g : internalGraphs) {
                        if (g.name == currentLabel) {
                            g.name = newLabel;
                            break;
                        }
                    }
                }
                break; // فقط یک نمودار باید هدف باشد
            }
        }

        // اگه هیچ نموداری کلیک نشد، کاری نکن
        if (!foundGraph) {
            qDebug() << "کلیک راست روی فضای خالی";
        }
    }
    });

    });
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