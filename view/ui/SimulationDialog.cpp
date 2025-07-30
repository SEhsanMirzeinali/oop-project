#include "simulationdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFormLayout>
SimulationDialog::SimulationDialog(QWidget *parent) : QDialog(parent),
    stackedWidget(nullptr),
    simTypeGroup(nullptr),
    transientRadio(nullptr),
    acAnalysisRadio(nullptr),
    dcSweepRadio(nullptr),
    noiseRadio(nullptr),
    dcTransferRadio(nullptr),
    dcOpPrtRadio(nullptr),  // تصحیح نام اینجا نیز
    stopTimeEdit(nullptr),
    startSaveEdit(nullptr),
    maxTimestepEdit(nullptr),
    variablesEdit(nullptr),
    syntaxEdit(nullptr),
    acSweepTypeCombo(nullptr),
    acNumPointsEdit(nullptr),
    acStartFreqEdit(nullptr),
    acEndFreqEdit(nullptr),
    dcSourceNameEdit(nullptr),
    dcSweepTypeCombo(nullptr),
    dcStartValueEdit(nullptr),
    dcStopValueEdit(nullptr),
    dcIncrementEdit(nullptr),
    startAtZeroCheck(nullptr),
    stopSteadyCheck(nullptr),
    dontResetCheck(nullptr),
    stepLoadCheck(nullptr),
    skipInitialCheck(nullptr),
    optionsGroup(nullptr),
    parametersGroup(nullptr),
    cancelButton(nullptr),
    okButton(nullptr)
{
    setWindowTitle("Edit Simulation Command");
    setupUI();
}

void SimulationDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create simulation type selector
    createSimulationTypeGroup();
    mainLayout->addWidget(simTypeGroup->button(0)->parentWidget());

    // Create stacked widget for different pages
    stackedWidget = new QStackedWidget;
    createTransientPage();
    createACAnalysisPage();
    createDCSweepPage();
    mainLayout->addWidget(stackedWidget);

    // Create common options
    createOptionsGroup();
    mainLayout->addWidget(optionsGroup);

    // Connect type change to page change
    connect(simTypeGroup, &QButtonGroup::idClicked,
            stackedWidget, &QStackedWidget::setCurrentIndex);

    // Add buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelButton = new QPushButton("Cancel");
    QPushButton *okButton = new QPushButton("OK");
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    mainLayout->addLayout(buttonLayout);

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
}

void SimulationDialog::createSimulationTypeGroup()
{
    QGroupBox *groupBox = new QGroupBox("Simulation Type");
    QHBoxLayout *layout = new QHBoxLayout();

    simTypeGroup = new QButtonGroup(this);

    QStringList types = {"Transient", "AC Analysis", "DC sweep", "Noise", "DC Transfer", "DC op prt"};
    for (int i = 0; i < types.size(); ++i) {
        QRadioButton *radio = new QRadioButton(types[i]);
        simTypeGroup->addButton(radio, i);
        layout->addWidget(radio);
    }
    simTypeGroup->button(0)->setChecked(true);

    groupBox->setLayout(layout);
}

