#include "RecipeDialog.h"


void RecipeDialog::setWidgetText(Recipe* recipe) {
    RecipeDialog::setWindowTitle(recipe->getName());
    ingredientsText->setText(recipe->getIngredients());
    instructionsText->setText(recipe->getInstructions());
}
void RecipeDialog::onEditButtonClicked() {
    ingredientsText->setReadOnly(false);
    instructionsText->setReadOnly(false);
    QMessageBox::information(this, "Edit recipe", "You can now edit the recipe.");
}
