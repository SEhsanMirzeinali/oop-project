#include "componentpropertydialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

ComponentPropertyDialog::ComponentPropertyDialog(QWidget *parent) 
    : QDialog(parent), lineEdit1(nullptr), lineEdit2(nullptr) {
   // setupUI();
    setWindowTitle("Enter new Value and name for component");
    setMinimumWidth(300);
}
void ComponentPropertyDialog::setDefaultText(const QString& val , const QString &name) {
    currentName = name;
    currentValue = val;
}
void ComponentPropertyDialog::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    lineEdit1 = new QLineEdit(currentValue, this);
    lineEdit2 = new QLineEdit(currentName, this);

    // انتخاب خودکار متن و فوکوس
    lineEdit1->selectAll();
    lineEdit1->setFocus();

    QLabel *label1 = new QLabel("Value: ", this);
    QLabel *label2 = new QLabel("Name: ", this);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    
    layout->addWidget(label1);
    layout->addWidget(lineEdit1);
    layout->addWidget(label2);
    layout->addWidget(lineEdit2);
    layout->addWidget(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString ComponentPropertyDialog::getLine1() const {
    return lineEdit1->text();
}

QString ComponentPropertyDialog::getLine2() const {
    return lineEdit2->text();
}