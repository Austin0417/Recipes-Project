#include "ListViewDelegate.h"


void ListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyleOptionViewItem myOption = option;
    initStyleOption(&myOption, index);
    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    if (!icon.isNull()) {
        QSize iconSize = myOption.decorationSize;
        myOption.rect.setWidth(myOption.rect.width() - iconSize.height());
    }
    QStyledItemDelegate::paint(painter, myOption, index);
}
