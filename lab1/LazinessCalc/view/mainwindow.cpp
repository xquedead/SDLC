#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "inputdialog.h"
#include "../controller/lazinesscontroller.h"
#include "../model/lazinessmodel.h"

#include <QMessageBox>

namespace {
// Заглушка в лейблах, пока данных нет.
// Через код символа, а не литералом: не зависит от кодировки файла.
const QChar kPlaceholder(0x2014);   // длинное тире
}

MainWindow::MainWindow(LazinessModel *model,
                       LazinessController *controller,
                       QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model(model)
    , m_controller(controller)
{
    Q_ASSERT(m_model);
    Q_ASSERT(m_controller);

    ui->setupUi(this);

    // ================== КЛЮЧЕВАЯ СТРОКА ВСЕЙ ЛАБЫ ==================
    // АКТИВНАЯ МОДЕЛЬ: View подписывается на сигнал модели напрямую.
    // Контроллер в этой связке не участвует — он про View не знает.
    connect(m_model, &LazinessModel::dataChanged,
            this,    &MainWindow::refresh);
    // ===============================================================

    connect(ui->pushButtonInput, &QPushButton::clicked,
            this,                &MainWindow::onInputClicked);

    refresh();   // нарисовать стартовое состояние (данных ещё нет)
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onInputClicked()
{
    InputDialog dlg(this);

    // Восстановление последних введённых данных.
    // Данные живут в модели, поэтому они не теряются
    // между открытиями диалога.
    if (m_model->hasData()) {
        dlg.setValues(QString::number(m_model->planned()),
                      QString::number(m_model->done()));
    }

    // Крутимся, пока пользователь не введёт корректные данные
    // или не нажмёт «Отмена» (тогда exec() вернёт Rejected и цикл прервётся).
    while (dlg.exec() == QDialog::Accepted) {
        QString error;

        if (m_controller->submit(dlg.plannedText(), dlg.doneText(), &error))
            break;   // успех: модель уже испустила сигнал, refresh() отработал

        // Ошибку показывает View, а не контроллер.
        QMessageBox::warning(this, tr("Ошибка ввода"), error);
        // Диалог откроется снова с теми же значениями — их можно поправить.
    }
}

void MainWindow::refresh()
{
    if (m_model->hasData())
        showResult();
    else
        showEmptyState();
}

void MainWindow::showEmptyState()
{
    ui->labelPlannedVal->setText(kPlaceholder);
    ui->labelDoneVal->setText(kPlaceholder);
    ui->labelCoefVal->setText(kPlaceholder);
    ui->labelLevelVal->setText(kPlaceholder);
    ui->labelPhraseVal->setText(tr("Данные ещё не вводились."));
}

void MainWindow::showResult()
{
    // View только читает из модели и раскладывает по лейблам.
    // Никаких вычислений здесь нет и быть не должно.
    ui->labelPlannedVal->setText(QString::number(m_model->planned()));
    ui->labelDoneVal->setText(QString::number(m_model->done()));

    ui->labelCoefVal->setText(
        tr("%1 %").arg(m_model->coefficient(), 0, 'f', 1));

    ui->labelLevelVal->setText(m_model->level());
    ui->labelPhraseVal->setText(m_model->phrase());
}
