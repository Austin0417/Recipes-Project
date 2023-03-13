#include "Recipe.h"

void Recipe::setName(QString name_) {
    name = name_;
}

void Recipe::listIngredients(QString ingredients_) {
    ingredients = ingredients_;
}

QString Recipe::getIngredients() {
    return ingredients;
}

QString Recipe::getName() {
    return name;
}

void Recipe::setInstructions(QString instructions_) {
    instructions = instructions_;
}

QString Recipe::getInstructions() {
    return instructions;
}
