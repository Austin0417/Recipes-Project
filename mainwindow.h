#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QTextStream>
#include <QInputDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QListView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QCalendarWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class Recipe {
public:
    void setName(QString name);
    void listIngredients(QString ingredients);
    void setInstructions(QString instructions);
    QString getName();
    QString getIngredients();
    QString getInstructions();

private:
    QString name;
    QString ingredients;
    QString instructions;
};


class RecipeListModel : public QAbstractListModel {
public:
    RecipeListModel(QList<Recipe*> recipes, QObject* parent = nullptr) : QAbstractListModel(parent), m_recipes(recipes) {}
    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid()) {
            return 0;
        }
        return m_recipes.count();
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) {
            return QVariant();
        }
        Recipe* recipe = m_recipes[index.row()];
        if (role == Qt::DisplayRole) {
            return recipe->getName();
        }
        if (role == Qt::UserRole) {
            return QVariant::fromValue(recipe);
        }
        return QVariant();
    }
    void addRecipe(Recipe* recipe) {
        m_recipes.append(recipe);
    }
private:
    QList<Recipe*> m_recipes;
};

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
    void setWidgetText(Recipe* recipe) {
        ingredientsText->setText(recipe->getIngredients());
        instructionsText->setText(recipe->getInstructions());
    }
    void onEditButtonClicked() {
        ingredientsText->setReadOnly(false);
        instructionsText->setReadOnly(false);
        QMessageBox::information(this, "Edit recipe", "You can now edit the recipe.");
    }
private:
    QLabel* ingredientsLabel;
    QLabel* instructionsLabel;
    QTextEdit* ingredientsText;
    QTextEdit* instructionsText;
    QPushButton* editBtn;
};

class CalendarDialog : public QDialog {
public:
    CalendarDialog(QWidget* parent = nullptr) : QDialog(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        calendar = new QCalendarWidget(this);
        layout->addWidget(calendar);

    }
private:
    QCalendarWidget* calendar;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onSearchClicked();
    void onAddClicked();
    void onRemoveClicked();
    void onCalendarBtnClicked();
    void onListItemClicked(const QModelIndex& index);
    int findRecipe(QString name);
    QList<Recipe*> getRecipeList();

private:
    Ui::MainWindow *ui;
    QPushButton* addBtn;
    QPushButton* searchBtn;
    QList <Recipe*> recipeList;
    QListView* listView;
    RecipeListModel* model;
    QPushButton* removeBtn;
    QPushButton* calendarBtn;
};

#endif // MAINWINDOW_H
