#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>  // нужно это инклудить - QLabel*
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);//  Здесь объявляем nullptr чтобы отказаться от родителя.
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void addlabel();
    QLabel *labelX = nullptr;     //  Активируем nullptr
    bool isXVisible = true;       //  переменная булка

private slots:
    void onYellowRectClicked();
};

#endif // MAINWINDOW_H

