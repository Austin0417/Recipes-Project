#include "RecipeDialog.h"


void RecipeDialog::setWidgetText(Recipe* recipe) {
    setWindowTitle(recipe->getName());
    RecipeDialog::setWindowTitle(recipe->getName());
    ingredientsText->setText(recipe->getIngredients());
    instructionsText->setText(recipe->getInstructions());
}
void RecipeDialog::onEditButtonClicked() {
    ingredientsText->setReadOnly(false);
    instructionsText->setReadOnly(false);
    saveBtn->setEnabled(true);
    QMessageBox::information(this, "Edit recipe", "You can now edit the recipe.");
}

void RecipeDialog::onSaveButtonClicked() {
    currentRecipe->listIngredients(ingredientsText->toPlainText());
    currentRecipe->setInstructions(instructionsText->toPlainText());
    if (window->getLoginStatus()) {
        QSqlQuery query(*db);
        query.prepare("UPDATE UserSavedRecipes SET ingredients = ?, instructions = ? WHERE id = ? AND name = ?");
        query.addBindValue(ingredientsText->toPlainText());
        query.addBindValue(instructionsText->toPlainText());
        query.addBindValue(window->getCurrentUserId());
        query.addBindValue(currentRecipe->getName());
        if (query.exec()) {
            qDebug() << "Successfully updated ingredients/instructions for " << currentRecipe->getName();
        }

    }
    close();
}
