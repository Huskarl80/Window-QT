#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector> // Инклдим динамический массив.

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow // объявляем класс и наследуем от QMainWindow
{
    Q_OBJECT // это поддержка сигналов и слотов.

public:
    MainWindow(QWidget *parent = nullptr); // Здесь у нас объявляеться конструктор и деструктор (ниже строчкой)
    ~MainWindow();

private slots:
    void onCellClicked();   // клик по любой из 9 ячеек/кнопок
    void onChooseX();       // выбор X в диалоге
    void onChooseO();       // выбор O в диалоге

    void onRestartYes();    // "Да" в диалоге перезапуска
    void onRestartNo();     // "Нет" в диалоге перезапуска

private:
    Ui::MainWindow *ui;

    QVector<QPushButton*> cells;        // Это ячейки поля - массив! (9 штук будет)
     QPushButton *currentCell = nullptr; // текущая ячейка, для которой сейчас выбираем крестик или нолик. Это указатель на кнопку.

    // Диалог выбора крестик или нолик
    QWidget *choiceDialog = nullptr; // Здесь объявляем само диалоговое окно выбора.
    QPushButton *btnX = nullptr;
    QPushButton *btnO = nullptr;

    // Диалог когда игра хавершена.
    QWidget *restartDialog = nullptr; // Здесь написал еще одно диалоговое - рациональнее как будто бы сделать его единым. Это на будущее.
    QPushButton *btnRestartYes = nullptr;
    QPushButton *btnRestartNo = nullptr;

    void setupUI();
    void createChoiceDialog();
    void createRestartDialog();
    void checkAllFilledAndAskRestart(); // условие проверки заполненности поля
    void resetField();                  // сброс всех ячеек
};

#endif // MAINWINDOW_H
