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
#include <windows.h>
#include <commdlg.h>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

#include "../components/ResistorComponent.h"
#include "../components/VoltageComponent.h"
#include "../components/CapacitorComponent.h"
#include "../components/InductorComponent.h"
#include "../components/GND.h"
#include "../components/CurrentComponent.h"
#include "../components/BlackBoxComponent.h"



#include "componentpropertydialog.h"
#include "MainWindow.h"
#include "simulationdialog.h"
#include "ThevenianNortonComponent.h"
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
    }else if(componentType == "current") {
        component = new CurrentComponent(this);
    }else if(componentType=="blackbox") {
        component=new BlackBoxComponent(this);
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
        }
        else {
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
    // if(event->key() == Qt::Key_N) {
    //     WirePropertyDialog dialog(this);
    //     if (dialog.exec() == QDialog::Accepted) {
    //         dialog.setupUI();
    //     }
    // }
    if(event->key() == Qt::Key_S) {
        handleAnalysis();
        // setComponentsWires();
        // netList = new NetListHandler();
        // netList->setNetList(createNetList(allComponents));
        // for(int i=0 ; i<allComponents.size() ; i++) {
        //     std::cout<<allComponents[i]->getComponentName().toStdString()<<" "<<
        //         allComponents[i]->getNodeName1().toStdString()<<" "<<
        //         allComponents[i]->getNodeName2().toStdString()<<" "<<
        //         allComponents[i]->getComponentValue().toStdString()<<std::endl;
        // }
        // //netList=nullptr;
        // SimulationDialog dialog(this);
        // if(isVProbe) {
        //     dialog.setTranVariables(variables);
        //     dialog.setPhaseVariables(variables);
        //     dialog.setACVariables(variables);
        // }
        // if (dialog.exec() == QDialog::Accepted) {
        //     // پردازش نوع شبیه‌سازی انتخاب شده
        //     switch(dialog.getSimulationType()) {
        //         case SimulationDialog::TRANSIENT:
        //             netList->transientHandler(dialog.getMaxTimestep(),dialog.getStopTime(),dialog.getStartSaveTime()
        //                 ,dialog.getTranVariables());
        //         //     qDebug() << "Transient :";
        //         // qDebug() << "stop time:" << dialog.getStopTime();
        //         // qDebug() << "time to start saving data:" << dialog.getStartSaveTime();
        //         // qDebug() << "max time step:" << dialog.getMaxTimestep();
        //         break;
        //
        //         case SimulationDialog::AC_ANALYSIS:
        //         //     qDebug() << "شبیه‌سازی AC Analysis با پارامترها:";
        //         // qDebug() << "نوع سوئیپ:" << dialog.getACSweepType();
        //         // qDebug() << "تعداد نقاط:" << dialog.getACNumPoints();
        //         // qDebug() << "فرکانس شروع:" << dialog.getACStartFreq();
        //         // qDebug() << "فرکانس پایان:" << dialog.getACEndFreq();
        //             netList->acSweepHandler(dialog.getACStartFreq(),dialog.getACEndFreq(),
        //                 dialog.getACNumPoints(),dialog.getACSweepType(),dialog.getACVariables(),dialog.getACOutputType());
        //
        //         break;
        //
        //         case SimulationDialog::PHASE_SWEEP:
        //             netList->phaseSweepHandler(dialog.getPhaseBaseFreq(),dialog.getStartPhase(),dialog.getEndPhase(),dialog.getPhaseNumPoints(),dialog.getPhaseVariables());
        //         // qDebug() << "phase:";
        //         // qDebug() << "n" << dialog.getPhaseNumPoints();
        //         // qDebug() << "start :" << dialog.getStartPhase();
        //         // qDebug() << " end:" << dialog.getEndPhase();
        //         break;
        //
        //         case SimulationDialog::DC_SWEEP:
        //             qDebug() << "شبیه‌سازی DC Sweep با پارامترها:";
        //         qDebug() << "نام منبع:" << dialog.getDCSourceName();
        //         qDebug() << "نوع سوئیپ:" << dialog.getDCSweepType();
        //         qDebug() << "مقدار شروع:" << dialog.getDCStartValue();
        //         qDebug() << "مقدار پایان:" << dialog.getDCStopValue();
        //         qDebug() << "افزایش:" << dialog.getDCIncrement();
        //         break;
        //
        //         // سایر انواع شبیه‌سازی
        //         case SimulationDialog::NOISE:
        //         case SimulationDialog::DC_TRANSFER:
        //         case SimulationDialog::DC_OP_PRT:
        //             qDebug() << "این نوع شبیه‌سازی هنوز پیاده‌سازی نشده است";
        //         break;
        //     }
        // }
    }
        if(event->key() == Qt:: Key_P) {
            //handleProbeAction();
            // if(!isVProbe) {
            //     isVProbe=true;
            //     scene()->addItem(variablesLabel);
            // }
            // else if(isVProbe) {
            //     isVProbe=false;
            //     scene()->removeItem(variablesLabel);
            // }
        }
    //if(event->key() == Qt::Key_N && !isCreatingUnipolar) {
        // setComponentsWires();
        // createUnipolar();
    //}
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)&& isCreatingUnipolar) {
        isCreatingUnipolar=false;
        saveNetList(unipolarInfo);
        scene()->removeItem(variablesLabel);
    }
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)&& isCreatingTNSubCircuit) {
        isCreatingTNSubCircuit=false;
        handleSubCircuitAnalysis();
        saveNetList(TNInfo);
        scene()->removeItem(variablesLabel);
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
    if(isCreatingTNSubCircuit && event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem*> items = scene()->items(scenePos);

        static QSet<QString> addedComponents; // برای پیگیری المان‌های اضافه شده

        for (QGraphicsItem* item : items) {
            if (dynamic_cast<WireComponent*>(item)) {
                WireComponent* wire = dynamic_cast<WireComponent*>(item);
                QString wireVar =  wire->getWireName() ;

                if (addedComponents.contains(wireVar)) {
                    TNInfo.pop_back();
                    variables.replace(wireVar + " ", "");
                    addedComponents.remove(wireVar);
                } else {
                    TNInfo.push_back(wireVar.toStdString());
                    variables += wireVar + " ";
                    addedComponents.insert(wireVar);
                }
                setupProbe();
            }
        }
    }

    if(isCreatingUnipolar && event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QList<QGraphicsItem*> items = scene()->items(scenePos);

        static QSet<QString> addedComponents; // برای پیگیری المان‌های اضافه شده

        for (QGraphicsItem* item : items) {
            if (dynamic_cast<WireComponent*>(item)) {
                WireComponent* wire = dynamic_cast<WireComponent*>(item);
                QString wireVar =  wire->getWireName() ;

                if (addedComponents.contains(wireVar)) {
                    unipolarInfo.pop_back();
                    variables.replace(wireVar + " ", "");
                    addedComponents.remove(wireVar);
                } else {
                    unipolarInfo.push_back(wireVar.toStdString());
                    variables += wireVar + " ";
                    addedComponents.insert(wireVar);
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
    std::vector<std::string> unipolarNetList;
    for(int i=0 ; i<v.size() ; i++) {

        auto* blackbox = dynamic_cast<BlackBoxComponent*>(v[i]);
        auto* TNSub = dynamic_cast<ThevenianNortonComponent*>(v[i]);

        if (blackbox) {
            unipolarNetList=blackbox->getNetList();
            std::string n1=v[i]->getNodeName1().toStdString();
            std::string n2=v[i]->getNodeName2().toStdString();
            unipolarNetList=blackbox->replaceCircuitNodes(unipolarNetList,n1,n2);
            for(const auto & line : unipolarNetList) {
                netList.push_back(line);
            }
        }
        if(TNSub) {
            unipolarNetList=TNSub->getNetList();
            std::string n1=v[i]->getNodeName1().toStdString();
            std::string n2=v[i]->getNodeName2().toStdString();
            unipolarNetList=TNSub->replaceCircuitNodes(unipolarNetList,n1,n2);
            for(const auto & line : unipolarNetList) {
                netList.push_back(line);
            }
        }
        if(!blackbox) {
            QString temp="";
            temp+=v[i]->getComponentName();temp+=" ";
            temp+=v[i]->getNodeName1();temp+=" ";
            temp+=v[i]->getNodeName2();temp+=" ";
            temp+=v[i]->getComponentValue();
            netList.push_back(temp.toStdString());
        }
    }
    return netList;
}


void ComponentView::saveNetList(std::vector<std::string> netList) {
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Netlist File");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    //dialog.setNameFilter("Text Files (*.txt);;All Files (*)");
    dialog.setNameFilter("Text Files (*.bin);;All Files (*)");
    dialog.setDefaultSuffix("bin");

    //dialog.setDefaultSuffix("txt");
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (dialog.exec() == QDialog::Accepted) {
        QString filePath = dialog.selectedFiles().first();
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const auto& line : netList) {
                out << QString::fromStdString(line) << "\n";
            }
            file.close();
            QMessageBox::information(this, "Success", "File saved successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Could not open file for writing");
        }
    }
}
void ComponentView::chooseThevenin(){loadTNSubCircuitNetList(true);}
void ComponentView::chooseNorton(){loadTNSubCircuitNetList(false);}

std::vector<std::string> ComponentView::loadUnipolarNetList() {
    std::vector<std::string> netList;

    QFileDialog dialog(this);
    dialog.setWindowTitle("Load Netlist File");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    //dialog.setNameFilter("Text Files (*.txt);;All Files (*)");
    dialog.setNameFilter("Text Files (*.bin);;All Files (*)");

    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (dialog.exec() == QDialog::Accepted) {
        QString filePath = dialog.selectedFiles().first();
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (!line.isEmpty()) {
                    netList.push_back(line.toStdString());
                }
            }
            file.close();
            QMessageBox::information(this, "Success", "File loaded successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Could not open file for reading");
        }
    }
    GraphicComponent* component = nullptr;
    component = new BlackBoxComponent(this);
    auto* blackbox = dynamic_cast<BlackBoxComponent*>(component);
    if (blackbox) {
        blackbox->handleUnipolarNetList(netList);
    }
    placing = true;
    if (component) {
        currentComponent = component->create(scene());
        allComponents.append(component);
        currentComponent->setFlag(QGraphicsItem::ItemIsMovable, true);
        QRectF rect = currentComponent->boundingRect();
        currentComponent->setTransformOriginPoint(rect.width()/2, rect.height()/2);
        currentComponent->setPos(0, 0);
        component->updateLabelPosition();
    }
    this->setFocus();
    // for(int i=0 ; i<netList.size() ; i++) {
    //     std::cout<<netList[i]<<std::endl;
    // }

    return netList;
}
std::vector<std::string> ComponentView::loadTNSubCircuitNetList(bool isThevenin) {
    std::vector<std::string> netList;

    QFileDialog dialog(this);
    dialog.setWindowTitle("Load Netlist File");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    //dialog.setNameFilter("Text Files (*.txt);;All Files (*)");
    dialog.setNameFilter("Text Files (*.bin);;All Files (*)");

    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (dialog.exec() == QDialog::Accepted) {
        QString filePath = dialog.selectedFiles().first();
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (!line.isEmpty()) {
                    netList.push_back(line.toStdString());
                }
            }
            file.close();
            QMessageBox::information(this, "Success", "File loaded successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Could not open file for reading");
        }
    }

    GraphicComponent* component = nullptr;
    component = new ThevenianNortonComponent(this);
    auto* blackbox = dynamic_cast<ThevenianNortonComponent*>(component);
    if (blackbox) {
        if(!isThevenin) {
        blackbox->setIsThevenian(false);
        }
        blackbox->handleUnipolarNetList(netList);
    }
    placing = true;
    if (component) {
        currentComponent = component->create(scene());
        allComponents.append(component);
        currentComponent->setFlag(QGraphicsItem::ItemIsMovable, true);
        QRectF rect = currentComponent->boundingRect();
        currentComponent->setTransformOriginPoint(rect.width()/2, rect.height()/2);
        currentComponent->setPos(0, 0);
        component->updateLabelPosition();
    }
    this->setFocus();
    // for(int i=0 ; i<netList.size() ; i++) {
    //     std::cout<<netList[i]<<std::endl;
    // }

    return netList;
}
void ComponentView::createUnipolar() {
    setComponentsWires();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Confirm Create Unipolar",
        "Are you sure you want to make this circuit an Unipolar?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        // اگر کاربر Yes را انتخاب کرد
        QMessageBox::information(
            this,
            "Important Notice",
            "Please choose just two nodes and then press Enter to sava."
        );
        unipolarInfo=createNetList(allComponents);
        isCreatingUnipolar=true;
        scene()->addItem(variablesLabel);
    } else {
        // اگر کاربر No را انتخاب کرد
        qDebug() << "Save operation canceled by user";
    }
}
void ComponentView::createTNSubCircuit() {
    setComponentsWires();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Confirm Create Sub Circuit",
        "Are you sure you want to make this circuit an Thevenin or Norton sub circuit?"
        "\nNOTE: the sub circuit should be LTI!",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        // اگر کاربر Yes را انتخاب کرد
        QMessageBox::information(
            this,
            "Important Notice",
            "Please choose just two nodes and then press Enter to sava."
        );
        TNInfo=createNetList(allComponents);
        isCreatingTNSubCircuit=true;
        scene()->addItem(variablesLabel);
    } else {
        // اگر کاربر No را انتخاب کرد
        qDebug() << "Save operation canceled by user";
    }
}
void ComponentView::setupProbe() {
    variablesLabel->setPlainText(variables); // متن اولیه
    variablesLabel->setDefaultTextColor(Qt::black); // رنگ متن
    QFont font("Arial", 12); // فونت Arial با سایز 10 و حالت Bold
    variablesLabel->setFont(font);
    variablesLabel->setZValue(20);
    variablesLabel->setPos(80,500);
}
void ComponentView::handleSubCircuitAnalysis() {
    std::vector<std::string> net;
    std::string refNode=TNInfo.back();
    std::string posNode=TNInfo[TNInfo.size()-2];

    for(int i=0 ; i<TNInfo.size()-2 ; i++) {
        net.push_back(TNInfo[i]);
    }

    bool hasGround = false;

    // مرحله 1: بررسی وجود 0 در کلمه دوم یا سوم هر خط
    for (const auto& line : net) {
        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
        if (words.size() >= 3) {
            if (words[1] == "0" || words[2] == "0") {
                hasGround = true;
                break;
            }
        }
    }
    if (!hasGround) {
        for (auto& line : net) {
            std::istringstream iss(line);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
            }
            for (size_t i = 1; i < words.size(); i++) {
                if (words[i] == refNode) {
                    words[i] = "0";
                }
            }
            line.clear();
            for (const auto& w : words) {
                if (!line.empty()) line += " ";
                line += w;
            }
        }
    }
    if(hasGround) {
        net.push_back("Rt "+posNode+" "+refNode+" 100G");
    }
    else if(!hasGround) {
        net.push_back("Rt "+posNode+" 0"+" 100G");
    }

    netList = new NetListHandler();
    netList->setNetList(net);
    std::unordered_map<std::string, double> res = netList->dcHandler();

    for (const auto& pair : res) {
            if(pair.first==posNode) {
                if(!hasGround) {
                    TNInfo.push_back(std::to_string(pair.second));
                } else {
                    for (const auto& pair2 : res) {
                        if(pair2.first==refNode) {
                            TNInfo.push_back(std::to_string(pair.second-pair2.second));
                        }
                    }
                }
        }
    }

    net.pop_back();
    if(hasGround) {
        net.push_back("Vt "+posNode+" "+refNode+" 1n");
    }
    else if(!hasGround) {
        net.push_back("Vt "+posNode+" 0"+" 1n");
    }
    for(int i = 0; i < net.size(); i++) {
        std::cout << net[i] << std::endl;
    }

    if (netList) {
        delete netList;
        netList = nullptr;
    }
    netList = new NetListHandler();
    netList->setNetList(net);
    std::unordered_map<std::string, double> res2 = netList->dcHandler();

    for (const auto& pair : res2) {
        if(pair.first=="I(Vt)") {
                TNInfo.push_back(std::to_string(pair.second));
            }
    }
    for(int i = 0; i < TNInfo.size(); i++) {
        std::cout << TNInfo[i] << std::endl;
    }
    // for(int i=0 ; i<allComponents.size() ; i++) {
    //     std::cout<<allComponents[i]->getComponentName().toStdString()<<" "<<
    //         allComponents[i]->getNodeName1().toStdString()<<" "<<
    //         allComponents[i]->getNodeName2().toStdString()<<" "<<
    //         allComponents[i]->getComponentValue().toStdString()<<std::endl;
    // }


    // std::cout<<"dc res:\n";
    //  for (const auto& pair : res) {
    //      if (pair.first.find("I(") == 0) { // اگر جریان منبع ولتاژ است
    //          std::cout << "Current through " << pair.first << ": " << pair.second << " A" << std::endl;
    //      } else { // اگر ولتاژ گره است
    //          std::cout << "Voltage at node " << pair.first << ": " << pair.second << " V" << std::endl;
    //      }
    //  }

}
void ComponentView::handleAnalysis() {
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
void ComponentView::handleProbeAction() {
    if(!isVProbe) {
        isVProbe=true;
        scene()->addItem(variablesLabel);
        QMessageBox::information(this,"Probe Status","Probe On");
    }
    else if(isVProbe) {
        isVProbe=false;
        scene()->removeItem(variablesLabel);
        QMessageBox::information(this,"Probe Status","Probe Off");
    }
}

void ComponentView::resetComponentView() {
    // پاکسازی تمام کامپوننت‌ها و لیبل‌های آنها
    for (GraphicComponent* comp : allComponents) {
        if (comp) {
            // پاکسازی لیبل‌های کامپوننت
            if (comp->getValueLabel() && comp->getValueLabel()->scene()) {
                scene()->removeItem(comp->getValueLabel());
            }
            if (comp->getNameLabel() && comp->getNameLabel()->scene()) {
                scene()->removeItem(comp->getNameLabel());
            }

            // پاکسازی آیتم گرافیکی اصلی
            if (comp->getGraphicItem() && comp->getGraphicItem()->scene()) {
                scene()->removeItem(comp->getGraphicItem());
            }

            delete comp;
        }
    }
    allComponents.clear();

    // پاکسازی تمام وایرها و لیبل‌های آنها
    for (WireComponent* wire : wires) {
        if (wire) {
            // پاکسازی لیبل وایر
            if (wire->getNodeLabel() && wire->getNodeLabel()->scene()) {
                scene()->removeItem(wire->getNodeLabel());
            }

            // پاکسازی خود وایر
            if (wire->scene()) {
                scene()->removeItem(wire);
            }

            delete wire;
        }
    }
    wires.clear();
    QList<QGraphicsItem*> allItems = scene()->items();
    for (QGraphicsItem* item : allItems) {
        if (item->data(0).canConvert<GND*>()) {
            GND* gnd = item->data(0).value<GND*>();
            if (gnd) {
                if (item->scene()) {
                    scene()->removeItem(item);
                }
                delete gnd;
            }
        }
    }
    // // پاکسازی crosshairs
    // if (crosshairV && crosshairV->scene()) {
    //     scene()->removeItem(crosshairV);
    //     delete crosshairV;
    //     crosshairV = nullptr;
    // }
    //
    // if (crosshairH && crosshairH->scene()) {
    //     scene()->removeItem(crosshairH);
    //     delete crosshairH;
    //     crosshairH = nullptr;
    // }

    // پاکسازی current items
    if (currentComponent) {
        // پاکسازی لیبل‌های currentComponent
        GraphicComponent* currentComp = currentComponent->data(0).value<GraphicComponent*>();
        if (currentComp) {
            if (currentComp->getValueLabel() && currentComp->getValueLabel()->scene()) {
                scene()->removeItem(currentComp->getValueLabel());
            }
            if (currentComp->getNameLabel() && currentComp->getNameLabel()->scene()) {
                scene()->removeItem(currentComp->getNameLabel());
            }
        }

        if (currentComponent->scene()) {
            scene()->removeItem(currentComponent);
        }
        delete currentComponent;
        currentComponent = nullptr;
    }

    if (currentWire) {
        if (currentWire->getNodeLabel() && currentWire->getNodeLabel()->scene()) {
            scene()->removeItem(currentWire->getNodeLabel());
        }

        if (currentWire->scene()) {
            scene()->removeItem(currentWire);
        }
        delete currentWire;
        currentWire = nullptr;
    }

    // پاکسازی probe items
    if (variablesLabel && variablesLabel->scene()) {
        scene()->removeItem(variablesLabel);
    }

    // ریست متغیرهای حالت
    placing = false;
    wiring = false;
    isVProbe = false;
    isCreatingUnipolar = false;
    variables.clear();
    unipolarInfo.clear();
    TNInfo.clear();
    clickedPoints.clear();
    lastHoveredItem = nullptr;

    // ریست netList
    if (netList) {
        delete netList;
        netList = nullptr;
    }

    // پاکسازی کامل صحنه (به عنوان آخرین مرحله)
    //scene()->clear();

    // ایجاد مجدد items ضروری
    variablesLabel = new QGraphicsTextItem();
    variablesLabel->setZValue(20);
}