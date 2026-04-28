#include "mainwindow.h"

#include <fstream>
#include <filesystem>
#include <iostream>

#include <QMessageBox>

const int MAX_COLUMNS = 3;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // creating the entire UI manually because Qt Creator sucks
    setupFilePath();
    setupUi();
    loadFromFile();
}

void MainWindow::setupUi() {
    newTimerName = new QLineEdit;
    newTimerDuration = new QLineEdit;

    QPushButton *addTimerButton = new QPushButton;
    addTimerButton->setText("Add Timer");
    QPushButton::connect(addTimerButton, &QPushButton::clicked, this, &MainWindow::addTimerButtonClicked);

    rightLayout = new QFormLayout;
    rightLayout->addRow("Timer name:", newTimerName);
    rightLayout->addRow("Timer duration (seconds): ", newTimerDuration);
    rightLayout->addRow(addTimerButton);

    QWidget* rightLayoutWidget = new QWidget;
    rightLayoutWidget->setMaximumWidth(280);
    rightLayoutWidget->setLayout(rightLayout);

    leftLayout = new QGridLayout;
    leftLayout->setSpacing(10);

    leftLayoutWidget = new QWidget;
    leftLayoutWidget->setLayout(leftLayout);

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(leftLayoutWidget);
    mainLayout->addWidget(rightLayoutWidget);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);

    // Set QWidget as the central layout of the main window
    setCentralWidget(window);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupFilePath() {
    std::string filePath;
    try {
        filePath = std::getenv("LOCALAPPDATA");
    } catch (std::exception e) {
        std::cout << "An error occured trying to set up the file path.";
    }
    filePath += "\\TimerApp";

    std::filesystem::create_directory(filePath);
    if (!std::filesystem::exists(filePath)) {
        std::ofstream output(filePath + "\\timers.txt");
        output << "";
        output.close();
    }
    timersFilePath = filePath;
}

bool MainWindow::saveToFile() {
    std::ofstream output(timersFilePath + "\\timers.txt");

    for (int i = 0; i < timers.size(); ++i) {
        const Timer timer = timers.at(i);

        output << timer.name << "|" << timer.duration;

        if (i != timers.size() - 1) output << "\n";
    }
    output.close();
    return true;
}

bool MainWindow::loadFromFile() {
    timers.clear();

    std::ifstream input(timersFilePath + "\\timers.txt");
    std::string curLine;
    while (!input.eof()) {
        std::getline(input, curLine, '\n');
        int splitter_index = curLine.find('|');
        if (splitter_index == std::string::npos) {
            std::cout << "Error\n";
            break;
        }
        std::string name = curLine.substr(0, splitter_index);

        Timer t(name, std::stoi(curLine.substr(splitter_index + 1)));
        TimerWidget *w = new TimerWidget(t);
        TimerWidget::connect(w, &TimerWidget::deleteRequested, this, &MainWindow::reorganizeTimerWidgets);
        leftLayout->addWidget(w, curRow, curColumn);
        w->setParent(leftLayoutWidget);
        timers.push_back(t);

        ++curColumn;
        if (curColumn >= MAX_COLUMNS) {
            ++curRow;
            curColumn = 0;
        }
    }
    return true;
}

void MainWindow::reorganizeTimerWidgets(TimerWidget* toBeDeleted) {
    int timerToRemove = -1;
    for (int i = 0; i < timers.size(); ++i) {
        if (timers.at(i) == toBeDeleted->getTimer()) timerToRemove = i;
    }
    if (timerToRemove != -1) timers.erase(timers.begin() + timerToRemove);

    centralWidget()->deleteLater();

    setupUi();

    curRow = 0;
    curColumn = 0;


    foreach (Timer timer, timers) {
        TimerWidget *w = new TimerWidget(timer);
        TimerWidget::connect(w, &TimerWidget::deleteRequested, this, &MainWindow::reorganizeTimerWidgets);
        leftLayout->addWidget(w, curRow, curColumn);
        w->setParent(leftLayoutWidget);

        ++curColumn;
        if (curColumn >= MAX_COLUMNS) {
            ++curRow;
            curColumn = 0;
        }
    }

    saveToFile();
}

void MainWindow::addTimerButtonClicked(bool checked) {
    std::string timerName = newTimerName->text().toStdString();
    int timerDuration = -1;
    if (timerName == "") return;
    try {
        timerDuration = std::stoi(newTimerDuration->text().toStdString());
    } catch (std::exception e) {
        QMessageBox::critical(this, "Error", "Timer duration must be a valid number of seconds.");
        return;
    }

    if (timerDuration <= 0) {
        QMessageBox::critical(this, "Error", "Timer duration must be a valid number of seconds.");
        return;
    }

    Timer timer(timerName, timerDuration);
    TimerWidget *w = new TimerWidget(timer);
    TimerWidget::connect(w, &TimerWidget::deleteRequested, this, &MainWindow::reorganizeTimerWidgets);
    leftLayout->addWidget(w, curRow, curColumn);
    w->setParent(leftLayoutWidget);
    timers.push_back(timer);

    ++curColumn;
    if (curColumn >= MAX_COLUMNS) {
        ++curRow;
        curColumn = 0;
    }

    saveToFile();
}



