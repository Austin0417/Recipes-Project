#ifndef RECIPEINFODIALOG_H
#define RECIPEINFODIALOG_H
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class RecipeInfoDialog : public QDialog {
public:
    RecipeInfoDialog(QWidget* parent = nullptr) : QDialog(parent) {
        layout = new QFormLayout(this);
        recipeName = new QLineEdit(this);
        recipeIngredients = new QLineEdit(this);
        recipeInstructions = new QLineEdit(this);
        enterBtn = new QPushButton("Enter");
        layout->addWidget(recipeName);
        layout->addWidget(recipeIngredients);
        layout->addWidget(recipeInstructions);
        layout->addWidget(enterBtn);
    }
private:
    QFormLayout* layout;
    QLineEdit* recipeName;
    QLineEdit* recipeIngredients;
    QLineEdit* recipeInstructions;
    QPushButton* enterBtn;
};

#endif // RECIPEINFODIALOG_H
