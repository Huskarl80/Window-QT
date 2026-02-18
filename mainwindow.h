#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onToggleButtonClicked();

private:
    Ui::MainWindow *ui;
    QPushButton *toggleButton;   // наша жёлтая кнопка
    bool textVisible = true;     // флаг: X виден/нет

    void setupUI();
};

#endif // MAINWINDOW_H
