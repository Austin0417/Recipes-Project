#include "RecipeInfoDialog.h"
#include "mainwindow.h"

void RecipeInfoDialog::onEnterButtonClicked() {
      if (recipeName->text().isEmpty() || recipeIngredients->toPlainText().isEmpty() || recipeInstructions->toPlainText().isEmpty()) {
          QMessageBox::information(this, "Error adding recipe", "One or more fields are empty. Please try again.");
          return;
      }
      MainWindow* window = qobject_cast<MainWindow*>(parent());
      window->getRecipeInformation()->append(recipeName->text());
      window->getRecipeInformation()->append(recipeIngredients->toPlainText());
      window->getRecipeInformation()->append(recipeInstructions->toPlainText());
      this->close();
}

