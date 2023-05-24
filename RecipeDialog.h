#ifndef RECIPEDIALOG_H
#define RECIPEDIALOG_H
#include "Recipe.h"
#include "mainwindow.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>

class RecipeDialog : public QDialog {
public:
    RecipeDialog(Recipe* recipe_, QSqlDatabase* db_, QWidget* parent_ = nullptr) : currentRecipe(recipe_), db(db_), QDialog(parent_) {
        window = qobject_cast<MainWindow*>(parent());

        layout = new QVBoxLayout(this);
        ingredientsLabel = new QLabel("Ingredients", this);
        instructionsLabel = new QLabel("Instructions", this);
        ingredientsText = new QTextEdit(this);
        instructionsText = new QTextEdit(this);
        editBtn = new QPushButton("Edit");
        saveBtn = new QPushButton("Save");
        saveBtn->setEnabled(false);
        ingredientsText->setReadOnly(true);
        instructionsText->setReadOnly(true);
        layout->addWidget(ingredientsLabel);
        layout->addWidget(ingredientsText);
        layout->addWidget(instructionsLabel);
        layout->addWidget(instructionsText);
        layout->addWidget(editBtn);
        layout->addWidget(saveBtn);

        connect(editBtn, &QPushButton::clicked, this, &RecipeDialog::onEditButtonClicked);
        connect(saveBtn, &QPushButton::clicked, this, &RecipeDialog::onSaveButtonClicked);
    }
    void setWidgetText(Recipe* recipe);
    void onEditButtonClicked();
    void onSaveButtonClicked();
    ~RecipeDialog() {
        delete layout;
        delete ingredientsLabel;
        delete instructionsLabel;
        delete ingredientsText;
        delete instructionsText;
        delete editBtn;
    }
private:
    QVBoxLayout* layout;
    QLabel* ingredientsLabel;
    QLabel* instructionsLabel;
    QTextEdit* ingredientsText;
    QTextEdit* instructionsText;
    QPushButton* editBtn;
    QPushButton* saveBtn;
    Recipe* currentRecipe;
    QSqlDatabase* db;
    MainWindow* window;
};

#endif // RECIPEDIALOG_H
