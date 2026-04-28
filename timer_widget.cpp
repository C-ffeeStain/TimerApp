#include "timer_widget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QSystemTrayIcon>
#include <QMenu>

Timer::Timer() : name("Unnamed"), duration(30) {};
Timer::Timer(std::string name, int duration) : name(name), duration(duration) {};

TimerWidget::TimerWidget(const Timer timer, QWidget *parent) : QFrame(parent)
{
    setMaximumSize(250, 200);
    setStyleSheet("TimerWidget {border: 1px solid white; padding: 5px; border-radius: 10px}");

    qTimer = new QTimer;
    this->timer = timer;

    QTimer::connect(qTimer, &QTimer::timeout, this, &TimerWidget::tick);

    // creating the entire UI manually because Qt Creator sucks

    // "header" layout

    QLabel *timerNameLabel = new QLabel;
    timerNameLabel->setText(QString::fromStdString(timer.name));
    timerNameLabel->setMaximumHeight(25);

    deleteButton = new QPushButton;
    deleteButton->setText("X");
    deleteButton->setStyleSheet("QPushButton {border: none; background: transparent;}");
    deleteButton->setMaximumWidth(25);
    deleteButton->setMaximumHeight(25);
    QPushButton::connect(deleteButton, &QPushButton::clicked, this, &TimerWidget::deleteButtonClicked);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(timerNameLabel);
    headerLayout->addWidget(deleteButton);

    // button layout

    startButton = new QPushButton;
    startButton->setText("Start");
    startButton->setCheckable(true);
    QPushButton::connect(startButton, &QPushButton::toggled, this, &TimerWidget::startButtonToggled);

    resetButton = new QPushButton;
    resetButton->setText("Reset");
    QPushButton::connect(resetButton, &QPushButton::clicked, this, &TimerWidget::resetButtonClicked);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(startButton, 0, Qt::AlignRight);
    buttonLayout->addWidget(resetButton, 0, Qt::AlignLeft);

    // timer label itself
    timerDurationLabel = new QLabel;
    timerDurationLabel->setText(secondsToQString(timer.duration));
    timerDurationLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    timerDurationLabel->setStyleSheet("QLabel{\nfont-size: 35pt;\n}");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(headerLayout);
    layout->addWidget(timerDurationLabel);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    if (!timer.paused && timer.timeLeft > 0) startButtonToggled(true);
}

QString TimerWidget::secondsToQString(int seconds) const {
    QTime time(0, 0);
    time = time.addSecs(seconds);
    return time.toString("hh:mm:ss");
}

TimerWidget::~TimerWidget() = default;

Timer TimerWidget::getTimer() const {
    return timer;
}

void TimerWidget::startButtonToggled(bool checked) {
    if (checked) {
        if (timer.timeLeft == 0) timer.timeLeft = timer.duration;

        startButton->setText("Pause");
        timer.paused = false;
        qTimer->start(1000);
        tick();
    } else {
        startButton->setText("Start");
        qTimer->stop();
        timer.paused = true;
    }
}

void TimerWidget::resetButtonClicked(bool checked) {
    timer.timeLeft = timer.duration;
    timerDurationLabel->setText(secondsToQString(timer.timeLeft));
    if (!timer.paused) {
        // restart timer to avoid awkward timing with reset
        qTimer->stop();
        qTimer->start(1000);
    }
}

bool Timer::operator==(Timer other) {
    if (other.duration == duration && other.name == name) return true;

    return false;
}

void TimerWidget::deleteButtonClicked(bool checked) { emit deleteRequested(this); }

void TimerWidget::tick() {
    if (timer.paused) return;

    if (timer.timeLeft <= 0) {
        qTimer->stop();
        QSystemTrayIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::CallStop));
        QMenu *menu = new QMenu("Timer Finished");
        icon.setContextMenu(menu);
        icon.show();
        icon.showMessage("Timer Finished!", QString::fromStdString("Your timer " + timer.name + " has finished."));
        return;
    }
    --timer.timeLeft;
    timerDurationLabel->setText(secondsToQString(timer.timeLeft));
}