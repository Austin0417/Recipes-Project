#include "RecipeDialog.h"


void RecipeDialog::setWidgetText(Recipe* recipe) {
    ingredientsText->setText(recipe->getIngredients());
    instructionsText->setText(recipe->getInstructions());
}
void RecipeDialog::onEditButtonClicked() {
    ingredientsText->setReadOnly(false);
    instructionsText->setReadOnly(false);
    QMessageBox::information(this, "Edit recipe", "You can now edit the recipe.");
}