void SimulationDialog::createTransientPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *desc = new QLabel("Perform a non-linear, time-domain simulation.");
    layout->addWidget(desc);

    QGroupBox *paramsGroup = new QGroupBox("Parameters");
    QGridLayout *paramsLayout = new QGridLayout();

    paramsLayout->addWidget(new QLabel("Stop time:"), 0, 0);
    stopTimeEdit = new QLineEdit("2");
    paramsLayout->addWidget(stopTimeEdit, 0, 1);

    paramsLayout->addWidget(new QLabel("Time to start saving data:"), 1, 0);
    startSaveEdit = new QLineEdit("0");
    paramsLayout->addWidget(startSaveEdit, 1, 1);

    paramsLayout->addWidget(new QLabel("Maximum Timestep:"), 2, 0);
    maxTimestepEdit = new QLineEdit("0.1");
    paramsLayout->addWidget(maxTimestepEdit, 2, 1);

    paramsLayout->addWidget(new QLabel("Variables:"), 3, 0);
    variablesEdit = new QLineEdit();
    paramsLayout->addWidget(variablesEdit, 3, 1);

    paramsGroup->setLayout(paramsLayout);
    layout->addWidget(paramsGroup);

    QLabel *syntaxLabel = new QLabel("Syntax: Tran <Tstop> [coption> [coption>] ...");
    layout->addWidget(syntaxLabel);

    liveLabel = new QLabel(this);
    layout->addWidget(liveLabel);

    auto updateLabel = [=]() {
        liveLabel->setText(QString(".tran %1 %2 %3 %4")
                          .arg(stopTimeEdit->text())
                          .arg(startSaveEdit->text())
                          .arg(maxTimestepEdit->text())
                          .arg(variablesEdit->text()));
    };

    connect(stopTimeEdit, &QLineEdit::textChanged, updateLabel);
    connect(startSaveEdit, &QLineEdit::textChanged, updateLabel);
    connect(maxTimestepEdit, &QLineEdit::textChanged, updateLabel);
    connect(variablesEdit, &QLineEdit::textChanged, updateLabel);

    updateLabel(); // مقداردهی اولیه

    QLabel *commandLabel = new QLabel("Iran 2");
    layout->addWidget(commandLabel);

    stackedWidget->addWidget(page);
}

void SimulationDialog::createACAnalysisPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *desc = new QLabel("Compute the small signal AC behavior of the circuit linearized about its DC operating point.");
    layout->addWidget(desc);

    QGroupBox *paramsGroup = new QGroupBox("Parameters");
    QGridLayout *paramsLayout = new QGridLayout();

    acSweepTypeCombo = new QComboBox();
    acSweepTypeCombo->addItems({"Linear", "Decade", "Octave"});
    paramsLayout->addWidget(new QLabel("Type of sweep:"), 0, 0);
    paramsLayout->addWidget(acSweepTypeCombo, 0, 1);

    paramsLayout->addWidget(new QLabel("Number of points:"), 1, 0);
    acNumPointsEdit = new QLineEdit("2");
    paramsLayout->addWidget(acNumPointsEdit, 1, 1);

    paramsLayout->addWidget(new QLabel("Start frequency:"), 2, 0);
    acStartFreqEdit = new QLineEdit();
    paramsLayout->addWidget(acStartFreqEdit, 2, 1);

    paramsLayout->addWidget(new QLabel("Stop frequency:"), 3, 0);
    acEndFreqEdit = new QLineEdit();
    paramsLayout->addWidget(acEndFreqEdit, 3, 1);

    paramsGroup->setLayout(paramsLayout);
    layout->addWidget(paramsGroup);

    QLabel *syntaxLabel = new QLabel("Syntax: ac <oct, dec, lin> <Npoints> <StartFreq> <EndFreq>");
    layout->addWidget(syntaxLabel);

    QLabel *commandLabel = new QLabel("ac 2");
    layout->addWidget(commandLabel);

    stackedWidget->addWidget(page);
}

