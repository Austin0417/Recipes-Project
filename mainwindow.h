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
    Recipe(QString name_, QString ingredients_, QString instructions_) : name(name_), ingredients(ingredients_), instructions(instructions_) {}
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
    QList<Recipe*> getList() {
        return m_recipes;
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

class MyDate : public QDate {
public:
    MyDate() : QDate() {}
    void setEvent(QString event_) {
        event = event_;
    }

private:
    QString event;
};


class CalendarDialog : public QDialog {
    Q_OBJECT
public:
    CalendarDialog(QWidget* parent = nullptr) : QDialog(parent) {
        layout = new QVBoxLayout(this);
        calendar = new QCalendarWidget(this);
        resetBtn = new QPushButton("Clear", this);
        layout->addWidget(calendar);
        layout->addWidget(resetBtn);
        connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
        connect(resetBtn, &QPushButton::clicked, this, &CalendarDialog::onResetBtnClick);
        connect(this, SIGNAL(rejected()), this, SLOT(handleExit()));
    }
    void onResetBtnClick() {
        delete calendar;
        calendar = new QCalendarWidget(this);
        layout->addWidget(calendar);
        connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(onDateClicked(QDate)));
    }
public slots:
    void handleExit() {
        qDebug() << "Exiting calendar...";
//        QMainWindow* window = qobject_cast<QMainWindow*>(parent());
//        if (window) {

//        }
    }
public slots:
    void onDateClicked(QDate date) {
//        QCalendarWidget* calendar = qobject_cast<QCalendarWidget*>(sender());
        if (calendar) {
            bool ok;
            QString event = QInputDialog::getText(this, tr("Event details"), tr("Enter event details: "), QLineEdit::Normal, "", &ok);
            if (ok and !event.isEmpty()) {
                QLabel* details = new QLabel(event, calendar);
                calendar->setDateTextFormat(date, QTextCharFormat());
                QTextCharFormat format;
                format.setBackground(Qt::red);
                calendar->setDateTextFormat(date, format);
                layout->addWidget(details);
            }
        }
    }
private:
    QCalendarWidget* calendar;
    QPushButton* resetBtn;
    QVBoxLayout* layout;
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
    void setSavedCalendar(CalendarDialog* calendar);
private:
    Ui::MainWindow *ui;
    QPushButton* addBtn;
    QPushButton* searchBtn;
    QList <Recipe*> recipeList;
    QListView* listView;
    RecipeListModel* model;
    QPushButton* removeBtn;
    QPushButton* calendarBtn;
    CalendarDialog* savedCalendar = nullptr;
};

#endif // MAINWINDOW_H
