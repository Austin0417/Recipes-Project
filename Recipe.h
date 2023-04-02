#ifndef RECIPE_H
#define RECIPE_H
#include <QString>
#include <QAbstractItemView>
#include <QMouseEvent>
#include <QMenu>


class Recipe  {
public:
    Recipe(QString name_, QString ingredients_, QString instructions_) : name(name_), ingredients(ingredients_), instructions(instructions_) {}
    void setName(QString name);
    void listIngredients(QString ingredients);
    void setInstructions(QString instructions);
    QString getName();
    QString getIngredients();
    QString getInstructions();
    void setFavorited(bool status);
    bool getFavoritedStatus();

private:
    QString name;
    QString ingredients;
    QString instructions;
    bool isFavorited = false;
};
#endif // RECIPE_H
