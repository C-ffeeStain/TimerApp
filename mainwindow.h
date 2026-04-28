#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "timer_widget.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QGridLayout>
#include <QFormLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setupFilePath();
    bool saveToFile();
    bool loadFromFile();

private:
    std::string timersFilePath;
    int curRow = 0;
    int curColumn = 0;

    std::vector<Timer> timers;

    QHBoxLayout *mainLayout;
    QGridLayout *leftLayout;
    QWidget *leftLayoutWidget;
    QFormLayout *rightLayout;

    QLineEdit *newTimerName;
    QLineEdit *newTimerDuration;

    void reorganizeTimerWidgets(TimerWidget* toBeDeleted);
};
#endif // MAINWINDOW_H
