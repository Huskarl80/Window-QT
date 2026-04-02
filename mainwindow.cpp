#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Privet");
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI() // Здесь мы строим наше игровое поле.
{
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: rgb(127, 255, 0);"); // Задник салатовый.

    QVBoxLayout *mainLayout = new QVBoxLayout(central); // Главный вертикальный лейаут.

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

    // Создаём 3x3 пустых ячеек
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            auto *cell = new QPushButton("", gridContainer); // стартуем БЕЗ текста
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

            // Индекс 11,12,13; 21,22,23; 31,32,33
            int index = (r + 1) * 10 + (c + 1);
            cell->setProperty("index", index); // пишем индекс в property

            connect(cell, &QPushButton::clicked,
                    this, &MainWindow::onCellClicked);

            cells.push_back(cell);        // добавляем в массив
            grid->addWidget(cell, r, c);  // и в сетку
        }
    }

    // Жёсткая ширина контейнера: 3 кнопки + 2 промежутка
    int gridWidth = 3 * 100 + 2 * 8;
    gridContainer->setFixedWidth(gridWidth);

    // Подпись снизу
    QLabel *label = new QLabel("Hello, Qt!", central); // пока пусть повисит левый тект
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

// Диалог выбора крестик или нолик
void MainWindow::createChoiceDialog()
{
    if (choiceDialog)
        return; // уже создано

    choiceDialog = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint);
    choiceDialog->setAttribute(Qt::WA_DeleteOnClose, false);
    choiceDialog->setWindowModality(Qt::ApplicationModal); // блокируем главное окно

    choiceDialog->setStyleSheet(
        "QWidget {"
        "    background-color: rgb(139, 69, 19);"   // коричневый
        "    border: 2px solid black;"
        "}"
        );

    QVBoxLayout *dlgLayout = new QVBoxLayout(choiceDialog);
    dlgLayout->setContentsMargins(20, 20, 20, 20);
    dlgLayout->setSpacing(20);

    // Первая строка: сюда будем писать индекс нажатой кнопки
    choiceInfoLabel = new QLabel("Вы нажали на кнопку с индексом ??", choiceDialog); // текст перезапишем при клике
    QFont infoFont;
    infoFont.setPointSize(14);
    choiceInfoLabel->setFont(infoFont);
    choiceInfoLabel->setStyleSheet("color: yellow;");
    choiceInfoLabel->setAlignment(Qt::AlignCenter);

    // Вторая строка: подсказка что делать дальше
    QLabel *title = new QLabel("Выберите что прописать в кнопке", choiceDialog);
    QFont titleFont;
    titleFont.setPointSize(14);
    title->setFont(titleFont);
    title->setStyleSheet("color: yellow;");
    title->setAlignment(Qt::AlignCenter);

    // Горизонтальный layout для двух кнопок X и O
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
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

    // Порядок: сначала строка с индексом, потом подсказка, потом кнопки
    dlgLayout->addWidget(choiceInfoLabel);
    dlgLayout->addWidget(title);
    dlgLayout->addLayout(buttonsLayout);

    // Подключаем слоты выбора
    connect(btnX, &QPushButton::clicked, this, &MainWindow::onChooseX);
    connect(btnO, &QPushButton::clicked, this, &MainWindow::onChooseO);

    choiceDialog->setLayout(dlgLayout);
    choiceDialog->setFixedSize(400, 200);
}

// Диалог когда игра завершена.
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

    QLabel *title = new QLabel("Вы заполнили все кнопки.\nХотите начать сначала?", restartDialog);
    QFont titleFont;
    titleFont.setPointSize(16);
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
    connect(btnRestartNo,  &QPushButton::clicked, this, &MainWindow::onRestartNo);

    restartDialog->setLayout(dlgLayout);
    restartDialog->setFixedSize(420, 200); //Окно фактически одинаковое надо на будущее придумать какой то конструктор с единым диалогом. Под условия булевы верстка.
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
        createChoiceDialog(); //запускаем диалог если его нет.

    // Обновляем строку с индексом нажатой кнопки в диалоге
    if (choiceInfoLabel) {
        int index = cell->property("index").toInt(); // читаем ранее записанный индекс
        QString text = QString("Вы нажали на кнопку с индексом %1").arg(index);
        choiceInfoLabel->setText(text);
    }

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
            "    color: blue;" // базовый цвет для X
            "    border: 2px solid black;"
            "    border-radius: 0;"
            "}"
            "QPushButton:pressed {"
            "    background-color: orange;"
            "}"
            );
    }
}
