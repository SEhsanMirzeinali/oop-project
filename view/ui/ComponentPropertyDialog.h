#ifndef COMPONENTPROPERTYDIALOG_H
#define COMPONENTPROPERTYDIALOG_H

#include <QDialog>
#include <QLineEdit>

class ComponentPropertyDialog : public QDialog {
    Q_OBJECT
public:
    explicit ComponentPropertyDialog(QWidget *parent = nullptr);

    QString getLine1() const;
    QString getLine2() const;

    void setDefaultText(const QString &name , const QString & val);

    void setupUI();
private:
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QString currentName;
    QString currentValue;
};

#endif // COMPONENTPROPERTYDIALOG_H