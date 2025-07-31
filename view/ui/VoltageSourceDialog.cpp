#include "VoltageSourceDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QDebug>

VoltageSourceDialog::VoltageSourceDialog(QWidget *parent)
    : QDialog(parent), functionGroup(nullptr) {
    setupUI();
}

VoltageSourceDialog::~VoltageSourceDialog() {
    delete functionGroup;
}

QLineEdit* VoltageSourceDialog::createValidatedLineEdit() {
    QLineEdit *lineEdit = new QLineEdit;
    QDoubleValidator *validator = new QDoubleValidator(lineEdit);
    validator->setNotation(QDoubleValidator::StandardNotation);
    lineEdit->setValidator(validator);
    lineEdit->setText("0");
    return lineEdit;
}

void VoltageSourceDialog::setupUI() {
    setWindowTitle("Independent Voltage Source - V1");
    resize(700, 500);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    /* Left Side Content */
    QWidget *leftWidget = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(5, 5, 5, 5);

    // Functions Group
    setupFunctionGroup();
    QGroupBox *functionBox = new QGroupBox("Functions");
    QVBoxLayout *functionLayout = new QVBoxLayout(functionBox);
    for (int i = 0; i < functionGroup->buttons().size(); ++i) {
        functionLayout->addWidget(functionGroup->button(i));
    }
    leftLayout->addWidget(functionBox);

    // Parameters Stack
    setupParameterPages();
    leftLayout->addWidget(parameterStack);
    leftLayout->addStretch();

    /* Right Side Content */
    QWidget *rightWidget = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(5, 5, 5, 5);

    // DC Value
    setupRightSideSection();
    QGroupBox *dcBox = new QGroupBox("DC Value");
    QFormLayout *dcLayout = new QFormLayout(dcBox);
    dcLayout->addRow("DC value [V]:", dcValueEdit);
    rightLayout->addWidget(dcBox);

    // AC Analysis
    QGroupBox *acBox = new QGroupBox("AC Analysis");
    QFormLayout *acLayout = new QFormLayout(acBox);
    acLayout->addRow("Amplitude [V]:", acAmplitudeEdit);
    acLayout->addRow("Phase [deg]:", acPhaseEdit);
    rightLayout->addWidget(acBox);

    // Parasitic Properties
    QGroupBox *parasiticBox = new QGroupBox("Parasitic Properties");
    QFormLayout *parasiticLayout = new QFormLayout(parasiticBox);
    parasiticLayout->addRow("Series R [Ω]:", seriesResistanceEdit);
    parasiticLayout->addRow("Parallel C [F]:", parallelCapacitanceEdit);
    rightLayout->addWidget(parasiticBox);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    QPushButton *okBtn = new QPushButton("OK");
    QPushButton *cancelBtn = new QPushButton("Cancel");
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(okBtn);
    rightLayout->addLayout(buttonLayout);

    // Connections
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(functionGroup, &QButtonGroup::idClicked, this, &VoltageSourceDialog::onFunctionChanged);

    // Add to main layout
    mainLayout->addWidget(leftWidget, 2);
    mainLayout->addWidget(rightWidget, 1);
}

void VoltageSourceDialog::setupFunctionGroup() {
    functionGroup = new QButtonGroup(this);
    functionGroup->setExclusive(true);

    const QStringList functions = {
        "None",
        "PULSE",
        "SINE",
        "EXP",
        "SFFM",
        "PWL"
    };

    for (int i = 0; i < functions.size(); ++i) {
        QRadioButton *radio = new QRadioButton(functions[i]);
        functionGroup->addButton(radio, i);
    }
}

void VoltageSourceDialog::setupParameterPages() {
    parameterStack = new QStackedWidget;
    functionParameters.clear();

    /* None Page */
    QWidget *nonePage = new QWidget;
    parameterStack->addWidget(nonePage);

    /* PULSE Page */
    QWidget *pulsePage = new QWidget;
    QFormLayout *pulseLayout = new QFormLayout(pulsePage);

    QMap<QString, QLineEdit*> pulseParams;
    const QStringList pulseLabels = {
        "Initial Value [V]", "Pulsed Value [V]",
        "Delay Time [s]", "Rise Time [s]",
        "Fall Time [s]", "On Time [s]",
        "Period [s]", "Number of Cycles"
    };

    const QStringList pulseKeys = {"V1", "V2", "Tdelay", "Trise", "Tfall", "Ton", "Period", "Ncycles"};

    for (int i = 0; i < pulseKeys.size(); ++i) {
        pulseParams[pulseKeys[i]] = createValidatedLineEdit();
        pulseLayout->addRow(pulseLabels[i], pulseParams[pulseKeys[i]]);
    }
    functionParameters["PULSE"] = pulseParams;
    parameterStack->addWidget(pulsePage);

    /* SINE Page */
    QWidget *sinePage = new QWidget;
    QFormLayout *sineLayout = new QFormLayout(sinePage);

    QMap<QString, QLineEdit*> sineParams;
    const QStringList sineLabels = {
        "Offset [V]", "Amplitude [V]",
        "Frequency [Hz]", "Delay [s]",
        "Theta [1/s]", "Phi [deg]",
        "Number of Cycles"
    };

    const QStringList sineKeys = {"Offset", "Amplitude", "Frequency", "Tdelay", "Theta", "Phi", "Ncycles"};

    for (int i = 0; i < sineKeys.size(); ++i) {
        sineParams[sineKeys[i]] = createValidatedLineEdit();
        sineLayout->addRow(sineLabels[i], sineParams[sineKeys[i]]);
    }
    functionParameters["SINE"] = sineParams;
    parameterStack->addWidget(sinePage);

    parameterStack->setCurrentIndex(0);
}

void VoltageSourceDialog::setupRightSideSection() {
    dcValueEdit = createValidatedLineEdit();
    acAmplitudeEdit = createValidatedLineEdit();
    acPhaseEdit = createValidatedLineEdit();
    seriesResistanceEdit = createValidatedLineEdit();
    parallelCapacitanceEdit = createValidatedLineEdit();
}

void VoltageSourceDialog::onFunctionChanged(int id) {
    parameterStack->setCurrentIndex(id);
}

QString VoltageSourceDialog::getSelectedFunction() const {
    if (!functionGroup) return "None";

    switch(functionGroup->checkedId()) {
        case 1: return "PULSE";
        case 2: return "SINE";
        case 3: return "EXP";
        case 4: return "SFFM";
        case 5: return "PWL";
        default: return "None";
    }
}

double VoltageSourceDialog::getDCValue() const {
    return dcValueEdit->text().toDouble();
}

QMap<QString, double> VoltageSourceDialog::getFunctionParameters() const {
    QMap<QString, double> params;
    QString func = getSelectedFunction();

    if (functionParameters.contains(func)) {
        for (auto it = functionParameters[func].constBegin(); it != functionParameters[func].constEnd(); ++it) {
            params[it.key()] = it.value()->text().toDouble();
        }
    }

    return params;
}

double VoltageSourceDialog::getACAmplitude() const {
    return acAmplitudeEdit->text().toDouble();
}

double VoltageSourceDialog::getACPhase() const {
    return acPhaseEdit->text().toDouble();
}

double VoltageSourceDialog::getSeriesResistance() const {
    return seriesResistanceEdit->text().toDouble();
}

double VoltageSourceDialog::getParallelCapacitance() const {
    return parallelCapacitanceEdit->text().toDouble();
}