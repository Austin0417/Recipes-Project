#include "ListViewDelegate.h"


void ListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyleOptionViewItem myOption = option;
    if (index.isValid() && index.model()) {
        Recipe* recipe = index.model()->data(index, Qt::UserRole).value<Recipe*>();
        if (recipe->getFavoritedStatus()) {
            myOption.backgroundBrush = QBrush(Qt::darkYellow);
        }
    }
    QStyledItemDelegate::paint(painter, myOption, index);
}
