#include "Recipe.h"

void Recipe::setName(QString name_) {
    name = name_;
}

void Recipe::listIngredients(QString ingredients_) {
    ingredients = ingredients_;
}

void Recipe::setInstructions(QString instructions_) {
    instructions = instructions_;
}

QString Recipe::getName() {
    return name;
}

QString Recipe::getIngredients() {
    return ingredients;
}

QString Recipe::getInstructions() {
    return instructions;
}

void Recipe::setFavorited(bool status) {
    isFavorited = status;
}

bool Recipe::getFavoritedStatus() {
    return isFavorited;
}

