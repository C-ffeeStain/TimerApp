#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "timer_widget.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void extracted(std::ofstream &output);
    bool saveToFile();

private:
    std::vector<Timer> timers;
    QVBoxLayout *rightLayout;

    QLineEdit *timerNameEdit;
    QLineEdit *timerDurationEdit;

    QPushButton *addTimerButton;
    QPushButton *addTimerFormButton;
};
#endif // MAINWINDOW_H
