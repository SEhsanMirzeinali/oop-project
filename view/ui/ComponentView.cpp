#include "ComponentView.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <ranges>

#include "../components/ResistorComponent.h"
#include "../components/VoltageComponent.h"
#include "../components/CapacitorComponent.h"
#include "../components/InductorComponent.h"
#include "../components/GND.h"

#include "componentpropertydialog.h"
#include "MainWindow.h"
#include "simulationdialog.h"
#include "VoltageSourceDialog.h"
#include "../wire/WirePropertyDialog.h"

ComponentView::ComponentView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent), currentComponent(nullptr), placing(false),
      wiring(false), currentWire(nullptr), crosshairV(nullptr), crosshairH(nullptr) {
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setFocusPolicy(Qt::StrongFocus);
    componentCursor=Qt::IBeamCursor;
    viewport()->installEventFilter(this);

}


bool ComponentView::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF scenePos = mapToScene(mouseEvent->pos());
        QList<QGraphicsItem*> items = scene()->items(scenePos);

        bool found = false;
        for (QGraphicsItem* item : items) {
            if (isComponent(item)||isWireLabel(item)) {
                updateHoverState(item, scenePos);
                found = true;
                break;
            }
            if(isWire(item)) {
                viewport()->setCursor(Qt::PointingHandCursor);
                found = true;
                break;
            }
        }

        if (!found) {
            updateHoverState(nullptr, scenePos);
        }
    }
    return QGraphicsView::eventFilter(watched, event);
}

bool ComponentView::isComponent(QGraphicsItem* item) const {
    return item && item->data(0).canConvert<GraphicComponent*>();
}
bool ComponentView::isWireLabel(QGraphicsItem* item) const {
    return dynamic_cast<QGraphicsTextItem*>(item) && dynamic_cast<WireComponent*>(item->parentItem());
}
bool ComponentView::isWire(QGraphicsItem* item) const {
    return dynamic_cast<WireComponent*>(item);
}

void ComponentView::updateHoverState(QGraphicsItem* item, const QPointF& scenePos) {
    //if (item != lastHoveredItem) {
        if (lastHoveredItem) {
            // ریست حالت قبلی
        }

        lastHoveredItem = item;
        viewport()->setCursor(item ? Qt::IBeamCursor : Qt::ArrowCursor);
    //}
}

void ComponentView::showComponentPropertiesDialog(QGraphicsItem* item) {
    ComponentPropertyDialog dialog;
    GraphicComponent* comp = item->data(0).value<GraphicComponent*>();
    dialog.setDefaultText(comp->getComponentValue(),comp->getComponentName());

dialog.setupUI();
    if (comp) {
        if (dialog.exec() == QDialog::Accepted) {
            comp->setComponentValue(dialog.getLine1());
            comp->setComponentName(dialog.getLine2());
        }
    }
}



void ComponentView::startWiring() {
    wiring = true;
    placing = false;
    viewport()->setCursor(Qt::BlankCursor);

    QPen crosshairPen(Qt::DashLine);
    crosshairPen.setColor(Qt::black);

    crosshairV = scene()->addLine(0, -10000, 0, 10000, crosshairPen);
    crosshairH = scene()->addLine(-10000, 0, 10000, 0, crosshairPen);

    crosshairV->setZValue(10);
    crosshairH->setZValue(10);

    currentWire = new WireComponent(this);
    scene()->addItem(currentWire);
    //scene()->addItem(currentWire->getNodeLabel());
    clickedPoints.clear();

    this->setFocus();
}

