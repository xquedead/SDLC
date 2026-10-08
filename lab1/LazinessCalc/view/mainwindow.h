#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class LazinessModel;
class LazinessController;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(LazinessModel *model,
               LazinessController *controller,
               QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onInputClicked();   // нажата кнопка «Ввести данные»
    void refresh();          // вызывается сигналом модели dataChanged()

private:
    void showEmptyState();   // «данные ещё не вводились»
    void showResult();       // разложить данные модели по лейблам

    Ui::MainWindow     *ui;
    LazinessModel      *m_model;
    LazinessController *m_controller;
};

#endif // MAINWINDOW_H
