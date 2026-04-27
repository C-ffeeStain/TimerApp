#ifndef FIXEDGRIDLAYOUT_H
#define FIXEDGRIDLAYOUT_H

#include <QGridLayout>

enum LayoutMode {
    ColumnsFirst,
    RowsFirst
};

class FixedGridLayout : public QGridLayout
{
    Q_OBJECT
public:
    FixedGridLayout(int maxRows, int maxColumns, LayoutMode layoutMode = LayoutMode::ColumnsFirst, QWidget *parent = nullptr);

    int getMaxRows() const;
    int getMaxColumns() const;
    std::vector<QWidget*> getLayoutChildren();
    void refresh(QWidget *removedElement = nullptr);

    LayoutMode getLayoutMode();
    void setLayoutMode(LayoutMode layoutMode);

    void addWidgetToEnd(QWidget* widget);
signals:
    void childDestroyed(QWidget *child);
private:
    LayoutMode layoutMode;
    int currentRow;
    int currentColumn;
    int maxRows;
    int maxColumns;

    std::vector<QWidget*> layoutChildren;

    void incrementPosition();
    void onChildDestroyed(QObject* obj);
};

#endif // FIXEDGRIDLAYOUT_H
