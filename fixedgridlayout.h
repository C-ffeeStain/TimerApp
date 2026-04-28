#ifndef FIXEDGRIDLAYOUT_H
#define FIXEDGRIDLAYOUT_H

#include <QGridLayout>
#include "timer_widget.h"


class FixedGridLayout : public QGridLayout
{
    Q_OBJECT
public:
    enum LayoutMode {
        ColumnsFirst,
        RowsFirst
    };

    FixedGridLayout(int maxRows, int maxColumns, LayoutMode layoutMode = LayoutMode::ColumnsFirst, QWidget *parent = nullptr);

    int getMaxRows() const;
    int getMaxColumns() const;
    std::vector<TimerWidget*> getLayoutChildren();
    void refresh(TimerWidget *removedElement = nullptr);

    LayoutMode getLayoutMode();
    void setLayoutMode(LayoutMode layoutMode);

    void addWidgetToEnd(TimerWidget* widget);
    void addWidget(TimerWidget *widget, int row, int column, Qt::Alignment alignment = Qt::Alignment());
signals:
    void childDestroyed(TimerWidget *child);
private:
    LayoutMode layoutMode;
    int currentRow;
    int currentColumn;
    int maxRows;
    int maxColumns;

    std::vector<TimerWidget*> layoutChildren;

    void incrementPosition();
    void onChildDestroyed(QObject* obj);
};

#endif // FIXEDGRIDLAYOUT_H
