#ifndef RECIPEDIALOG_H
#define RECIPEDIALOG_H
#include "Recipe.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>

class RecipeDialog : public QDialog {
public:
    RecipeDialog(QWidget* parent = nullptr) : QDialog(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        ingredientsLabel = new QLabel("Ingredients", this);
        instructionsLabel = new QLabel("Instructions", this);
        ingredientsText = new QTextEdit(this);
        instructionsText = new QTextEdit(this);
        editBtn = new QPushButton("Edit");
        ingredientsText->setReadOnly(true);
        instructionsText->setReadOnly(true);
        layout->addWidget(ingredientsLabel);
        layout->addWidget(instructionsLabel);
        layout->addWidget(ingredientsText);
        layout->addWidget(instructionsText);
        layout->addWidget(editBtn);
        connect(editBtn, &QPushButton::clicked, this, &RecipeDialog::onEditButtonClicked);
    }
    void setWidgetText(Recipe* recipe);
    void onEditButtonClicked();
private:
    QLabel* ingredientsLabel;
    QLabel* instructionsLabel;
    QTextEdit* ingredientsText;
    QTextEdit* instructionsText;
    QPushButton* editBtn;
};

#endif // RECIPEDIALOG_H
