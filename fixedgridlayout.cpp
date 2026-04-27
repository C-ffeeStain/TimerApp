#include "fixedgridlayout.h"
#include <QWidget>;

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

std::vector<QWidget*> FixedGridLayout::getLayoutChildren() {
    return layoutChildren;
}

void FixedGridLayout::refresh(QWidget *removedElement) {
    currentColumn = 0;
    currentRow = 0;

    std::vector<QWidget*> newLayoutChildren;
    foreach (QWidget *widget, layoutChildren) {
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

void FixedGridLayout::addWidgetToEnd(QWidget* widget) {
    if (layoutChildren.size() == maxRows * maxColumns) {
        throw std::runtime_error("Could not add widget, already at max capacity");
        return; // is this necessary?
    }

    QWidget::connect(widget, &QWidget::destroyed, this, &FixedGridLayout::onChildDestroyed);
    addWidget(widget, currentRow, currentColumn);
    incrementPosition();
    layoutChildren.push_back(widget);
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
    emit childDestroyed((QWidget*)obj);
    refresh();
}