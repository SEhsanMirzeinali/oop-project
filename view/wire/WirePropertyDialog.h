#ifndef WIREPROPERTYDIALOG_H
#define WIREPROPERTYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>

class WirePropertyDialog : public QDialog {
    Q_OBJECT
public:
    explicit WirePropertyDialog(QWidget *parent = nullptr);
    void setWireName(const QString &name); // تنظیم نام فعلی وایر
    QString getWireName() const; // دریافت نام جدید وایر
    void setupUI(); // تنظیم رابط کاربری

private:
    QLineEdit *nameEdit; // فیلد ویرایش نام
};

#endif // WIREPROPERTYDIALOG_H