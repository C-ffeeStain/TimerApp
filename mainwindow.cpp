#include "mainwindow.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLineEdit>
#include <QLabel>

#include <fstream>
#include <filesystem>
#include <iostream>

const int MAX_COLUMNS = 3;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // creating the entire UI manually because Qt Creator sucks
    setupFilePath();

    QFormLayout *timerAdditionLayout = new QFormLayout;

    this->timerNameEdit = new QLineEdit;
    this->timerNameEdit->setPlaceholderText("Example timer name");

    this->timerDurationEdit = new QLineEdit;
    this->timerDurationEdit->setPlaceholderText("01:30:00");

    this->addTimerFormButton = new QPushButton;
    this->addTimerFormButton->setText("Add Timer");

    timerAdditionLayout->addRow("Timer name:", this->timerNameEdit);
    timerAdditionLayout->addRow("Timer duration:", this->timerDurationEdit);

    this->rightLayout = new QVBoxLayout;
    this->rightLayout->addLayout(timerAdditionLayout);
    this->rightLayout->addWidget(this->addTimerFormButton);

    this->leftLayout = new QGridLayout;

    QHBoxLayout *layout = new QHBoxLayout;
    // layout->addWidget(testLabel);
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    QWidget *window = new QWidget();
    window->setLayout(layout);

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

    for (int i = 0; i < timerWidgets.size(); ++i) {
        const Timer timer = timerWidgets.at(i).getTimer();

        output << timer.name << "|" << timer.duration;

        if (i != timerWidgets.size() - 1) output << "\n";
    }
    output.close();
    return true;
}

bool MainWindow::loadFromFile() {
    for (int i = 0; i < timerWidgets.size(); ++i) {
        timerWidgets.at(i).deleteLater();
    }
    timerWidgets.clear();

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

        TimerWidget *w = new TimerWidget(nullptr, Timer(name, std::stoi(curLine.substr(splitter_index + 1))));
        leftLayout->addWidget(w, curRow, curColumn);
        ++curColumn;
        if (curColumn >= MAX_COLUMNS) {
            ++curRow;
            curColumn = 0;
        }
    }
    return true;
}

MainWindow::~MainWindow() = default;
