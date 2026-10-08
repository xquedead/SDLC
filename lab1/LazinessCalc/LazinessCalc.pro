QT += core gui widgets

CONFIG += c++17

# Заголовки подключаются от корня проекта: "model/...", "view/..."
INCLUDEPATH += $$PWD

# Раскладка по слоям MVC: model / view / controller.
SOURCES += \
    main.cpp \
    controller/lazinesscontroller.cpp \
    model/lazinessmodel.cpp \
    view/inputdialog.cpp \
    view/mainwindow.cpp

HEADERS += \
    controller/lazinesscontroller.h \
    model/lazinessmodel.h \
    view/inputdialog.h \
    view/mainwindow.h

FORMS += \
    view/inputdialog.ui \
    view/mainwindow.ui

TRANSLATIONS +=
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Диаграммы и прочая документация — не код, но пусть видны в дереве проекта.
DISTFILES +=
