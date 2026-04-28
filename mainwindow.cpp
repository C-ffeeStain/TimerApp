#include "mainwindow.h"

#include <fstream>
#include <filesystem>
#include <iostream>

const int MAX_COLUMNS = 3;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(600, 600);
    // creating the entire UI manually because Qt Creator sucks
    setupFilePath();

    newTimerName = new QLineEdit;
    newTimerDuration = new QLineEdit;

    rightLayout = new QFormLayout;
    rightLayout->addRow("Timer name:", newTimerName);
    rightLayout->addRow("Timer duration (seconds): ", newTimerDuration);

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

    loadFromFile();
}

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
    QWidget* central = centralWidget();
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

    leftLayoutWidget->deleteLater();

    newTimerName = new QLineEdit;
    newTimerDuration = new QLineEdit;

    rightLayout = new QFormLayout;
    rightLayout->addRow("Timer name:", newTimerName);
    rightLayout->addRow("Timer duration (seconds): ", newTimerDuration);

    QWidget* rightLayoutWidget = new QWidget;
    rightLayoutWidget->setMaximumWidth(280);
    rightLayoutWidget->setLayout(rightLayout);

    leftLayout = new QGridLayout;

    leftLayoutWidget = new QWidget;
    leftLayoutWidget->setLayout(leftLayout);

    mainLayout = new QHBoxLayout;
    mainLayout->addWidget(leftLayoutWidget);
    mainLayout->addWidget(rightLayoutWidget);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);

    // Set QWidget as the central layout of the main window
    setCentralWidget(window);

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
}

MainWindow::~MainWindow() = default;
