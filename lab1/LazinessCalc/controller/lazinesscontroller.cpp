#include "lazinesscontroller.h"

#include "../model/lazinessmodel.h"

LazinessController::LazinessController(LazinessModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
{
    Q_ASSERT(m_model);
}

bool LazinessController::submit(const QString &plannedText,
                                const QString &doneText,
                                QString *errorMessage)
{
    // Вспомогательная лямбда: записать текст ошибки, если его вообще просили.
    auto fail = [errorMessage](const QString &text) {
        if (errorMessage)
            *errorMessage = text;
        return false;
    };

    const QString planned = plannedText.trimmed();
    const QString done    = doneText.trimmed();

    // 1. Пустые поля
    if (planned.isEmpty() || done.isEmpty())
        return fail(tr("Оба поля должны быть заполнены."));

    // 2. Не числа. toInt() выставит ok = false на "abc", "1.5", "5 дел" и т.п.
    bool okPlanned = false;
    bool okDone    = false;
    const int p = planned.toInt(&okPlanned);
    const int d = done.toInt(&okDone);

    if (!okPlanned || !okDone)
        return fail(tr("Вводить нужно целые числа "
                       "без букв, пробелов и точек."));

    // 3. Логические границы. Сами числа берём из модели: диапазон —
    //    это её правило, контроллер только переводит его в текст ошибки.
    if (p < LazinessModel::kMinPlanned)
        return fail(tr("Запланированных дел должно быть "
                       "хотя бы одно, иначе делить не на что."));

    if (d < LazinessModel::kMinDone)
        return fail(tr("Количество сделанных дел "
                       "не может быть отрицательным."));

    if (p > LazinessModel::kMaxTasks || d > LazinessModel::kMaxTasks)
        return fail(tr("Слишком большие числа. "
                       "Допустимый диапазон — до %1 дел.")
                        .arg(LazinessModel::kMaxTasks));

    // Данные корректны — отдаём их модели.
    // Модель сама испустит dataChanged(), и View обновится.
    if (!m_model->setData(p, d))
        return fail(tr("Модель отвергла данные. Проверьте введённые значения."));

    return true;
}
