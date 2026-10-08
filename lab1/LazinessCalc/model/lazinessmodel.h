#ifndef LAZINESSMODEL_H
#define LAZINESSMODEL_H

#include <QObject>
#include <QString>

class LazinessModel : public QObject
{
    Q_OBJECT

public:
    // Границы допустимых значений — это правило предметной области,
    // поэтому они живут в модели, а контроллер на них ссылается.
    static constexpr int kMinPlanned = 1;      // делить на ноль нельзя
    static constexpr int kMinDone    = 0;
    static constexpr int kMaxTasks   = 1000;   // чтобы не вводили 999999999

    explicit LazinessModel(QObject *parent = nullptr);

    // Единственная точка изменения данных.
    // Возвращает false и ничего не меняет, если пара значений
    // нарушает инварианты модели. Модель защищает себя сама и
    // не полагается на то, что её вызвали через контроллер.
    bool setData(int planned, int done);

    // Вернуться в состояние «данные ещё не вводились».
    void clear();

    // Проверка пары значений без записи. Нужна контроллеру,
    // чтобы не дублировать у себя границы диапазона.
    static bool isValid(int planned, int done);

    // Геттеры — только чтение, ничего не меняют (потому и const).
    int     planned()     const { return m_planned; }
    int     done()        const { return m_done; }
    double  coefficient() const { return m_coefficient; }
    QString level()       const { return m_level; }
    QString phrase()      const { return m_phrase; }
    bool    hasData()     const { return m_hasData; }

signals:
    // Вот это и делает модель АКТИВНОЙ.
    // Модель сообщает «я изменилась» и не знает, кто её слушает.
    void dataChanged();

private:
    void recalculate();   // вся математика здесь

    int     m_planned     = 0;
    int     m_done        = 0;
    double  m_coefficient = 0.0;
    QString m_level;
    QString m_phrase;
    bool    m_hasData     = false;   // были ли данные введены хоть раз
};

#endif // LAZINESSMODEL_H