void SimulationDialog::createDCSweepPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *desc = new QLabel("Compute the DC operating point of a circuit while stepping independent sources and treating capacitances as open circuits and inductances as short circuits.");
    layout->addWidget(desc);

    QGroupBox *paramsGroup = new QGroupBox("Parameters");
    QGridLayout *paramsLayout = new QGridLayout();

    paramsLayout->addWidget(new QLabel("Name of source to sweep:"), 0, 0);
    dcSourceNameEdit = new QLineEdit("2");
    paramsLayout->addWidget(dcSourceNameEdit, 0, 1);

    dcSweepTypeCombo = new QComboBox();
    dcSweepTypeCombo->addItems({"Linear", "Decade", "Octave"});
    paramsLayout->addWidget(new QLabel("Type of sweep:"), 1, 0);
    paramsLayout->addWidget(dcSweepTypeCombo, 1, 1);

    paramsLayout->addWidget(new QLabel("Start value:"), 2, 0);
    dcStartValueEdit = new QLineEdit();
    paramsLayout->addWidget(dcStartValueEdit, 2, 1);

    paramsLayout->addWidget(new QLabel("Stop value:"), 3, 0);
    dcStopValueEdit = new QLineEdit();
    paramsLayout->addWidget(dcStopValueEdit, 3, 1);

    paramsLayout->addWidget(new QLabel("Increment:"), 4, 0);
    dcIncrementEdit = new QLineEdit();
    paramsLayout->addWidget(dcIncrementEdit, 4, 1);

    paramsGroup->setLayout(paramsLayout);
    layout->addWidget(paramsGroup);

    QLabel *syntaxLabel = new QLabel("Syntax: dc [<act.dec.lin>] <Source I> <Start> <Stop> [<lncr>] [<source2> ...]");
    layout->addWidget(syntaxLabel);

    QLabel *commandLabel = new QLabel("Jdc 2");
    layout->addWidget(commandLabel);

    stackedWidget->addWidget(page);
}

void SimulationDialog::createOptionsGroup()
{
    optionsGroup = new QGroupBox("Options");
    QVBoxLayout *layout = new QVBoxLayout();

    startAtZeroCheck = new QCheckBox("Start external DC supply voltages at 0V");
    stopSteadyCheck = new QCheckBox("Stop simulating if steady state is detected");
    dontResetCheck = new QCheckBox("Don't reset T=0 when steady state is detected");
    stepLoadCheck = new QCheckBox("Step the load current source");
    skipInitialCheck = new QCheckBox("Skip initial operating point solution");

    layout->addWidget(startAtZeroCheck);
    layout->addWidget(stopSteadyCheck);
    layout->addWidget(dontResetCheck);
    layout->addWidget(stepLoadCheck);
    layout->addWidget(skipInitialCheck);

    optionsGroup->setLayout(layout);
}

// Getter methods implementation
SimulationDialog::SimulationType SimulationDialog::getSimulationType() const
{
    return static_cast<SimulationType>(simTypeGroup->checkedId());
}

std::string SimulationDialog::getStopTime() const { return stopTimeEdit->text().toStdString(); }
std::string SimulationDialog::getStartSaveTime() const { return startSaveEdit->text().toStdString(); }
std::string SimulationDialog::getMaxTimestep() const { return maxTimestepEdit->text().toStdString(); }
std::string SimulationDialog::getVariables() const { return variablesEdit->text().toStdString(); }

bool SimulationDialog::getStartAtZero() const { return startAtZeroCheck->isChecked(); }
bool SimulationDialog::getStopSteadyState() const { return stopSteadyCheck->isChecked(); }
bool SimulationDialog::getDontResetTime() const { return dontResetCheck->isChecked(); }
bool SimulationDialog::getStepLoadCurrent() const { return stepLoadCheck->isChecked(); }
bool SimulationDialog::getSkipInitialSolution() const { return skipInitialCheck->isChecked(); }

QString SimulationDialog::getACSweepType() const { return acSweepTypeCombo->currentText(); }
QString SimulationDialog::getACNumPoints() const { return acNumPointsEdit->text(); }
QString SimulationDialog::getACStartFreq() const { return acStartFreqEdit->text(); }
QString SimulationDialog::getACEndFreq() const { return acEndFreqEdit->text(); }

QString SimulationDialog::getDCSourceName() const { return dcSourceNameEdit->text(); }
QString SimulationDialog::getDCSweepType() const { return dcSweepTypeCombo->currentText(); }
QString SimulationDialog::getDCStartValue() const { return dcStartValueEdit->text(); }
QString SimulationDialog::getDCStopValue() const { return dcStopValueEdit->text(); }
QString SimulationDialog::getDCIncrement() const { return dcIncrementEdit->text(); }