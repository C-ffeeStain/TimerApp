#ifndef TIMER_WIDGET_H
#define TIMER_WIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>
#include <QLabel>
// #include <QProgressBar>

struct Timer {
    std::string name = "Unnamed";
    int duration = 30;

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
private:
    int timerDuration;
    int timeLeft;
    bool paused;

    QLabel *timerDurationLabel;
    // QProgressBar *timerProgressBar;

    QPushButton *deleteButton;
    QPushButton *playButton;
    QPushButton *resetButton;
};
#endif // TIMER_WIDGET_H
