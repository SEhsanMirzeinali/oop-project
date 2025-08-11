#include "wirepropertydialog.h"
#include <QVBoxLayout>
#include <QLabel>

WirePropertyDialog::WirePropertyDialog(QWidget *parent) 
    : QDialog(parent), nameEdit(nullptr) {
    setWindowTitle("Change Wire Name");
    setMinimumWidth(300);
    setupUI();
}

void WirePropertyDialog::setWireName(const QString &name) {
    if (nameEdit) {
        nameEdit->setText(name);
        nameEdit->selectAll(); // انتخاب خودکار متن برای ویرایش سریع
        nameEdit->setFocus(); // فوکوس روی فیلد نام
    }
}

QString WirePropertyDialog::getWireName() const {
    return nameEdit->text();
}

void WirePropertyDialog::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // فیلد ویرایش نام
    nameEdit = new QLineEdit(this);
    QLabel *label = new QLabel("Wire Name:", this);
    
    // دکمه‌های تایید/لغو
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    
    layout->addWidget(label);
    layout->addWidget(nameEdit);
    layout->addWidget(buttonBox);
    
    // اتصال سیگنال‌ها
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}