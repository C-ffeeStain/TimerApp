#ifndef TIMER_WIDGET_H
#define TIMER_WIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>
#include <QLabel>
// #include <QProgressBar>

struct Timer {
    std::string name = "Unnamed";
    unsigned int duration = 30;
    unsigned int timeLeft = 0;
    bool paused = true;

    Timer();
    Timer(std::string name, int duration);
};

class TimerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimerWidget(QWidget *parent = nullptr, Timer timerObj = Timer());
    ~TimerWidget() override;
    QString secondsToQString(int seconds);
    Timer getTimer();
private:
    Timer timer;

    QLabel *timerDurationLabel;
    // QProgressBar *timerProgressBar;

    QPushButton *deleteButton;
    QPushButton *startButton;
    QPushButton *resetButton;
};
#endif // TIMER_WIDGET_H
