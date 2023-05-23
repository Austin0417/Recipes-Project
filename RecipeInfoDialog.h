#ifndef RECIPEINFODIALOG_H
#define RECIPEINFODIALOG_H
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
class MainWindow;

class RecipeInfoDialog : public QDialog {
public:
    RecipeInfoDialog(QWidget* parent = nullptr) : QDialog(parent) {
        layout = new QFormLayout(this);
        recipeName = new QLineEdit(this);
        recipeIngredients = new QTextEdit(this);
        recipeInstructions = new QTextEdit(this);
        enterBtn = new QPushButton("Enter");
        layout->addRow(new QLabel(tr("Recipe Name: "), this), recipeName);
        layout->addRow(new QLabel(tr("Recipe Ingredients: "), this), recipeIngredients);
        layout->addRow(new QLabel(tr("Recipe Instructions: "), this), recipeInstructions);
        layout->addWidget(enterBtn);
        connect(enterBtn, &QPushButton::clicked, this, &RecipeInfoDialog::onEnterButtonClicked);
    }
    ~RecipeInfoDialog() {
        delete layout;
        delete recipeName;
        delete recipeIngredients;
        delete recipeInstructions;
        delete enterBtn;
    }
    void onEnterButtonClicked();
private:
    QFormLayout* layout;
    QLineEdit* recipeName;
    QTextEdit* recipeIngredients;
    QTextEdit* recipeInstructions;
    QPushButton* enterBtn;
};

#endif // RECIPEINFODIALOG_H
