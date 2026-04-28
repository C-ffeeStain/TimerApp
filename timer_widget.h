#ifndef TIMER_WIDGET_H
#define TIMER_WIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>
#include <QTimer>
#include <QLabel>

struct Timer {
    std::string name = "Unnamed";
    unsigned int duration = 30;
    unsigned int timeLeft = 0;
    bool paused = true;

    Timer();
    Timer(std::string name, int duration);
    bool operator==(Timer other);
};

class TimerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimerWidget(const Timer timer, QWidget *parent = nullptr);
    ~TimerWidget() override;
    QString secondsToQString(int seconds) const;
    Timer getTimer() const;
signals:
    void deleteRequested(TimerWidget* toBeDeleted);
private:
    Timer timer;
    QTimer *qTimer;

    QLabel *timerDurationLabel;

    QPushButton *deleteButton;
    QPushButton *startButton;
    QPushButton *resetButton;

    void startButtonToggled(bool checked = false);
    void resetButtonClicked(bool checked = false);
    void deleteButtonClicked(bool checked = false);
    void tick();
};
#endif // TIMER_WIDGET_H