void ComponentView::startPlacing(const QString& type) {
    wiring = false;
    placing = true;
    componentType = type;

    GND* ground=nullptr;
    GraphicComponent* component = nullptr;
    if (componentType == "resistor") {
        component = new ResistorComponent(this);
    } else if (componentType == "voltage") {
        component = new VoltageComponent(this);
    } else if(componentType == "capacitor") {
        component = new CapacitorComponent(this);
    } else if(componentType == "inductor") {
        component = new InductorComponent(this);
    }else if(componentType=="ground") {
        ground=new GND(this);
    }

    if (component) {
        currentComponent = component->create(scene());
        allComponents.append(component);
        currentComponent->setFlag(QGraphicsItem::ItemIsMovable, true);
        QRectF rect = currentComponent->boundingRect();
        currentComponent->setTransformOriginPoint(rect.width()/2, rect.height()/2);
        currentComponent->setPos(0, 0);
        component->updateLabelPosition();
    }if(ground) {

        currentComponent = ground->create(scene());
        currentComponent->setFlag(QGraphicsItem::ItemIsMovable, true);
        QRectF rect = currentComponent->boundingRect();
        currentComponent->setTransformOriginPoint(rect.width()/2, rect.height()/2);
        currentComponent->setPos(0, 0);
    }

    //std::cout << "size: " << allComponents.size() << std::endl;
    this->setFocus();
}

void ComponentView::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const ViewportAnchor anchor = transformationAnchor();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        double angle = event->angleDelta().y();
        double factor = (angle > 0) ? 1.1 : 0.9;
        scale(factor, factor);
        setTransformationAnchor(anchor);
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void ComponentView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_R) {
        if (event->modifiers() & Qt::ControlModifier) {
            if (placing && currentComponent) {
                int angel = qRound((currentComponent->rotation() + 90)/90 ) *90;
                currentComponent->setRotation(angel);
                GraphicComponent* comp = currentComponent->data(0).value<GraphicComponent*>();
                comp->updateLabelPosition(angel);
                std::cout<<"angel: "<<angel<<std::endl;
            }
        } else {
            if (!placing) {
                startPlacing("resistor");
            }
        }
    }
    if (event->key() == Qt::Key_V) {
        if (!placing) {
            startPlacing("voltage");
        }
    }
    if (event->key() == Qt::Key_C) {
        if (!placing) {
            startPlacing("capacitor");
        }
    }
    if (event->key() == Qt::Key_L) {
        if (!placing) {
            startPlacing("inductor");
        }
    }
    if (event->key() == Qt::Key_Escape && wiring) {
        finishCurrentWire(false);
        wiring = false;
        event->accept();
        return;
    }
    if(event->key() == Qt::Key_N) {
        WirePropertyDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            dialog.setupUI();
        }
    }
    if(event->key() == Qt::Key_S) {
        setComponentsWires();
        netList = new NetListHandler();
        netList->setNetList(createNetList(allComponents));
        for(int i=0 ; i<allComponents.size() ; i++) {
            std::cout<<allComponents[i]->getComponentName().toStdString()<<" "<<
                allComponents[i]->getNodeName1().toStdString()<<" "<<
                allComponents[i]->getNodeName2().toStdString()<<" "<<
                allComponents[i]->getComponentValue().toStdString()<<std::endl;
        }
        //netList=nullptr;
        SimulationDialog dialog(this);
        if(isVProbe) {
            dialog.setTranVariables(variables);
            dialog.setPhaseVariables(variables);
            dialog.setACVariables(variables);
        }
        if (dialog.exec() == QDialog::Accepted) {
            // پردازش نوع شبیه‌سازی انتخاب شده
            switch(dialog.getSimulationType()) {
                case SimulationDialog::TRANSIENT:
                    netList->transientHandler(dialog.getMaxTimestep(),dialog.getStopTime(),dialog.getStartSaveTime()
                        ,dialog.getTranVariables());
                //     qDebug() << "Transient :";
                // qDebug() << "stop time:" << dialog.getStopTime();
                // qDebug() << "time to start saving data:" << dialog.getStartSaveTime();
                // qDebug() << "max time step:" << dialog.getMaxTimestep();
                break;

                case SimulationDialog::AC_ANALYSIS:
                //     qDebug() << "شبیه‌سازی AC Analysis با پارامترها:";
                // qDebug() << "نوع سوئیپ:" << dialog.getACSweepType();
                // qDebug() << "تعداد نقاط:" << dialog.getACNumPoints();
                // qDebug() << "فرکانس شروع:" << dialog.getACStartFreq();
                // qDebug() << "فرکانس پایان:" << dialog.getACEndFreq();
                    netList->acSweepHandler(dialog.getACStartFreq(),dialog.getACEndFreq(),
                        dialog.getACNumPoints(),dialog.getACSweepType(),dialog.getACVariables(),dialog.getACOutputType());
                qDebug() << "type:"<<dialog.getACOutputType();

                break;

                case SimulationDialog::PHASE_SWEEP:
                    netList->phaseSweepHandler(dialog.getPhaseBaseFreq(),dialog.getStartPhase(),dialog.getEndPhase(),dialog.getPhaseNumPoints(),dialog.getPhaseVariables());
                // qDebug() << "phase:";
                // qDebug() << "n" << dialog.getPhaseNumPoints();
                // qDebug() << "start :" << dialog.getStartPhase();
                // qDebug() << " end:" << dialog.getEndPhase();
                break;

                case SimulationDialog::DC_SWEEP:
                    qDebug() << "شبیه‌سازی DC Sweep با پارامترها:";
                qDebug() << "نام منبع:" << dialog.getDCSourceName();
                qDebug() << "نوع سوئیپ:" << dialog.getDCSweepType();
                qDebug() << "مقدار شروع:" << dialog.getDCStartValue();
                qDebug() << "مقدار پایان:" << dialog.getDCStopValue();
                qDebug() << "افزایش:" << dialog.getDCIncrement();
                break;

                // سایر انواع شبیه‌سازی
                case SimulationDialog::NOISE:
                case SimulationDialog::DC_TRANSFER:
                case SimulationDialog::DC_OP_PRT:
                    qDebug() << "این نوع شبیه‌سازی هنوز پیاده‌سازی نشده است";
                break;
            }
        }
    }
        if(event->key() == Qt:: Key_P) {
            if(!isVProbe) {
                isVProbe=true;
                scene()->addItem(variablesLabel);
            }
            else if(isVProbe) {
                isVProbe=false;
                scene()->removeItem(variablesLabel);
            }
        }
    QGraphicsView::keyPressEvent(event);
}

