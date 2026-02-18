#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , toggleButton(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Privet");
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: rgb(127, 255, 0);");

    QVBoxLayout *layout = new QVBoxLayout(central);

    toggleButton = new QPushButton("X", central);
    toggleButton->setFixedSize(100, 50);

    QFont btnFont;
    btnFont.setPointSize(24);
    toggleButton->setFont(btnFont);

    toggleButton->setStyleSheet(  // верстка самой кнопки
        "QPushButton {"
        "    background-color: yellow;"
        "    color: blue;"
        "    border: 2px solid black;"
        "    border-radius: 0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: orange;"
        "}"
        );

    connect(toggleButton, &QPushButton::clicked, // сигнал
            this, &MainWindow::onToggleButtonClicked); // ловит сигнал - слот

    QLabel *label = new QLabel("Hello, Qt!", central); // пока пусть повисит левый тект
    QFont labelFont;
    labelFont.setPointSize(24);
    label->setFont(labelFont);
    label->setStyleSheet("color: blue;");

    label->setAlignment(Qt::AlignCenter);

    layout->addWidget(toggleButton, 0, Qt::AlignCenter);
    layout->addSpacing(80);
    layout->addWidget(label);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(0);

    central->setLayout(layout);
    setCentralWidget(central);
}


void MainWindow::onToggleButtonClicked() // здесь логика слота
{
    textVisible = !textVisible;

    if (textVisible) {
        toggleButton->setText("X");// Вкл
    } else {
        toggleButton->setText("");// Выкл
    }
}
