#ifndef SIMULATIONDIALOG_H
#define SIMULATIONDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

class SimulationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationDialog(QWidget *parent = nullptr);
    ~SimulationDialog() = default;

    enum SimulationType { TRANSIENT, AC_ANALYSIS, DC_SWEEP, NOISE, DC_TRANSFER, DC_OP_PRT };

    // Common getters
    SimulationType getSimulationType() const;
    std::string getStopTime() const;
    std::string getStartSaveTime() const;
    std::string getMaxTimestep() const;
    std::string getVariables() const;
    bool getStartAtZero() const;
    bool getStopSteadyState() const;
    bool getDontResetTime() const;
    bool getStepLoadCurrent() const;
    bool getSkipInitialSolution() const;

    // AC Analysis getters
    QString getACSweepType() const;
    QString getACNumPoints() const;
    QString getACStartFreq() const;
    QString getACEndFreq() const;

    // DC Sweep getters
    QString getDCSourceName() const;
    QString getDCSweepType() const;
    QString getDCStartValue() const;
    QString getDCStopValue() const;
    QString getDCIncrement() const;

private:
    void setupUI();
    void createSimulationTypeGroup();
    void createTransientPage();
    void createACAnalysisPage();
    void createDCSweepPage();
    void createOptionsGroup();

    // Widget members
    QStackedWidget *stackedWidget;
    QButtonGroup *simTypeGroup;

    // Radio buttons for simulation types
    QRadioButton *transientRadio;
    QRadioButton *acAnalysisRadio;
    QRadioButton *dcSweepRadio;
    QRadioButton *noiseRadio;
    QRadioButton *dcTransferRadio;
    QRadioButton *dcOpPrtRadio;  // تصحیح نام از dcOpPrtiRadio به dcOpPrtRadio

    // Transient widgets
    QLineEdit *stopTimeEdit;
    QLineEdit *startSaveEdit;
    QLineEdit *maxTimestepEdit;
    QLineEdit *variablesEdit;
    QLineEdit *syntaxEdit;
    QLabel *liveLabel; // برای نمایش لحظه‌ای مقادیر


    // AC Analysis widgets
    QComboBox *acSweepTypeCombo;
    QLineEdit *acNumPointsEdit;
    QLineEdit *acStartFreqEdit;
    QLineEdit *acEndFreqEdit;

    // DC Sweep widgets
    QLineEdit *dcSourceNameEdit;
    QComboBox *dcSweepTypeCombo;
    QLineEdit *dcStartValueEdit;
    QLineEdit *dcStopValueEdit;
    QLineEdit *dcIncrementEdit;

    // Common options
    QCheckBox *startAtZeroCheck;
    QCheckBox *stopSteadyCheck;
    QCheckBox *dontResetCheck;
    QCheckBox *stepLoadCheck;
    QCheckBox *skipInitialCheck;

    // Groups
    QGroupBox *optionsGroup;
    QGroupBox *parametersGroup;

    // Buttons
    QPushButton *cancelButton;
    QPushButton *okButton;
};

#endif // SIMULATIONDIALOG_H