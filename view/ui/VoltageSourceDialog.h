#ifndef VOLTAGESOURCEDIALOG_H
#define VOLTAGESOURCEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

class VoltageSourceDialog : public QDialog {
    Q_OBJECT

public:
    explicit VoltageSourceDialog(QWidget *parent = nullptr);
    ~VoltageSourceDialog();

    QString getSelectedFunction() const;
    double getDCValue() const;
    QMap<QString, double> getFunctionParameters() const;
    double getACAmplitude() const;
    double getACPhase() const;
    double getSeriesResistance() const;
    double getParallelCapacitance() const;

    private slots:
        void onFunctionChanged(int id);

private:
    void setupUI();
    void setupFunctionGroup();
    void setupParameterPages();
    void setupRightSideSection();
    void setupButtons();
    QLineEdit* createValidatedLineEdit();

    QButtonGroup *functionGroup;
    QStackedWidget *parameterStack;

    // Controls
    QLineEdit *dcValueEdit;
    QLineEdit *acAmplitudeEdit;
    QLineEdit *acPhaseEdit;
    QLineEdit *seriesResistanceEdit;
    QLineEdit *parallelCapacitanceEdit;

    QMap<QString, QMap<QString, QLineEdit*>> functionParameters;
};

#endif // VOLTAGESOURCEDIALOG_H