void ComponentView::mouseMoveEvent(QMouseEvent* event) {
    QPointF scenePos = mapToScene(event->pos());
    QPointF snapped = snapToGrid(scenePos);
    
    if (placing && currentComponent) {
        QRectF rect = currentComponent->boundingRect();
        QPointF centerPos = scenePos - QPointF(rect.width() / 2, rect.height() / 2);
        QPointF snappedPos = snapToGrid(centerPos);
        currentComponent->setPos(snappedPos);
        
        GraphicComponent* comp = currentComponent->data(0).value<GraphicComponent*>();
        GND* gnd =currentComponent->data(0).value<GND*>();
        if (comp) {
            int angel =currentComponent->rotation();

            comp->updatePorts();
            comp->updateLabelPosition(angel);
            drawPortMarkers(comp);
            //comp->drawBoundingBox(scene());
        }
        if(gnd) {
            gnd->updatePorts();
        }
    }
    
    if (wiring && currentWire) {
        updateCrosshair(snapped);
        
        if (!clickedPoints.isEmpty()) {
            QPainterPath mainPath;
            QList<QList<QPointF>> segments;
            QList<QPointF> currentSegment;

            currentSegment.append(clickedPoints.first());
            for (int i = 1; i < clickedPoints.size(); ++i) {
                if (currentWire->shouldConnect(clickedPoints[i-1], clickedPoints[i])) {
                    currentSegment.append(clickedPoints[i]);
                } else {
                    segments.append(currentSegment);
                    currentSegment.clear();
                    currentSegment.append(clickedPoints[i]);
                }
            }
            segments.append(currentSegment);

            for (const auto& segment : segments) {
                if (!segment.isEmpty()) {
                    mainPath.moveTo(segment.first());
                    for (int i = 1; i < segment.size(); ++i) {
                        mainPath.lineTo(segment[i]);
                    }
                }
            }

            if (!(event->modifiers() & Qt::ControlModifier)) {
                QPointF last = clickedPoints.last();
                QPointF tempPoint = snapped;

                if (qAbs(snapped.x() - last.x()) > qAbs(snapped.y() - last.y())) {
                    tempPoint.setY(last.y());
                } else {
                    tempPoint.setX(last.x());
                }

                mainPath.moveTo(last);
                mainPath.lineTo(tempPoint);
            }

            currentWire->setPath(mainPath);
        }
    }


    QGraphicsView::mouseMoveEvent(event);
}

