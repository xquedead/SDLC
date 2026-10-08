#include "controller/lazinesscontroller.h"
#include "model/lazinessmodel.h"
#include "view/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

// Точка сборки приложения: здесь три слоя MVC
// знакомятся друг с другом. Модель не знает ни про кого,
// контроллер знает только модель, окно получает обоих готовыми.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = QStringLiteral("LazinessCalc_")
                               + QLocale(locale).name();
        if (translator.load(QStringLiteral(":/i18n/") + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    LazinessModel      model;
    LazinessController controller(&model);
    MainWindow         window(&model, &controller);

    window.show();
    return QApplication::exec();
}
