#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class InputDialog; }
QT_END_NAMESPACE


class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

    // Подставить в поля последние введённые значения
    // (восстановление данных при повторном вводе).
    void setValues(const QString &planned, const QString &done);

    // Забрать введённое как есть, без разбора.
    QString plannedText() const;
    QString doneText() const;

private:
    Ui::InputDialog *ui;
};

#endif // INPUTDIALOG_H
