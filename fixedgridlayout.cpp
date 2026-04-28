#include "fixedgridlayout.h"
#include <QWidget>

FixedGridLayout::FixedGridLayout(int maxRows, int maxColumns, LayoutMode layoutMode, QWidget* parent) : QGridLayout(parent) {
    this->maxRows = maxRows;
    this->maxColumns = maxColumns;

    currentColumn = 0;
    currentRow = 0;
}

int FixedGridLayout::getMaxRows() const {
    return maxRows;
}
int FixedGridLayout::getMaxColumns() const {
    return maxColumns;
}

std::vector<TimerWidget*> FixedGridLayout::getLayoutChildren() {
    return layoutChildren;
}

void FixedGridLayout::refresh(TimerWidget *removedElement) {
    currentColumn = 0;
    currentRow = 0;

    std::vector<TimerWidget*> newLayoutChildren;
    foreach (TimerWidget *widget, layoutChildren) {
        if (widget == nullptr) continue;
        if (removedElement != nullptr && removedElement == widget) continue;

        removeWidget(widget);
        addWidget(widget, currentRow, currentColumn);
        incrementPosition();
        newLayoutChildren.push_back(widget);
    }
    layoutChildren.clear();
    layoutChildren.swap(newLayoutChildren);
}

void FixedGridLayout::addWidget(TimerWidget *widget, int row, int column, Qt::Alignment alignment) {
    if (layoutChildren.size() == maxRows * maxColumns) {
        throw std::runtime_error("Could not add widget, already at max capacity");
        return; // is this necessary?
    }

    TimerWidget::connect(widget, &TimerWidget::deleteRequested, this, &FixedGridLayout::onChildDestroyed);

    QGridLayout::addWidget(widget, row, column, alignment);

    if (row == currentRow && column == currentColumn) {
        layoutChildren.push_back(widget);
    } else if (layoutMode == LayoutMode::ColumnsFirst) {
        layoutChildren.insert(layoutChildren.begin() + (row * 3) + column, widget);
    } else {
        layoutChildren.insert(layoutChildren.begin() + (column * 3) + row, widget);
    }

    incrementPosition();
}

void FixedGridLayout::addWidgetToEnd(TimerWidget* widget) {
    addWidget(widget, currentRow, currentColumn);
}

void FixedGridLayout::incrementPosition() {
    if (layoutMode == LayoutMode::ColumnsFirst) {
        ++currentColumn;
        if (currentColumn == maxColumns - 1) {
            currentColumn = 0;
            ++currentRow;
        }
    } else {
        ++currentRow;
        if (currentRow == maxRows - 1) {
            currentRow = 0;
            ++currentColumn;
        }
    }
}
void FixedGridLayout::onChildDestroyed(QObject *obj) {
    TimerWidget *widget = (TimerWidget*)obj;
    TimerWidget::disconnect(widget, &TimerWidget::destroyed, this, &FixedGridLayout::onChildDestroyed);

    refresh(widget);
    obj->deleteLater();
    emit childDestroyed(widget);
}