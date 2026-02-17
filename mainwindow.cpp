#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QPushButton> //  инклудим кнопку
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Privet");
    addlabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addlabel() //  наша функция. разрослась конечно. потом может подумать про более нарядное разделение.
{
    int spacing = 80;
    int fontSize = 24;

    auto *central = new QWidget(this);
    auto *layout  = new QVBoxLayout(central);

    auto *yellowRect = new QPushButton("", central);
    yellowRect->setFixedSize(100, 50);
    yellowRect->setStyleSheet(  //  тут окантовка
        "QPushButton {"
        "    background-color: yellow;"
        "    border: 2px solid black;"
        "    border-radius: 0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: orange;"
        "}"
        );

    connect(yellowRect, &QPushButton::clicked, this, &MainWindow::onYellowRectClicked);

    labelX = new QLabel("X", central);
    auto *label = new QLabel("Hello, Qt!", central);

    QFont font;
    font.setPointSize(fontSize);
    labelX->setFont(font);
    label->setFont(font);

    labelX->setStyleSheet("color: blue;");
    label->setStyleSheet("color: blue;");

    layout->addWidget(yellowRect, 0, Qt::AlignCenter);
    layout->addWidget(labelX);
    layout->addSpacing(spacing);//  здесь пространство между для верстки - не потерять.
    layout->addWidget(label);
    layout->setSpacing(0);

    labelX->setAlignment(Qt::AlignCenter);
    label->setAlignment(Qt::AlignCenter);
    layout->setAlignment(Qt::AlignCenter);

    central->setStyleSheet("background-color: rgb(127, 255, 0);");//  если что могу прям их фотошопа добавить кодировку цвета.
    central->setLayout(layout);
    setCentralWidget(central);
}

void MainWindow::onYellowRectClicked() //  мой флип/флоп
{
    isXVisible = !isXVisible;

    if (isXVisible) {
        labelX->show();
        qDebug() << "X показан";
    } else {
        labelX->hide();
        qDebug() << "X скрыт";
    }
}
