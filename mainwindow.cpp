#include "mainwindow.h"
#include "ui_mainwindow.h" // вначале тащим нужное из mainwindow.h

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QDebug>  // Инклудим все что нам нужно для верстки - строки 4-9

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) // инициализируем базовый класс QMainWindow с родителем
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // базовое окно - рамка
    setWindowTitle("Tic-tac-toe game");
    setupUI(); //вызываем нашу функцию, которая собирает «игровое поле» и диалоги
}

MainWindow::~MainWindow() // здесь деконструктор
{
    delete ui;
}

void MainWindow::setupUI() // Это основное окно.
{
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: rgb(127, 255, 0);");

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Контейнер для сетки 3x3
    QWidget *gridContainer = new QWidget(central);
    QGridLayout *grid = new QGridLayout(gridContainer);
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(8);
    grid->setContentsMargins(0, 0, 0, 0);

    QFont btnFont;
    btnFont.setPointSize(24);

    const int rows = 3;
    const int cols = 3;

    // Создаём 3x3 пустых ячеек через два вложенных цикла
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            auto *cell = new QPushButton("", gridContainer);
            cell->setFixedSize(100, 50);
            cell->setFont(btnFont);
            cell->setStyleSheet(
                "QPushButton {"
                "    background-color: yellow;"
                "    color: blue;"             // цвет текста по умолчанию (для X)
                "    border: 2px solid black;"
                "    border-radius: 0;"
                "}"
                "QPushButton:pressed {"
                "    background-color: orange;"
                "}"
                );

            // Индекс в нужном формате 11,12,13; 21,22,23; 31,32,33
            int index = (r + 1) * 10 + (c + 1); // r умножаем на 10 и прибавляем текущуюю c - колонну.
            cell->setProperty("index", index); // теперь это свойство самой кнопки.

            connect(cell, &QPushButton::clicked, //сигнал.
                    this, &MainWindow::onCellClicked);

            cells.push_back(cell);
            grid->addWidget(cell, r, c); //Кладём кнопку в сетку grid на позицию (r, c).
        }
    }

    // Жёсткая ширина контейнера: 3 кнопки + 2 промежутка
    int gridWidth = 3 * 100 + 2 * 8;
    gridContainer->setFixedWidth(gridWidth);

    // Подпись снизу - ну это мусор уже наверно.
    QLabel *label = new QLabel("Hello, Qt!", central);
    QFont labelFont;
    labelFont.setPointSize(24);
    label->setFont(labelFont);
    label->setStyleSheet("color: blue;");
    label->setAlignment(Qt::AlignCenter);

    mainLayout->addStretch();
    mainLayout->addWidget(gridContainer, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(label, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->setSpacing(0);

    central->setLayout(mainLayout);
    setCentralWidget(central);
}

// А здесь делаем диалоговое окно.
void MainWindow::createChoiceDialog()
{
    if (choiceDialog)
        return; // условие - Если уже создано то выходим.

    choiceDialog = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint); // Делаем окно без крестика потом что по умолчанию QT делает крестик.
    choiceDialog->setAttribute(Qt::WA_DeleteOnClose, false);// Тут спорно - специально окно не удаляеться а только хайдится - пока не знаю как лучше - может WA_DeleteOnClose = true оптимальнее. Вопрос.
    choiceDialog->setWindowModality(Qt::ApplicationModal); // блокируем основное окно при диалоге.

    choiceDialog->setStyleSheet(
        "QWidget {"
        "    background-color: rgb(139, 69, 19);"   // коричневый а ниже черная рамка
        "    border: 2px solid black;"
        "}"
        );

    QVBoxLayout *dlgLayout = new QVBoxLayout(choiceDialog);
    dlgLayout->setContentsMargins(20, 20, 20, 20);
    dlgLayout->setSpacing(20);

    QLabel *title = new QLabel("Выберите что прописать в кнопке", choiceDialog);
    QFont titleFont;
    titleFont.setPointSize(14);
    title->setFont(titleFont);
    title->setStyleSheet("color: yellow;");
    title->setAlignment(Qt::AlignCenter);

    QHBoxLayout *buttonsLayout = new QHBoxLayout(); // бокс для кнопок
    buttonsLayout->setSpacing(20);

    QFont choiceFont;
    choiceFont.setPointSize(24);

    btnX = new QPushButton("X", choiceDialog);
    btnX->setFixedSize(80, 50);
    btnX->setFont(choiceFont);
    btnX->setStyleSheet(
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

    btnO = new QPushButton("O", choiceDialog);
    btnO->setFixedSize(80, 50);
    btnO->setFont(choiceFont);
    btnO->setStyleSheet(
        "QPushButton {"
        "    background-color: yellow;"
        "    color: red;"   // O красная
        "    border: 2px solid black;"
        "    border-radius: 0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: orange;"
        "}"
        );

    buttonsLayout->addWidget(btnX);
    buttonsLayout->addWidget(btnO);

    dlgLayout->addWidget(title);
    dlgLayout->addLayout(buttonsLayout);

    //Сигналы от кнопок X и O соединяем с соответствующими слотами:
    connect(btnX, &QPushButton::clicked, this, &MainWindow::onChooseX);
    connect(btnO, &QPushButton::clicked, this, &MainWindow::onChooseO);

    choiceDialog->setLayout(dlgLayout);
    choiceDialog->setFixedSize(400, 200);
}

// Диалог перезапуска
void MainWindow::createRestartDialog()
{
    if (restartDialog)
        return;

    restartDialog = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint);
    restartDialog->setAttribute(Qt::WA_DeleteOnClose, false);
    restartDialog->setWindowModality(Qt::ApplicationModal);

    restartDialog->setStyleSheet(
        "QWidget {"
        "    background-color: rgb(139, 69, 19);"
        "    border: 2px solid black;"
        "}"
        );

    QVBoxLayout *dlgLayout = new QVBoxLayout(restartDialog);
    dlgLayout->setContentsMargins(20, 20, 20, 20);
    dlgLayout->setSpacing(20);

    QLabel *title = new QLabel("Вы заполнили все кнопки. \nХотите начать сначала?", restartDialog); //С новой строчи красивее
    QFont titleFont;
    titleFont.setPointSize(14);
    title->setFont(titleFont);
    title->setStyleSheet("color: yellow;");
    title->setAlignment(Qt::AlignCenter);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(20);

    QFont choiceFont;
    choiceFont.setPointSize(18);

    btnRestartYes = new QPushButton("Да", restartDialog);
    btnRestartYes->setFixedSize(100, 50);
    btnRestartYes->setFont(choiceFont);
    btnRestartYes->setStyleSheet(
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

    btnRestartNo = new QPushButton("Нет", restartDialog);
    btnRestartNo->setFixedSize(100, 50);
    btnRestartNo->setFont(choiceFont);
    btnRestartNo->setStyleSheet(
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

    buttonsLayout->addWidget(btnRestartYes);
    buttonsLayout->addWidget(btnRestartNo);

    dlgLayout->addWidget(title);
    dlgLayout->addLayout(buttonsLayout);

    connect(btnRestartYes, &QPushButton::clicked, this, &MainWindow::onRestartYes);
    connect(btnRestartNo, &QPushButton::clicked, this, &MainWindow::onRestartNo);

    restartDialog->setLayout(dlgLayout);
    restartDialog->setFixedSize(400, 200); //Окно фактически одинаковое надо на будущее придумать какой то конструктор с единым диалогом. Под усовия булевы верстка.
}

// Обработка клика по ячейке
void MainWindow::onCellClicked()
{
    auto *cell = qobject_cast<QPushButton*>(sender()); //кто послал сигнал? Какая кнопка?
    if (!cell) // если не кнопка то выходим.
        return;

    // если уже стоит X или O — ничего не делаем - повторный клик по заполненой невозможен.
    if (!cell->text().isEmpty())
        return;

    currentCell = cell; //выбор для текущей кнопки

    if (!choiceDialog)
        createChoiceDialog(); //запускаем диалог если ео нет.

    // центрируем диалог выбора относительно окна
    QPoint center = this->geometry().center();
    QRect dlgGeom = choiceDialog->frameGeometry();
    dlgGeom.moveCenter(center);
    choiceDialog->move(dlgGeom.topLeft());

    //Показ окна, активация и приоритет - выше поднять.
    choiceDialog->show();
    choiceDialog->raise();
    choiceDialog->activateWindow();
}

// Выбор X - ниже условие на проверку пустоты в ячейке - то же самое с ноликом ниже.
void MainWindow::onChooseX()
{
    if (currentCell) {
        currentCell->setText("X");
        // цвет X — синий
        currentCell->setStyleSheet(
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
    }
    choiceDialog->hide();
    currentCell = nullptr;

    checkAllFilledAndAskRestart();
}

// Выбор O
void MainWindow::onChooseO()
{
    if (currentCell) {
        currentCell->setText("O");
        // O красная
        currentCell->setStyleSheet(
            "QPushButton {"
            "    background-color: yellow;"
            "    color: red;"
            "    border: 2px solid black;"
            "    border-radius: 0;"
            "}"
            "QPushButton:pressed {"
            "    background-color: orange;"
            "}"
            );
    }
    choiceDialog->hide();
    currentCell = nullptr;

    checkAllFilledAndAskRestart();
}

// Проверка, что все 9 ячеек заполнены. Здесь еще делаем цикл на проверку.
void MainWindow::checkAllFilledAndAskRestart()
{
    for (QPushButton *cell : cells) {
        if (cell->text().isEmpty())
            return; // есть ещё пустые, ничего не делаем
    }

    // Все ячейки заполнены — спрашиваем про перезапуск
    if (!restartDialog)
        createRestartDialog();

    QPoint center = this->geometry().center();
    QRect dlgGeom = restartDialog->frameGeometry();
    dlgGeom.moveCenter(center);
    restartDialog->move(dlgGeom.topLeft());

    restartDialog->show();
    restartDialog->raise();
    restartDialog->activateWindow();
}

// "Да" — сброс поля
void MainWindow::onRestartYes()
{
    resetField();
    restartDialog->hide(); // тут не забывать захайдить при рестарте.
}

// "Нет"
void MainWindow::onRestartNo()
{
    restartDialog->hide(); // Если нет то только хайдим диалог.
}

// Сброс всех ячеек в начальное состояние
void MainWindow::resetField()
{
    for (QPushButton *cell : cells) {
        cell->setText(""); // Стираем текст.
        cell->setStyleSheet(
            "QPushButton {"
            "    background-color: yellow;"
            "    color: blue;"          // базовый цвет для X
            "    border: 2px solid black;"
            "    border-radius: 0;"
            "}"
            "QPushButton:pressed {"
            "    background-color: orange;"
            "}"
            );
    }
}
