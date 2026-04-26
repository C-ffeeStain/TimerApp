#include "timer_widget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>

Timer::Timer() : name("Unnamed"), duration(30) {};
Timer::Timer(std::string name, int duration) : name(name), duration(duration) {};

Timer TimerWidget::getTimer() {
    return timer;
}

TimerWidget::TimerWidget(QWidget *parent, Timer timerObj) : QWidget(parent)
{
    this->timer = timerObj;

    // creating the entire UI manually because Qt Creator sucks

    // "header" layout

    QLabel *timerNameLabel = new QLabel;
    timerNameLabel->setText(QString::fromStdString(timerObj.name));

    this->deleteButton = new QPushButton;
    this->deleteButton->setText("X");
    this->deleteButton->setFlat(true);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(timerNameLabel);
    headerLayout->addWidget(this->deleteButton);

    // button layout

    this->startButton = new QPushButton;
    this->startButton->setText("Start");
    this->startButton->setCheckable(true);

    this->resetButton = new QPushButton;

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(this->startButton, 0, Qt::AlignRight);
    buttonLayout->addWidget(this->resetButton);
    buttonLayout->addWidget(this->resetButton, 0, Qt::AlignLeft);

    // timer label itself
    this->timerDurationLabel = new QLabel;
    this->timerDurationLabel->setText(secondsToQString(this->timer.duration));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(headerLayout);
    layout->addWidget(this->timerDurationLabel);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    this->resetButton->show();
    this->deleteButton->show();
    this->timerDurationLabel->show();
    timerNameLabel->show();
}

QString TimerWidget::secondsToQString(int seconds) {
    QTime time(0, 0);
    time = time.addSecs(seconds);
    return time.toString("hh:mm:ss");
}

TimerWidget::~TimerWidget() = default;
