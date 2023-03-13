#include "RecipeListModel.h"

int RecipeListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_recipes.count();
}
QVariant RecipeListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    Recipe* recipe = m_recipes[index.row()];
    if (role == Qt::DisplayRole) {
        return recipe->getName();
    }
    if (role == Qt::UserRole) {
        return QVariant::fromValue(recipe);
    }
    return QVariant();
}
void RecipeListModel::addRecipe(Recipe* recipe) {
    m_recipes.append(recipe);
}
QList<Recipe*> RecipeListModel::getList() {
    return m_recipes;
}
