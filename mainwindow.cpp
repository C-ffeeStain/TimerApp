#include "mainwindow.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLineEdit>
#include <QLabel>

#include <fstream>
#include <filesystem>
#include <iostream>

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

    QHBoxLayout *layout = new QHBoxLayout;
    // layout->addWidget(testLabel);
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
        int duration = std::stoi(curLine.substr(splitter_index + 1));
        timers.push_back(Timer(name, duration));
    }
    return true;
}

MainWindow::~MainWindow() = default;
