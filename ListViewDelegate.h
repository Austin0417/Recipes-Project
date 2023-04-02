#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H
#include <QStyledItemDelegate>
#include "mainwindow.h"

class ListViewDelegate : public QStyledItemDelegate {
public:
    ListViewDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {
        qDebug() << "Creating list view delegate...";
    }
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:

};

#endif // LISTVIEWDELEGATE_H
