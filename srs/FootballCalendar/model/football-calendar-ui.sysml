// Футбольный календарь — графическая часть спецификации требований (SRS)
// Язык описания: SysML v2 (текстовая нотация)
// Автор: Коньков Я. А., гр. 434701

package FootballCalendar {
    private import ScalarValues::*;
    private import StandardViewDefinitions::*;

    // ------------------------------------------------------------------
    // 1. Контекст и варианты использования
    // ------------------------------------------------------------------
    package UseCases {
        part def FootballCalendarApp {
            doc /* Desktop-приложение для просмотра расписания и статистики футбольных матчей */
        }
        part def Fan {
            doc /* Болельщик — основной пользователь приложения */
        }
        part def Blogger {
            doc /* Спортивный блогер — использует статистику для публикаций */
        }
        part def FootballDataApi {
            doc /* Внешний сервис football-data.org (REST API v4) */
        }
        part def OperatingSystem {
            doc /* ОС Windows: системные уведомления, календарь пользователя */
        }

        use case def 'Просмотр расписания матчей' {
            subject app : FootballCalendarApp;
            actor user : Fan;
            actor api : FootballDataApi;
            objective {
                doc /* Пользователь видит матчи выбранных турниров за день, неделю или месяц */
            }
        }
        use case def 'Фильтрация по турнирам и командам' {
            subject app : FootballCalendarApp;
            actor user : Fan;
        }
        use case def 'Просмотр карточки матча' {
            subject app : FootballCalendarApp;
            actor user : Fan;
            actor api : FootballDataApi;
        }
        use case def 'Просмотр турнирной таблицы и бомбардиров' {
            subject app : FootballCalendarApp;
            actor user : Fan;
            actor blogger : Blogger;
            actor api : FootballDataApi;
        }
        use case def 'Просмотр статистики команды' {
            subject app : FootballCalendarApp;
            actor fan : Fan;
            actor blogger : Blogger;
            actor api : FootballDataApi;
        }
        use case def 'Ведение избранных команд' {
            subject app : FootballCalendarApp;
            actor user : Fan;
        }
        use case def 'Получение напоминания о матче' {
            subject app : FootballCalendarApp;
            actor user : Fan;
            actor os : OperatingSystem;
        }
        use case def 'Экспорт календаря в iCalendar' {
            subject app : FootballCalendarApp;
            actor user : Fan;
            actor os : OperatingSystem;
        }
    }

    // ------------------------------------------------------------------
    // 2. Элементы пользовательского интерфейса (библиотека)
    // ------------------------------------------------------------------
    package Widgets {
        abstract part def UIElement {
            attribute caption : String;
        }
        part def Window :> UIElement;
        part def Panel :> UIElement;
        part def Button :> UIElement;
        part def Label :> UIElement;
        part def TextField :> UIElement;
        part def CheckBox :> UIElement;
        part def ComboBox :> UIElement {
            attribute items : String[*];
        }
        part def TabBar :> UIElement {
            attribute tabs : String[*];
        }
        part def ListView :> UIElement;
        part def Table :> UIElement {
            attribute columns : String[*];
        }
        part def StatusBar :> UIElement;
    }

    // ------------------------------------------------------------------
    // 3. Мокапы экранов (UI-01 ... UI-04)
    // ------------------------------------------------------------------
    package Screens {
        private import Widgets::*;

        // UI-01 Главное окно — календарь матчей
        part mainWindow : Window {
            :>> caption = "Футбольный календарь";

            part navigation : TabBar {
                :>> tabs = ("Календарь", "Таблицы", "Команды", "Мой календарь");
            }

            part toolbar : Panel {
                part btnPrev : Button { :>> caption = "<"; }
                part lblPeriod : Label { :>> caption = "Текущий период"; }
                part btnNext : Button { :>> caption = ">"; }
                part btnToday : Button { :>> caption = "Сегодня"; }
                part cbPeriod : ComboBox { :>> items = ("День", "Неделя", "Месяц"); }
                part btnRefresh : Button { :>> caption = "Обновить"; }
            }

            part filterPanel : Panel {
                part competitions : ListView { :>> caption = "Турниры"; }
                part teamSearch : TextField { :>> caption = "Поиск команды"; }
                part onlyFavorites : CheckBox { :>> caption = "Только избранные"; }
            }

            part matchList : Table {
                :>> columns = ("Дата", "Время", "Турнир", "Хозяева", "Счёт", "Гости", "Статус");
            }

            part statusBar : StatusBar {
                :>> caption = "Обновлено: <время> · Football data provided by the Football-Data.org API";
            }
        }

        // UI-02 Карточка матча
        part matchCard : Window {
            :>> caption = "Карточка матча";

            part header : Panel {
                part homeTeam : Label;
                part score : Label;
                part awayTeam : Label;
                part matchInfo : Label { :>> caption = "Турнир, тур, дата, время, стадион, судья"; }
            }

            part goals : Table { :>> columns = ("Мин.", "Команда", "Автор гола"); }
            part headToHead : Table { :>> columns = ("Дата", "Матч", "Счёт"); }

            part actions : Panel {
                part btnRemind : Button { :>> caption = "Напомнить"; }
                part cbRemindBefore : ComboBox { :>> items = ("15 мин", "30 мин", "60 мин"); }
                part btnAddToCalendar : Button { :>> caption = "В мой календарь"; }
            }
        }

        // UI-03 Турнирная таблица и бомбардиры
        part standingsScreen : Window {
            :>> caption = "Таблицы";

            part toolbar : Panel {
                part cbCompetition : ComboBox { :>> caption = "Турнир"; }
                part cbSeason : ComboBox { :>> caption = "Сезон"; }
            }

            part standings : Table {
                :>> columns = ("№", "Команда", "И", "В", "Н", "П", "Мячи", "О", "Форма");
            }
            part scorers : Table { :>> columns = ("№", "Игрок", "Команда", "Голы"); }
        }

        // UI-04 Мой календарь (избранные команды и напоминания)
        part myCalendarScreen : Window {
            :>> caption = "Мой календарь";

            part favoritesPanel : Panel {
                part favorites : ListView { :>> caption = "Избранные команды"; }
                part btnAddTeam : Button { :>> caption = "Добавить команду"; }
            }

            part upcomingPanel : Panel {
                part conflictBanner : Label {
                    :>> caption = "Предупреждение о пересечении матчей по времени";
                }
                part upcoming : Table { :>> columns = ("Дата", "Время", "Матч", "Напоминание"); }
                part btnExportIcs : Button { :>> caption = "Экспорт в .ics"; }
            }
        }
    }

    // ------------------------------------------------------------------
    // 4. Представления модели (Views)
    // ------------------------------------------------------------------
    package Views {
        view def UseCaseView :> GeneralView;

        view 'Варианты использования' : UseCaseView {
            expose UseCases::*;
        }

        view 'UI-01 Главное окно' : InterconnectionView {
            expose Screens::mainWindow::**;
        }

        view 'UI-02 Карточка матча' : InterconnectionView {
            expose Screens::matchCard::**;
        }

        view 'UI-03 Турнирная таблица и бомбардиры' : InterconnectionView {
            expose Screens::standingsScreen::**;
        }

        view 'UI-04 Мой календарь' : InterconnectionView {
            expose Screens::myCalendarScreen::**;
        }
    }
}
