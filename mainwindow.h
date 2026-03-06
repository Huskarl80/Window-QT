#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:   // Здесь у нас заготовка для конструктора/деконструктора. Объявляем
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onToggleButtonClicked();  // один слот для всех кнопок

private:
    Ui::MainWindow *ui;
    QVector<QPushButton*> buttons; // 9 кнопок в сетке

    void setupUI();
};

#endif // MAINWINDOW_H

