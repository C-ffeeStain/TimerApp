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
}

bool MainWindow::saveToFile() {
    std::string filePath;
    try {
        filePath = std::getenv("LOCALAPPDATA");
    } catch (std::exception e) {
        std::cout << "An error occured trying to save to file.";
        return false;
    }
    filePath += "\\TimerApp";

    std::filesystem::create_directory(filePath);
    std::ofstream output(filePath + "\\timers.txt");

    for (int i = 0; i < timers.size(); ++i) {
        const Timer timer = timers.at(i);

        output << timer.name << "|" << timer.duration;

        if (i != timers.size() - 1) output << "\n";
    }
    output.close();
    return true;
}

MainWindow::~MainWindow() = default;
