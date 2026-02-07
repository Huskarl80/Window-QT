#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>

void MainWindow::addlabel()
{
    int spacing = 100;
    int fontSize = 24;

    auto *label2 = new QLabel ("Slovo");
    auto *central = new QWidget(this);
    auto *layout  = new QVBoxLayout(central);

    auto *label = new QLabel("Hello, Qt!", central);

    // Создаем шрифт нужного размера
    QFont font;
    font.setPointSize(fontSize);

    // Применяем шрифт к обоим label'ам
    label2->setFont(font);
    label->setFont(font);

    // Синий цвет текста
    label2->setStyleSheet("color: blue;");
    label->setStyleSheet("color: blue;");

    layout->addWidget(label2);
    layout->addSpacing(spacing);
    layout->addWidget(label);
    layout->setSpacing(0);

    label2->setAlignment(Qt::AlignCenter);
    label->setAlignment(Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);

    central->setStyleSheet("background-color: rgb(127, 255, 0);");

    central->setLayout(layout);
    setCentralWidget(central);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Privet");
    this->addlabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}