void ComponentView::mousePressEvent(QMouseEvent* event) {
    QPointF scenePos = mapToScene(event->pos());
    QPointF snapped = snapToGrid(scenePos);

    if(isVProbe && event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem*> items = scene()->items(scenePos);

        static QSet<QString> addedComponents; // برای پیگیری المان‌های اضافه شده

        for (QGraphicsItem* item : items) {
            if (dynamic_cast<WireComponent*>(item)) {
                WireComponent* wire = dynamic_cast<WireComponent*>(item);
                QString wireVar = "V(" + wire->getWireName() + ")";

                if (addedComponents.contains(wireVar)) {
                    variables.replace(wireVar + " ", "");
                    addedComponents.remove(wireVar);
                } else {
                    variables += wireVar + " ";
                    addedComponents.insert(wireVar);
                }
                setupProbe();
            }
            else if (item->data(0).canConvert<GraphicComponent*>()) {
                GraphicComponent* comp = item->data(0).value<GraphicComponent*>();
                QString compVar = "I(" + comp->getComponentName() + ")";

                if (addedComponents.contains(compVar)) {
                    variables.replace(compVar + " ", "");
                    addedComponents.remove(compVar);
                } else {
                    variables += compVar + " ";
                    addedComponents.insert(compVar);
                }
                setupProbe();
            }
        }
    }

    if (placing && event->button() == Qt::LeftButton && currentComponent) {
        setGndWire(currentComponent);
        currentComponent = nullptr;
        placing = false;
    }

    if (wiring && event->button() == Qt::LeftButton) {
        WireComponent* existingWire = findWireContainingPoint(snapped);
        GraphicComponent* clickedComponent = findComponentAtPoint(snapped);

        if (existingWire) {
            QGraphicsRectItem* square = new QGraphicsRectItem(snapped.x() - 4, snapped.y() - 4, 8, 8);
            square->setBrush(Qt::blue);
            square->setPen(Qt::NoPen);
            square->setZValue(10);
            scene()->addItem(square);

            if (clickedPoints.isEmpty()) {
                currentWire = existingWire;
                clickedPoints = currentWire->getPoints();
                clickedPoints << snapped;
                currentWire->clearPoints();
                for (const auto& point : clickedPoints) {
                    currentWire->addPoint(point);
                }
                event->accept();
                QGraphicsView::mousePressEvent(event);
                return;
            }
            else if (currentWire && !clickedPoints.isEmpty()) {
                currentWire->addPoint(snapped);
                mergeWires(currentWire, existingWire);
                finishCurrentWire(false);
                wiring = false;
                event->accept();
                QGraphicsView::mousePressEvent(event);
                return;
            }
        }
        if (clickedComponent) {
            if (currentWire) {
                if(clickedPoints.isEmpty()) {
                    currentWire->addConnectedComponent(clickedComponent);
                    clickedPoints << snapped;
                    currentWire->clearPoints();
                    for (const auto& point : clickedPoints) {
                        currentWire->addPoint(point);
                    }
                    event->accept();
                    QGraphicsView::mousePressEvent(event);
                    return;
                }

                clickedPoints << snapped;
                currentWire->clearPoints();
                for (const auto& point : clickedPoints) {
                    currentWire->addPoint(point);
                }
                currentWire->setAllPoints();

                currentWire->addConnectedComponent(clickedComponent);
                finishCurrentWire();
                wiring = false;
                event->accept();
                QGraphicsView::mousePressEvent(event);
                return;
            }
        }
        if (currentWire) {
            for (const QPointF& point : currentWire->getAllPoints()) {
                if (point == snapped) {
                    currentWire->addPoint(snapped);
                    currentWire->setAllPoints();
                    finishCurrentWire();
                    wiring = false;
                    event->accept();
                    QGraphicsView::mousePressEvent(event);
                    return;
                }
            }
        }

        if (!clickedPoints.isEmpty() && !(event->modifiers() & Qt::ControlModifier)) {
            QPointF last = clickedPoints.last();
            if (qAbs(snapped.x() - last.x()) > qAbs(snapped.y() - last.y())) {
                snapped.setY(last.y());
            } else {
                snapped.setX(last.x());
            }
            currentWire->setAllPoints();
        }

        if (!currentWire) {
            currentWire = new WireComponent(this);
            scene()->addItem(currentWire);
        }

        clickedPoints << snapped;
        currentWire->addPoint(snapped);
        currentWire->setAllPoints();
    }
    else if (wiring && event->button() == Qt::RightButton) {
        finishCurrentWire();
        wiring = false;
        event->accept();
    }
    if (event->button() == Qt::RightButton) {
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem*> items = scene()->items(scenePos);

        for (QGraphicsItem* item : items) {
            if(dynamic_cast<WireComponent*>(item)) {
                WireComponent* wire = dynamic_cast<WireComponent*>(item);
                wire->updateLabelPosition(snapped);
            }
             if (/*dynamic_cast<WireComponent*>(item) ||*/(dynamic_cast<QGraphicsTextItem*>(item) && dynamic_cast<WireComponent*>(item->parentItem()))) {
                WireComponent* wire =/* dynamic_cast<WireComponent*>(item) ?dynamic_cast<WireComponent*>(item) :*/
                                      dynamic_cast<WireComponent*>(item->parentItem());

                WirePropertyDialog dialog;
                dialog.setWireName(wire->getWireName());

                if (dialog.exec() == QDialog::Accepted) {
                    wire->setWireName(dialog.getWireName());
                }
                event->accept();
                return;
              }
            if (item->data(0).canConvert<GraphicComponent*>()) {
                GraphicComponent* comp = item->data(0).value<GraphicComponent*>();
                VoltageComponent *voltageSource = dynamic_cast<VoltageComponent*>(comp);
                if(!voltageSource) {
                if(item == comp->getValueLabel() || item == comp->getNameLabel() || item == comp->getGraphicItem()) {
                        showComponentPropertiesDialog(item);
                        event->accept();
                        return;
                    }
                }

                if(voltageSource) {
                    if( item == comp->getNameLabel() || item == comp->getGraphicItem()) {
                        showComponentPropertiesDialog(item);
                        event->accept();
                        return;
                    }
                    if(item == comp->getValueLabel()){

                        VoltageSourceDialog dialog;
                        if (dialog.exec() == QDialog::Accepted) {
                            QString selectedFunction = dialog.getSelectedFunction();
                            double dcValue = dialog.getDCValue();
                            double acAmplitude = dialog.getACAmplitude();
                            double acPhase = dialog.getACPhase();
                            QMap<QString, double> params = dialog.getFunctionParameters();

                            if (selectedFunction == "SINE") {
                                double offset = params["Offset"];
                                double  ampl = params["Amplitude"];
                                double frequency = params["Frequency"];
                                double TDelay = params["Tdelay"];
                                double theta = params["Theta"];
                                double phi = params["Phi"];
                                double NCycles = params["Ncycles"];
                                QString value ="SINE "+QString::number(offset)+ " "
                                +QString::number(ampl)+" "+QString::number(frequency)+" "+
                                    QString::number(TDelay)+" "+QString::number(theta)+" "+
                                        QString::number(phi)+" "+QString::number(NCycles);
                                voltageSource->setComponentValue(value);
                            }
                            else if (selectedFunction == "PULSE") {
                                // پردازش برای پالس
                                double vInitial = params["VInitial"];
                                double vOn = params["VOn"];
                                double tDelay = params["Tdelay"];
                                double tRise = params["Trise"];
                                double tFall = params["Tfall"];
                                double tOn = params["Ton"];
                                double period =  params["Period"];
                                double nCycles = params["Ncycles"];
                                QString value ="PULSE "+QString::number(vInitial)+ " "
                                +QString::number(vOn)+" "+QString::number(tDelay)+" "+
                                    QString::number(tRise)+" "+QString::number(tFall)+" "+
                                        QString::number(tOn)+" "+QString::number(period)+
                                            " "+QString::number(nCycles);
                                voltageSource->setComponentValue(value);
                            }
                            else if (selectedFunction == "None") {
                                if(dcValue!=0) {
                                    voltageSource->setComponentValue(QString::number(dcValue));
                                }
                                else if(acAmplitude!= 0 || acPhase!= 0) {
                                    QString value = "AC "+QString::number(acAmplitude)+" "+QString::number(acPhase);
                                    voltageSource->setComponentValue(value);
                                }
                                // پردازش برای سیگنال DC ساده
                                // createDCSource(dcValue);
                            }
                        }
                }
                }
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void ComponentView::drawPortMarkers(GraphicComponent* comp) {
    static QGraphicsEllipseItem* port1Marker = nullptr;
    static QGraphicsEllipseItem* port2Marker = nullptr;

    if (port1Marker) scene()->removeItem(port1Marker);
    if (port2Marker) scene()->removeItem(port2Marker);

    if (!comp) return;

    port1Marker = new QGraphicsEllipseItem(
        comp->getPort1().x() - 3, comp->getPort1().y() - 3, 6, 6);
    port2Marker = new QGraphicsEllipseItem(
        comp->getPort2().x() - 3, comp->getPort2().y() - 3, 6, 6);

    port1Marker->setBrush(Qt::red);
    port2Marker->setBrush(Qt::blue);
    port1Marker->setZValue(20);
    port2Marker->setZValue(20);

    scene()->addItem(port1Marker);
    scene()->addItem(port2Marker);
}

GraphicComponent* ComponentView::findComponentAtPoint(const QPointF& point) {
    for (GraphicComponent* comp : allComponents) {
        if (comp->getPort1() == point || comp->getPort2() == point) {
            return comp;
        }
    }
    return nullptr;
}

void ComponentView::updateCrosshair(const QPointF& pos) {
    if (crosshairV && crosshairH) {
        crosshairV->setLine(pos.x(), -10000, pos.x(), 10000);
        crosshairH->setLine(-10000, pos.y(), 10000, pos.y());
    }
}

QPointF ComponentView::snapToGrid(const QPointF& point) {
    int x = qRound(point.x() / gridSize) * gridSize;
    int y = qRound(point.y() / gridSize) * gridSize;
    return QPointF(x, y);
}

void ComponentView::endWiring() {
    wiring=false;
    currentWire->updatePath();
    currentWire->getNodeLabel()->setPos(200,200);
    //currentWire->updateLabelPosition();
    currentWire = nullptr;
    clickedPoints.clear();
    if (crosshairV) scene()->removeItem(crosshairV);
    if (crosshairH) scene()->removeItem(crosshairH);
    crosshairV = crosshairH = nullptr;
    viewport()->setCursor(Qt::ArrowCursor);

    //setCursor(Qt::ArrowCursor);
}

WireComponent* ComponentView::findWireContainingPoint(const QPointF& point) {
    for (WireComponent* wire : wires) {
        for (const QPointF& wirePoint : wire->getAllPoints()) {
            if (wirePoint == point) {
                return wire;
            }
        }
    }
    return nullptr;
}

void ComponentView::finishCurrentWire(bool addToVector) {
    if (!currentWire) return;

    bool isNewWire = !wires.contains(currentWire);
    //currentWire->updateLabelPosition();

    if (addToVector && !clickedPoints.isEmpty() && isNewWire) {
        wires.append(currentWire);
    } else if (!addToVector && isNewWire) {
        if (currentWire->scene()) {
            scene()->removeItem(currentWire);
        }
        delete currentWire;
    }

    currentWire = nullptr;
    clickedPoints.clear();

    if (crosshairV && crosshairV->scene() == scene()) {
        scene()->removeItem(crosshairV);
    }
    if (crosshairH && crosshairH->scene() == scene()) {
        scene()->removeItem(crosshairH);
    }
    crosshairV = crosshairH = nullptr;
    viewport()->setCursor(Qt::ArrowCursor);

}

void ComponentView::mergeWires(WireComponent* wire1, WireComponent* wire2) {
    WireComponent* mergedWire = new WireComponent(this);
    mergedWire->mergeWire(wire1);
    mergedWire->mergeWire(wire2);

    scene()->addItem(mergedWire);
    wires.append(mergedWire);

    wires.removeOne(wire1);
    wires.removeOne(wire2);

    if (wire1->scene()) {
        wire1->scene()->removeItem(wire1);
    }
    if (wire2->scene()) {
        wire2->scene()->removeItem(wire2);
    }

    wire1->setParent(nullptr);
    wire2->setParent(nullptr);

    wire1->deleteLater();
    wire2->deleteLater();
}
void ComponentView::setGndWire(QGraphicsItem* currentComp) {
     GND* gnd =currentComp->data(0).value<GND*>();
    if (gnd) {
    std::cout<<"exist\n";

        QPointF portLoc=gnd->getPort();
        for(int i=0 ; i<wires.size() ; i++) {
            for(int j=0 ; j<wires[i]->getAllPoints().size() ; j++) {
                if(portLoc == wires[i]->getAllPoints()[j]) {
                    std::cout<<"set "<<wires[i]->getWireName().toStdString()<<" as gnd.\n";
                    wires[i]->setWireName("GND");
                    wires[i]->setAsGnd(true);
                }
            }
        }
    }
}
void ComponentView::setComponentsWires() {
    for(int i=0 ; i<allComponents.size() ; i++) {
        for(int j=0 ; j<wires.size() ; j++) {
            for(int k=0 ; k<wires[j]->getAllPoints().size() ; k++) {
             if(allComponents[i]->getPort1()==wires[j]->getAllPoints()[k]) {
                allComponents[i]->setNodeName1(wires[j]->getWireName());
             }
                if(allComponents[i]->getPort2()==wires[j]->getAllPoints()[k]) {
                    allComponents[i]->setNodeName2(wires[j]->getWireName());
                }
            }
        }
    }
}
std::vector<std::string> ComponentView:: createNetList(QVector<GraphicComponent*> v) {
   std::vector<std::string> netList;
    for(int i=0 ; i<v.size() ; i++) {
        QString temp="";
        temp+=v[i]->getComponentName();temp+=" ";
        temp+=v[i]->getNodeName1();temp+=" ";
        temp+=v[i]->getNodeName2();temp+=" ";
        temp+=v[i]->getComponentValue();
        netList.push_back(temp.toStdString());
    }
    return netList;
}
void ComponentView::setupProbe() {
    variablesLabel->setPlainText(variables); // متن اولیه
    variablesLabel->setDefaultTextColor(Qt::black); // رنگ متن
    QFont font("Arial", 12); // فونت Arial با سایز 10 و حالت Bold
    variablesLabel->setFont(font);
    variablesLabel->setZValue(20);
    variablesLabel->setPos(80,500);
}