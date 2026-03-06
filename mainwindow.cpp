#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) // Конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Тело конструктора
    setWindowTitle("tic-tac-toe");
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui; // Удаляем
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: rgb(127, 255, 0);");

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // ВНУТРЕННИЙ виджет под сетку, чтобы не растягивало
    QWidget *gridContainer = new QWidget(central);
    QGridLayout *grid = new QGridLayout(gridContainer);

    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(8);
    grid->setContentsMargins(0, 0, 0, 0);

    QFont btnFont;
    btnFont.setPointSize(24);

    const int rows = 3;
    const int cols = 3;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            auto *btn = new QPushButton("X", gridContainer);
            btn->setFixedSize(100, 50);
            btn->setFont(btnFont);
            btn->setStyleSheet(
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
            connect(btn, &QPushButton::clicked, // Здесь объект испкскает сигнал
                    this, &MainWindow::onToggleButtonClicked);

            buttons.push_back(btn);
            grid->addWidget(btn, r, c);
        }
    }

    // Жёсткая ширина контейнера: 3 кнопки + 2 промежутка по 8
    int gridWidth = 3 * 100 + 2 * 8;
    gridContainer->setFixedWidth(gridWidth);

    QLabel *label = new QLabel("Hello, Qt!", central);
    QFont labelFont;
    labelFont.setPointSize(24);
    label->setFont(labelFont);
    label->setStyleSheet("color: blue;");
    label->setAlignment(Qt::AlignCenter);

    mainLayout->addStretch();
    mainLayout->addWidget(gridContainer, 0, Qt::AlignHCenter);  // центрируем всю сетку
    mainLayout->addSpacing(40);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);

    central->setLayout(mainLayout);
    setCentralWidget(central);
}


void MainWindow::onToggleButtonClicked()
{
    // Определяем, какая именно кнопка вызвала слот
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
        return;

    // Переключаем текст X / пусто
    if (btn->text() == "X") {
        btn->setText("");
    } else {
        btn->setText("X");
    }
}
