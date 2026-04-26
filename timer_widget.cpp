#include "timer_widget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>

Timer::Timer() : name("Unnamed"), duration(30) {};
Timer::Timer(std::string name, int duration) : name(name), duration(duration) {};


TimerWidget::TimerWidget(QWidget *parent, Timer timerObj) : QWidget(parent)
{
    this->timerDuration = timerObj.duration;

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

    this->playButton = new QPushButton;
    this->playButton->setCheckable(true);
    this->playButton->setIcon(QIcon("imgs/play_button.png"));

    this->resetButton = new QPushButton;
    this->resetButton->setIcon(QIcon("imgs/reset_button.png"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(this->playButton);
    buttonLayout->addWidget(this->resetButton);

    // timer label itself
    this->timerDurationLabel = new QLabel;
    this->timerDurationLabel->setText(secondsToQString(this->timerDuration));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(headerLayout);
    layout->addWidget(this->timerDurationLabel);
    layout->addLayout(buttonLayout);
    setLayout(layout);
}

QString TimerWidget::secondsToQString(int seconds) {
    QTime time(0, 0);
    time = time.addSecs(seconds);
    return time.toString("hh:mm:ss");
}

TimerWidget::~TimerWidget() = default;
