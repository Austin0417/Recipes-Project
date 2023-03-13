#ifndef RECIPELISTMODEL_H
#define RECIPELISTMODEL_H
#include "Recipe.h"
#include <QVariant>
#include <QAbstractListModel>

class RecipeListModel : public QAbstractListModel {
public:
    RecipeListModel(QList<Recipe*> recipes, QObject* parent = nullptr) : QAbstractListModel(parent), m_recipes(recipes) {}
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addRecipe(Recipe* recipe);
    QList<Recipe*> getList();


private:
    QList<Recipe*> m_recipes;
};
#endif // RECIPELISTMODEL_H
