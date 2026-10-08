#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputDialog)
{
    ui->setupUi(this);

    // ОК     -> диалог закрывается с результатом Accepted
    // Отмена -> с результатом Rejected
    connect(ui->pushButtonOk,     &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::setValues(const QString &planned, const QString &done)
{
    ui->lineEditPlanned->setText(planned);
    ui->lineEditDone->setText(done);

    // Курсор сразу в первое поле, текст выделен — можно печатать поверх.
    ui->lineEditPlanned->setFocus();
    ui->lineEditPlanned->selectAll();
}

QString InputDialog::plannedText() const
{
    return ui->lineEditPlanned->text();
}

QString InputDialog::doneText() const
{
    return ui->lineEditDone->text();
}
