#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "timer_widget.h"

#include <QMainWindow>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    bool saveToFile();
    bool loadFromFile();
    void setupFilePath();

private:
    std::string timersFilePath;
    int curRow = 0;
    int curColumn = 0;

    std::vector<TimerWidget> timerWidgets;

    QGridLayout *leftLayout;
    QVBoxLayout *rightLayout;

    QLineEdit *timerNameEdit;
    QLineEdit *timerDurationEdit;

    QPushButton *addTimerButton;
    QPushButton *addTimerFormButton;
};
#endif // MAINWINDOW_H
