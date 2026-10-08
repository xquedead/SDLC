#ifndef LAZINESSCONTROLLER_H
#define LAZINESSCONTROLLER_H

#include <QObject>
#include <QString>

class LazinessModel;

class LazinessController : public QObject
{
    Q_OBJECT

public:
    explicit LazinessController(LazinessModel *model, QObject *parent = nullptr);

    // Возвращает true, если данные корректны и записаны в модель.
    // Если false — в errorMessage лежит текст ошибки для пользователя.
    // errorMessage можно передать nullptr, если текст не нужен.
    bool submit(const QString &plannedText,
                const QString &doneText,
                QString *errorMessage);

private:
    LazinessModel *m_model;
};

#endif // LAZINESSCONTROLLER_H
