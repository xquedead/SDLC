#include "lazinessmodel.h"

#include <algorithm>

namespace {

// Шкала прокрастинации: верхняя граница коэффициента (включительно)
// и соответствующий ей вердикт.
//
// Таблица вместо лестницы из if-ов: новый уровень добавляется одной
// строкой, а порядок и непрерывность шкалы видны с одного взгляда.
struct Grade
{
    double      upperBound;   // проценты, включительно
    const char *level;
    const char *phrase;
};

const Grade kScale[] = {
    {   0.0, "Отсутствует",
        "План выполнен полностью. Либо ты машина, либо план был занижен." },

    {  20.0, "Низкий",
        "Почти всё сделано. Остаток можно списать на человечность." },

    {  40.0, "Умеренный",
        "Рабочий режим большинства людей. Не рекорд, но и не позор." },

    {  60.0, "Высокий",
        "Половина дел ушла в никуда. Завтра они вернутся и приведут друзей." },

    {  85.0, "Критический",
        "Список дел прожит, но не выполнен. Классика жанра." },

    // Последняя строка — замыкающая: её граница совпадает с максимумом
    // коэффициента, поэтому поиск по таблице всегда находит вердикт.
    { 100.0, "Терминальная стадия",
        "Планирование состоялось. Это уже половина дела, "
        "как принято себя утешать." },
};

} // namespace

LazinessModel::LazinessModel(QObject *parent)
    : QObject(parent)
{
}

bool LazinessModel::isValid(int planned, int done)
{
    return planned >= kMinPlanned
        && done    >= kMinDone
        && planned <= kMaxTasks
        && done    <= kMaxTasks;
}

bool LazinessModel::setData(int planned, int done)
{
    if (!isValid(planned, done))
        return false;

    m_planned = planned;
    m_done    = done;
    m_hasData = true;

    recalculate();

    emit dataChanged();   //Активная модель.
    return true;
}

void LazinessModel::clear()
{
    if (!m_hasData)
        return;

    m_planned     = 0;
    m_done        = 0;
    m_coefficient = 0.0;
    m_level.clear();
    m_phrase.clear();
    m_hasData     = false;

    emit dataChanged();
}

void LazinessModel::recalculate()
{
    // Сюда попадаем только после проверки isValid(), поэтому
    // m_planned гарантированно не ноль и делить на него безопасно.
    //
    // static_cast<double> обязателен: два int дали бы целочисленное
    // деление, и 3/5 превратилось бы в 0.
    const double ratio = static_cast<double>(m_done)
                       / static_cast<double>(m_planned);

    // Коэффициент лени: какая доля запланированного НЕ сделана, в процентах.
    // Перевыполнил план — лени нет, в минус не уходим.
    m_coefficient = std::clamp((1.0 - ratio) * 100.0, 0.0, 100.0);

    for (const Grade &grade : kScale) {
        if (m_coefficient <= grade.upperBound) {
            m_level  = QString::fromUtf8(grade.level);
            m_phrase = QString::fromUtf8(grade.phrase);
            break;
        }
    }
}
